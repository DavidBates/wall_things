#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include "FastLED.h"
#include <JPEGDecoder.h>

#include "strangerThings.h"

#define NUM_LEDS 50 
#define DATA_PIN 9
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

char ssid[] = "YOUR_SSID";       // your network SSID (name)
char password[] = "YOUR_PASSWORD";

CRGB leds[NUM_LEDS];
uint8_t gHue = 0;
const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(3) + 330;

TFT_eSPI tft = TFT_eSPI();
//const char* json = "{\"user\":\"Will\",\"tweet\":\"@wall_things say:Run Mom Run\",\"chars\":[8, 5, 14]}";


void setup() {
  Serial.begin(115200);
//  Setup LEDS
  LEDS.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(100);

   tft.begin();
//   Draw Initial Loading Screen
   tft.setRotation(1);
   tft.fillScreen(TFT_BLACK);
   drawArrayJpeg(Stranger40, sizeof(Stranger40), 0, 0);
   tft.setCursor(47, 109, 2);
   tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(2);
   tft.println("Connecting to WIFI");

   //   Connect to wifi:
   WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

}

void loop() {
    if(gHue > 100){
    gHue = 1;
  }
  tft.setRotation(1);  // portrait
  // tft.fillScreen(random(0xFFFF));
  // drawArrayJpeg(Stranger40, sizeof(Stranger40), 0, 0);
  tft.drawRect(0,109, 360, 300, TFT_BLACK);
  tft.fillRect(0,109, 360, 300, TFT_BLACK);
  String json = getTweet();
  
  Serial.print("JSON DATA: ");
  Serial.println(json);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  delay(1000);
  Serial.println("Parsing JSON");
  JsonObject& root = jsonBuffer.parseObject(json.c_str());
  
   const char* user = root["user"]; // "Electrobro"
   const char* tweet = root["tweet"]; // "@wall_things say: Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu"
    
    tft.setCursor(47, 109, 2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);  
    tft.setTextSize(2);
    Serial.println(user);
    tft.println(user);
    tft.setTextSize(1);
    tft.println(tweet);
    Serial.println(tweet);
   
   JsonArray& myArray = root["chars"];
   delay(25);
   Serial.println("flash");
   setBlack();
   delay(500);
   setWhite();
   delay(500);
   setBlack();
   delay(1000);
   Serial.println("Preparing Message");
 for (auto myValue : myArray) {
   Serial.println(atoi(myValue.as<char*>()));
   int curChar = atoi(myValue.as<char*>());
   blinkLetter(curChar);
   delay(1000);
   } 
    delay(2000);
   for(int i = NUM_LEDS; i >= 0; i--) {
     leds[i] = CRGB::White;
     FastLED.show(); 
     delay(25);
   }
   setBlack();
   delay(2000);

  rainbow();
  FastLED.show();  
  FastLED.delay(1000/120); 
  delay(5000);
  gHue = gHue+ 10;
        
        
delay(5000);
}
void blinkLetter(int position){
  Serial.println("Blinking");
  leds[position] = CRGB::Red;
  FastLED.show();
  delay(500);
  leds[position] = CRGB::Black;
  FastLED.show();
  delay(500);
  leds[position] = CRGB::Red;
  FastLED.show();
  delay(500);
}
void setBlack(){
     for(int i = 0; i <= NUM_LEDS; i++) {
     leds[i] = CRGB::Black;
   }
     FastLED.show(); 
     delay(25);
}
void setWhite(){
     for(int i = 0; i <= NUM_LEDS; i++) {
     leds[i] = CRGB::White;
   }
     FastLED.show(); 
     delay(25);
}
String getTweet(){
     HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        http.begin("http://stranger-things-wall.us-east-1.elasticbeanstalk.com/"); //HTTP

        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.print("Tweet Received: ");
                Serial.println(payload);
                delay(500);
                http.end();
                return payload;
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        return "";
  }

//Helper Functions: 
void drawArrayJpeg(const uint8_t arrayname[], uint32_t array_size, int xpos, int ypos) {

  int x = xpos;
  int y = ypos;

  JpegDec.decodeArray(arrayname, array_size);
  
//  jpegInfo(); // Print information from the JPEG file (could comment this line out)
  
  renderJPEG(x, y);
  
  Serial.println("#########################");
}

void renderJPEG(int xpos, int ypos) {
  

  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while (JpegDec.read()) {
    
    // save a pointer to the image block
    pImg = JpegDec.pImage ;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
    {
      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setWindow(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);

      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) {
        // Push each pixel to the TFT MCU area
        tft.pushColor(*pImg++);
      }

    }
    else if ( (mcu_y + win_h) >= tft.height()) JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime;

  // print the results to the serial port
//  Serial.print(F(  "Total render time was    : ")); Serial.print(drawTime); Serial.println(F(" ms"));
//  Serial.println(F(""));
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}
