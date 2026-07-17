# Revue de conception et guide de production

## 1. Dimensionnement électrique

Hypothèse de départ : quatre moteurs 6 V, environ 1 A chacun en régime et jusqu'à 3 A au blocage. Le moteur représente ~4 A moyens et 12 A transitoires sur VBAT. Le Raspberry Pi 4 et ses périphériques USB peuvent demander 4 A sur 5 V ; ESP32, capteurs et LED ajoutent environ 0,8 A. À 7,4 V et 90 % de rendement, la charge 5 V/4,8 A demande environ 3,6 A à la batterie. Le courant moyen de pack peut donc approcher 7,6 A, avec des pointes supérieures à 15 A.

Recommandations : pack 2S avec BMS ≥20 A, câbles batterie AWG16, interrupteur et arrêt d'urgence ≥20 A DC, F1 15 A à temporisation adaptée, buck 5 V/9 A. Si les essais au blocage dépassent la capacité des TB6612, ne pas augmenter simplement le fusible : changer les drivers.

## 2. Mesure batterie

L'INA226 est retenu plutôt que l'INA219 : meilleure résolution, plage de bus jusqu'à 36 V, temps de conversion configurable et meilleure marge pour les transitoires. Avec 5 mΩ, 15 A donnent 75 mV et 1,125 W ; le shunt 3 W garde une marge thermique. La calibration logicielle doit utiliser la valeur réellement mesurée du shunt.

## 3. Adaptation HC-SR04

Chaque ECHO passe par 10 kΩ en série puis 20 kΩ vers la masse, soit environ 3,33 V pour une sortie 5 V. Pour une liaison unidirectionnelle lente, ce pont est plus simple et plus robuste qu'un convertisseur bidirectionnel à MOSFET. Les résistances doivent rester près de l'ESP32. TRIG est directement en 3,3 V ; les HC-SR04 usuels le reconnaissent. Si une variante ne le reconnaît pas, ajouter un buffer AHCT.

## 4. Commande des moteurs

Deux 74HC595 génèrent les huit directions et les LED d'état ; les quatre PWM restent directement reliés aux timers ESP32. Au démarrage, maintenir MOTOR_STBY bas, charger les registres, verrouiller les sorties puis seulement activer STBY. Les TB6612FNG intègrent les diodes de roue libre ; des diodes externes ne sont pas nécessaires. C7/C9 et C8/C10 doivent être au plus près des broches VCC/VM, C2 près de la zone puissance, et un 100 nF doit être soudé aux bornes de chaque moteur.

## 5. Implantation et routage

- Carte 4 couches : F.Cu signaux/puissance, In1 signaux horizontaux, In2 signaux verticaux/retours courts, B.Cu plan GND continu.
- Zone forte intensité à gauche, logique au centre, Raspberry Pi et connecteurs capteurs à droite.
- Garder au moins 20 mm entre boucles moteurs/buck et connecteurs capteurs.
- Largeurs après calcul IPC et validation fabricant : batterie/VMOTOR 4 mm sur cuivre externe 2 oz ou polygone équivalent ; chaque moteur ≥2 mm ; 5 V principal ≥2,5 mm ; logique 0,25 mm ; vias puissance multiples 0,4 mm finis minimum.
- Réunir les retours moteur au plan près de C2 et du shunt ; ne pas créer une coupure étroite sous ESP32/INA226.
- Les pistes de mesure IN+/IN− doivent être Kelvin jusqu'aux pads internes du shunt et éloignées des sorties moteur.

Le PCB fourni contient un pré-routage en grille et deux plans GND. Avant fabrication, inspecter les croisements, retoucher les pistes de puissance en polygones, ajouter les vias de couture et relancer le DRC avec les règles du fabricant.

## 6. Fabrication JLCPCB / PCBWay

- 4 couches, FR-4 TG150, 1,6 mm, cuivre externe 2 oz et interne 1 oz.
- Finition ENIG recommandée pour le shunt et les petits boîtiers ; HASL sans plomb acceptable pour un prototype manuel.
- Espacement/piste logique ≥0,20 mm, idéal 0,25 mm.
- Via fini 0,40 mm / diamètre 0,80 mm ; trous connecteurs 0,9 à 1,3 mm selon pièce ; montage 2,7 mm pour vis M2.5.
- Masque épargne des deux côtés, sérigraphie blanche, contrôle d'impédance inutile.
- Commander d'abord 5 cartes, assembler une seule carte et terminer les essais avant le lot.

## 7. Assemblage

