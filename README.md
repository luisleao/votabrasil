votabrasil
==========

Twitter Voting Machine Display


This project is based on @lfcipriani Tutorial using Ruby and Twitter to get atendees feelings about presentations in QCon São Paulo 2014. I recreated the script to get atendee's votes and build a physical interface (using Arduino and NeoPixel led strip) to show the results.

Each atendee can have one vote, but they can change whenever they want.

I'm not using any database and only one presentation at time. You can create other version using Ethernet Shield and create a site to store the records and distribute between devices. Please fork this project and contribute if you can. :D

#List of materials
* 1 Arduino Uno (including usb cable)
* 1 NeoPixel RGB led strip (but you can use any neoPixel's product)
* 3 Cables and jumpers
* 1 computer runnig node.js


#Install and run

##Twitter
* you need to create and application in https://apps.twitter.com. Once you had done this, copy the credencial's informations (consumer key and secret) and generate your access_token and secret too.

##Node.js
* install Node.js [tutorial]
* rename the file config.json.sample to config.json and put your keys.
* also in config.json, change the main filter (using hashtags) and the keywords you need to use (they have to be hashtags too, but do not include "#").
* rum "npm install" to get all modules (ntwitter and serialport).
* rum "npm start" to run this app.


##Arduino
* just copy the files inside the folder library in arduino/libraries to your library folder (documents/Arduino/libraries)
* open the project and check if it compile
* uplaod the project to your arduino
* open serial monitor (baudrate: 9600) and type "rainbow". Make shure "new line" is selected.



