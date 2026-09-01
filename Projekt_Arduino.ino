// Motoren (L298N)
const int ENA = 10;  // PWM Links
const int IN1 = 9;
const int IN2 = 8;
const int IN3 = 7;
const int IN4 = 6;
const int ENB = 5;  // PWM Rechts
 
// Ultraschallsensor (HC-SR04)
const int TRIG = A2;
const int ECHO = A3;
 
// Liniensensoren (IR)
const int SENSOR_LINKS = A0;
const int SENSOR_RECHTS = A1;
 
// Neuer Button-Pin
const int BUTTON_PIN = 2;
 
// Variablen für die Modi
int roboterMode = 0; // 0 = Stop, 1 = Hindernis, 2 = Linienfolger
bool letzterButtonStatus = HIGH;
 
void setup() {
  // Motor-Pins als Ausgang
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
 
  // Sensor-Pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(SENSOR_LINKS, INPUT);
  pinMode(SENSOR_RECHTS, INPUT);
 
  // Button mit internem Pullup-Widerstand
  pinMode(BUTTON_PIN, INPUT_PULLUP);
 
  Serial.begin(9600);
}
 
void loop() {
  bool aktuellerButtonStatus = digitalRead(BUTTON_PIN);
 
if (letzterButtonStatus == HIGH && aktuellerButtonStatus == LOW) {
  delay(50);
  roboterMode++;
 
  if (roboterMode > 2) roboterMode = 0;
 
  Serial.print("Modus: ");
  Serial.println(roboterMode);
}
 
letzterButtonStatus = aktuellerButtonStatus;
 
  // 2. MODUS AUSFÜHREN
  switch (roboterMode) {
    case 0:
      motorenStoppen();
      break;
     
    case 1:
      modusHindernisAusweichen();
      break;
     
    case 2:
      modusLinienFolgen();
      break;
  }
}
 
 
void modusHindernisAusweichen() {
  long distanz = messeDistanz();
 
   Serial.print("Distanz: ");
  Serial.println(distanz);
 
  if (distanz > 0 && distanz < 20) { // Hindernis näher als 20cm
    motorenStoppen();
    delay(200);
    rueckwaerts();
    delay(600); // Fährt kurz rückwärts
    drehe90Grad();
    delay(500); // Zeitspanne für ca. 90 Grad Drehung (Musst du evtl. anpassen!)
  } else {
    vorwaerts();
  }
}
 
void modusLinienFolgen() {
  int links = digitalRead(SENSOR_LINKS);
  int rechts = digitalRead(SENSOR_RECHTS);
 
 
  if (links == HIGH && rechts == HIGH) {
    vorwaerts(); 
  }
  else if (links == LOW && rechts == HIGH) {
    kurveRechts(); // Linker Sensor verlässt die Bahn -> nach rechts korrigieren
  }
  else if (links == HIGH && rechts == LOW) {
    kurveLinks(); // Rechter Sensor verlässt die Bahn -> nach links korrigieren
  }
  else {
    motorenStoppen(); // Kreuzung oder Linie verloren
  }
}
 
// --- MOTORSTEUERUNG ---
 
void vorwaerts() {
  analogWrite(ENA, 180); analogWrite(ENB, 180); // Geschwindigkeit (0-255)
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
 
void rueckwaerts() {
  analogWrite(ENA, 150); analogWrite(ENB, 150);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}
 
void drehe90Grad() {
  analogWrite(ENA, 180); analogWrite(ENB, 180);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Linke Kette/Räder vorwärts
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Rechte Kette/Räder rückwärts
}
 
void kurveLinks() {
  analogWrite(ENA, 80);  analogWrite(ENB, 180); // Links langsamer als rechts
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
 
void kurveRechts() {
  analogWrite(ENA, 180); analogWrite(ENB, 80);  // Rechts langsamer als links
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
 
void motorenStoppen() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
 
// --- ULTRASCHALL-MESSUNG ---
long messeDistanz() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long dauer = pulseIn(ECHO, HIGH);
  return dauer * 0.034 / 2; // Umrechnung in cm
}