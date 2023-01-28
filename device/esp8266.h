#define NO_GLOBAL_INSTANCES
#define NO_GLOBAL_SERIAL
#define NO_GLOBAL_SERIAL1
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
WiFiUDP UDP;

void setup(){
	UDP.begin(1024);
	WiFi.mode(WIFI_STA);
	WiFi.setPhyMode(WIFI_PHY_MODE_11N);
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	WiFi.hostname(WiFi.macAddress());
	WiFi.begin(WIFI_SSID,WIFI_KEY);
}

#include "doutput.h"
DOutput BuiltInLED(2);
DOutput Output1(0);
DOutput Output2(1);

#include "interval.h"
Interval HalfSec(0x200);

bool WiFiLED(unsigned char status){
	switch(status){
		case WL_CONNECTED:return true;
		case WL_CONNECT_FAILED:
		case WL_CONNECTION_LOST:
		case WL_NO_SSID_AVAIL:return HalfSec.Get();
		default:return false;}
}
#include "timer.h"
unsigned char Status=0;
static Timer OnTimer;

void loop(){
	BuiltInLED.Set(WiFiLED(WiFi.status()));
	if(unsigned int len=UDP.parsePacket()){
		unsigned char data[len];
		if(UDP.read(data,len)){
			if(len>1){
				if(data[0]==DEVICE_GROUP){ //Group address
					Status=data[1];
					OnTimer.Reset(TIMER_LIMIT);
				}
			}
		}
	}
	Output1.Set(OnTimer.Running()&&Status==1);
	Output2.Set(OnTimer.Running()&&Status==2);
}
