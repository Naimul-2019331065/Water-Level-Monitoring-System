
/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#define trig D0
#define echo D1
#define motor D4
#define y D3
#define red D5
bool ok = 0;
bool Relay = 0;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
double duration, distance, percentage, dis;

char ssid[] = "wifi-name";
char pass[] = "wifi-password";

void ultra()
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    // Sets the trig on HIGH state for 10 micro seconds
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    // Reads the echo, returns the sound wave travel time in microseconds
    duration = pulseIn(echo, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    dis = 20-distance;
    percentage = (dis*100)/17;
    if(percentage<30 and motor == LOW and ok == 0) {    
        Blynk.logEvent("level","Your tank is about to empty. Please turn on motor.");
        ok = 1;
    }
    if(percentage>=70 and motor == HIGH and ok==0) {
        Blynk.logEvent("level","Your tank is almost full. Turn off the motor.");
        ok = 1;
    }
    if(percentage>=30 and percentage<70) 
        ok = 0;
    if(percentage<=30) {
        digitalWrite(D5, HIGH);
        digitalWrite(D3, LOW);
    }
    else if(percentage<50) {
        digitalWrite(D3, HIGH);
        digitalWrite(D5, LOW);
    }
    else {
        digitalWrite(D3, LOW);
        digitalWrite(D5, LOW);
    }
    if (percentage<=20) {
        digitalWrite(motor, HIGH);
        Blynk.virtualWrite(V3, HIGH);
    }
    
    else if (percentage>=90) {
        digitalWrite(motor, LOW);
        Blynk.virtualWrite(V3, LOW);
    }
    
    Serial.println(distance);
    Blynk.virtualWrite(V0, distance);
    Blynk.virtualWrite(V1, percentage);
}
BLYNK_WRITE(V3) {
    Relay = param.asInt();

    if (Relay == 1) {
        digitalWrite(motor, HIGH);
    } 
    else {
        digitalWrite(motor, LOW);
    }
}

void setup()
{
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(motor, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(y, OUTPUT);

    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 8080);
    
    timer.setInterval(50, ultra);
}

void loop()
{
    Blynk.run();
    
    timer.run(); 
}
