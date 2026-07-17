EESchema Schematic File Version 4
LIBS:power
LIBS:device
LIBS:Connector_Generic
EELAYER 29 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "Carte centrale rover IA - Uknozmeh01"
Date "2026-07-17"
Rev "A - prototype"
Comp "Uknozmeh01"
Comment1 "Batterie 2S, Raspberry Pi 4, ESP32, 4 moteurs"
Comment2 "Le fichier PCB et CONNEXIONS_NETS.csv font autorité pour le brochage"
$EndDescr
Text Notes 700 600 0    120  ~ 24
ALIMENTATION ET SECURITE
Text Notes 5900 600 0    120  ~ 24
CALCUL / COMMANDES
Text Notes 11100 600 0    120  ~ 24
CAPTEURS / EXTENSIONS
Text Notes 700 10800 0    100  ~ 20
ATTENTION : batterie Li-ion 2S avec BMS et chargeur certifies externes. Verifier le courant de blocage de chaque moteur.
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 86E128A0
P 900 1100
F 0 "J1" H 1100 1200 50  0000 C CNN
F 1 "ENTREE_BATTERIE_2S" H 1200 1000 50  0000 C CNN
	1    900 1100
	1    0    0    -1
$EndComp
Text Notes 450 1270 0    35   ~ 0
1:VBAT_RAW
Text Notes 450 1355 0    35   ~ 0
2:GND
$Comp
L Device:R F1
U 1 1 29DDD3C3
P 2400 1100
F 0 "F1" H 2600 1200 50  0000 C CNN
F 1 "FUSIBLE_15A" H 2700 1000 50  0000 C CNN
	1    2400 1100
	1    0    0    -1
$EndComp
Text Notes 1950 1270 0    35   ~ 0
1:VBAT_RAW
Text Notes 1950 1355 0    35   ~ 0
2:VBAT_FUSED
$Comp
L Connector_Generic:Conn_01x03 Q1
U 1 1 6B1599FF
P 3900 1100
F 0 "Q1" H 4100 1200 50  0000 C CNN
F 1 "IPD90P03P4L-04" H 4200 1000 50  0000 C CNN
	1    3900 1100
	1    0    0    -1
$EndComp
Text Notes 3450 1270 0    35   ~ 0
1:Q_GATE
Text Notes 3450 1355 0    35   ~ 0
2:VBAT_PROTECTED
Text Notes 3450 1440 0    35   ~ 0
3:VBAT_FUSED
$Comp
L Device:R R1
U 1 1 78E50958
P 900 1750
F 0 "R1" H 1100 1850 50  0000 C CNN
F 1 "100k" H 1200 1650 50  0000 C CNN
	1    900 1750
	1    0    0    -1
$EndComp
Text Notes 450 1920 0    35   ~ 0
1:Q_GATE
Text Notes 450 2005 0    35   ~ 0
2:GND
$Comp
L Device:D D1
U 1 1 77E98C45
P 2400 1750
F 0 "D1" H 2600 1850 50  0000 C CNN
F 1 "BZT52C10" H 2700 1650 50  0000 C CNN
	1    2400 1750
	1    0    0    -1
$EndComp
Text Notes 1950 1920 0    35   ~ 0
1:Q_GATE
Text Notes 1950 2005 0    35   ~ 0
2:VBAT_PROTECTED
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 145E19A6
P 3900 1750
F 0 "J2" H 4100 1850 50  0000 C CNN
F 1 "INTERRUPTEUR_GENERAL" H 4200 1650 50  0000 C CNN
	1    3900 1750
	1    0    0    -1
$EndComp
Text Notes 3450 1920 0    35   ~ 0
1:VBAT_PROTECTED
Text Notes 3450 2005 0    35   ~ 0
2:VBAT_SW
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 D5E6BD65
P 900 2400
F 0 "J3" H 1100 2500 50  0000 C CNN
F 1 "ARRET_URGENCE_NF" H 1200 2300 50  0000 C CNN
	1    900 2400
	1    0    0    -1
$EndComp
Text Notes 450 2570 0    35   ~ 0
1:VBAT_SW
Text Notes 450 2655 0    35   ~ 0
2:VBAT_ESTOP
$Comp
L Device:R RSH1
U 1 1 67A4A25D
P 2400 2400
F 0 "RSH1" H 2600 2500 50  0000 C CNN
F 1 "5mR_3W_1%" H 2700 2300 50  0000 C CNN
	1    2400 2400
	1    0    0    -1
$EndComp
Text Notes 1950 2570 0    35   ~ 0
1:VBAT_ESTOP
Text Notes 1950 2655 0    35   ~ 0
2:VMOTOR
$Comp
L Connector_Generic:Conn_01x10 U1
U 1 1 5A555690
P 3900 2400
F 0 "U1" H 4100 2500 50  0000 C CNN
F 1 "INA226AIDGSR" H 4200 2300 50  0000 C CNN
	1    3900 2400
	1    0    0    -1
