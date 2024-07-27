#include <Dictionary.h>
#include <DictionaryDeclarations.h>

#include <Wire.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SPI.h>

#include <HTTPClient.h>

#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSerifBoldItalic12pt7b.h>

#include <WiFi.h>


//Establish Wifi SSID and Password
const char* ssid = "YOUR_WIFI_HERE";      //choose your wireless ssid
const char* password = "PASSWORD";  //put your wireless password here.

//Establish Variables For API Url
String apiURL = "https://api.openweathermap.org/data/3.0/onecall?";
String city;
String apiKEY = "YOURAPIHERE";

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 128  // OLED display height, in pixels
#define OLED_RESET -1      // can set an oled reset pin if desired

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

//Variables to keep track of each menu display and welcome display
int menu = 1;
int data = 1;
int connection = 0;

//Variables for Inputs and Outputs
int upButton = 4;
int downButton = 19;
int selectButton = 18;

//Boolean Variables To Know State of Menus
bool inWeatherStats = false;
bool exitButton = false;

//Dictionary to Keep Weather Stats in For easier Printing.
Dictionary& result = *(new Dictionary(5));
Dictionary& Cities = *(new Dictionary(5));



void setup() {
  Serial.begin(9600);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  display.begin(0x3C, true);
  DisplayWelcome();


  //Connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    connection++;
    DisplayWelcome();
    delay(500);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  Cities("Miami, FL", "lat=25.761681&lon=-80.191788");
  Cities("Hialeah, FL", "lat=25.857595&lon=-80.278107");
  Cities("San Francisco, CA", "lat=37.773972&lon=-122.431297");
  Cities("San Jose, CA", "lat=37.335480&lon=-121.893028");
  //Updates to menu screen after connecting to WiFi.
  menuScreen();
}


void loop() {
  if (!inWeatherStats) {
    if (!digitalRead(downButton)) {
      menu++;
      menuScreen();
      delay(100);
      while (!digitalRead(downButton));
    }
    if (!digitalRead(upButton)) {
      menu--;
      menuScreen();
      delay(100);
      while (!digitalRead(upButton));
    }
    if (!digitalRead(selectButton)) {
      inWeatherStats = true;
      while (result.count()) result.remove(result(0));
      display.clearDisplay();
      display.setCursor(0, 64);
      display.setTextSize(2);
      display.println("Loading...");
      display.display();
      retrieveData();
      weatherStats();
      delay(100);
      while (!digitalRead(selectButton));
    }
  } 
  else {
    if (!digitalRead(downButton)) {
      data++;
      weatherStats();
      delay(100);
      while (!digitalRead(downButton));
    }
    if (!digitalRead(upButton)) {
      data--;
      weatherStats();
      delay(100);
      while (!digitalRead(upButton));
    }
    if (!digitalRead(selectButton) && exitButton) {
      data = 1;
      while (result.count()) result.remove(result(0));
      menuScreen();
      inWeatherStats = false;
      delay(100);
      while (!digitalRead(selectButton));
    }
  }

}



void retrieveData(){

  if (WiFi.status() == WL_CONNECTED){
    //Make reuquest to API for Specific City Selected
    HTTPClient client;
    client.begin(apiURL + Cities[city] + "&units=imperial&appid=" + apiKEY);
    int httpCode = client.GET();

    if(httpCode > 0){

      //Access the json data
      String JSON_Data = client.getString();
      //Deserialize the Json data to access elements
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      // //Store the Weather information
      const char* description = obj["current"]["weather"][0]["description"].as<const char*>();
      const float temp = obj["current"]["temp"].as<float>();
      const float humidity = obj["current"]["humidity"].as<float>();
      const float wind = obj["current"]["wind_speed"].as<float>();
      const float uvi = obj["current"]["uvi"].as<float>();

      //Print them In Serial Monitor

      //Store the Weather Statistics just gathered in result dictionary. 
      result("Description", String(description));
      result("Temperature: ", String(temp));
      result("Humidity: ", String(humidity));
      result("Wind:", String(wind));
      result("UV Index: ", String(uvi));
      }

      client.end();

  }
  else{
    display.clearDisplay();
    display.println("Lost Connection.");
    display.display();
    delay(30000);
  }
}

