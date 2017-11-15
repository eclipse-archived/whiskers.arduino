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
 *
 * Contributors:
 *    Tania Khalafbeigi
 */

#include "SensorThings.h"
#include <SPI.h>
#include <PubSubClient.h>


const char header01[] PROGMEM = "POST /v1.0/";
const char header02[] PROGMEM = " HTTP/1.1\r\nHost: ";
const char header03[] PROGMEM = "\r\nConnection: close\r\nContent-Type: application/json\r\nCache-Control: no-cache\r\nContent-Length: ";
const char newline[] PROGMEM ="\n\n";


void SensorThings::reconnectMqtt() {
    while (!client.connected()) {
        if (!client.connect("arduinoClient")) {
            Serial.print(F("Attempt to connect to MQTT failed, rc= "));
            Serial.print(client.state());
            Serial.println(F(". try again in 5 seconds"));
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void SensorThings::disconnectMqtt(){
    client.disconnect();
}

SensorThings::SensorThings()
{
}

SensorThings::SensorThings(Client& _client, char* _server, int _port, char* _thingId, char* _datastreamId)
{
    nwClient = &_client;
    server = _server;
    port = _port;
    client.setClient(*nwClient);
    client.setServer(server, port);
    strcpy(datastreamId, _datastreamId);
    strcpy(thingId, _thingId);
}


SensorThings::SensorThings(Client& _client, char* _restfulService, int _restfulPort, char* _server, int _port)
{
    restfulService = _restfulService;
    restfulPort = _restfulPort;
    nwClient = &_client;
    server = _server;
    port = _port;
    client.setClient(*nwClient);
    client.setServer(server, port);
}

SensorThings::SensorThings(Client& _client, char* _server, char* _thingId, char* _datastreamId)
{
    SensorThings(_client, _server, 1883, _thingId, _datastreamId);
}

SensorThings::SensorThings(Client& _client, char* _restfulService, char* _server, int _port)
{
    SensorThings(_client, _restfulService, 80, _server, _port);
}

SensorThings::SensorThings(Client& _client, char* _restfulService, int _restfulPort, char* _server)
{
    SensorThings(_client, _restfulService, _restfulPort, _server, 1883);
}

SensorThings::SensorThings(Client& _client, char* _restfulService, char* _server)
{
    SensorThings(_client, _restfulService, 80, _server, 1883);
}

void SensorThings::sendObservation(char* result, char* _datastreamId)
{
    if (!client.connected()) {
        reconnectMqtt();
    }
    
    /** Create Observation mqtt request JSON body **/
    char* str = (char *)malloc(sizeof(char) * (14+strlen(result)));
    strcpy(str,"{\"result\":\"");
    strcat(str,result);
    strcat(str,"\"}");
    
    /** Sending MQTT request to create Observation**/
    char* url = (char *)malloc(sizeof(char) * (33+strlen(_datastreamId)));
    strcpy_P(url,PSTR("v1.0/Datastreams("));
    strcat(url,_datastreamId);
    strcat_P(url,PSTR(")/Observations"));
    
    /** Publish Observation **/
    client.publish(url,str);
    
    free(str);
    free(url);
    
    Serial.println(F("Observation sent over MQTT successfully."));
    
}

void SensorThings::sendObservation(char* result, char* lat, char* lon, char* _datastreamId)
{
    bool isSuccessful = setLocation(lat , lon);
    if(isSuccessful){
        sendObservation(result, _datastreamId);
    } else {
        Serial.println(F("Cannot send Observation, Location update faild."));
    }
}

void SensorThings::sendObservation(char* result, char* lat, char* lon)
{
    sendObservation(result, lat, lon, datastreamId);
}

void SensorThings::sendObservation(char* result)
{
    sendObservation(result, datastreamId);
}

void SensorThings::createThing(char* thingDescription)
{
    disconnectMqtt();
    
    Serial.println(F("Creating Thing..."));
    
    /** Sending HTTP request to create Thing**/
    int connection = nwClient->connect(restfulService, restfulPort);
    if (connection == 1) { // if connection to the server is established
        client_print_P(header01);
        nwClient->print(F("Things"));
        client_print_P(header02);
        nwClient->print(restfulService);
        client_print_P(header03);

        
        nwClient->print(18+strlen(thingDescription));

        client_print_P(newline);
        nwClient->print(F("{\"description\":\""));
        nwClient->print(thingDescription ) ;
        nwClient->print(F("\"}"));
        
        /** Reading HTTP response **/
        boolean flag = false;
        while(true) {
            if(nwClient->available()){
                char str = (char)nwClient->read();
                if(str=='@'){
                    flag = true;
                }
                if(flag){
                    if(str==':'){
                        str = (char)nwClient->read();
                        while(str!='('){
                            str = (char)nwClient->read();
                        }
                        str = (char)nwClient->read();
                        int i = 0;
                        while(str!=')'){
                            thingId[i]=str;
                            str = (char)nwClient->read();
                            i = i+1;
                        }
                        thingId[i] = '\0';
                        
                        /** Printing Thing ID **/
                        Serial.print(F("ThingId="));
                        Serial.println(thingId);
                        break;
                    }
                }
            }
        }
        nwClient->stop();
        
    } else {  // if connection to the server is NOT established
        Serial.println(F("Connection failed, cannot create Thing"));
    }
}

void SensorThings::createDatastream(char* datastreamDescription,char* unitOfMeasurement, char* sensorDescription, char* obsPropDescription, char* _datastreamId)
{
    createDatastream(datastreamDescription, "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement", unitOfMeasurement, unitOfMeasurement, "NA",
                     sensorDescription, "http://schema.org/description", sensorDescription,
                     obsPropDescription, "NA", obsPropDescription, _datastreamId);
}

void SensorThings::createDatastream(char* datastreamDescription, char* observationType, char* uomName, char* uomSymbol, char* uomDefinition,
                                    char* sensorDescription, char* sensorEncodingType, char* sensorMetadata,
                                    char* obsPropDescription, char* obsPropDefinition, char* obsPropName, char* _datastreamId)
{
    disconnectMqtt();
    
    Serial.println(F("Creating Datastream..."));
    
    /** Sending HTTP request to create Datastream **/
    int connection = nwClient->connect(restfulService, restfulPort);
    if (connection == 1) { // if connection to the server is established
        
        client_print_P(header01);
        nwClient->print(F("Things("));
        nwClient->print(thingId);
        nwClient->print(F(")/Datastreams"));
        client_print_P(header02);
        nwClient->print(restfulService);
        client_print_P(header03);
        nwClient->print(223+strlen(datastreamDescription)+strlen(observationType)+strlen(uomDefinition)+strlen(uomName)+strlen(uomSymbol)+
                        strlen(obsPropDescription)+strlen(obsPropDefinition)+strlen(obsPropName)+
                        strlen(sensorDescription)+strlen(sensorMetadata)+strlen(sensorEncodingType));
        client_print_P(newline);
        
        nwClient->print(F("{\"unitOfMeasurement\":{\"name\":\""));
        nwClient->print(uomName);
        nwClient->print(F("\",\"symbol\":\""));
        nwClient->print(uomSymbol);
        nwClient->print(F("\",\"definition\":\""));
        nwClient->print(uomDefinition);
        nwClient->print(F("\"},\"description\":\"")) ;
        nwClient->print(datastreamDescription) ;
        nwClient->print(F("\",\"observationType\":\""));
        nwClient->print(observationType);
        nwClient->print(F("\",\"ObservedProperty\":{\"name\":\""));
        nwClient->print( obsPropName);
        nwClient->print(F("\",\"definition\":\""));
        nwClient->print( obsPropDefinition);
        nwClient->print(F("\",\"description\":\""));
        nwClient->print( obsPropDescription);
        nwClient->print(F("\"},\"Sensor\":{\"description\":\""));
        nwClient->print(sensorDescription);
        nwClient->print(F("\",\"encodingType\":\""));
        nwClient->print(sensorEncodingType);
        nwClient->print(F("\",\"metadata\":\""));
        nwClient->print( sensorMetadata);
        nwClient->print(F("\"}}"));
        
        /** Reading HTTP response **/
        boolean flag = false;
        while(true) {
            if(nwClient->available()){
                char str = (char)nwClient->read();
                if(str=='@'){
                    flag = true;
                }
                if(flag){
                    if(str==':'){
                        str = (char)nwClient->read();
                        while(str!='('){
                            str = (char)nwClient->read();
                        }
                        str = (char)nwClient->read();
                        int i = 0;
                        while(str!=')'){
                            _datastreamId[i]=str;
                            str = (char)nwClient->read();
                            i=i+1;
                        }
                        _datastreamId[i]='\0';
                        
                        /** Printing Datastream ID **/
                        Serial.print(F("DatastreamId="));
                        Serial.println(_datastreamId);
                        break;
                    }
                }
            }
        }
        nwClient->stop();
    } else { // if connection to the server is NOT established
        Serial.println(F("Connection failed, cannot create Sensor"));
    }
}

void SensorThings::createDatastream(char* datastreamDescription,char* unitOfMeasurement, char* sensorDescription, char* obsPropDescription)
{
    createDatastream(datastreamDescription,unitOfMeasurement, sensorDescription, obsPropDescription, datastreamId);
}


void SensorThings::createEntities(char* thingDescription, char* datastreamDescription,char* unitOfMeasurement, char* sensorDescription, char* obsPropDescription){
    createThing(thingDescription);
    createDatastream(datastreamDescription,unitOfMeasurement, sensorDescription, obsPropDescription);
}

bool SensorThings::setLocation(char* lat, char* lon){
    
    disconnectMqtt();
    
    /** Sending HTTP request to update Location **/
    int connection = nwClient->connect(restfulService, restfulPort);
    if (connection == 1) {  // if connection to the server is established
                
        client_print_P(header01);
        nwClient->print(F("Things("));
        nwClient->print(thingId);
        nwClient->print(F(")/Locations"));
        client_print_P(header02);
        nwClient->print(restfulService);
        client_print_P(header03);
        nwClient->print(120+strlen(lat)+strlen(lon));
        //nwClient->print(F("\n\n"));
        client_print_P(newline);
        
        nwClient->print(F("{\"description\":\"auto-generate\",\"location\":{\"type\":\"Point\",\"coordinates\":[")); 
        nwClient->print(lon);
        nwClient->print(F(","));
        nwClient->print(lat);
        nwClient->print(F("]},\"encodingType\": \"application/vnd.geo+json\"}"));
        nwClient->stop();
        
        return true;
    } else {  // if connection to the server is NOT established
        Serial.println(F("Connection failed, cannot update Location"));
        return false;
    }
}


void SensorThings::client_print_P(const char *str) {
    char c;
    if(!str) return;
    while((c = pgm_read_byte_near(str++))){
        nwClient->write(c);
    }
}
