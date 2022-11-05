// #include <string>
// #include <Wire.h>
// #include <ESP8266WiFi.h>
// #include <Ticker.h>
// #include <PubSubClient.h>
// #include "Adafruit_SHT31.h"
// #include "Adafruit_SGP40.h"
// #include "DartSensor.h"

// const char *wifi_ssid       = "4G UFI_ECB";
// const char *wifi_password   = "1234567890";
// // const char *wifi_ssid       = "CMCC-2500";
// // const char *wifi_password   = "abcd1234";
// const char *mqtt_ip         = "home.maomao.run";
// const int   mqtt_port       = 1883;
// const char *mqtt_user       = "mqtt";
// const char *mqtt_password   = "mqtt";
// const char *mqtt_client_id  = "ESP32Client-3";

// WiFiClient wifi_client;
// PubSubClient mqtt_client{ wifi_client };

// Adafruit_SHT31 sht31;
// Adafruit_SGP40 sgp40;
// DartSensor     dart1{13, 15};
// DartSensor     dart2{12, 14};

// float   temp  = 0.0;
// float   humi  = 0.0;
// float   hcho1 = 0.0;
// float   hcho2 = 0.0;
// int32_t tvoc  = 0;
// int32_t sraw  = 0;

// bool can_read = false;
// bool can_send = false;

// bool is_wifi_connecting = false;
// bool is_mqtt_connecting = false;

// Ticker read_ticker;
// Ticker send_ticker;

// void setup()
// {
//   // put your setup code here, to run once:
//   Serial.begin(9600);

//   dart1.init();
//   Serial.println("Found Dart Sensor 1");
//   dart2.init();
//   Serial.println("Found Dart Sensor 2");

//   while (!sgp40.begin())
//   {
//     Serial.println("SGP40 not found.");
//     delay(10000);
//   }
//   Serial.println("Found SGP40.");

//   while (!sht31.begin())
//   {
//     Serial.println("SHT31 not found.");
//     delay(10000);
//   }
//   Serial.println("Found SHT31.");

//   mqtt_client.setServer(mqtt_ip, mqtt_port);

//   read_ticker.attach(
//     1,
//     []()
//     {
//       can_read = true;
//     }
//   );

//   send_ticker.attach(
//     30,
//     []()
//     {
//       can_send = true;
//     }
//   );
// }

// void loop()
// {
//   if (can_read)
//   {
//     can_read = false;

//     auto hcho_value_1 = dart1.fetch_value();
//     Serial.print("hcho value 1:");
//     Serial.println(hcho_value_1);
//     if (hcho_value_1 > 0)
//     {
//       hcho1 = hcho_value_1 * 30.03 / 22.4 / 1000;
//     }

//     auto hcho_value_2 = dart2.fetch_value();
//     Serial.print("hcho value 2:");
//     Serial.println(hcho_value_2);
//     if (hcho_value_2 > 0)
//     {
//       hcho2 = hcho_value_2 * 30.03 / 22.4 / 1000;
//     }
//     auto new_temp = sht31.readTemperature();
//     auto new_humi = sht31.readHumidity();
//     if (!isnan(new_temp) && !isnan(new_humi))
//     {
//       temp = new_temp;
//       humi = new_humi;
//     }
//     else
//     {
//       Serial.println("read temp and humi error.");
//     }
//     sraw = sgp40.measureRaw(temp, humi);
//     tvoc = sgp40.measureVocIndex(temp, humi);

//     Serial.print("TEMP:");
//     Serial.print(temp);
//     Serial.print(",");
//     Serial.print("HUMI:");
//     Serial.print(humi);
//     Serial.print(",");
//     Serial.print("HCHO1:");
//     Serial.print(hcho1);
//     Serial.print(",");
//     Serial.print("HCHO2:");
//     Serial.print(hcho2);
//     Serial.print(",");
//     Serial.print("TVOC:");
//     Serial.print(tvoc);
//     Serial.print(",");
//     Serial.print("SRAW:");
//     Serial.println(sraw);
//   }

//   if (can_send)
//   {
//     if (mqtt_client.connected())
//     {
//       Serial.println("Connected to MQTT");
//       is_mqtt_connecting = false;

//       char line[100];
//       sprintf(line, "{ \"temp\":%.2f, \"humi\":%.2f, \"hcho1\":%.3f, \"hcho2\":%.3f, \"sraw\":%d, \"tvoc\":%d }", temp, humi, hcho1, hcho2, sraw, tvoc);
//       Serial.println(line);
//       if (mqtt_client.publish("homeassistant/sensor/room3/state", line))
//       {
//         Serial.println("Publish ok.");
//       }
//       else
//       {
//         Serial.println("Publish failed;");
//       }
//       mqtt_client.disconnect();
//       WiFi.forceSleepBegin(0);
//       can_send = false;
//     }
//     else
//     {
//       if (is_mqtt_connecting)
//       {
//         // pass
//       }
//       else
//       {
//         if (WiFi.status() == WL_CONNECTED)
//         {
//           // wifi on, mqtt off
//           Serial.println("Connected to WiFi.");
//           is_wifi_connecting = false;
//           if (mqtt_client.connect(mqtt_client_id, mqtt_user, mqtt_password))
//           {
//             Serial.println("Connecting to MQTT.");
//             is_mqtt_connecting = true;
//           }
//           else
//           {
//             mqtt_client.disconnect();
//             is_wifi_connecting = false;
//             WiFi.forceSleepBegin(0);
//             can_send = false;
//           }
//         }
//         else if (WiFi.status() == WL_DISCONNECTED)
//         {
//           if (is_wifi_connecting)
//           {
//             // pass
//           }
//           else
//           {
//             // wifi off, mqtt off
//             WiFi.forceSleepWake();
//             WiFi.begin(wifi_ssid, wifi_password);
//             Serial.println("Connecting to WiFi.");
//             is_wifi_connecting = true;
//           }
//         }
//         else
//         {
//           // wifi connect error
//           Serial.print("WiFi connect error ");
//           Serial.println(WiFi.status());
//           is_wifi_connecting = false;
//           WiFi.forceSleepBegin(0);
//           can_send = false;
//         }
//       }
//     }
//   }
// }

