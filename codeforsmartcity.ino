#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "yo";
const char* password = "hellyeah";
#define ORG "hxy4ny"
#define DEVICE_TYPE "BS2"
#define DEVICE_ID "5678"
#define TOKEN "BRIJESH5678"
int flow,capacity;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";// brocker
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
#define  yfsensor1 D2
#define led D1
int ldrValue = 0; 
volatile int flowing_frequency;
float waterquantity_prhour;
unsigned long currentTime1;
unsigned long cloopTime1;
int duration;
int level;
void rpm()
{
 flowing_frequency++;
 
}

void setup() {
  pinMode(yfsensor1,INPUT);
 int ldrValue = 0; // variable to store the value coming from the sensor
Serial.begin(115200); //sets serial port for communication
pinMode(led,OUTPUT);
pinMode(A0,INPUT);
pinMode(D5, OUTPUT);
pinMode(D8, INPUT);

}

void loop(){
  
   attachInterrupt(4, rpm, RISING); // Setup Interrupt m
   sei();
   currentTime1 = millis();
   // Every second, calculate and print litres/hour
   if(currentTime1 >= (cloopTime1 + 1000))
   { Serial.println("yes");    
      cloopTime1 = currentTime1;              // Updates cloopTime
      waterquantity_prhour = (flowing_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      flowing_frequency = 0;                   // Reset Counter
      Serial.print(waterquantity_prhour, DEC);            // Print litres/hour
      Serial.println(" L/hour");
   }
   int duration, distance;
digitalWrite(D5, HIGH);
delay(10);
digitalWrite(D5, LOW);
duration = pulseIn(D8, HIGH);
distance = 0.0343 * (duration/2);
digitalRead(distance);


Serial.print("distance:=");
Serial.println(distance);
capacity=distance;
flow=waterquantity_prhour;

delay(1000);
ldrValue = analogRead(A0); // read the value from the sensor
ldrValue=map(ldrValue,0,1023,0,255);
Serial.println(ldrValue); //prints the values coming from the sensor on the screen

if(ldrValue>175)
digitalWrite(D1,ldrValue);

else
digitalWrite(D1,LOW); 

 Serial.begin(115200);
 Serial.println();
 
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());

PublishData(flow,capacity);
 if (!client.loop()) {
    mqttConnect();
  }
delay(100);
}
void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
   
    Serial.println();
  }
}



void PublishData(float capacity, float flow){
  
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"capacity\":";
  payload += capacity;
  payload+="," "\"flow\":";
  payload += flow;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
