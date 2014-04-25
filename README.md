votabrasil
==========

Twitter Voting Machine Display



#Install and run


#Twitter
* you need to create and application in https://apps.twitter.com. Once you had done this, copy the credencial's informations (consumer key and secret) and generate your access_token and secret too.

#Node.js
* install Node.js [tutorial]
* rename the file config.json.sample to config.json and put your keys.
* also in config.json, change the main filter (using hashtags) and the keywords you need to use (they have to be hashtags too, but do not include "#").
* rum "npm install" to get all modules (ntwitter and serialport).
* rum "npm start" to run this app.


#Arduino
* just copy the files inside the folder library in arduino/libraries to your library folder (documents/Arduino/libraries)
* open the project and check if it compile
* uplaod the project to your arduino
* open serial monitor (baudrate: 9600) and type "rainbow". Make shure "new line" is selected.



