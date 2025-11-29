# Temperature-Dashboard-ESP32-with-Node-red-and-Hive-MQ
We have some project from my campus laboratory and we get some logical function from node red how to decide whether the temp is cold, normal or hot
We used a plugins VS Code with Wokwi Simulator and Platform IO to simulate this project. File FINAL MOBCOM IoT Simulation.json please import to your Node Red Configuration, if your have "uknown ui_gauage" or etc, you must delete that node and manually add the node, please check my reference PDF Presention to have a overview with my project.

Note please : the MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS please change by your own "Overview", "Access Management" to get your Credentials "MQTT_USER" & "MQTT_PASS" 
and make sure you run in sequence:
1. Prepare all files are downloaded and ready to run
2. Get "Connected" with your Hive MQ Connection Details with your credentials
3. Please click "Deploy" in make sure there are'nt some errors or malfunctins
4. Run your wokwi simulator (i used from VS Code Plugins)
5. Check your HIVE MQ Cloud wheter have receive a JSON Data or not
6. Check your debug Node Red wheter hace receive a RAW Json Data or not
7. Please change a temp to prove that the changes that occur in wokwi will be changes on other platforms

Thanks 
Btw i used **library version** :
- [env:esp32doit-devkit-v1]
- platform = espressif32
- board = esp32doit-devkit-v1
- framework = arduino
- lib_deps = 
	1. marcoschwartz/LiquidCrystal_I2C@^1.1.4
	2. knolleary/PubSubClient@^2.8
	3. beegee-tokyo/DHT sensor library for ESPx@^1.19
	
**PlatformIO Project Configuration File**
Build options: build flags, source filter
Upload options: custom upload port, speed and extra flags
Library options: dependencies, extra library storages
Advanced options: extra scripting

Please visit documentation for the other options and examples
https://docs.platformio.org/page/projectconf.html
and other official documentation

