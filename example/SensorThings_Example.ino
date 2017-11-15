/*******************************************************************************
 * Copyright (c) 2017 SensorUp Inc.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 */
 
/*
*  Simple code to work with SensorThings Arduino API.
*  It works with Linkit One board . (https://www.seeedstudio.com/item_detail.html?p_id=2017)
*/

#include <LWiFi.h>
#include <LWiFiClient.h>
#include <PubSubClient.h>
#include <SensorThings.h>

/*
  Modify to your WIFI Access Credentials.
*/
#define WIFI_AP "wifi-name"
#define WIFI_PASSWORD "wifi-password"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
LWiFiClient client;

SensorThings sta;
char* server = "sta-server-address";
char* broker = "sta-mqtt-broker-address";

void setup()
{
  Serial.begin( 115200 );
  delay(1000);
  Serial.println("Start connecting your Linkit One to SensorThings.");
  InitLWiFi();

  /** Initialize SensorThings Entity **/
  Serial.println("init SensorThings");
  sta = SensorThings (client,server,80, broker,1883);

  /** Create Entities for SensorThings API **/
  sta.createEntities("Device","Datastream","Unit Of Measurement","Sensor","ObservedProperty");
  /** Send an Observation with Location **/
  sta.sendObservation("14","51","114");
  /** Send a standalone Observation **/
  sta.sendObservation("15");
}

void loop()
{
}

void InitLWiFi()
{
  LWiFi.begin();
  // Keep retrying until connected to AP
  Serial.println("Connecting");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD))) {
    delay(1000);
  }
  Serial.println("Connected");
}




