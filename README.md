# Tiny Power Consumption Meter

- Max: 20V supply
- 0.1mA resolution
- Max: 400mA load (1A or 2A also possible with lower resolution)
- Measure frequency 70Hz


Displays:

- mA and mW
- average mA and mW since start
- mAh and mWh since start
- Load V
- Measure frequency
- Run time

<img src=/pics/20191018_121937.jpg>

# Parts needed: 

- Arduino Nano - ATmega328P
- Adafruit Feather INA219
- Display 0.91 inch 128x32 i2c SSD1306

Optional:
- 1A Schottky diode
- 100uF 30V (connect parallell to load)
  
# 3D print housing: 
  
https://www.thingiverse.com/thing:3922847  
  
<img src=/pics/20191018_113226.jpg>

# Hooking it up

## __Running meter and also load from USB power only:__
  
| Display . . . . 	| Arduino . . . . 	| INA219 . . . . 	| LOAD   	|
|-----------------	|-----------------	|----------------	|--------	|
| VCC             	| 3.3V            	| 3.3V           	|        	|
| SCL             	| SCL             	| SCL            	|        	|
| SDA             	| SDA             	| SDA            	|        	|
|                 	| D2              	| RST (button)   	|        	|
| VCC             	| 3.3V            	| 3.3V           	|        	|
|                 	| 5V              	| +              	|        	|
|                 	|                 	| -              	| +4.7V 	|
| GND             	| GND             	| GND            	| GND    	|
  
.  
## __Running meter and load from external power supply 7-20V:__
  
| Display . . . . . 	| Arduino . . . . . 	| INA219 . . . . . 	| Power supply . . .	| LOAD    	|
|-----------------	|-----------------	|----------------	|--------------------------	|---------	|
| VCC             	| 3.3V            	| 3.3V           	|                          	|         	|
| SCL             	| SCL             	| SCL            	|                          	|         	|
| SDA             	| SDA             	| SDA            	|                          	|         	|
|                 	| D2              	| RST (button)   	|                          	|         	|
| VCC             	| 3.3V            	| 3.3V           	|                          	|         	|
|                    	| VIN             	| +              	| +7-20V                  	|         	|
|                 	|                 	| -              	|                          	| +7-20V 	|
| GND             	| GND             	| GND            	| GND                      	| GND     	|
  
__Optionally__ you can connect a Schottky diode from the Arduino boards USB connector 5V pin to the Arduino boards VIN pin. Then you can connect power to either the USB connector __OR__ an external power supply to the VIN pin without rewiring anything.

# Logging

All data is printed over the USB port

<img src=/pics/Untitled.png>

<img src=/pics/panel.png>
