"""Detecteur d'objets TensorFlow Lite generique, optimise pour EfficientDet-Lite0."""

from __future__ import annotations

from dataclasses import asdict, dataclass
from datetime import datetime
from pathlib import Path
from typing import Any

import cv2
import numpy as np

import config
from logger import log

try:
    from tflite_runtime.interpreter import Interpreter
except ImportError:
    try:
        from tensorflow.lite.python.interpreter import Interpreter  # type: ignore[no-redef]
    except ImportError:
        Interpreter = None  # type: ignore[assignment,misc]


@dataclass(frozen=True)
class Detection:
    label: str
    confidence: float
    box: dict[str, int]
    horizontal_position: str
    center_x: float
    detected_at: str

    def to_dict(self) -> dict[str, Any]:
        return asdict(self)


class AIDetector:
    def __init__(
        self,
        model_path: Path = config.MODEL_PATH,
        labels_path: Path = config.LABELS_PATH,
        threshold: float = config.AI_CONFIDENCE_THRESHOLD,
    ) -> None:
        self.model_path = Path(model_path)
        self.labels_path = Path(labels_path)
        self.threshold = max(0.0, min(1.0, threshold))
        self.interpreter = None
        self.input_details: list[dict[str, Any]] = []
        self.output_details: list[dict[str, Any]] = []
        self.labels: list[str] = []
        self.available = False
        self.last_error = ""

    def initialize(self) -> bool:
        if Interpreter is None:
            self.last_error = "Runtime TensorFlow Lite non installe"
            log.error(self.last_error)
            return False
        if not self.model_path.is_file() or not self.labels_path.is_file():
            self.last_error = f"Modele ou labels absents: {self.model_path}, {self.labels_path}"
            log.error(self.last_error)
            return False
        try:
            self.labels = self._load_labels(self.labels_path)
            self.interpreter = Interpreter(
                model_path=str(self.model_path), num_threads=max(1, config.AI_THREADS)
            )
            self.interpreter.allocate_tensors()
            self.input_details = self.interpreter.get_input_details()
            self.output_details = self.interpreter.get_output_details()
            if len(self.input_details) != 1:
                raise RuntimeError("Le modele doit posseder une seule entree image")
            self.available = True
            self.last_error = ""
            shape = self.input_details[0]["shape"]
            log.info("Modele IA charge: %s, entree=%s", self.model_path.name, shape.tolist())
            return True
        except Exception as exc:
            self.last_error = str(exc)
            self.available = False
            log.exception("Chargement du modele IA impossible")
            return False

    @staticmethod
    def _load_labels(path: Path) -> list[str]:
        labels: list[str] = []
        for raw in path.read_text(encoding="utf-8").splitlines():
            line = raw.strip()
            if not line:
                continue
            pieces = line.split(maxsplit=1)
            labels.append(pieces[1] if len(pieces) == 2 and pieces[0].isdigit() else line)
        if not labels:
            raise RuntimeError("Le fichier de labels est vide")
        return labels

    @staticmethod
    def _dequantize(array: np.ndarray, detail: dict[str, Any]) -> np.ndarray:
        scale, zero = detail.get("quantization", (0.0, 0))
        if scale and np.issubdtype(array.dtype, np.integer):
            return (array.astype(np.float32) - zero) * scale
        return array.astype(np.float32)

    def _prepare_input(self, frame_rgb: np.ndarray) -> np.ndarray:
        detail = self.input_details[0]
        _, height, width, channels = detail["shape"]
        if channels != 3:
            raise RuntimeError(f"Nombre de canaux non pris en charge: {channels}")
        resized = cv2.resize(frame_rgb, (int(width), int(height)), interpolation=cv2.INTER_AREA)
        dtype = detail["dtype"]
        if np.issubdtype(dtype, np.floating):
            tensor = (resized.astype(np.float32) - 127.5) / 127.5
        elif dtype == np.int8:
            scale, zero = detail.get("quantization", (1.0, 0))
            tensor = np.clip(np.rint(resized / scale + zero), -128, 127).astype(np.int8)
        else:
            tensor = resized.astype(dtype)
        return np.expand_dims(tensor, axis=0)

    def _extract_outputs(self) -> tuple[np.ndarray, np.ndarray, np.ndarray, int]:
        outputs = [
            self._dequantize(self.interpreter.get_tensor(detail["index"]), detail)
            for detail in self.output_details
        ]
        boxes_index = next(
            (i for i, value in enumerate(outputs) if value.ndim >= 2 and value.shape[-1] == 4),
            None,
        )
        if boxes_index is None:
            raise RuntimeError("Sortie de boites englobantes introuvable")
        boxes = np.squeeze(outputs[boxes_index])
        candidates = [i for i in range(len(outputs)) if i != boxes_index and outputs[i].size > 1]
        names = [str(detail.get("name", "")).lower() for detail in self.output_details]
        score_index = next((i for i in candidates if "score" in names[i]), None)
        class_index = next((i for i in candidates if "class" in names[i]), None)
        if score_index is None or class_index is None:
            # Convention SSD/EfficientDet TFLite: boxes, classes, scores, count.
            # Elle evite de confondre une liste de classes toutes egales a zero avec des scores.
            if len(candidates) == 2:
                class_index, score_index = candidates
            else:
                vectors = [(i, np.squeeze(outputs[i])) for i in candidates]
                score_index = next(
                    (i for i, value in vectors if value.size and np.nanmin(value) >= 0 and np.nanmax(value) <= 1.001),
                    candidates[-1] if candidates else None,
                )
                class_index = next((i for i in candidates if i != score_index), None)
        if score_index is None or class_index is None:
            raise RuntimeError("Sorties classes/scores introuvables")
        scores = np.ravel(outputs[score_index])
        classes = np.ravel(outputs[class_index])
        count_outputs = [
            np.ravel(outputs[i])
            for i in range(len(outputs))
            if i not in {boxes_index, score_index, class_index} and outputs[i].size == 1
        ]
        count = int(count_outputs[0][0]) if count_outputs else min(len(boxes), len(scores), len(classes))
        return boxes, classes, scores, count

    def detect(self, frame_rgb: np.ndarray) -> list[dict[str, Any]]:
        if not self.available and not self.initialize():
            return []
        try:
            input_tensor = self._prepare_input(frame_rgb)
            detail = self.input_details[0]
            self.interpreter.set_tensor(detail["index"], input_tensor)
            self.interpreter.invoke()
            boxes, classes, scores, count = self._extract_outputs()
            height, width = frame_rgb.shape[:2]
            timestamp = datetime.now().astimezone().isoformat(timespec="milliseconds")
            results: list[Detection] = []
            for i in range(min(count, len(scores), config.AI_MAX_RESULTS)):
                score = float(scores[i])
                if not np.isfinite(score) or score < self.threshold:
                    continue
                ymin, xmin, ymax, xmax = [float(value) for value in boxes[i]]
                xmin, xmax = sorted((max(0.0, min(1.0, xmin)), max(0.0, min(1.0, xmax))))
                ymin, ymax = sorted((max(0.0, min(1.0, ymin)), max(0.0, min(1.0, ymax))))
                center_x = (xmin + xmax) / 2.0
                position = "left" if center_x < 0.4 else "right" if center_x > 0.6 else "center"
                class_id = int(round(float(classes[i])))
                label = self.labels[class_id] if 0 <= class_id < len(self.labels) else f"class_{class_id}"
                results.append(
                    Detection(
                        label=label,
                        confidence=round(score, 4),
                        box={
                            "x": int(xmin * width),
                            "y": int(ymin * height),
                            "width": int((xmax - xmin) * width),
                            "height": int((ymax - ymin) * height),
                        },
                        horizontal_position=position,
                        center_x=round(center_x, 4),
                        detected_at=timestamp,
                    )
                )
            return [detection.to_dict() for detection in sorted(results, key=lambda item: item.confidence, reverse=True)]
        except Exception as exc:
            self.last_error = str(exc)
            self.available = False
            log.exception("Inference IA impossible")
            return []
