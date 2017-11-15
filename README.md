# Eclipse Whiskers for Arduino

Whiskers for Arduino is a library for connecting Arduino devices to [OGC SensorThings API](https://en.wikipedia.org/wiki/SensorThings_API). The SensorThings API is an OGC (Open Geospatial Consortium) standard that allows IoT (Internet of Things) devices and their data to be connected; a major goal is to foster a healthy and open IoT ecosystem, as opposed to one dominated by proprietary information silos.


## Using Eclipse Whiskers for Ardunio

Eclipse Whiskers for Arduino is using two other Arduino libraries:
* [MediaTek LWiFi](https://github.com/brucetsao/techbang/tree/master/201511/LinkIt-ONE-IDE/hardware/arduino/mtk/libraries/LWiFi)
* [PubSubClient](https://github.com/knolleary/pubsubclient)
Note that you need to add these libraries for Whiskers for Arduino to work.

The library has methods for connecting to a SensorThings server as well as SensorThings MQTT broker.
After connecting to the server you can use the library method to create SensorThings entities and start uploading Observations.
Example folder contains example usage of the library.