void menuScreen() {  //This function Updates the City Selection Menu Screen//
  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(1);
  display.println("Make a Selection");
  display.drawLine(0, 10, 128, 10, SH110X_WHITE);
  int xPos = 15;
  int yPos = 15;
  for (int i = 0; i < Cities.count(); i++) {
    display.setCursor(xPos, yPos);
    yPos += 10;
    display.println(Cities(i));
  }
  xPos = 0;
  switch (menu) {
    case 0:
      menu = 4;
      yPos = 45;
      display.setCursor(3, yPos);
      display.println("->");
      display.drawRect(xPos, yPos, 120, 10, SH110X_WHITE);
      city = Cities(3);
      break;
    case 1:
      yPos = 15;
      display.setCursor(3, yPos);
      display.println("->");
      display.drawRect(xPos, yPos, 120, 10, SH110X_WHITE);
      city = Cities(0);
      break;
    case 2:
      yPos = 25;
      display.setCursor(3, yPos);
      display.println("->");
      display.drawRect(xPos, yPos, 120, 10, SH110X_WHITE);
      city = Cities(1);
      break;
    case 3:
      yPos = 35;
      display.setCursor(3, yPos);
      display.println("->");
      display.drawRect(xPos, yPos, 120, 10, SH110X_WHITE);
      city = Cities(2);
      break;
    case 4:
      yPos = 45;
      display.setCursor(3, yPos);
      display.println("->");
      display.drawRect(xPos, yPos, 120, 10, SH110X_WHITE);
      city = Cities(3);
      break;
    case 5:
      menu = 1;
      yPos = 15;
      display.setCursor(3, yPos);
      display.println("->");
      display.drawRect(xPos, yPos, 120, 10, SH110X_WHITE);
      city = Cities(0);
      break;
  }
  display.display();
}


void weatherStats() {  //This function updates the weather statistics screen. Same for all cities. Only changes City display at the top and recorded values.
  int xPos = 15; 
  int yPos = 20;
  display.clearDisplay(); 
  display.setCursor(10, 0);
  // display.setFont(&FreeSerifBoldItalic12pt7b);
  display.setTextSize(1);
  display.println("Current Weather in ");
  display.setCursor(40, 10);
  display.println("Miami");
  display.drawLine(0, 17, 128, 17, SH110X_WHITE);
  // display.setFont(&FreeMono12pt7b);
  // display.setTextSize(1);
  for(int i = 0; i < result.count(); i++){
    display.setCursor(xPos,yPos);
    display.println(result(i));
    display.setCursor(xPos + 15, yPos + 10);
    display.println(result[i]);
    yPos+=20; 
  }
  display.setCursor(xPos, yPos);
  display.println("Exit");
  switch (data) {
    case 0:
      data = 6;
      yPos = 120; 
      display.setCursor(3,yPos);
      display.setTextSize(1);
      display.println("->");
      exitButton = true;
      break;
    case 1:
      display.setTextSize(1);
      yPos = 20;
      display.setCursor(3, yPos);
      display.println("->");
      break;
    case 2:
      yPos = 40; 
      display.setCursor(3, yPos);
      display.setTextSize(1);
      display.println("->");
      break;
    case 3:
      yPos = 60; 
      display.setCursor(3,yPos);
      display.setTextSize(1);
      display.println("->");
      break;
    case 4: 
      yPos = 80; 
      display.setCursor(3,yPos);
      display.setTextSize(1);
      display.println("->");
      break;
    case 5:
      exitButton = false;
      yPos = 100; 
      display.setCursor(3,yPos);
      display.setTextSize(1);
      display.println("->");
      break;
    case 6:
      yPos = 120; 
      display.setCursor(3,yPos);
      display.setTextSize(1);
      display.println("->");
      exitButton = true;
      break;
    case 7:
      data = 1;
      display.setTextSize(1);
      yPos = 20;
      display.setCursor(3, yPos);
      display.println("->");
      break;
  }
  display.display();
}

void DisplayWelcome() {
  display.clearDisplay();
  display.setTextSize(1.8);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(35, 0);
  display.println("Welcome!");
  display.setCursor(10, 15);
  display.println("WeatherStation 2.0");
  // drawAnimation(40, 40, RainyFrames);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(2, 100);

  switch (connection) {
    case 0:
      display.print("Connecting to Wifi.");
      break;
    case 1:
      display.print("Connecting to Wifi..");
      break;
    case 2:
      display.print("Connecting to Wifi...");
      break;
    case 3:
      display.print("Connecting to Wifi.");
      connection = 0;
      break;
  }
  display.display();
}


