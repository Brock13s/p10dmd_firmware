#include <SPI.h>
#include <ESP8266WiFi.h>
#include "indexPage.h"
#include "wificonfig.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DMD2.h>
#include <fonts/Arial14.h>

#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15
#define btn 2

#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1
#define rows 32
#define columns 16

AsyncWebServer server(80);
SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk);
DMD_TextBox box(dmd, 0, 0, 32, 16);
bool pixelStates[rows][columns]={false};
unsigned long modeStartTime = 0;
String displayText = "Hello world";

enum DisplayMode{
  TEXT_MODE,
  ANIMATION_MODE,
  PIXEL_ART_MODE
};
DisplayMode currentMode = TEXT_MODE;

void wifiSetup(){
  Serial.println("Going into wifi configuration mode....");
  WiFi.softAP("DMDCONFIG", "configthatdmd666");
//  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/html", wifipageContent);
//  });
  while(true){
    
  }
}

void handleRequest(AsyncWebServerRequest *request){
  if(request->method()==HTTP_GET){
    request->send(200, "text/html", indexContent);
  } else if(request->method()==HTTP_POST){
    int row = request->arg("row").toInt();
    int column = request->arg("column").toInt();
    int state = request->arg("state").toInt();
    pixelStates[row][column] = (state != 0);
    DMDGraphicsMode mode1 = DMDGraphicsMode(state);
    request->send(200, "text/plain", "pixel toggled");
  }
  
}

void setup() {
  Serial.begin(115200);
  pinMode(btn, INPUT);
//  if(digitalRead(btn) == HIGH){
//    //wifiSetup();
//  }
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  WiFi.begin("BROCKSNETGEAR_AP", "icykayak861");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connected to wifi ");
  Serial.println(WiFi.localIP());
  dmd.selectFont(Arial14);
  dmd.begin();
  server.on("/", HTTP_ANY, handleRequest);
  server.on("/clearall", HTTP_POST, [](AsyncWebServerRequest *request){
        for(int row=0; row < 32; row++){
      for(int col=0; col < 16; col++){
        pixelStates[row][col] = 0;
      }
    }
    request->send(200, "text/plain" "Cleared all pixels");
    dmd.fillScreen(false);
  });
  server.on("/getButtonStates", HTTP_GET, [](AsyncWebServerRequest *request){
        String buttonStatesStr;
    for(int col=0; col < 16; col++){
      for(int row=0; row < 32; row++){
        buttonStatesStr += pixelStates[row][col] ? "1" : "0";
      }
    }
    Serial.print(buttonStatesStr);
    request->send(200, "text/plain", buttonStatesStr);
  });

  server.on("/text", HTTP_POST, [](AsyncWebServerRequest *request){
    displayText = request->arg("text");
    request->send(200, "text/plain", "text: " + displayText + " successfuly recived");
  });
  server.begin();

  
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(currentMode){
    case TEXT_MODE:
    if(millis() - modeStartTime >= 10000){
      currentMode = PIXEL_ART_MODE;
      modeStartTime = millis();
      dmd.fillScreen(false);
    } else{
      dmd.drawString(0, 0, displayText);      
    }
    break;

   case PIXEL_ART_MODE:
   if(millis() - modeStartTime >=10000){
    currentMode = TEXT_MODE;
    modeStartTime = millis();
    dmd.fillScreen(false);
   } else{
    
    for(int row=0; row < 32; row++){
      for(int col=0; col < 16; col++){
        DMDGraphicsMode mode1 = DMDGraphicsMode(pixelStates[row][col]);
        dmd.setPixel(row,col, mode1);
      }
    }
   }
   break;

   default:
     break;
  }
}