$EndComp
Text Notes 3450 2570 0    35   ~ 0
1:GND
Text Notes 3450 2655 0    35   ~ 0
2:GND
Text Notes 3450 2740 0    35   ~ 0
3:INA_ALERT
Text Notes 3450 2825 0    35   ~ 0
4:I2C_SDA
Text Notes 3450 2910 0    35   ~ 0
5:I2C_SCL
Text Notes 3450 2995 0    35   ~ 0
6:+3V3
Text Notes 3450 3080 0    35   ~ 0
... 10 broches: voir CONNEXIONS_NETS.csv
$Comp
L Device:C C1
U 1 1 28B54001
P 900 3050
F 0 "C1" H 1100 3150 50  0000 C CNN
F 1 "100n" H 1200 2950 50  0000 C CNN
	1    900 3050
	1    0    0    -1
$EndComp
Text Notes 450 3220 0    35   ~ 0
1:+3V3
Text Notes 450 3305 0    35   ~ 0
2:GND
$Comp
L Device:D D2
U 1 1 023BC303
P 2400 3050
F 0 "D2" H 2600 3150 50  0000 C CNN
F 1 "SMBJ9.0A" H 2700 2950 50  0000 C CNN
	1    2400 3050
	1    0    0    -1
$EndComp
Text Notes 1950 3220 0    35   ~ 0
1:VMOTOR
Text Notes 1950 3305 0    35   ~ 0
2:GND
$Comp
L Device:C C2
U 1 1 9CD1D831
P 3900 3050
F 0 "C2" H 4100 3150 50  0000 C CNN
F 1 "1000u_16V_lowESR" H 4200 2950 50  0000 C CNN
	1    3900 3050
	1    0    0    -1
$EndComp
Text Notes 3450 3220 0    35   ~ 0
1:VMOTOR
Text Notes 3450 3305 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x04 U2
U 1 1 9BB2F720
P 900 3700
F 0 "U2" H 1100 3800 50  0000 C CNN
F 1 "BUCK_5V_9A_D24V90F5" H 1200 3600 50  0000 C CNN
	1    900 3700
	1    0    0    -1
$EndComp
Text Notes 450 3870 0    35   ~ 0
1:VMOTOR
Text Notes 450 3955 0    35   ~ 0
2:GND
Text Notes 450 4040 0    35   ~ 0
3:+5V
Text Notes 450 4125 0    35   ~ 0
4:GND
$Comp
L Device:C C3
U 1 1 E0D4A9C9
P 2400 3700
F 0 "C3" H 2600 3800 50  0000 C CNN
F 1 "470u_10V_lowESR" H 2700 3600 50  0000 C CNN
	1    2400 3700
	1    0    0    -1
$EndComp
Text Notes 1950 3870 0    35   ~ 0
1:+5V
Text Notes 1950 3955 0    35   ~ 0
2:GND
$Comp
L Device:R F2
U 1 1 C1B56F95
P 3900 3700
F 0 "F2" H 4100 3800 50  0000 C CNN
F 1 "PTC_5A" H 4200 3600 50  0000 C CNN
	1    3900 3700
	1    0    0    -1
$EndComp
Text Notes 3450 3870 0    35   ~ 0
1:+5V
Text Notes 3450 3955 0    35   ~ 0
2:+5V_RPI
$Comp
L Device:R F3
U 1 1 E477CB64
P 900 4350
F 0 "F3" H 1100 4450 50  0000 C CNN
F 1 "PTC_1.5A" H 1200 4250 50  0000 C CNN
	1    900 4350
	1    0    0    -1
$EndComp
Text Notes 450 4520 0    35   ~ 0
1:+5V
Text Notes 450 4605 0    35   ~ 0
2:+5V_LOGIC
$Comp
L Device:R R2
U 1 1 F2E07243
P 2400 4350
F 0 "R2" H 2600 4450 50  0000 C CNN
F 1 "100k" H 2700 4250 50  0000 C CNN
	1    2400 4350
	1    0    0    -1
$EndComp
Text Notes 1950 4520 0    35   ~ 0
1:EN_3V3
Text Notes 1950 4605 0    35   ~ 0
2:+5V_LOGIC
$Comp
L Device:LED D3
U 1 1 AC3E066C
P 3900 4350
F 0 "D3" H 4100 4450 50  0000 C CNN
F 1 "LED_VERTE_POWER" H 4200 4250 50  0000 C CNN
	1    3900 4350
	1    0    0    -1
$EndComp
Text Notes 3450 4520 0    35   ~ 0
1:PWR_LED_K
Text Notes 3450 4605 0    35   ~ 0
2:GND
$Comp
L Device:R R3
U 1 1 3BB6FDEB
P 900 5000
F 0 "R3" H 1100 5100 50  0000 C CNN
F 1 "1k" H 1200 4900 50  0000 C CNN
	1    900 5000
	1    0    0    -1
$EndComp
Text Notes 450 5170 0    35   ~ 0
1:+5V_LOGIC
Text Notes 450 5255 0    35   ~ 0
2:PWR_LED_K
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 39C66A7F
P 2400 5000
F 0 "J4" H 2600 5100 50  0000 C CNN
F 1 "STAT_CHARGE_OPEN_DRAIN" H 2700 4900 50  0000 C CNN
	1    2400 5000
	1    0    0    -1
$EndComp
Text Notes 1950 5170 0    35   ~ 0
1:CHG_STAT
Text Notes 1950 5255 0    35   ~ 0
2:GND
$Comp
L Device:LED D4
U 1 1 D0B3A055
P 3900 5000
F 0 "D4" H 4100 5100 50  0000 C CNN
F 1 "LED_ORANGE_CHARGE" H 4200 4900 50  0000 C CNN
	1    3900 5000
	1    0    0    -1
