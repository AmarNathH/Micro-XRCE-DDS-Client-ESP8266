# Micro-XRCE-DDS-Client for ESP8266
The folder microxrce_dds can be downloaded and added to lib folder in Platform IO's project workspace. If you plan to test the examples then the files in examples has to be added to the src folder in the Platform IO's project workspace. This can be then compiled and then uploaded to the ESP8266 Board.

Tested for NODE MCU 0.9 (ESP-12E Module) using the PlatformIO VS Code Extension and the Micro XRCE DDS library version was 1.1.0. (Make sure to stick to these or please test it, improve the code and give a pull request :))

You can change the config for Micro XRCE DDS from the ```config.h``` file, the project uses the ```PLATFORM_NAME_ESP8266``` macro defination, to compile the functions and files necessary for the ESP8266.

**Iniital development of library done, UDP and Discovery functions are now implemented. Some bugs still exists, feel free to use the library and contribute :)**

**Known Issues** : 
There is a need to make a copy of the IP Address after Discovery, as there is a possible case of dangling pointer. Current workaround is by taking a copy of IP address by using ```strdup()``` function.
Discovery feature is not working properly.
