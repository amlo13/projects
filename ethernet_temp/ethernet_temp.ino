/*
 
 Forgefun.com
 Arduino Ethernet Weather Station
 Author: Calvin Leung Huang
 Created: 30 Jul 2012
 Last updated: 2 Aug 2012
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x0D, 0x83, 0xB0 }; //Make sure to change MAC address to that of your shield
IPAddress server(74,125,224,72); // Google
boolean holder = false;
boolean tagFlag = false;
boolean dataFlag = false;
boolean finish_parsing = false;
String temp = "";
String value = "";

EthernetClient client;

LiquidCrystal lcd(9, 8, 7, 6, 5, 3);

void setup() {
  //Serial.begin(9600);

  //Serial.println("FF Weather Station");
  //Serial.println("connecting...");
  
  lcd.begin(16,2);
  lcd.home();
  lcd.print("FF Weather Stn");
  lcd.setCursor(0,1);
  lcd.print("connecting...");

  Ethernet.begin(mac);
  delay(1000);

  if (client.connect(server, 80)) {
    lcd.clear();
    lcd.print("Connected!");
    //Serial.println("Connected!");
    client.println("GET /ig/api?weather=12180 HTTP/1.1"); //Make http request
    client.println();
  } 
  else {
    lcd.clear();
    lcd.print("connection failed");
    //Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available()) {
    parse_info();
  }

  if (!client.connected()) {
    lcd.clear();
    lcd.print("Disconnected");
    //Serial.println("Disconnected");
    client.stop();

    // Do nothing
    for(;;)
      ;
  }
}

void parse_info(){

  if (finish_parsing){
    print_data();
    clear_values();    
  }
  char c = client.read();

  if (c == '<'){
    tagFlag = true;
    return;
  }
  if (c == '>' || c == '/'){
    tagFlag = false;
    dataFlag = false;
    finish_parsing = true;
    return;
  }
  if (c == '='){
    dataFlag = true;
    return;
  }

  if (dataFlag){
    value += c;
    return;
  }

  if (tagFlag && !dataFlag){
    temp +=c;
    return;
  }


}

void print_data(){
  
  if (temp == "city data"){
    lcd.clear();
    lcd.print("City: " + value);
    //Serial.println("City: " + value);
  }
  /*
  if (temp == "current_date_time data"){
    Serial.println("Last updated: " + value);
  }
  */
  if (temp == "temp_f data"){
    lcd.setCursor(0,1);
    lcd.print("Degrees F: " + value);
    //Serial.println("Degrees F: " + value);
  }
  finish_parsing = false;
}

void clear_values(){
  temp = "";
  value = "";
}




