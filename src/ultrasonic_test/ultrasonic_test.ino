#define PIN_TRIG 5
#define PIN_ECHO 6
long pulse;

void setup() {
  pinMode(PIN_TRIG, INPUT);
  pinMode(PIN_ECHO, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //Launch pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  pulse = pulseIn(PIN_ECHO, HIGH);
  
  Serial.println(pulse);
}
