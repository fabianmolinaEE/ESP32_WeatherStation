# ESP32_WeatherStation
## Description ##
This is intended to be a weather station that pulls data for the weather of a region from an open source API(OpenWeatherMap API). This uses an ESP32, LCD1602, and tactile switch buttons(to navigate the menu and make selections).The following shows the layout for displaying the weather for certain regions using a selection menu on the LCD1602. The LCD1602 is intended to output various City selections of which you can scroll through. Weather statistics are found by storing a city's latitude and longitude which are then used in the OpenWeatherMap API request. The desired weather qualities are pulled from the API request which then go into a dictionary used to display statistics after the city has been selected. The statistics are also shown in a menu through which you can scroll through. The exit button at the bottom of the statistics menu takes you back to the selection screen, displaying the cities. 
## Hardware Required ##
MicroController: [ESP32](https://www.google.com/aclk?sa=l&ai=DChcSEwi9sNKJ472HAxWYnVoFHVcLCzcYABAmGgJ2dQ&co=1&ase=2&gclid=CjwKCAjwqf20BhBwEiwAt7dtdSMa17L6wcsWtO2vI76B6ZHWl3spopFQYSUxy96fmpE7-wM9qyM6aRoCNiQQAvD_BwE&sig=AOD64_3JSN0waTJF7cB-ssieZWGqKf5t3A&ctype=5&q=&nis=4&ved=2ahUKEwjS6sqJ472HAxVKRjABHWjyBGsQ9aACKAB6BAgFEBU&adurl=) <br /> 
Display: [LCD1602](https://docs.arduino.cc/learn/electronics/lcd-displays/) or [OLED 128x128 SH1107](https://www.google.com/aclk?sa=l&ai=DChcSEwi3w5WO1MeHAxWHpVoFHR9dA8AYABAVGgJ2dQ&co=1&ase=2&gclid=Cj0KCQjwtZK1BhDuARIsAAy2VzsxdFVcsIYDX6cnk37Fd-a3NS5Te-WWkmNJGYS73cauWbyNpSs8Kd8aAucHEALw_wcB&sig=AOD64_2n-rCDMjfxUQ-tlRtYJfh8PnTc1Q&ctype=5&q=&nis=4&ved=2ahUKEwiQ6I6O1MeHAxVKQjABHbz4CKAQ9aACKAB6BAgGEBo&adurl=) <br /> 
Wires <br /> 
[Tactile Switch Buttons](https://www.google.com/aclk?sa=l&ai=DChcSEwjx7ciF5b2HAxVim1oFHYPAAYIYABAiGgJ2dQ&co=1&ase=2&gclid=CjwKCAjwqf20BhBwEiwAt7dtdVEgaqtpBaEtlGoJmhAlyMqF_AKAoMw4e_8df5D9cyOcbJxvcVAZwRoCGlsQAvD_BwE&sig=AOD64_23zwJMiIADuf5-CysWjZReDPxERQ&ctype=5&q=&nis=4&ved=2ahUKEwie0sGF5b2HAxXvTDABHZiPAjQQ9aACKAB6BAgKEBo&adurl=) <br />
[Potentiometer](https://www.google.com/aclk?sa=l&ai=DChcSEwiWjeGs5b2HAxXMmloFHcqoBYAYABAJGgJ2dQ&co=1&ase=2&gclid=CjwKCAjwqf20BhBwEiwAt7dtdZyvZRRJn5gTcIG8lfv2in1cQTEL3WnYOVR_IVcemIOIAygXbRYnzxoC6GEQAvD_BwE&sig=AOD64_04WVU9qAHu2qxc7cRw4hlXgAUIWQ&ctype=5&q=&nis=4&ved=2ahUKEwiq7tus5b2HAxWyTDABHW29D0IQ9aACKAB6BAgEECo&adurl=) (if your using LCD Display) <br />
## Circuit Diagram for LCD Display ##
Link to Circuit JPG and Fritzing: [here](https://github.com/fabianmolinaEE/ESP32_WeatherStation/tree/main/Circuit%20Diagrams) <br />
Input/Output Layout: <br />
| LCD Pin       | Connection    |
| ------------- | ------------- |
| 4             | ESP D15       |
| 6             | ESP D04       |
| 11            | ESP D05       |
| 12            | ESP D18       |
| 13            | ESP D19       |
| 14            | ESP D21       | <br />

| Button        | Connection    |
| ------------- | ------------- |
| UpButton      | ESP D25       |
| DownButton    | ESP D33       |
| SelectButton  | ESP D32       |

## Circuit Diagram for OLED Display ##
Link to Circuit JPG and Fritzing: [here](https://github.com/fabianmolinaEE/ESP32_WeatherStation/tree/main/Circuit%20Diagrams) <br />
| OLED Pin      | Connection    |
| ------------- | ------------- |
| SDA           | ESP D21       |
| SCL             | ESP D23       | <br />

| Button        | Connection    |
| ------------- | ------------- |
| UpButton      | ESP D4        |
| DownButton    | ESP D19       |
| SelectButton  | ESP D18       |

## Libraries and Usage ##
[Dictionary Library](https://github.com/arkhipenko/Dictionary): <br />
This library allowed me to store weather statistics in a dictionary data type. Stored in a key-value format. <br />
[HTTP Client Library:](https://github.com/arduino-libraries/ArduinoHttpClient) <br /> 
This library was mainly used to access the API and make requests. It allowed the retrieval of the weather statistics. <br />
[Arduino JSON:](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties) <br />
This library allowed for the handling of the data retrieved from the weather API. It is needed to access the desired elements. <br /> 
[Adafruit GFX Library: ] (https://github.com/adafruit/Adafruit-GFX-Library) <br />
This library helps to work with the oled display module <br />
[Adafruit_SH110X](https://github.com/adafruit/Adafruit_SH110X) <br />
This libary is a driver libary for the specific OLED display. Look for the specific driver libary for your respective OLED display. <br />
<br />
All other libaries (Wifi, Wire, and LiquidCrystal) are apart of the base arduino IDE. More info on them can be found [here](https://www.arduino.cc/reference/en/libraries/).

## API Usage ##
The API used can be found [here](https://openweathermap.org/api). <br />
It returns a Json when you request the url. <br />
You need an account and that requires linking a card, but the API subscription I used is free with 1000 calls per day. <br />
**Url format**: <br />
The capitalized sections below depend on what units you want, what your api key is and what location you want.
https://api.openweathermap.org/data/3.0/onecall?lat=LATITUDEGOESHERE&lon=LONGITUDEGOESHERE&units=DESIREDUNITS&appid=YOURAPIKEY <br />
See the link above for more info and help with the API. <br />

## OLED vs LCD ##
Within the code folder, there is both the arduino IDE code for displaying the weather for a city on either an LCD1602 or an OLED 128x128 SH1107 display. The OLED display has a much neater display and is my favorite. 

## Customizing Cities ##
To add more cities, you would need to find their respective longitude and latitude and add them to their respective variables. In the OLED code add each city and latitude and longitude to the Cities dictionary following the format provided. You would also need to add more cases to each screen update, following my format. 




