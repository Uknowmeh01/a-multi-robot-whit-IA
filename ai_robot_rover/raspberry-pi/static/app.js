"use strict";

const socket = io({ reconnection: true, reconnectionDelay: 500, timeout: 3000 });
const $ = (id) => document.getElementById(id);
const movementButtons = [...document.querySelectorAll("[data-action]")];
const keyMap = {
  KeyW: "FORWARD", ArrowUp: "FORWARD",
  KeyS: "BACKWARD", ArrowDown: "BACKWARD",
  KeyA: "LEFT", ArrowLeft: "LEFT",
  KeyD: "RIGHT", ArrowRight: "RIGHT",
  KeyQ: "ROTATE_LEFT", KeyE: "ROTATE_RIGHT",
};
let activeAction = null;
let activeKey = null;
let repeatTimer = null;
let toastTimer = null;
let latestState = {};

function speed() { return Number.parseInt($("speedSlider").value, 10) || 0; }
function emitMove() {
  if (!activeAction || !socket.connected) return;
  socket.emit("move", { action: activeAction, speed: speed() });
}
function markActive(action, enabled) {
  movementButtons.filter((button) => button.dataset.action === action)
    .forEach((button) => button.classList.toggle("active", enabled));
}
function startMove(action, key = null) {
  if (activeAction === action) return;
  stopMove();
  activeAction = action;
  activeKey = key;
  markActive(action, true);
  emitMove();
  repeatTimer = window.setInterval(emitMove, 220);
}
function stopMove() {
  if (repeatTimer) window.clearInterval(repeatTimer);
  repeatTimer = null;
  if (activeAction) markActive(activeAction, false);
  activeAction = null;
  activeKey = null;
  if (socket.connected) socket.emit("stop");
}
function showToast(message) {
  const toast = $("toast");
  toast.textContent = message;
  toast.classList.remove("hidden");
  if (toastTimer) clearTimeout(toastTimer);
  toastTimer = setTimeout(() => toast.classList.add("hidden"), 3200);
}
function formatValue(value, digits = 1) {
  return typeof value === "number" && Number.isFinite(value) ? value.toFixed(digits) : "—";
}
function renderObjects(objects) {
  const list = $("objectsList");
  if (!Array.isArray(objects) || objects.length === 0) {
    list.innerHTML = '<p class="muted">Aucune détection</p>';
    return;
  }
  list.replaceChildren(...objects.slice(0, 10).map((object) => {
    const card = document.createElement("article");
    card.className = "object-card";
    const name = document.createElement("strong");
    name.textContent = String(object.label || "objet");
    const detail = document.createElement("span");
    detail.textContent = `${Math.round((Number(object.confidence) || 0) * 100)} % · ${object.horizontal_position || "—"}`;
    card.append(name, detail);
    return card;
  }));
}
function renderTelemetry(state) {
  latestState = state || {};
  const online = socket.connected;
  $("connectionBadge").classList.toggle("online", online);
  $("connectionBadge").classList.toggle("offline", !online);
  $("connectionText").textContent = online ? "Téléphone connecté" : "Hors ligne";
  $("currentMode").textContent = state.mode || "—";
  $("currentSpeed").textContent = state.speed ?? "—";
  $("serialState").textContent = state.serial_connected ? `Connecté ${state.serial_port || ""}` : "Déconnecté";
  $("bluetoothState").textContent = state.bluetooth_connected ? "Connecté" : "Déconnecté";
  if (document.activeElement !== $("modeSelect") && state.mode) $("modeSelect").value = state.mode;
  if (document.activeElement !== $("speedSlider") && Number.isFinite(state.speed)) {
    $("speedSlider").value = state.speed;
    $("speedOutput").textContent = state.speed;
  }
  const distances = state.distances || {};
  $("distanceLeft").textContent = formatValue(distances.left);
  $("distanceCenter").textContent = formatValue(distances.center);
  $("distanceRight").textContent = formatValue(distances.right);
  $("lightLevel").textContent = formatValue(state.light_lux);
  $("soundLevel").textContent = formatValue(state.sound_level, 0);
  $("aiState").textContent = state.ai_state || state.autonomous_state || "ARRÊT";
  $("lastEvent").textContent = state.last_event || "—";
  $("emergencyBanner").classList.toggle("hidden", !state.emergency);
  renderObjects(state.objects);
  const errors = Array.isArray(state.errors) ? state.errors : [];
  const errorList = $("errorsList");
  errorList.replaceChildren(...(errors.length ? errors : ["Aucune erreur"]).map((error) => {
    const li = document.createElement("li");
    li.textContent = error;
    if (!errors.length) li.className = "muted";
    return li;
  }));
}

movementButtons.forEach((button) => {
  button.addEventListener("pointerdown", (event) => {
    event.preventDefault();
    button.setPointerCapture(event.pointerId);
    startMove(button.dataset.action);
  });
  ["pointerup", "pointercancel", "lostpointercapture"].forEach((name) =>
    button.addEventListener(name, (event) => { event.preventDefault(); stopMove(); })
  );
});

$("stopButton").addEventListener("pointerdown", (event) => { event.preventDefault(); stopMove(); });
$("emergencyButton").addEventListener("click", () => { stopMove(); socket.emit("emergency_stop"); });
$("clearEmergency").addEventListener("click", () => socket.emit("clear_emergency"));
$("modeSelect").addEventListener("change", (event) => { stopMove(); socket.emit("set_mode", { mode: event.target.value }); });
$("speedSlider").addEventListener("input", (event) => { $("speedOutput").textContent = event.target.value; });
$("speedSlider").addEventListener("change", () => socket.emit("set_speed", { speed: speed() }));
$("ledToggle").addEventListener("change", (event) => socket.emit("set_led", { enabled: event.target.checked }));

window.addEventListener("keydown", (event) => {
  if (["INPUT", "SELECT", "TEXTAREA"].includes(document.activeElement?.tagName)) return;
  if (event.code === "Space") {
    event.preventDefault();
    stopMove();
    return;
  }
  const action = keyMap[event.code];
  if (action && !event.repeat) {
    event.preventDefault();
    startMove(action, event.code);
  }
});
window.addEventListener("keyup", (event) => {
  if (event.code === activeKey || keyMap[event.code] === activeAction) {
    event.preventDefault();
    stopMove();
  }
});
window.addEventListener("blur", stopMove);
document.addEventListener("visibilitychange", () => { if (document.hidden) stopMove(); });
window.addEventListener("pagehide", stopMove);

socket.on("connect", () => { renderTelemetry(latestState); socket.emit("heartbeat"); });
socket.on("disconnect", () => { stopMove(); renderTelemetry(latestState); showToast("Connexion perdue : STOP de sécurité"); });
socket.on("telemetry", renderTelemetry);
socket.on("command_error", (payload) => { stopMove(); showToast(payload?.message || "Commande refusée"); });
setInterval(() => { if (socket.connected) socket.emit("heartbeat"); }, 250);
