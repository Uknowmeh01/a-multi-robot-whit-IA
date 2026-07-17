# Programmes de test Raspberry Pi

Exécuter depuis `raspberry-pi` avec l'environnement installé :

```bash
.venv/bin/python tests/test_serial_communication.py
.venv/bin/python tests/test_camera.py
.venv/bin/python tests/test_ai_model.py
.venv/bin/python tests/test_ai_model.py chemin/vers/image.jpg
.venv/bin/python tests/test_web_interface.py
.venv/bin/python tests/test_emergency_stop.py --confirm-wheels-off-ground
```

Le test Web ne déplace aucun moteur. Les tests série commencent et terminent par `STOP`. Ne réarmez jamais un arrêt d'urgence avant d'avoir retiré l'obstacle et identifié sa cause.
