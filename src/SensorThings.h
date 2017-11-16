#ifndef SensorThings_h
#define SensorThings_h
#include <PubSubClient.h>


class SensorThings
{
  public:
	SensorThings();
	SensorThings(Client& _client, char* _server, char* _thingId, char* _datastreamId);
	SensorThings(Client& _client, char* _server, int _port, char* _thingId, char* _datastreamId);
	SensorThings(Client& _client, char* _server, int _port, char* _datastreamId);
	SensorThings(Client& _client, char* _restfulService, int _restfulPort, char* _server, int _port);
	SensorThings(Client& _client, char* _restfulService, char* _server, int _port);
	SensorThings(Client& _client, char* _restfulService, int _restfulPort, char* _server);
	SensorThings(Client& _client, char* _restfulService, char* _server);
	void sendObservation(char* result);
	void sendObservation(char* result, char* lat, char* lon);
	void sendObservation(char* result, char* _datastreamId);
	void sendObservation(char* result, char* lat, char* lon, char* _datastreamId);
	void createThing(char* thingDescription);
	void createDatastream(char* datastreamDescription,char* unitOfMeasurement, char* sensorDescription, char* obsPropDescription);
	void createDatastream(char* datastreamDescription,char* unitOfMeasurement, char* sensorDescription, char* obsPropDescription, char* _datastreamId);
	void createDatastream(char* datastreamDescription, char* observationType, char* uomName, char* uomSymbol, char* uomDefinition, 
						  char* sensorDescription, char* sensorEncodingType, char* sensorMetadata, 
						  char* obsPropDescription, char* obsPropDefinition, char* obsPropName, char* _datastreamId);
	void createEntities(char* thingDescription, char* datastreamDescription,char* unitOfMeasurement, char* sensorDescription, char* obsPropDescription);
	bool setLocation(char* lat, char* lon);
private:
	char* server;
	char* restfulService;
	int port;
	int restfulPort;
	char thingId[10];
	Client* nwClient;
	PubSubClient client;
	char datastreamId[10];
	void reconnectMqtt() ;
	void disconnectMqtt() ;
	void client_print_P(const char *str);
};

#endif