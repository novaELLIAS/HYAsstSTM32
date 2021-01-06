#include <Arduino.h>
#include <TinyGPS++.h>
#include <Metro.h>
#include <accelgyro_functions.h>

#define SSID "ELLIAS"
#define PASSWORD "Akimihomura!"
#define HOST_NAME "api.heclouds.com"
#define DEVICE_ID "644250210" //device id
const String APIKey = "fhAS54e5X8HL5wcaB6ZW74oA3vo="; //device api-key
const String TEL_NUM = "+8613384009298"; //accident report target tel
#define HOST_PORT (80)

Metro dataUpdate      = Metro(5000);
Metro dataFetch       = Metro(1000);
Metro accidentMonitor = Metro(100);

acceFunc Filter;

void setup() {
	// put your setup code here, to run once:
}

void loop() {
	// put your main code here, to run repeatedly:
}