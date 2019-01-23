# GPIO Expander with DHT22 (temp and humidity) feedback
By: Marshall Wingerson
For: xCraft, Coeur D'alene, ID, 83814

# Arduino Uno Pinout
 Arduino 0 - not connected
 Arduino 1 - not connected
 Arduino 2 - DHT22 data line
 Arduino 3 - GPIO 3
 Arduino 4 - GPIO 4
 Arduino 5 - GPIO 5
 Arduino 6 - GPIO 6
 Arduino 7 - GPIO 7

# Note GPIO pin numbers match Arduino numbers

# Serial Settings
Baud: 115200
data bits: 8
parity: none
stop bit: one

# Supported Commands:
set 3  clr 3  stat 3
set 4  clr 4  stat 4
set 5  clr 5  stat 5
set 6  clr 6  stat 6
set 7  clr 7  stat 7
humid 

'set x' commands set the GPIO channel(x) to HIGH
'clr x' commands set the GPIO channel(x) to LOW
'stat x' commands return '1' if the channel is set to HIGH
			or returns '0' if the channel is set to LOW
'humid' returns the temperature in fahrenheit and humidity
			in percent in floating point numbers





