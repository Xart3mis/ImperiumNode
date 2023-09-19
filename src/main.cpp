#include "core_esp8266_features.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <AsyncJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include <NeoPixelBusLg.h>

#define MAX_BRIGHTNESS 127
#define MIN_BRIGHTNESS 32

#define SELECT_1 14
#define SELECT_2 2

#define MAX_PIXEL_N 300
#define STRIP_PIXEL_N 125
#define RING_PIXEL_N 24

enum output_select { output_ring, output_strip };

bool connecting_LED_state = false;

unsigned long long previous_millis_connecting = 0;
unsigned long long previous_millis_fadeinout = 0;
unsigned long long previous_millis_gradient = 0;
unsigned long long previous_millis_timeout = 0;
unsigned long long previous_millis_begin = 0;
unsigned long long previous_millis_pick = 0;

const float rad_increment = 0.08;

output_select selection = output_strip;

uint8_t luminance[2] = {0};     // 0: strip, 1: ring
int8_t direction[2] = {-1, -1}; // 0: strip, 1: ring

RgbColor color(0, 0, 0);

NeoPixelBusLg<NeoGrbFeature, Neo800KbpsMethod, NeoGammaTableMethod>
    strip(MAX_PIXEL_N, 0);

AsyncWebServer server(80);

void DrawGradient(RgbColor, RgbColor, uint16_t, uint16_t);
void select_output(output_select);
void rest_server_routing();
// void init_websocket();
void toggle_output();
void smart_config();
void mdns_setup();
void wifi_setup();
// void ota_setup();

void setup() {
  GPF(SELECT_1) = GPFFS(GPFFS_GPIO(SELECT_1));
  GPF(SELECT_2) = GPFFS(GPFFS_GPIO(SELECT_2));

  GPC(SELECT_1) = GPC(SELECT_1) & (0x0F << GPCI);
  GPC(SELECT_2) = GPC(SELECT_2) & (0x0F << GPCI);

  GPES = (1 << SELECT_1);
  GPES = (1 << SELECT_2);

  // pinMode(LED_R, OUTPUT);
  //
  // while (!ESP.checkFlashCRC()) {
  //   delay(500);
  //   for (size_t i = 0; i < 3; i++) {
  //     digitalWrite(LED_R, HIGH);
  //     delay(250);
  //     digitalWrite(LED_R, LOW);
  //     delay(250);
  //   }
  //   for (size_t i = 0; i < 3; i++) {
  //     delay(750);
  //     digitalWrite(LED_R, HIGH);
  //     delay(750);
  //     digitalWrite(LED_R, LOW);
  //   }
  //   for (size_t i = 0; i < 3; i++) {
  //     delay(250);
  //     digitalWrite(LED_R, HIGH);
  //     delay(250);
  //     digitalWrite(LED_R, LOW);
  //   }
  // }
  //
  // pinMode(LED_B, OUTPUT);
  // pinMode(LED_G, OUTPUT);
  //
  Serial.begin(115200);
  Serial.flush();

  Serial.setDebugOutput(true);

  strip.Begin();

  WiFi.setOutputPower(15);
  wifi_setup();
  Serial.println(WiFi.localIP());
  smart_config();
  WiFi.printDiag(Serial);

  mdns_setup();
  // ota_setup();

  select_output(output_ring);
  for (size_t i = 0; i < RING_PIXEL_N; i++)
    strip.SetPixelColor(i, RgbColor(0, 255, 0));
  strip.Show();
  delay(2500);
  for (size_t i = 0; i < RING_PIXEL_N; i++)
    strip.SetPixelColor(i, RgbColor(0, 0, 0));
  strip.Show();
  delay(1000);

  rest_server_routing();
  server.onNotFound([](AsyncWebServerRequest *request) { request->send(404); });

  server.begin();
  Serial.println(F("HTTP server started"));
}

void loop() {
  unsigned long long currentm = millis();
  if (currentm - previous_millis_gradient >= 10) {
    noInterrupts();
    toggle_output();
    previous_millis_gradient = currentm;
    int idx = selection == output_ring;
    if (direction[idx] < 0 && luminance[idx] <= MIN_BRIGHTNESS)
      direction[idx] = 1;
    else if (direction[idx] > 0 && luminance[idx] >= MAX_BRIGHTNESS)
      direction[idx] = -1;
    else
      luminance[idx] += (direction[idx] * 1);

    for (size_t i = 0; i < (idx ? STRIP_PIXEL_N : RING_PIXEL_N); i++) {
      if (i % 3 == 0)
        yield();
      strip.SetPixelColor(i, color);
    }
    strip.SetLuminance(luminance[idx]);
    strip.Show();
    interrupts();
    yield();
  }

  MDNS.update();
  // ArduinoOTA.handle();
  // ws.cleanupClients();
}

void toggle_output() {
  if (selection == output_strip) {
    select_output(output_ring);
    selection = output_ring;
  } else {
    select_output(output_strip);
    selection = output_strip;
  }
}

void select_output(output_select _selection) {
  noInterrupts();
  uint32_t temp = GPO;
  if (_selection == output_strip) {
    temp &= ~(1 << SELECT_1);
    temp |= 1 << SELECT_2;
    selection = output_strip;
  } else {
    temp &= ~(1 << SELECT_2);
    temp |= 1 << SELECT_1;
    selection = output_ring;
  }
  GPO = temp;
  interrupts();
}