$EndComp
Text Notes 3450 5170 0    35   ~ 0
1:CHG_LED_A
Text Notes 3450 5255 0    35   ~ 0
2:CHG_STAT
$Comp
L Device:R R4
U 1 1 75D4957E
P 900 5650
F 0 "R4" H 1100 5750 50  0000 C CNN
F 1 "1k" H 1200 5550 50  0000 C CNN
	1    900 5650
	1    0    0    -1
$EndComp
Text Notes 450 5820 0    35   ~ 0
1:+5V_LOGIC
Text Notes 450 5905 0    35   ~ 0
2:CHG_LED_A
$Comp
L Connector_Generic:Conn_01x24 U7
U 1 1 B9299C6B
P 2400 5650
F 0 "U7" H 2600 5750 50  0000 C CNN
F 1 "TB6612FNG_M1_M2" H 2700 5550 50  0000 C CNN
	1    2400 5650
	1    0    0    -1
$EndComp
Text Notes 1950 5820 0    35   ~ 0
1:GND
Text Notes 1950 5905 0    35   ~ 0
2:M1_O1
Text Notes 1950 5990 0    35   ~ 0
3:M1_O1
Text Notes 1950 6075 0    35   ~ 0
4:GND
Text Notes 1950 6160 0    35   ~ 0
5:GND
Text Notes 1950 6245 0    35   ~ 0
6:M1_O2
Text Notes 1950 6330 0    35   ~ 0
... 24 broches: voir CONNEXIONS_NETS.csv
$Comp
L Connector_Generic:Conn_01x24 U8
U 1 1 AA03268B
P 3900 5650
F 0 "U8" H 4100 5750 50  0000 C CNN
F 1 "TB6612FNG_M3_M4" H 4200 5550 50  0000 C CNN
	1    3900 5650
	1    0    0    -1
$EndComp
Text Notes 3450 5820 0    35   ~ 0
1:GND
Text Notes 3450 5905 0    35   ~ 0
2:M3_O1
Text Notes 3450 5990 0    35   ~ 0
3:M3_O1
Text Notes 3450 6075 0    35   ~ 0
4:GND
Text Notes 3450 6160 0    35   ~ 0
5:GND
Text Notes 3450 6245 0    35   ~ 0
6:M3_O2
Text Notes 3450 6330 0    35   ~ 0
... 24 broches: voir CONNEXIONS_NETS.csv
$Comp
L Device:C C7
U 1 1 607FE17A
P 900 6300
F 0 "C7" H 1100 6400 50  0000 C CNN
F 1 "100n" H 1200 6200 50  0000 C CNN
	1    900 6300
	1    0    0    -1
$EndComp
Text Notes 450 6470 0    35   ~ 0
1:+3V3
Text Notes 450 6555 0    35   ~ 0
2:GND
$Comp
L Device:C C8
U 1 1 C7FF04FA
P 2400 6300
F 0 "C8" H 2600 6400 50  0000 C CNN
F 1 "10u_16V" H 2700 6200 50  0000 C CNN
	1    2400 6300
	1    0    0    -1
$EndComp
Text Notes 1950 6470 0    35   ~ 0
1:VMOTOR
Text Notes 1950 6555 0    35   ~ 0
2:GND
$Comp
L Device:C C9
U 1 1 E5F5584D
P 3900 6300
F 0 "C9" H 4100 6400 50  0000 C CNN
F 1 "100n" H 4200 6200 50  0000 C CNN
	1    3900 6300
	1    0    0    -1
$EndComp
Text Notes 3450 6470 0    35   ~ 0
1:+3V3
Text Notes 3450 6555 0    35   ~ 0
2:GND
$Comp
L Device:C C10
U 1 1 93712FF7
P 900 6950
F 0 "C10" H 1100 7050 50  0000 C CNN
F 1 "10u_16V" H 1200 6850 50  0000 C CNN
	1    900 6950
	1    0    0    -1
$EndComp
Text Notes 450 7120 0    35   ~ 0
1:VMOTOR
Text Notes 450 7205 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 7B9104CD
P 2400 6950
F 0 "J8" H 2600 7050 50  0000 C CNN
F 1 "MOTEUR_1" H 2700 6850 50  0000 C CNN
	1    2400 6950
	1    0    0    -1
$EndComp
Text Notes 1950 7120 0    35   ~ 0
1:M1_O1
Text Notes 1950 7205 0    35   ~ 0
2:M1_O2
$Comp
L Connector_Generic:Conn_01x02 J9
U 1 1 7476E39E
P 3900 6950
F 0 "J9" H 4100 7050 50  0000 C CNN
F 1 "MOTEUR_2" H 4200 6850 50  0000 C CNN
	1    3900 6950
	1    0    0    -1
$EndComp
Text Notes 3450 7120 0    35   ~ 0
1:M2_O1
Text Notes 3450 7205 0    35   ~ 0
2:M2_O2
$Comp
L Connector_Generic:Conn_01x02 J10
U 1 1 34CFA01F
P 900 7600
F 0 "J10" H 1100 7700 50  0000 C CNN
F 1 "MOTEUR_3" H 1200 7500 50  0000 C CNN
	1    900 7600
	1    0    0    -1
