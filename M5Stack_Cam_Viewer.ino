#define SSID_NAME "M5Cam"
#define SSID_PASSWORD ""
#define URL "http://192.168.4.1/jpg"
#include <M5Stack.h>
#include <WiFi.h>
#include <HTTPClient.h>
void setup() {
  M5.begin();
  M5.Lcd.setBrightness(255);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("Video Live Stream");
  M5.Lcd.setCursor(3, 35);
  M5.Lcd.println("Start by press BTN B >500ms");
  M5.Lcd.setTextColor(RED);
  while(-1) {
	M5.update();
	if (M5.BtnA.wasReleased()) {
	    M5.Lcd.print('A');
	  } else if (M5.BtnB.wasReleased()) {
	    M5.Lcd.print('B');
	  } else if (M5.BtnC.wasReleased()) {
	    M5.Lcd.print('C');
	  } else if (M5.BtnB.wasReleasefor(500)) {
	    M5.Lcd.clear(BLACK);
	    M5.Lcd.setCursor(0, 0);
	    break;
	  }
	}
}

void loop() {
  // wait for the video connection
  while (WiFi.status() != WL_CONNECTED);
  // create a http client
  HTTPClient http;
  http.begin(URL);
  int httpCode = http.GET();
  // HTTP header has been send and Server response header has been handled
  if (httpCode <= 0) {
    } else {
      if (httpCode != HTTP_CODE_OK) {
      } else {
        // get lenght of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();
        if (len <= 0) {
        } else {
          int size = len;
          uint8_t *buff;
          buff = (uint8_t *) malloc(size *sizeof(uint8_t));
          // get tcp stream
          WiFiClient * stream = http.getStreamPtr();
          // read all data from server
          uint8_t* p = buff;
          int l = len;
          while (http.connected() && (l > 0 || len == -1)) {
            // get available data size
            size_t size = stream->available();
            if (size) {
              int s = ((size > sizeof(buff)) ? sizeof(buff) : size);
              int c = stream->readBytes(p, s);
              p += c;
              if (l > 0) {
                l -= c;
              }
            }
          }
          M5.Lcd.drawJpg(buff, len);
          free(buff);
        }
      }
    }
    http.end();
}
