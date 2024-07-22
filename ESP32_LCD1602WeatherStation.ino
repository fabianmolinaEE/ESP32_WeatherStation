#include <Dictionary.h>
#include <DictionaryDeclarations.h>

#include <LiquidCrystal.h>
#include <Wire.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <HTTPClient.h>

#include <WiFi.h>


//Establish Wifi SSID and Password
const char* ssid = "YOUR_WIFI"; //choose your wireless ssid
const char* password =  "YOUR_SSID"; //put your wireless password here.
 
//Establish Variables For API Url
String apiURL = "https://api.openweathermap.org/data/3.0/onecall?";
String latitude;
String longitude;
String city; 
String apiKEY = "YOUR_API"; 

//Variables to keep track of each menu display (both the city selection and weather statistics)
int menu = 1; 
int data = 1; 

//Variables for Inputs and Outputs 
LiquidCrystal lcd(15, 4, 5, 18, 19, 21);
int upButton = 25;
int downButton = 33;
int selectButton = 32;

//Boolean Variables To Know State of Menus
bool inWeatherStats = false; 
bool exitButton = false; 

//Dictionary to Keep Weather Stats in For easier Printing. 
Dictionary &result = *(new Dictionary(5));



void setup() {
  lcd.begin(16,2);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  lcd.print("Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("Loading Weather...");
  Serial.begin(9600);
  delay(1000);

  //Connect WiFi 
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  //Updates to menu screen after connecting to WiFi.
  menuScreen();
}


void loop() {
  if (!inWeatherStats) {
    if (!digitalRead(downButton)){
    menu++;
    menuScreen();
    delay(100);
    while (!digitalRead(downButton));
   }
    if (!digitalRead(upButton)){
      menu--;
      menuScreen();
      delay(100);
      while(!digitalRead(upButton));
    }
    if (!digitalRead(selectButton)){
      inWeatherStats = true; 
      retrieveData(latitude, longitude);
      weatherStats();
      delay(100);
      while (!digitalRead(selectButton));
    }
  }
  else{
    if (!digitalRead(downButton)){
      data++;
      weatherStats();
      delay(100);
      while (!digitalRead(downButton));
    }
    if (!digitalRead(upButton)){
      data--;
      weatherStats();
      delay(100);
      while(!digitalRead(upButton));
    }
    if (!digitalRead(selectButton) && exitButton){
      data = 1;
      while ( result.count() ) result.remove(result(0));
      menuScreen();
      inWeatherStats = false;
      delay(100);
      while (!digitalRead(selectButton));
    }    
  }
}



void retrieveData(String lat, String lon){
    
  if (WiFi.status() == WL_CONNECTED){
    //Make reuquest to API for Specific City Selected
    HTTPClient client; 
    client.begin(apiURL + latitude + longitude + "&units=imperial&appid=" + apiKEY);
    int httpCode = client.GET();

    if(httpCode > 0){

      //Access the json data
      String JSON_Data = client.getString();

      //Deserialize the Json data to access elements
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      //Store the Weather information
      const char* description = obj["current"]["weather"][0]["description"].as<const char*>();
      const float temp = obj["current"]["temp"].as<float>();
      const float humidity = obj["current"]["humidity"].as<float>();
      const float wind = obj["current"]["wind_speed"].as<float>();
      const float uvi = obj["current"]["uvi"].as<float>();

      //Print them In Serial Monitor
      Serial.println("Here is the weather in Miami:");
      Serial.println("Description:" + String(description));
      Serial.println("Temperature:" + String(temp));
      Serial.println("Humidity:" + String(humidity));
      Serial.println("Wind Speed:" + String(wind));
      Serial.println("UV Index:" + String(uvi));

      //Store the Weather Statistics just gathered in result dictionary. 
      result("Description", String(description));
      result("Temp", String(temp));
      result("Humidity", String(humidity));
      result("Wind", String(wind));
      result("Uvi", String(uvi));
      }

      client.end();
    
  }
  else{
    lcd.clear();
    lcd.print("Lost Connection.");
    delay(30000);
  }
}

void menuScreen(){ //This function Updates the City Selection Menu Screen//
  switch(menu){
    //Keeps track of menu variable and updates screen according to value of it. 
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Hialeah, FL");
      lcd.setCursor(0, 1);
      lcd.print(" Miami, FL");
      latitude = "lat=25.857595";
      longitude = "&lon=-80.278107";
      city = "Current Weather in Hialeah :)";
      break;
    case 2:
      lcd.clear();
      lcd.print(" Hialeah, FL");
      lcd.setCursor(0, 1);
      lcd.print(">Miami, FL");
      latitude = "lat=25.761681";
      longitude = "&lon=-80.191788";
      city = "Current Weather in Miami :)";
      break;
    case 3:
      lcd.clear();
      lcd.print(">San Francisco, CA");
      lcd.setCursor(0, 1);
      lcd.print(" San Jose, CA");
      latitude = "lat=37.773972";
      longitude = "&lon=-122.431297";
      city = "Current Weather in San Francisco :)";
      break;
    case 4:
      lcd.clear();
      lcd.print(" San Francisco, CA");
      lcd.setCursor(0, 1);
      lcd.print(">San Jose, CA");
      latitude = "lat=37.335480";
      longitude = "&lon=-121.893028";
      city = "Current Weather in San Jose :)";
      break;
    case 5:
      menu = 4;
      break;
  }
}

void weatherStats(){ //This function updates the weather statistics screen. Same for all cities. Only changes City display at the top and recorded values. 
  switch(data){
    case 0:
      data ++;
      break; 
    case 1:
      lcd.clear(); 
      lcd.print(city);
      lcd.setCursor(0, 1);
      lcd.print(">Desc: " + result[0]);
      break;
    case 2:
      lcd.clear(); 
      lcd.print("Desc " + result[0]);
      lcd.setCursor(0, 1);
      lcd.print(">Temp: " + result[1] + " F");
      break;
    case 3: 
      lcd.clear(); 
      lcd.print("Temp: " + result[1] + " F");
      lcd.setCursor(0, 1);
      lcd.print(">Humidity: " + result[2] + "%");
      break;
    case 4:
      lcd.clear(); 
      lcd.print("Humidity: " + result[2] + "%");
      lcd.setCursor(0, 1);
      lcd.print(">Wind: " + result[3] + "mi/h");
      break;
    case 5:
      lcd.clear(); 
      lcd.print("Wind: " + result[3] + "mi/h");
      lcd.setCursor(0, 1);
      lcd.print(">UV Index : " + result[4]);
      exitButton = false; 
      break;
    case 6: 
      lcd.clear(); 
      lcd.print("UV Index : " + result[4]);
      lcd.setCursor(0, 1);
      lcd.print(">Exit");
      exitButton = true;
      break;
  }
}






