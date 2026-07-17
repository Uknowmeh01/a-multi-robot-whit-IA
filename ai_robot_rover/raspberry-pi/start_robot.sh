#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
VENV_DIR="$PROJECT_DIR/.venv"

if [[ ! -x "$VENV_DIR/bin/python" ]]; then
  echo "Environnement virtuel absent. Executez d'abord : ./install.sh" >&2
  exit 1
fi

mkdir -p "$PROJECT_DIR/logs" "$PROJECT_DIR/data/debug_images"
cd "$PROJECT_DIR"
exec "$VENV_DIR/bin/python" -u main.py