$EndComp
Text Notes 450 7770 0    35   ~ 0
1:M3_O1
Text Notes 450 7855 0    35   ~ 0
2:M3_O2
$Comp
L Connector_Generic:Conn_01x02 J11
U 1 1 57F0D83B
P 2400 7600
F 0 "J11" H 2600 7700 50  0000 C CNN
F 1 "MOTEUR_4" H 2700 7500 50  0000 C CNN
	1    2400 7600
	1    0    0    -1
$EndComp
Text Notes 1950 7770 0    35   ~ 0
1:M4_O1
Text Notes 1950 7855 0    35   ~ 0
2:M4_O2
$Comp
L Device:C C11
U 1 1 E11150AA
P 3900 7600
F 0 "C11" H 4100 7700 50  0000 C CNN
F 1 "100n_MOTEUR" H 4200 7500 50  0000 C CNN
	1    3900 7600
	1    0    0    -1
$EndComp
Text Notes 3450 7770 0    35   ~ 0
1:M1_O1
Text Notes 3450 7855 0    35   ~ 0
2:M1_O2
$Comp
L Device:C C12
U 1 1 23590FFB
P 900 8250
F 0 "C12" H 1100 8350 50  0000 C CNN
F 1 "100n_MOTEUR" H 1200 8150 50  0000 C CNN
	1    900 8250
	1    0    0    -1
$EndComp
Text Notes 450 8420 0    35   ~ 0
1:M2_O1
Text Notes 450 8505 0    35   ~ 0
2:M2_O2
$Comp
L Device:C C13
U 1 1 AB9F19FD
P 2400 8250
F 0 "C13" H 2600 8350 50  0000 C CNN
F 1 "100n_MOTEUR" H 2700 8150 50  0000 C CNN
	1    2400 8250
	1    0    0    -1
$EndComp
Text Notes 1950 8420 0    35   ~ 0
1:M3_O1
Text Notes 1950 8505 0    35   ~ 0
2:M3_O2
$Comp
L Device:C C14
U 1 1 E746C8D2
P 3900 8250
F 0 "C14" H 4100 8350 50  0000 C CNN
F 1 "100n_MOTEUR" H 4200 8150 50  0000 C CNN
	1    3900 8250
	1    0    0    -1
$EndComp
Text Notes 3450 8420 0    35   ~ 0
1:M4_O1
Text Notes 3450 8505 0    35   ~ 0
2:M4_O2
$Comp
L Connector_Generic:Conn_01x02 J20
U 1 1 291041F7
P 900 8900
F 0 "J20" H 1100 9000 50  0000 C CNN
F 1 "ALIM_5V_AUX" H 1200 8800 50  0000 C CNN
	1    900 8900
	1    0    0    -1
$EndComp
Text Notes 450 9070 0    35   ~ 0
1:+5V_LOGIC
Text Notes 450 9155 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x02 TP1
U 1 1 9D205182
P 2400 8900
F 0 "TP1" H 2600 9000 50  0000 C CNN
F 1 "TP_GND" H 2700 8800 50  0000 C CNN
	1    2400 8900
	1    0    0    -1
$EndComp
Text Notes 1950 9070 0    35   ~ 0
1:GND
$Comp
L Connector_Generic:Conn_01x02 TP2
U 1 1 FF05F513
P 3900 8900
F 0 "TP2" H 4100 9000 50  0000 C CNN
F 1 "TP_VMOTOR" H 4200 8800 50  0000 C CNN
	1    3900 8900
	1    0    0    -1
$EndComp
Text Notes 3450 9070 0    35   ~ 0
1:VMOTOR
$Comp
L Connector_Generic:Conn_01x02 TP3
U 1 1 B4B7B7C0
P 900 9550
F 0 "TP3" H 1100 9650 50  0000 C CNN
F 1 "TP_5V" H 1200 9450 50  0000 C CNN
	1    900 9550
	1    0    0    -1
$EndComp
Text Notes 450 9720 0    35   ~ 0
1:+5V
$Comp
L Connector_Generic:Conn_01x02 TP4
U 1 1 5BB68969
P 2400 9550
F 0 "TP4" H 2600 9650 50  0000 C CNN
F 1 "TP_3V3" H 2700 9450 50  0000 C CNN
	1    2400 9550
	1    0    0    -1
$EndComp
Text Notes 1950 9720 0    35   ~ 0
1:+3V3
$Comp
L Connector_Generic:Conn_01x05 U3
U 1 1 7FC1CBEF
P 5900 1100
F 0 "U3" H 6100 1200 50  0000 C CNN
F 1 "AP2112K-3.3" H 6200 1000 50  0000 C CNN
	1    5900 1100
	1    0    0    -1
$EndComp
Text Notes 5450 1270 0    35   ~ 0
1:+5V_LOGIC
Text Notes 5450 1355 0    35   ~ 0
2:GND
Text Notes 5450 1440 0    35   ~ 0
3:EN_3V3
Text Notes 5450 1525 0    35   ~ 0
4:NC
Text Notes 5450 1610 0    35   ~ 0
5:+3V3
$Comp
L Device:C C4
U 1 1 000D6138
P 7400 1100
F 0 "C4" H 7600 1200 50  0000 C CNN
F 1 "1u" H 7700 1000 50  0000 C CNN
	1    7400 1100
	1    0    0    -1
