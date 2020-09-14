
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>

String FirmwareVer = "1.8"; // current firmware version.
const char* string1 = FirmwareVer.c_str();

#define URL_fw_Version "https://raw.githubusercontent.com/vadaliya/ota/master/version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/vadaliya/ota/master/firmware1.bin"
#define LED_BUILTIN 2

//#define URL_fw_Version "http://vadaliya2007.000webhostapp.com/atte/ota/version.txt"
//#define URL_fw_Bin "http://vadaliya2007.000webhostapp.com/atte/ota/firmware.bin"

HTTPClient http;

const char* ssid = "Honor8X"; // Put your wifi name here
const char* password = "1414141414"; // put your wifi password here, if it's open then only declare = ""

void FirmwareUpdate()
{
  Serial.print("\n Current Version is: ");
  Serial.print(string1);
  
  http.begin(URL_fw_Version, "‎‎5B FB D1 D4 49 D3 0F A9 C6 40 03 34 BA E0 24 05 AA D2 E2 01");    // check version URL
  delay(100);
  int httpCode = http.GET();            // get data from version file
  delay(100);
  String newVersion;
  const char* string2 = newVersion.c_str();


  if (httpCode == HTTP_CODE_OK)         // if version received
  {
    newVersion = http.getString();  // save received version
    Serial.print("\n New Version is : ");
    Serial.print(newVersion);
  }
  else
  {
    Serial.print("\n error in downloading version file:");
    Serial.println(httpCode);

  }

  http.end();
  if (httpCode == HTTP_CODE_OK)         // if version received
  {
    int ver = strcmp(string1, string2); // compare both version
    int dif = ver; // assign compare value in dif variable 
    Serial.printf("\n differrence is : %d", dif);
    if (ver == -10)
    {
      Serial.println("\n Device already on latest firmware version");
    }
    else if(ver == dif)
    {
      Serial.println("\n New firmware detected");
      WiFiClient client;

      t_httpUpdate_return ret = ESPhttpUpdate.update(URL_fw_Bin, "", "‎5B FB D1 D4 49 D3 0F A9 C6 40 03 34 BA E0 24 05 AA D2 E2 01");

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("\n HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("\n HTTP_UPDATE_NO_UPDATES");
          break;

        case HTTP_UPDATE_OK:
          Serial.println("\n HTTP_UPDATE_OK");
          break;
      }
    }
  }
}

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 60*1000;

void repeatedCall() {
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= interval)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    FirmwareUpdate();
  }
}


void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  repeatedCall();
}