#include <string>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include "Adafruit_SHT31.h"
#include "Adafruit_SGP40.h"
#include "DartSensor.h"

const char *wifi_ssid = "CMCC-2500";
const char *wifi_password = "abcd1234";
const char *mqtt_ip = "iot.maomao.run";
const int mqtt_port = 1883;
const char *mqtt_user = "mqtt_producer";
const char *mqtt_password = "P@ssw0rd!Mqtt";
const char *mqtt_client_id = "ESP8266Client-3";

WiFiClient wifi_client;
PubSubClient mqtt_client{wifi_client};

Adafruit_SHT31 sht31;
Adafruit_SGP40 sgp40;
DartSensor dart{13, 15};
// DartSensor     dart{12, 14};

float temp = 0.0;
float humi = 0.0;
float hcho = 0.0;
int32_t tvoc = 0;
int32_t sraw = 0;

bool can_read = false;
bool can_send = false;

bool is_wifi_connecting = false;
bool is_mqtt_connecting = false;

Ticker read_ticker;
Ticker send_ticker;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

    dart.init();
    Serial.println("Found Dart Sensor");

    while (!sgp40.begin())
    {
        Serial.println("SGP40 not found.");
        delay(10000);
    }
    Serial.println("Found SGP40.");

    while (!sht31.begin())
    {
        Serial.println("SHT31 not found.");
        delay(10000);
    }
    Serial.println("Found SHT31.");

    mqtt_client.setServer(mqtt_ip, mqtt_port);

    read_ticker.attach(
        1,
        []()
        {
            can_read = true;
        });

    send_ticker.attach(
        30,
        []()
        {
            can_send = true;
        });
}

void loop()
{
    if (can_read)
    {
        can_read = false;

        auto hcho_value = dart.fetch_value();
        Serial.print("hcho value:");
        Serial.println(hcho_value);
        if (hcho_value > 0)
        {
            hcho = hcho_value * 30.03 / 22.4 / 1000;
        }

        auto new_temp = sht31.readTemperature();
        auto new_humi = sht31.readHumidity();
        if (!isnan(new_temp) && !isnan(new_humi))
        {
            temp = new_temp;
            humi = new_humi;
        }
        else
        {
            Serial.println("read temp and humi error.");
        }
        sraw = sgp40.measureRaw(temp, humi);
        tvoc = sgp40.measureVocIndex(temp, humi);

        Serial.print("TEMP:");
        Serial.print(temp);
        Serial.print(",");
        Serial.print("HUMI:");
        Serial.print(humi);
        Serial.print(",");
        Serial.print("HCHO:");
        Serial.print(hcho);
        Serial.print(",");
        Serial.print("TVOC:");
        Serial.print(tvoc);
        Serial.print(",");
        Serial.print("SRAW:");
        Serial.println(sraw);
    }

    if (can_send)
    {
        if (mqtt_client.connected())
        {
            Serial.println("Connected to MQTT");
            is_mqtt_connecting = false;

            char line[100];
            sprintf(line, "{ \"temp\":%.2f, \"humi\":%.2f, \"hcho\":%.3f, \"sraw\":%d, \"tvoc\":%d }", temp, humi, hcho, sraw, tvoc);
            Serial.println(line);
            if (mqtt_client.publish("homeassistant/sensor/room3/state", line))
            {
                Serial.println("Publish ok.");
            }
            else
            {
                Serial.println("Publish failed;");
            }
            mqtt_client.disconnect();
            WiFi.forceSleepBegin(0);
            can_send = false;
        }
        else
        {
            if (is_mqtt_connecting)
            {
                // pass
            }
            else
            {
                if (WiFi.status() == WL_CONNECTED)
                {
                    // wifi on, mqtt off
                    Serial.println("Connected to WiFi.");
                    is_wifi_connecting = false;
                    if (mqtt_client.connect(mqtt_client_id, mqtt_user, mqtt_password))
                    {
                        Serial.println("Connecting to MQTT.");
                        is_mqtt_connecting = true;
                    }
                    else
                    {
                        mqtt_client.disconnect();
                        is_wifi_connecting = false;
                        WiFi.forceSleepBegin(0);
                        can_send = false;
                    }
                }
                else if (WiFi.status() == WL_DISCONNECTED)
                {
                    if (is_wifi_connecting)
                    {
                        // pass
                    }
                    else
                    {
                        // wifi off, mqtt off
                        WiFi.forceSleepWake();
                        WiFi.begin(wifi_ssid, wifi_password);
                        Serial.println("Connecting to WiFi.");
                        is_wifi_connecting = true;
                    }
                }
                else
                {
                    // wifi connect error
                    Serial.print("WiFi connect error ");
                    Serial.println(WiFi.status());
                    is_wifi_connecting = false;
                    WiFi.forceSleepBegin(0);
                    can_send = false;
                }
            }
        }
    }
}
