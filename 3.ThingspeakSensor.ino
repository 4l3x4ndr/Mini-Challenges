//Referencia: https://www.youtube.com/watch?v=rv7WpgsXATg 

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

const int trigPin1 = D1;
const int echoPin1 = D2;

unsigned long ch_no = 1234733;//Replace with Thingspeak Channel number
const char * write_api = "K2EH29M1RWHPJIX4";//Replace with Thingspeak write API

char ssid[] = "LUSAFMA";
char pass[] = "Altma4020";

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;

WiFiClient  client;
long duration1;
int distance1;

void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
  startMillis = millis();  //initial start time
}
void loop()
{
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.println(distance1);

  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    ThingSpeak.setField(1, distance1);
    ThingSpeak.writeFields(ch_no, write_api);
    startMillis = currentMillis;
  }
  delay(20000);
}
