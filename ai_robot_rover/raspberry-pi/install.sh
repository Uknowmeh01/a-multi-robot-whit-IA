#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
VENV_DIR="$PROJECT_DIR/.venv"
SERVICE_SOURCE="$PROJECT_DIR/robot.service"
SERVICE_TARGET="/etc/systemd/system/robot.service"
MODEL_PATH="$PROJECT_DIR/models/efficientdet_lite0.tflite"

step() { printf '\n\033[1;34m==> %s\033[0m\n' "$1"; }
fail() { printf '\nERREUR: %s\n' "$1" >&2; exit 1; }

step "Verification du systeme"
command -v python3 >/dev/null 2>&1 || fail "Python 3 est absent."
python3 -c 'import sys; raise SystemExit(0 if sys.version_info >= (3, 9) else 1)' || fail "Python 3.9 minimum est requis."
command -v sudo >/dev/null 2>&1 || fail "sudo est requis pour les paquets et systemd."

for required in main.py config.py requirements.txt start_robot.sh robot.service templates/index.html static/app.js; do
  [[ -f "$PROJECT_DIR/$required" ]] || fail "Fichier requis absent: $required"
done

step "Installation des paquets Raspberry Pi OS"
sudo apt-get update
sudo apt-get install -y python3-venv python3-picamera2 python3-opencv python3-numpy curl

step "Creation de l'environnement Python"
python3 -m venv --system-site-packages "$VENV_DIR"
"$VENV_DIR/bin/python" -m pip install --upgrade pip wheel
"$VENV_DIR/bin/pip" install -r "$PROJECT_DIR/requirements.txt"

step "Preparation des dossiers"
mkdir -p "$PROJECT_DIR/logs" "$PROJECT_DIR/data/debug_images" "$PROJECT_DIR/models"
chmod +x "$PROJECT_DIR/start_robot.sh" "$PROJECT_DIR/install.sh"

if [[ ! -s "$MODEL_PATH" ]]; then
  step "Telechargement d'EfficientDet-Lite0 INT8"
  temp_model="${MODEL_PATH}.part"
  curl --fail --location --retry 3 --output "$temp_model" "$("$VENV_DIR/bin/python" -c 'import config; print(config.MODEL_DOWNLOAD_URL)' 2>/dev/null)"
  [[ -s "$temp_model" ]] || fail "Le modele telecharge est vide."
  mv "$temp_model" "$MODEL_PATH"
fi

step "Droits serie et camera"
sudo usermod -aG dialout,video "${SUDO_USER:-$USER}"

step "Preparation du service systemd"
escaped_dir=$(printf '%s' "$PROJECT_DIR" | sed 's/[&|]/\\&/g')
escaped_user=$(printf '%s' "${SUDO_USER:-$USER}" | sed 's/[&|]/\\&/g')
sed -e "s|@@ROBOT_DIR@@|$escaped_dir|g" -e "s|@@ROBOT_USER@@|$escaped_user|g" "$SERVICE_SOURCE" | sudo tee "$SERVICE_TARGET" >/dev/null
sudo systemctl daemon-reload

step "Verification Python"
"$VENV_DIR/bin/python" -m compileall -q "$PROJECT_DIR"

cat <<EOF

Installation terminee.

1. Deconnectez-vous puis reconnectez-vous pour activer les groupes dialout/video.
2. Verifiez SERIAL_PORT et les seuils dans: $PROJECT_DIR/config.py
3. Testez sans roues au sol: cd "$PROJECT_DIR" && ./start_robot.sh
4. Activez le demarrage automatique seulement apres les tests:
   sudo systemctl enable --now robot.service
5. Consultez les logs:
   journalctl -u robot.service -f

Le service demarre en mode MANUAL et envoie STOP: aucune mise en mouvement automatique.
EOF