$EndComp
Text Notes 6950 1270 0    35   ~ 0
1:+3V3
Text Notes 6950 1355 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x30 U4
U 1 1 47C91E6B
P 8900 1100
F 0 "U4" H 9100 1200 50  0000 C CNN
F 1 "ESP32_DEVKIT_V1_30P" H 9200 1000 50  0000 C CNN
	1    8900 1100
	1    0    0    -1
$EndComp
Text Notes 8450 1270 0    35   ~ 0
1:ESP_EN
Text Notes 8450 1355 0    35   ~ 0
2:ECHO_AV
Text Notes 8450 1440 0    35   ~ 0
3:GPIO39_RES
Text Notes 8450 1525 0    35   ~ 0
4:ECHO_G
Text Notes 8450 1610 0    35   ~ 0
5:ECHO_D
Text Notes 8450 1695 0    35   ~ 0
6:TRIG_AV
Text Notes 8450 1780 0    35   ~ 0
... 30 broches: voir CONNEXIONS_NETS.csv
$Comp
L Connector_Generic:Conn_01x40 J5
U 1 1 33F746AB
P 5900 1750
F 0 "J5" H 6100 1850 50  0000 C CNN
F 1 "RASPBERRY_PI_4_HEADER_40" H 6200 1650 50  0000 C CNN
	1    5900 1750
	1    0    0    -1
$EndComp
Text Notes 5450 1920 0    35   ~ 0
1:RPI_3V3
Text Notes 5450 2005 0    35   ~ 0
2:+5V_RPI
Text Notes 5450 2090 0    35   ~ 0
3:PI_GPIO2
Text Notes 5450 2175 0    35   ~ 0
4:+5V_RPI
Text Notes 5450 2260 0    35   ~ 0
5:PI_GPIO3
Text Notes 5450 2345 0    35   ~ 0
6:GND
Text Notes 5450 2430 0    35   ~ 0
... 40 broches: voir CONNEXIONS_NETS.csv
$Comp
L Connector_Generic:Conn_01x16 U5
U 1 1 AEEA9F2A
P 7400 1750
F 0 "U5" H 7600 1850 50  0000 C CNN
F 1 "74HC595D_MOTEURS" H 7700 1650 50  0000 C CNN
	1    7400 1750
	1    0    0    -1
$EndComp
Text Notes 6950 1920 0    35   ~ 0
1:M1_IN2
Text Notes 6950 2005 0    35   ~ 0
2:M2_IN1
Text Notes 6950 2090 0    35   ~ 0
3:M2_IN2
Text Notes 6950 2175 0    35   ~ 0
4:M3_IN1
Text Notes 6950 2260 0    35   ~ 0
5:M3_IN2
Text Notes 6950 2345 0    35   ~ 0
6:M4_IN1
Text Notes 6950 2430 0    35   ~ 0
... 16 broches: voir CONNEXIONS_NETS.csv
$Comp
L Connector_Generic:Conn_01x16 U6
U 1 1 0BB11390
P 8900 1750
F 0 "U6" H 9100 1850 50  0000 C CNN
F 1 "74HC595D_LED" H 9200 1650 50  0000 C CNN
	1    8900 1750
	1    0    0    -1
$EndComp
Text Notes 8450 1920 0    35   ~ 0
1:LED_AI_A
Text Notes 8450 2005 0    35   ~ 0
2:LED_AUTO_A
Text Notes 8450 2090 0    35   ~ 0
3:LED_USER_A
Text Notes 8450 2175 0    35   ~ 0
4:SR_QD_RES
Text Notes 8450 2260 0    35   ~ 0
5:SR_QE_RES
Text Notes 8450 2345 0    35   ~ 0
6:SR_QF_RES
Text Notes 8450 2430 0    35   ~ 0
... 16 broches: voir CONNEXIONS_NETS.csv
$Comp
L Device:C C5
U 1 1 6BD1462A
P 5900 2400
F 0 "C5" H 6100 2500 50  0000 C CNN
F 1 "100n" H 6200 2300 50  0000 C CNN
	1    5900 2400
	1    0    0    -1
$EndComp
Text Notes 5450 2570 0    35   ~ 0
1:+3V3
Text Notes 5450 2655 0    35   ~ 0
2:GND
$Comp
L Device:C C6
U 1 1 28DCBAC1
P 7400 2400
F 0 "C6" H 7600 2500 50  0000 C CNN
F 1 "100n" H 7700 2300 50  0000 C CNN
	1    7400 2400
	1    0    0    -1
$EndComp
Text Notes 6950 2570 0    35   ~ 0
1:+3V3
Text Notes 6950 2655 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x04 J18
U 1 1 83CA21A5
P 8900 2400
F 0 "J18" H 9100 2500 50  0000 C CNN
F 1 "EXTENSION_UART_ESP" H 9200 2300 50  0000 C CNN
	1    8900 2400
	1    0    0    -1
$EndComp
Text Notes 8450 2570 0    35   ~ 0
1:+3V3
Text Notes 8450 2655 0    35   ~ 0
2:GND
Text Notes 8450 2740 0    35   ~ 0
3:UART_TX_ESP
Text Notes 8450 2825 0    35   ~ 0
4:UART_RX_ESP
$Comp
L Device:R R13
U 1 1 A553DB58
P 5900 3050
F 0 "R13" H 6100 3150 50  0000 C CNN
F 1 "1k" H 6200 2950 50  0000 C CNN
	1    5900 3050
	1    0    0    -1
