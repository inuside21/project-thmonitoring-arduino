#include "DHT.h"
#define DHTPIN D2    

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic
const int TRIG_PIN = D3;
const int ECHO_PIN = D4;

// Distance Sensor
int pin_sensor = A0;
int distance = 0;

// Offset
float tempOffset = 0;
float humiOffset = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  
  // init ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // init distance
  pinMode(pin_sensor, INPUT);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Read Ultrasonic 
  long duration, distanceCm, distanceIn;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;

  // Read Distance
  distance = analogRead(pin_sensor);

  // offset
  t = t + tempOffset;
  h = h + humiOffset;

  // output
  Serial.println(String(t) + "," + String(h) + "," + String(distance));
}
