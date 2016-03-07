# ASS - Ambient Sport Spinner
Project developed as a gift for a former colleagues 40th birtday.

## Description
The client. Witch is basicaly an RGB backlit, six frames mechanical stop motion animation powered
by a stepper, subscribes to a topic on an mqtt broker. When someone clicks one off the buttons in
the front end a JSON-encoded message gets publiched to the subscribed topic and the client(s)
starts spinning according to the contents of said message.

## Overview
The project consists of 3 distinct folders / sub projects. 
* ass-client
  * C++ code running on the physical client.
    Install the esp8266 addon for Arduino IDE to develop and upload. 
* ass-hardware
  * 3D models in Rhino and printer friendly STL format.
* ass-web
  * Simple Meteor full stack project. 