$EndComp
Text Notes 5450 3220 0    35   ~ 0
1:LED_ESP_A
Text Notes 5450 3305 0    35   ~ 0
2:LED_ESP_A_R
$Comp
L Device:LED D5
U 1 1 A633CD43
P 7400 3050
F 0 "D5" H 7600 3150 50  0000 C CNN
F 1 "LED_BLEUE_ESP" H 7700 2950 50  0000 C CNN
	1    7400 3050
	1    0    0    -1
$EndComp
Text Notes 6950 3220 0    35   ~ 0
1:LED_ESP_A_R
Text Notes 6950 3305 0    35   ~ 0
2:GND
$Comp
L Device:R R14
U 1 1 916D7705
P 8900 3050
F 0 "R14" H 9100 3150 50  0000 C CNN
F 1 "1k" H 9200 2950 50  0000 C CNN
	1    8900 3050
	1    0    0    -1
$EndComp
Text Notes 8450 3220 0    35   ~ 0
1:LED_AI_A
Text Notes 8450 3305 0    35   ~ 0
2:LED_AI_A_R
$Comp
L Device:LED D6
U 1 1 4327AEF2
P 5900 3700
F 0 "D6" H 6100 3800 50  0000 C CNN
F 1 "LED_VIOLETTE_AI" H 6200 3600 50  0000 C CNN
	1    5900 3700
	1    0    0    -1
$EndComp
Text Notes 5450 3870 0    35   ~ 0
1:LED_AI_A_R
Text Notes 5450 3955 0    35   ~ 0
2:GND
$Comp
L Device:R R15
U 1 1 F0FDA913
P 7400 3700
F 0 "R15" H 7600 3800 50  0000 C CNN
F 1 "1k" H 7700 3600 50  0000 C CNN
	1    7400 3700
	1    0    0    -1
$EndComp
Text Notes 6950 3870 0    35   ~ 0
1:LED_AUTO_A
Text Notes 6950 3955 0    35   ~ 0
2:LED_AUTO_A_R
$Comp
L Device:LED D7
U 1 1 A00C8725
P 8900 3700
F 0 "D7" H 9100 3800 50  0000 C CNN
F 1 "LED_VERTE_AUTO" H 9200 3600 50  0000 C CNN
	1    8900 3700
	1    0    0    -1
$EndComp
Text Notes 8450 3870 0    35   ~ 0
1:LED_AUTO_A_R
Text Notes 8450 3955 0    35   ~ 0
2:GND
$Comp
L Device:R R16
U 1 1 41A70C47
P 5900 4350
F 0 "R16" H 6100 4450 50  0000 C CNN
F 1 "1k" H 6200 4250 50  0000 C CNN
	1    5900 4350
	1    0    0    -1
$EndComp
Text Notes 5450 4520 0    35   ~ 0
1:LED_USER_A
Text Notes 5450 4605 0    35   ~ 0
2:LED_USER_A_R
$Comp
L Device:LED D8
U 1 1 AF25F1C2
P 7400 4350
F 0 "D8" H 7600 4450 50  0000 C CNN
F 1 "LED_JAUNE_USER" H 7700 4250 50  0000 C CNN
	1    7400 4350
	1    0    0    -1
$EndComp
Text Notes 6950 4520 0    35   ~ 0
1:LED_USER_A_R
Text Notes 6950 4605 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x05 U9
U 1 1 FA67AC95
P 8900 4350
F 0 "U9" H 9100 4450 50  0000 C CNN
F 1 "SN74AHCT1G125" H 9200 4250 50  0000 C CNN
	1    8900 4350
	1    0    0    -1
$EndComp
Text Notes 8450 4520 0    35   ~ 0
1:GND
Text Notes 8450 4605 0    35   ~ 0
2:RGB_DATA_3V3
Text Notes 8450 4690 0    35   ~ 0
3:RGB_DATA_5V
Text Notes 8450 4775 0    35   ~ 0
4:GND
Text Notes 8450 4860 0    35   ~ 0
5:+5V_LOGIC
$Comp
L Device:R R17
U 1 1 F2A3F8DC
P 5900 5000
F 0 "R17" H 6100 5100 50  0000 C CNN
F 1 "330R" H 6200 4900 50  0000 C CNN
	1    5900 5000
	1    0    0    -1
$EndComp
Text Notes 5450 5170 0    35   ~ 0
1:RGB_DATA_5V
Text Notes 5450 5255 0    35   ~ 0
2:RGB_DATA_OUT
$Comp
L Device:C C15
U 1 1 1E4A4FD9
P 7400 5000
F 0 "C15" H 7600 5100 50  0000 C CNN
F 1 "100n" H 7700 4900 50  0000 C CNN
	1    7400 5000
	1    0    0    -1
$EndComp
Text Notes 6950 5170 0    35   ~ 0
1:+5V_LOGIC
Text Notes 6950 5255 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x02 J21
U 1 1 DDA11012
P 8900 5000
F 0 "J21" H 9100 5100 50  0000 C CNN
F 1 "TEST_EN_STBY" H 9200 4900 50  0000 C CNN
	1    8900 5000
	1    0    0    -1
