# Eclipse Whiskers for Arduino

Whiskers for Arduino is a library for connecting Arduino devices to [OGC SensorThings API](https://en.wikipedia.org/wiki/SensorThings_API). The SensorThings API is an OGC (Open Geospatial Consortium) standard that allows IoT (Internet of Things) devices and their data to be connected; a major goal is to foster a healthy and open IoT ecosystem, as opposed to one dominated by proprietary information silos.

*Thanks to the financial support of Natural Resources Canada [GeoConnections](http://www.nrcan.gc.ca/earth-sciences/geomatics/canadas-spatial-data-infrastructure/10783)*.


## Using Eclipse Whiskers for Ardunio

Eclipse Whiskers for Arduino is using one other Arduino library for MQTT connections:
* [PubSubClient](https://github.com/knolleary/pubsubclient)

Note that you need to add this library for Whiskers for Arduino to work.

The library has methods for connecting to a SensorThings server as well as SensorThings MQTT broker.
After connecting to the server, you can use the library method to create SensorThings entities and start uploading Observations.

Example folder contains example usage of the library. The example code provided is for [Linkit One](https://www.seeedstudio.com/LinkIt-ONE-p-2017.html) and for internet connection it is using [MediaTek LWiFi](https://github.com/brucetsao/techbang/tree/master/201511/LinkIt-ONE-IDE/hardware/arduino/mtk/libraries/LWiFi) Arduino library.

There are two main approach to use the library:
* You create all the required entities, and use the device only for sending Observations.
* The device is plug-and-play which means it creates all the required entities and then send Observations.

In following two Sections, we explain the steps for using the library for each of these approaches.

### Plug-and-Play Device

* Connect your device to internet. SensorThings library accepts a "Client" connected to internet as one of the parameters.
* Create a SensorThings instance by passing the "Client", SensorThings server address, port used for HTTP requests, SensorThings MQTT broker address, and port used for MQTT.
* Use createEntities method of the SensorThings instance to create all the required entities. The parameters required for this method are  Thing description, Datastream description, unitOfMeasurement name, Sensor description and ObservedProperty description. This method is simplified for device maker and because of that the library generate all the other entity properties based on these parameters. User can update these properties at any time with sending HTTP request to the SensorThings server.
* Use sendObservation method of the SensorThings instance to send the Observations. The parameters are result, latitude, and longitude. If your device is not moving, after the first sendObservation request, you can use sendObservation only with result parameter.

### Sending Observations only

* Create Thing, Location, Datastream, Sensor, and ObservedProperty in the SensorThings server. Keep note of the @iot.id for the Datastream that you created.
* Connect your device to internet. SensorThings library accepts a "Client" connected to internet as one of the parameters.
* Create a SensorThings instance by passing the "Client", SensorThings MQTT broker address, port used for MQTT, and @iot.id for Datastream.
* Use sendObservation method of the SensorThings instance with result parameter to send the Observations.
