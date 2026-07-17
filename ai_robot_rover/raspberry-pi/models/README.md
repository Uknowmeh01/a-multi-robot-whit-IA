# Modèle IA

`install.sh` télécharge automatiquement `efficientdet_lite0.tflite` depuis le stockage public MediaPipe si le fichier est absent. Le modèle INT8 reconnaît les 80 classes COCO listées dans `coco_labels.txt`.

Pour remplacer le modèle, modifiez `MODEL_PATH`, `LABELS_PATH` et, si nécessaire, `AI_CONFIDENCE_THRESHOLD` dans `config.py`. Le détecteur accepte les sorties TFLite usuelles `boxes`, `classes`, `scores` et `count`.