1. Contrôler PCB nu à l'ohmmètre : aucune continuité VBAT/GND, 5V/GND ou 3V3/GND.
2. Monter passifs, INA226, 74HC595, AP2112 et TB6612 ; inspecter les ponts sous loupe.
3. Monter protections et buck, sans Raspberry Pi, ESP32, capteurs ni moteurs.
4. Mettre une alimentation de laboratoire à 7,4 V, limite 100 mA ; vérifier polarité et absence d'échauffement.
5. Régler/vérifier le buck à 5,10 V. Vérifier 3,3 V.
6. Tester l'I2C et les registres avec l'ESP32 seul, puis brancher le Pi.
7. Tester un moteur roue levée avec limite de courant, puis quatre moteurs.
8. Tester l'arrêt d'urgence en charge et vérifier que MOTOR_STBY revient bas au redémarrage.

## 8. Procédure de mise sous tension initiale

- Interrupteur ouvert, arrêt d'urgence enclenché, aucun moteur.
- Vérifier résistance vers GND de chaque rail.
- Alimentation de laboratoire 7,4 V limitée à 0,1 A, puis 0,5 A.
- Mesurer VMOTOR, +5V, +5V_RPI, +5V_LOGIC, +3V3 aux TP.
- Contrôler consommation à vide et température pendant 5 minutes.
- Insérer ESP32, charger un firmware minimal avec STBY=0.
- Valider LED, INA226, BH1750 et trois ECHO.
- Insérer le Raspberry Pi éteint, puis augmenter progressivement la limite.

## 9. Checklist de débogage

- Reset ESP32 au démarrage moteur : vérifier chute VBAT, C2, câblage en étoile et masse.
- Pi sous-tension : mesurer +5V_RPI au connecteur J5 sous charge ; viser 5,0–5,1 V sans dépasser 5,25 V.
- Valeur INA226 incorrecte : vérifier routage Kelvin, signe IN+/IN−, valeur du shunt et calibration.
- Distance HC-SR04 erratique : vérifier 5 V, ponts 10k/20k, masse, temporisation et éloignement des pistes PWM.
- Moteur inversé : permuter le bornier ou le mapping logiciel, jamais le shunt.
- LED WS2812 instable : masse commune, buffer AHCT, résistance 330 Ω et condensateur local.

## 10. Améliorations possibles

- Drivers DRV8873 avec mesure de courant et protections intégrées pour moteurs plus puissants.
- Deuxième convertisseur 5 V dédié au Raspberry Pi pour isoler davantage les transitoires.
- Superviseur d'alimentation et arrêt propre du Pi avant coupure batterie.
- Connecteurs verrouillables Micro-Fit pour puissance et JST-GH pour capteurs soumis aux vibrations.
- CAN ou RS-485 pour extensions longues.
- Entrées encodeurs avec filtrage Schmitt et protection ESD.
- Chargeur/BMS intégré uniquement dans une révision séparée, avec étude thermique et conformité batterie.

## 11. Erreurs courantes à éviter

- Utiliser une batterie 3S directement avec des moteurs 6 V sans limitation PWM sûre.
- Choisir les TB6612 sans mesurer le courant de blocage.
- Alimenter le Pi par sa broche 5 V avec un buck mal réglé.
- Relier deux sources 3,3 V ensemble : RPI_3V3 n'est pas relié au +3V3 de la carte.
- Monter plusieurs pull-up I2C en parallèle jusqu'à obtenir une résistance trop faible.
- Faire passer CSI, ECHO ou I2C à côté des sorties moteurs.
- Oublier que le microphone et le haut-parleur USB consomment sur le budget 5 V du Pi.

## 12. Checklist finale de production

- [ ] Courant de blocage moteurs mesuré et compatible.
- [ ] Pinout ESP32 réel comparé au PCB.
- [ ] Empreintes comparées aux datasheets et modèles 1:1 imprimés.
- [ ] ERC sans erreur non justifiée.
- [ ] DRC sans violation non justifiée.
- [ ] Zones remplies, aucun îlot cuivre, vias de couture ajoutés.
- [ ] Pistes Kelvin INA226 vérifiées.
- [ ] Polarités J1, condensateurs, diodes et LED vérifiées.
- [ ] Buck réglé à 5,10 V et testé à 6 A.
- [ ] Test arrêt d'urgence et fusible effectué.
- [ ] Gerbers, perçages, IPC-356, BOM et plan d'assemblage relus.
- [ ] Une carte prototype validée thermiquement avant série.