$EndComp
Text Notes 8450 5170 0    35   ~ 0
1:MOTOR_STBY
Text Notes 8450 5255 0    35   ~ 0
2:GND
$Comp
L Device:C C16
U 1 1 013F6FDA
P 5900 5650
F 0 "C16" H 6100 5750 50  0000 C CNN
F 1 "100n" H 6200 5550 50  0000 C CNN
	1    5900 5650
	1    0    0    -1
$EndComp
Text Notes 5450 5820 0    35   ~ 0
1:+3V3
Text Notes 5450 5905 0    35   ~ 0
2:GND
$Comp
L Device:C C17
U 1 1 1A359BF3
P 7400 5650
F 0 "C17" H 7600 5750 50  0000 C CNN
F 1 "10u" H 7700 5550 50  0000 C CNN
	1    7400 5650
	1    0    0    -1
$EndComp
Text Notes 6950 5820 0    35   ~ 0
1:+5V_LOGIC
Text Notes 6950 5905 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x20 J6
U 1 1 91B0911F
P 11100 1100
F 0 "J6" H 11300 1200 50  0000 C CNN
F 1 "BREAKOUT_GPIO_PI_A" H 11400 1000 50  0000 C CNN
	1    11100 1100
	1    0    0    -1
$EndComp
Text Notes 10650 1270 0    35   ~ 0
1:RPI_3V3
Text Notes 10650 1355 0    35   ~ 0
2:GND
Text Notes 10650 1440 0    35   ~ 0
3:PI_GPIO2
Text Notes 10650 1525 0    35   ~ 0
4:PI_GPIO3
Text Notes 10650 1610 0    35   ~ 0
5:PI_GPIO4
Text Notes 10650 1695 0    35   ~ 0
6:PI_GPIO17
Text Notes 10650 1780 0    35   ~ 0
... 20 broches: voir CONNEXIONS_NETS.csv
$Comp
L Connector_Generic:Conn_01x16 J7
U 1 1 4AD533AB
P 12600 1100
F 0 "J7" H 12800 1200 50  0000 C CNN
F 1 "BREAKOUT_GPIO_PI_B" H 12900 1000 50  0000 C CNN
	1    12600 1100
	1    0    0    -1
$EndComp
Text Notes 12150 1270 0    35   ~ 0
1:RPI_3V3
Text Notes 12150 1355 0    35   ~ 0
2:GND
Text Notes 12150 1440 0    35   ~ 0
3:PI_GPIO12
Text Notes 12150 1525 0    35   ~ 0
4:PI_GPIO13
Text Notes 12150 1610 0    35   ~ 0
5:PI_GPIO19
Text Notes 12150 1695 0    35   ~ 0
6:PI_GPIO16
Text Notes 12150 1780 0    35   ~ 0
... 16 broches: voir CONNEXIONS_NETS.csv
$Comp
L Connector_Generic:Conn_01x04 J12
U 1 1 EC4B6C49
P 14100 1100
F 0 "J12" H 14300 1200 50  0000 C CNN
F 1 "HC_SR04_AV" H 14400 1000 50  0000 C CNN
	1    14100 1100
	1    0    0    -1
$EndComp
Text Notes 13650 1270 0    35   ~ 0
1:+5V_LOGIC
Text Notes 13650 1355 0    35   ~ 0
2:TRIG_AV
Text Notes 13650 1440 0    35   ~ 0
3:ECHO_AV_5V
Text Notes 13650 1525 0    35   ~ 0
4:GND
$Comp
L Device:R R5
U 1 1 EA828FED
P 11100 1750
F 0 "R5" H 11300 1850 50  0000 C CNN
F 1 "10k" H 11400 1650 50  0000 C CNN
	1    11100 1750
	1    0    0    -1
$EndComp
Text Notes 10650 1920 0    35   ~ 0
1:ECHO_AV_5V
Text Notes 10650 2005 0    35   ~ 0
2:ECHO_AV
$Comp
L Device:R R6
U 1 1 7D4B914A
P 12600 1750
F 0 "R6" H 12800 1850 50  0000 C CNN
F 1 "20k" H 12900 1650 50  0000 C CNN
	1    12600 1750
	1    0    0    -1
$EndComp
Text Notes 12150 1920 0    35   ~ 0
1:ECHO_AV
Text Notes 12150 2005 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x04 J13
U 1 1 717D50A2
P 14100 1750
F 0 "J13" H 14300 1850 50  0000 C CNN
F 1 "HC_SR04_G" H 14400 1650 50  0000 C CNN
	1    14100 1750
	1    0    0    -1
$EndComp
Text Notes 13650 1920 0    35   ~ 0
1:+5V_LOGIC
Text Notes 13650 2005 0    35   ~ 0
2:TRIG_G
Text Notes 13650 2090 0    35   ~ 0
3:ECHO_G_5V
Text Notes 13650 2175 0    35   ~ 0
4:GND
$Comp
L Device:R R7
U 1 1 B5D4CFF1
P 11100 2400
F 0 "R7" H 11300 2500 50  0000 C CNN
F 1 "10k" H 11400 2300 50  0000 C CNN
	1    11100 2400
	1    0    0    -1
