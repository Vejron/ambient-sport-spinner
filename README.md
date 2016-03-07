# ASS - Ambient Sport Spinner
Project developed as a gift for a former colleagues 40th birthday.

## Description
The client. Witch is basically an RGB backlit, six frames mechanical stop motion animation powered
by a stepper, subscribes to a topic on a mqtt broker. When someone clicks one of the buttons in
the front end a JSON-encoded message gets published to the subscribed topic and the client(s)
starts spinning according to the contents of said message.

## Overview
The project consists of 3 distinct folders / sub projects. 
* ass-client
  * C++ code running on the physical client.
    Install the esp8266 add-on for Arduino IDE to develop and upload. 
* ass-hardware
  * 3D models in Rhino and printer friendly STL format.
* ass-web
  * Simple Meteor full-stack JS project. 
