#define TRIG 9
#define ECHO 8

float dist;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(TRIG, LOW);

  Serial.begin(9600);
}

void loop() {
  // Pulse
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Echo
  dist = pulseIn(ECHO, HIGH)/58.82;

  //Serial print
  Serial.print("Distancia: ");
  Serial.println(dist);

  // Delay
  delay(250);
}
