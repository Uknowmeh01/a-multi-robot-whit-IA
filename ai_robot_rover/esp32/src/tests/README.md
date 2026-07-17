# Tests matériels ESP32

Chaque fichier est un programme PlatformIO indépendant sélectionné par un environnement de `platformio.ini`.

```bash
pio run -e test_motor_single -t upload
pio run -e test_motors_four -t upload
pio run -e test_ultrasonic -t upload
pio run -e test_bh1750 -t upload
pio run -e test_sound -t upload
pio run -e test_leds -t upload
pio run -e test_bluetooth -t upload
pio run -e test_serial -t upload
pio run -e test_emergency -t upload
pio device monitor -b 115200
```

Les tests moteur et arrêt d'urgence exigent le mot `START` sur le moniteur série. Retirer les roues du sol, dégager les câbles, prévoir un interrupteur physique sur l'alimentation moteur et commencer avec `MIN_TEST_SPEED` dans `include/config.h`.
