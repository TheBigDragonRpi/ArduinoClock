#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
#include <FS.h>

AsyncWebServer server(80);


const char* PARAM_COLOR = "inputColor";
const char* PARAM_THEME = "inputTheme";
const char* PARAM_SONG = "inputSong";
const char* PARAM_TIME_START = "inputTimeStart";
const char* PARAM_TIME_END = "inputTimeEnd";

String readFile(const char * path){
  //Serial.printf("Reading file: %s\r\n", path);
  File file = SPIFFS.open(path, "r");
  if(!file){
    //Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  //Serial.println(fileContent);
  return fileContent;
}

void writeFile(const char * path, char const  * message){
  //Serial.printf("Writing file: %s\r\n", path);
  File file = SPIFFS.open(path, "w");
  if(!file){
    //Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    //Serial.println("- file written");
  } else {
    //Serial.println("- write failed");
  }
  file.close();
}
void insertFile(const char * path,const char * data){
  Serial.printf("Writing file: %s\r\n", path);
  File file = SPIFFS.open(path, "a");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  file.print("<br>");
  if(file.print(data)){
    Serial.println("-file written");
  }else{
    Serial.println("- write failed");
  }
  file.close();
}

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1"><script>function submitMessage(){alert("Saved value to ESP SPIFFS"); setTimeout(function(){document.location.reload(false);}, 500);}</script><title>JoJo</title></head><body><style>header{text-align: center;}body{font-family: system-ui;font-size: 20px;font-weight: 600;}#color{margin-left: 18px;}#b,body{margin-left: 20px;}#a{margin-left: 32px}#alarms{text-align: center}#backTo{position : fixed; right:50px; background-color: #2B4970; color: #FFFFFF; padding: 14px 18px; text-align: center; text-decoration: none; font-size: 16px; margin-left: 20px; opacity: 0.9; border-radius:50%}.top{position : fixed; bottom: 50px;}.bottom{position : fixed; top: 50px;}</style> <a id="top"></a><header><h1 id="titre" style="text-decoration:underline;">LEDs</h1></header><br><div id="LED"><form action="/get" target="hidden-form"> Color (current value %inputColor%):<br><input type="color"id="fname"value="#FF9938"style="border-color:transparent;"name="inputColor"><label for="fname"id="color">Color</label><br><input type="submit" value="Submit" onclick="submitMessage()"></form><br><br><form action="/get" target="hidden-form">Theme (current value %inputTheme%):<br><input type="radio"id="b"name="inputTheme"value="Full"checked><label for="JoJo"id="a">Full</label><br><input type="radio"id="b"name="inputTheme"value="Rainbow"><label for="gravity"id="a">Rainbow</label><br><input type="radio"id="b"name="inputTheme"value="Basic"><label for="none"id="a">Basic</label><br><br><input type="radio"id="b"name="inputTheme"value="Off"><label for="none"id="a">OFF</label><br><input type="submit" value="Submit" onclick="submitMessage()"></form></div><br><br><header> <h1 id="titre"style="text-decoration:underline;">Alarm</h1></header><br><div id="SONG"> <form action="/get" target="hidden-form"> Song (current value %inputSong%):<br><input type="radio"id="b"name="inputSong"value="JoJo"checked><label for="JoJo"id="a">JoJo</label><br><input type="radio"id="b"name="inputSong"value="Gravity"><label for="gravity"id="a">Gravity Falls</label><br><br><input type="radio"id="b"name="inputSong"value="None"><label for="gravity"id="a">None</label><br><br><input type="radio"id="b"name="inputSong"value="Off"><label for="none"id="a">OFF</label><br><input type="submit" value="Submit" onclick="submitMessage()"> </form></div><br><div id="b"> <form action="/get" target="hidden-form"> On from <input type="time"id="appt"name="inputTimeStart"required> to <input type="time"id="appt"name="inputTimeEnd"required> (leave blank for no automatic alarm presets)<br>(current : %inputTimeStart% - %inputTimeEnd%)<br><input type="submit" value="Submit" onclick="submitMessage()"> </form></div></div><iframe style="display:none" name="hidden-form"></iframe><br><br><hr><br><br><div id="alarms"> <span id="alarms">%alarms%</span><br></div><br><a id="backTo"class="top"href="#top">/\</a> <a id="bot"></a><a id="backTo"class="bottom"href="#bot">\/</a><br></body><script>setInterval(function ( ){var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function(){if (this.readyState==4 && this.status==200){document.getElementById("alarms").innerHTML=this.responseText;}}; xhttp.open("GET", "/alarms", true); xhttp.send();}, 10000) ;</script></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "inputColor"){
    return readFile("/color.txt");
  }
  else if(var == "inputTheme"){
    return readFile("/theme.txt");
  }
  else if(var == "inputSong"){
    return readFile("/song.txt");
  }
  else if(var == "inputTimeStart"){
    return readFile("/start.txt");
  }
  else if(var == "inputTimeEnd"){
    return readFile("/end.txt");
  }
  else if(var == "alarms"){
    return readFile("/alarms.txt");
  }
  return String();
}

void setup() {
  Serial.begin(9600);
  
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("Obi-Wan Kenobi","hello there") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  SPIFFS.begin();
  
  //Serial.println(readFile("/color.txt"));
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    
    if (request->hasParam(PARAM_TIME_END)) {
      inputMessage = request->getParam(PARAM_TIME_END)->value();
      writeFile("/end.txt", inputMessage.c_str());
      Serial.print("stop : "+ String(inputMessage.c_str()));
    }
    if (request->hasParam(PARAM_COLOR)) {
      inputMessage = request->getParam(PARAM_COLOR)->value();
      writeFile("/color.txt", inputMessage.c_str());
      Serial.print("color : "+ String(inputMessage.c_str()));
    }
    else if (request->hasParam(PARAM_THEME)) {
      inputMessage = request->getParam(PARAM_THEME)->value();
      writeFile("/theme.txt", inputMessage.c_str());
      Serial.print("theme : "+ String(inputMessage.c_str()));
    }
    else if (request->hasParam(PARAM_SONG)) {
      inputMessage = request->getParam(PARAM_SONG)->value();
      writeFile("/song.txt", inputMessage.c_str());
      Serial.print("song : "+ String(inputMessage.c_str()));
    }
    else if (request->hasParam(PARAM_TIME_START)) {
      inputMessage = request->getParam(PARAM_TIME_START)->value();
      writeFile("/start.txt", inputMessage.c_str());
      Serial.println("start : "+ String(inputMessage.c_str()));
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/text", inputMessage);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  String incomingString = "";
  bool stringReady = false;
  if(Serial.available()){
    incomingString = Serial.readString();
    if(incomingString.substring (0,6) == "#EVENT"){
      Serial.println("New alarm : "+incomingString.substring(9));
      insertFile("/alarms.txt", incomingString.substring(9).c_str());
    }
  }
}
