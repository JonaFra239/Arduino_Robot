#  Arduino/STM32 Roboter – Hinderniserkennung & Linienfolger

##  Projektbeschreibung

In diesem Projekt wurde ein kleiner autonomer Roboter entwickelt, der mit einem **STM32 NUCLEO-F446RE** gesteuert wird.

Der Roboter verfügt über zwei verschiedene Betriebsmodi:

*  **Hindernisvermeidung:** Der Roboter erkennt Hindernisse mit einem Ultraschallsensor und reagiert darauf.
*  **Linienfolger:** Mithilfe von zwei Infrarot-Liniensensoren kann der Roboter einer vorgegebenen Linie folgen.
*  **Stop-Modus:** Der Roboter bleibt stehen.

Über einen Taster kann zwischen den verschiedenen Modi gewechselt werden.

---

##  Verwendete Komponenten

| Komponente          | Funktion                    |
| ------------------- | --------------------------- |
| STM32 NUCLEO-F446RE | Mikrocontroller             |
| L298N               | Motorsteuerung              |
| 4x Motoren          | Antrieb                     |
| HC-SR04             | Ultraschall-Abstandsmessung |
| 2x IR-Sensoren      | Linienerkennung             |
| Taster              | Umschalten der Betriebsmodi |

---

##  Funktionsweise

### 1. Hindernisvermeidung

Der HC-SR04 misst regelmäßig den Abstand vor dem Roboter.

Wird ein Hindernis in einer Entfernung von weniger als **20 cm** erkannt, führt der Roboter folgende Schritte aus:

1. Stoppen
2. Kurz rückwärts fahren
3. Eine Drehung um ungefähr 90° durchführen
4. Anschließend weiterfahren

Die Drehung wird dabei zeitgesteuert. Je nach Untergrund und Motoren kann die Dauer angepasst werden.

### 2. Linienfolger

Die beiden IR-Sensoren überprüfen, ob sich der Roboter noch auf der Linie befindet.

Je nach Kombination der Sensorsignale wird die Geschwindigkeit der beiden Motoren angepasst:

* Beide Sensoren erkennen die gewünschte Position → geradeaus fahren
* Linker Sensor weicht ab → Korrektur nach rechts
* Rechter Sensor weicht ab → Korrektur nach links
* Keine eindeutige Erkennung → Roboter stoppt

### 3. Betriebsmodi

Mit dem Taster können die drei Modi nacheinander ausgewählt werden:

```text
0 → Stop
1 → Hindernisvermeidung
2 → Linienfolger
```

Nach Modus 2 beginnt die Auswahl wieder bei Modus 0.

---

##  Pinbelegung

### Motorsteuerung – L298N

| Pin | Funktion                |
| --- | ----------------------- |
| D10 | ENA – PWM linker Motor  |
| D9  | IN1                     |
| D8  | IN2                     |
| D7  | IN3                     |
| D6  | IN4                     |
| D5  | ENB – PWM rechter Motor |

### Sensoren

| Pin | Funktion         |
| --- | ---------------- |
| A2  | HC-SR04 TRIG     |
| A3  | HC-SR04 ECHO     |
| A0  | IR-Sensor links  |
| A1  | IR-Sensor rechts |
| D2  | Taster           |

Schaltplan unter "Schaltplan.jpg"
---

## 💻 Software

Der Code ist in C/C++ für die ArduinoIDE geschrieben.

Die einzelnen Funktionen wurden bewusst aufgeteilt, damit die verschiedenen Aufgaben des Roboters voneinander getrennt sind.

Beispiele:

```cpp
void vorwaerts();
void rueckwaerts();
void drehe90Grad();

void kurveLinks();
void kurveRechts();
void motorenStoppen();

void modusHindernisAusweichen();
void modusLinienFolgen();

long messeDistanz();
```

Dadurch bleibt der Code übersichtlich und einzelne Funktionen können unabhängig voneinander angepasst und getestet werden.

---