void DrawGradient(RgbColor startColor, RgbColor finishColor,
                  uint16_t startIndex, uint16_t finishIndex) {
  uint16_t delta = finishIndex - startIndex;

  for (uint16_t index = startIndex; index < finishIndex; index++) {
    float progress = static_cast<float>(index - startIndex) / delta;
    RgbColor color = RgbColor::LinearBlend(startColor, finishColor, progress);
    strip.SetPixelColor(index, color);
  }
}

void mdns_setup() {
  if (MDNS.begin(WiFi.BSSIDstr() + F("_LUX"))) {
    Serial.println(F("MDNS started"));
  }

  MDNS.addService(F("http"), F("tcp"), 80);
  MDNS.addServiceTxt(F("http"), F("tcp"), F("ID"), F("ImperiumNode"));
  MDNS.addServiceTxt(F("http"), F("tcp"), F("Product"), F("Imperium Lux RGB"));
  MDNS.addServiceTxt(F("http"), F("tcp"), F("MAC"), WiFi.macAddress());

  Serial.printf("MDNS:\n\thostname: %s\n\t_http._tcp : 80\n...\n",
                (WiFi.BSSIDstr() + F("_LUX.local")).c_str());
}

void rest_server_routing() {
  server.on("/setColor", HTTP_POST, [](AsyncWebServerRequest *request) {
    unsigned long long current_millis_pick = millis();
    if (current_millis_pick - previous_millis_pick < 250) {
      return;
    }
    previous_millis_pick = current_millis_pick;

    static unsigned int red, green, blue = 255;
    if (request->hasParam("red", true)) {
      AsyncWebParameter *p = request->getParam("red", true);
      red = p->value().toInt();
    }
    if (request->hasParam("green", true)) {
      AsyncWebParameter *p = request->getParam("green", true);
      green = p->value().toInt();
    }
    if (request->hasParam("blue", true)) {
      AsyncWebParameter *p = request->getParam("blue", true);
      blue = p->value().toInt();
    }

    color = RgbColor(red, green, blue);
    request->send(200);
  });
}

void smart_config() {
  select_output(output_ring);
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
  }

  int rad = M_PI * 2.5;
  while (WiFi.status() != WL_CONNECTED) {
    yield();

    unsigned long long current_millis_begin = millis();

    if (current_millis_begin - previous_millis_begin >= 5'000) {
      previous_millis_begin = current_millis_begin;
      WiFi.beginSmartConfig();
    }

    while (1) {
      yield();

      unsigned long long current_millis_connecting = millis();

      if (current_millis_connecting - previous_millis_connecting >= 250) {
        previous_millis_connecting = current_millis_connecting;

        connecting_LED_state = !connecting_LED_state;
        for (size_t i = 0; i < RING_PIXEL_N; i++)
          strip.SetPixelColor(i, RgbColor(connecting_LED_state * 255,
                                          connecting_LED_state * 255, 0));
        strip.Show();
      }

      if (WiFi.smartConfigDone()) {
        WiFi.begin();
        for (size_t i = 0; i < RING_PIXEL_N; i++)
          strip.SetPixelColor(i, RgbColor(0, 0, 0));

        while (WiFi.status() != WL_CONNECTED) {
          yield();
          unsigned long long current_millis_timeout = millis();

          if (current_millis_timeout - previous_millis_timeout >= 60'000) {
            previous_millis_timeout = current_millis_timeout;
            break;
          }

          unsigned long long current_millis_fadeinout = millis();

          if (current_millis_fadeinout - previous_millis_fadeinout >= 5) {
            previous_millis_fadeinout = current_millis_fadeinout;
            rad += rad_increment;
            if (rad >= 2.0 * M_PI) {
              rad -= 2.0 * M_PI;
            }

            int val = (255.5 / 9.0 * (pow(10, (0.5 * sin(rad)) + 0.5) - 1.0));
            for (size_t i = 0; i < RING_PIXEL_N; i++)
              strip.SetPixelColor(i, RgbColor(0, 0, val));

            strip.Show();
            yield();
          }
        }

        break;
      }
    }

    for (size_t i = 0; i < RING_PIXEL_N; i++)
      strip.SetPixelColor(i, RgbColor(0, 0, 0));
    strip.Show();
  }

  Serial.println(F(""));
}

void wifi_setup() {
  select_output(output_ring);

  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.forceSleepWake();

  WiFi.setAutoConnect(true);
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA);

  if (WiFi.SSID().length() > 0 || WiFi.psk().length() > 0) {
    Serial.printf("Previous SSID: %s\n Previous psk: %s\n", WiFi.SSID().c_str(),
                  WiFi.psk().c_str());

    Serial.println(F("Connecting to WiFi..."));

    WiFi.begin();

    float rad = 1.5 * M_PI;

    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      yield();

      for (int i = 0; i <= 255; i++) {
        yield();
        rad += rad_increment;
        if (rad >= 2.0 * M_PI) {
          rad -= 2.0 * M_PI;
        }

        int val = (255.5 / 9.0 * (pow(10, (0.5 * sin(rad)) + 0.5) - 1.0));

        yield();
        for (size_t i = 0; i < RING_PIXEL_N; i++)
          strip.SetPixelColor(i, RgbColor(val, val, 0));
        strip.Show();

        delay(10);
      }

      if (cnt++ >= 15)
        break;
    }

    for (size_t i = 0; i < RING_PIXEL_N; i++)
      strip.SetPixelColor(i, RgbColor(0, 0, 0));
    strip.Show();
  }
}