$EndComp
Text Notes 10650 2570 0    35   ~ 0
1:ECHO_G_5V
Text Notes 10650 2655 0    35   ~ 0
2:ECHO_G
$Comp
L Device:R R8
U 1 1 4FADAEB5
P 12600 2400
F 0 "R8" H 12800 2500 50  0000 C CNN
F 1 "20k" H 12900 2300 50  0000 C CNN
	1    12600 2400
	1    0    0    -1
$EndComp
Text Notes 12150 2570 0    35   ~ 0
1:ECHO_G
Text Notes 12150 2655 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x04 J14
U 1 1 4055A178
P 14100 2400
F 0 "J14" H 14300 2500 50  0000 C CNN
F 1 "HC_SR04_D" H 14400 2300 50  0000 C CNN
	1    14100 2400
	1    0    0    -1
$EndComp
Text Notes 13650 2570 0    35   ~ 0
1:+5V_LOGIC
Text Notes 13650 2655 0    35   ~ 0
2:TRIG_D
Text Notes 13650 2740 0    35   ~ 0
3:ECHO_D_5V
Text Notes 13650 2825 0    35   ~ 0
4:GND
$Comp
L Device:R R9
U 1 1 4D0CDDC1
P 11100 3050
F 0 "R9" H 11300 3150 50  0000 C CNN
F 1 "10k" H 11400 2950 50  0000 C CNN
	1    11100 3050
	1    0    0    -1
$EndComp
Text Notes 10650 3220 0    35   ~ 0
1:ECHO_D_5V
Text Notes 10650 3305 0    35   ~ 0
2:ECHO_D
$Comp
L Device:R R10
U 1 1 37F2306D
P 12600 3050
F 0 "R10" H 12800 3150 50  0000 C CNN
F 1 "20k" H 12900 2950 50  0000 C CNN
	1    12600 3050
	1    0    0    -1
$EndComp
Text Notes 12150 3220 0    35   ~ 0
1:ECHO_D
Text Notes 12150 3305 0    35   ~ 0
2:GND
$Comp
L Connector_Generic:Conn_01x04 J15
U 1 1 B49B7A21
P 14100 3050
F 0 "J15" H 14300 3150 50  0000 C CNN
F 1 "BH1750_I2C" H 14400 2950 50  0000 C CNN
	1    14100 3050
	1    0    0    -1
$EndComp
Text Notes 13650 3220 0    35   ~ 0
1:+3V3
Text Notes 13650 3305 0    35   ~ 0
2:GND
Text Notes 13650 3390 0    35   ~ 0
3:I2C_SCL
Text Notes 13650 3475 0    35   ~ 0
4:I2C_SDA
$Comp
L Device:R R11
U 1 1 B1473AC1
P 11100 3700
F 0 "R11" H 11300 3800 50  0000 C CNN
F 1 "4k7_DNP" H 11400 3600 50  0000 C CNN
	1    11100 3700
	1    0    0    -1
$EndComp
Text Notes 10650 3870 0    35   ~ 0
1:+3V3
Text Notes 10650 3955 0    35   ~ 0
2:I2C_SCL
$Comp
L Device:R R12
U 1 1 D50A60BF
P 12600 3700
F 0 "R12" H 12800 3800 50  0000 C CNN
F 1 "4k7_DNP" H 12900 3600 50  0000 C CNN
	1    12600 3700
	1    0    0    -1
$EndComp
Text Notes 12150 3870 0    35   ~ 0
1:+3V3
Text Notes 12150 3955 0    35   ~ 0
2:I2C_SDA
$Comp
L Connector_Generic:Conn_01x04 J16
U 1 1 05FAD07A
P 14100 3700
F 0 "J16" H 14300 3800 50  0000 C CNN
F 1 "EXTENSION_I2C" H 14400 3600 50  0000 C CNN
	1    14100 3700
	1    0    0    -1
$EndComp
Text Notes 13650 3870 0    35   ~ 0
1:+3V3
Text Notes 13650 3955 0    35   ~ 0
2:GND
Text Notes 13650 4040 0    35   ~ 0
3:I2C_SCL
Text Notes 13650 4125 0    35   ~ 0
4:I2C_SDA
$Comp
L Connector_Generic:Conn_01x06 J17
U 1 1 5883CCE1
P 11100 4350
F 0 "J17" H 11300 4450 50  0000 C CNN
F 1 "EXTENSION_SPI" H 11400 4250 50  0000 C CNN
	1    11100 4350
	1    0    0    -1
$EndComp
Text Notes 10650 4520 0    35   ~ 0
1:+3V3
Text Notes 10650 4605 0    35   ~ 0
2:GND
Text Notes 10650 4690 0    35   ~ 0
3:SR_CLK
Text Notes 10650 4775 0    35   ~ 0
4:SR_DATA
Text Notes 10650 4860 0    35   ~ 0
5:SPI_MISO
Text Notes 10650 4945 0    35   ~ 0
6:SPI_CS
$Comp
L Connector_Generic:Conn_01x03 J19
U 1 1 E06F209C
P 12600 4350
F 0 "J19" H 12800 4450 50  0000 C CNN
F 1 "RGB_WS2812" H 12900 4250 50  0000 C CNN
	1    12600 4350
	1    0    0    -1
$EndComp
Text Notes 12150 4520 0    35   ~ 0
1:+5V_LOGIC
Text Notes 12150 4605 0    35   ~ 0
2:RGB_DATA_OUT
Text Notes 12150 4690 0    35   ~ 0
3:GND
$EndSCHEMATC
