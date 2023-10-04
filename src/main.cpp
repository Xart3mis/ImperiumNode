#include "core_esp8266_features.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include <NeoPixelBus.h>
#include "Utils.hpp"
#include "AsyncJson.h"
#include "ArduinoJson.h"

const uint16_t CIEL_10_12[] = {
    0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 19, 20, 20, 21,
    21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 39, 40,
    40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 54, 54, 55, 55, 56, 56, 57, 58, 58, 59, 59, 60, 61, 61, 62, 63, 63, 64,
    65, 65, 66, 67, 67, 68, 69, 69, 70, 71, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 78, 79, 80, 81, 81, 82, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 93, 94, 95, 96,
    97, 98, 98, 99, 100, 101, 102, 103, 104, 105, 106, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 143, 144, 145, 146, 147, 148, 149, 150, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162, 163, 165, 166, 167,
    168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 183, 185, 186, 187, 189, 190, 191, 193, 194, 195, 197, 198, 200, 201, 202, 204, 205, 206, 208, 209, 211, 212,
    214, 215, 217, 218, 219, 221, 222, 224, 225, 227, 228, 230, 231, 233, 234, 236, 238, 239, 241, 242, 244, 245, 247, 249, 250, 252, 253, 255, 257, 258, 260, 262, 263, 265,
    267, 268, 270, 272, 273, 275, 277, 278, 280, 282, 284, 285, 287, 289, 291, 292, 294, 296, 298, 300, 301, 303, 305, 307, 309, 311, 313, 314, 316, 318, 320, 322, 324, 326,
    328, 330, 331, 333, 335, 337, 339, 341, 343, 345, 347, 349, 351, 353, 355, 357, 359, 361, 363, 365, 368, 370, 372, 374, 376, 378, 380, 382, 384, 387, 389, 391, 393, 395,
    397, 400, 402, 404, 406, 408, 411, 413, 415, 417, 420, 422, 424, 426, 429, 431, 433, 436, 438, 440, 443, 445, 447, 450, 452, 454, 457, 459, 462, 464, 467, 469, 471, 474,
    476, 479, 481, 484, 486, 489, 491, 494, 496, 499, 501, 504, 507, 509, 512, 514, 517, 519, 522, 525, 527, 530, 533, 535, 538, 541, 543, 546, 549, 551, 554, 557, 560, 562,
    565, 568, 571, 573, 576, 579, 582, 585, 588, 590, 593, 596, 599, 602, 605, 608, 610, 613, 616, 619, 622, 625, 628, 631, 634, 637, 640, 643, 646, 649, 652, 655, 658, 661,
    664, 667, 671, 674, 677, 680, 683, 686, 689, 692, 696, 699, 702, 705, 708, 712, 715, 718, 721, 725, 728, 731, 734, 738, 741, 744, 748, 751, 754, 758, 761, 764, 768, 771,
    775, 778, 781, 785, 788, 792, 795, 799, 802, 806, 809, 813, 816, 820, 823, 827, 830, 834, 837, 841, 845, 848, 852, 856, 859, 863, 867, 870, 874, 878, 881, 885, 889, 892,
    896, 900, 904, 908, 911, 915, 919, 923, 927, 930, 934, 938, 942, 946, 950, 954, 958, 962, 966, 970, 973, 977, 981, 985, 989, 993, 997, 1002, 1006, 1010, 1014, 1018, 1022,
    1026, 1030, 1034, 1038, 1043, 1047, 1051, 1055, 1059, 1063, 1068, 1072, 1076, 1080, 1085, 1089, 1093, 1097, 1102, 1106, 1110, 1115, 1119, 1123, 1128, 1132, 1137, 1141, 1145,
    1150, 1154, 1159, 1163, 1168, 1172, 1177, 1181, 1186, 1190, 1195, 1199, 1204, 1208, 1213, 1218, 1222, 1227, 1232, 1236, 1241, 1246, 1250, 1255, 1260, 1264, 1269, 1274, 1279,
    1283, 1288, 1293, 1298, 1303, 1307, 1312, 1317, 1322, 1327, 1332, 1337, 1341, 1346, 1351, 1356, 1361, 1366, 1371, 1376, 1381, 1386, 1391, 1396, 1401, 1406, 1411, 1417, 1422,
    1427, 1432, 1437, 1442, 1447, 1453, 1458, 1463, 1468, 1473, 1479, 1484, 1489, 1494, 1500, 1505, 1510, 1516, 1521, 1526, 1532, 1537, 1542, 1548, 1553, 1559, 1564, 1570, 1575,
    1580, 1586, 1591, 1597, 1603, 1608, 1614, 1619, 1625, 1630, 1636, 1642, 1647, 1653, 1658, 1664, 1670, 1676, 1681, 1687, 1693, 1698, 1704, 1710, 1716, 1722, 1727, 1733, 1739,
    1745, 1751, 1757, 1762, 1768, 1774, 1780, 1786, 1792, 1798, 1804, 1810, 1816, 1822, 1828, 1834, 1840, 1846, 1852, 1859, 1865, 1871, 1877, 1883, 1889, 1895, 1902, 1908, 1914,
    1920, 1927, 1933, 1939, 1945, 1952, 1958, 1964, 1971, 1977, 1983, 1990, 1996, 2003, 2009, 2015, 2022, 2028, 2035, 2041, 2048, 2054, 2061, 2067, 2074, 2081, 2087, 2094, 2100,
    2107, 2114, 2120, 2127, 2134, 2140, 2147, 2154, 2161, 2167, 2174, 2181, 2188, 2195, 2201, 2208, 2215, 2222, 2229, 2236, 2243, 2250, 2257, 2263, 2270, 2277, 2284, 2291, 2299,
    2306, 2313, 2320, 2327, 2334, 2341, 2348, 2355, 2362, 2370, 2377, 2384, 2391, 2398, 2406, 2413, 2420, 2428, 2435, 2442, 2449, 2457, 2464, 2472, 2479, 2486, 2494, 2501, 2509,
    2516, 2524, 2531, 2539, 2546, 2554, 2561, 2569, 2576, 2584, 2592, 2599, 2607, 2615, 2622, 2630, 2638, 2645, 2653, 2661, 2669, 2676, 2684, 2692, 2700, 2708, 2716, 2723, 2731,
    2739, 2747, 2755, 2763, 2771, 2779, 2787, 2795, 2803, 2811, 2819, 2827, 2835, 2843, 2851, 2860, 2868, 2876, 2884, 2892, 2900, 2909, 2917, 2925, 2933, 2942, 2950, 2958, 2967,
    2975, 2983, 2992, 3000, 3009, 3017, 3026, 3034, 3042, 3051, 3059, 3068, 3077, 3085, 3094, 3102, 3111, 3119, 3128, 3137, 3145, 3154, 3163, 3171, 3180, 3189, 3198, 3207, 3215,
    3224, 3233, 3242, 3251, 3260, 3268, 3277, 3286, 3295, 3304, 3313, 3322, 3331, 3340, 3349, 3358, 3367, 3376, 3385, 3395, 3404, 3413, 3422, 3431, 3440, 3450, 3459, 3468, 3477,
    3487, 3496, 3505, 3515, 3524, 3533, 3543, 3552, 3562, 3571, 3580, 3590, 3599, 3609, 3618, 3628, 3637, 3647, 3657, 3666, 3676, 3685, 3695, 3705, 3714, 3724, 3734, 3744, 3753,
    3763, 3773, 3783, 3793, 3802, 3812, 3822, 3832, 3842, 3852, 3862, 3872, 3882, 3892, 3902, 3912, 3922, 3932, 3942, 3952, 3962, 3972, 3982, 3992, 4003, 4013, 4023, 4033, 4044,
    4054, 4064, 4074, 4085, 4095};

#define INTERVAL_SIZE 1024

int brightness = 4096;
int brightness_max = 4096;

#define BRIGHTNESS_STEP 1

#define SELECT_1 14
#define SELECT_2 2

#define MAX_PIXEL_N 60 * 5
#define STRIP_PIXEL_N 60 * 5
#define RING_PIXEL_N 24

bool connecting_LED_state = false;

bool update = true;

uint16_t fadeinout_cie_idx = 1024;
uint16_t fadeinout_dir = -12;
bool fadeinout_mode = false;

bool party_mode = false;

bool flashing_mode = false;
bool flashing_blackout = false;
byte triadic_color_idx = 0;

unsigned long long previous_millis_connection_check = 0;
unsigned long long previous_millis_connecting = 0;
unsigned long long previous_millis_fadeeffect = 0;
unsigned long long previous_millis_fadeinout = 0;
unsigned long long previous_millis_flashing = 0;
unsigned long long previous_millis_cleanup = 0;
unsigned long long previous_millis_timeout = 0;
unsigned long long previous_millis_party = 0;
unsigned long long previous_millis_begin = 0;

const float rad_increment = 0.08;

RgbColor color(0, 0, 0);
RgbColor triadic_colors[3] = {RgbColor(0, 0, 0)};

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(MAX_PIXEL_N);

Utils utils(SELECT_1, SELECT_2);

AsyncWebSocket ws("/ws");
AsyncWebServer server(80);

RgbColor ColorWithBrightness(RgbColor, bool withMax = false);
void triadic();
void smart_config();
void mdns_setup();
void wifi_setup();

RgbColor ColorWithBrightness(RgbColor c, bool withMax)
{
  float fact;
  if (withMax)
    fact = ((float)map(brightness, 0, 4096, 0, brightness_max)) / 4096.0;
  else
    fact = ((float)brightness) / 4096.0;

  Serial.println(255.0 * fact);

  return RgbColor(c.R * fact, c.G * fact, c.B * fact);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    char data_buffer[1024] = {0};
    strncpy(data_buffer, (char *)data, len);

    if (strncmp(data_buffer, "COLORS", strlen("COLORS")) == 0)
    {
      int colors[3] = {-1};
      byte idx = 0;

      char *out;
      char *in = &data_buffer[0];
      in += strlen("COLORS");
      while ((out = strsep(&in, ",")) != NULL && idx < 3)
        colors[idx++] = atoi(out);

      char buffer[15] = {0};
      sprintf(buffer, "RGB = %x,%x,%x", colors[0], colors[1], colors[2]);
      Serial.println(buffer);

      color = ColorWithBrightness(RgbColor(colors[0], colors[1], colors[2]));
      strip.ClearTo(color);

      if (party_mode || fadeinout_mode)
        triadic();
      update = true;
    }

    if (strncmp(data_buffer, "LUMA", strlen("LUMA")) == 0)
    {
      uint16_t luma = 0;
      char *in = &data_buffer[0];
      in += strlen("LUMA");
      luma = atoi(in);

      brightness = CIEL_10_12[luma];
      strip.ClearTo(ColorWithBrightness(color));

      update = true;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void triadic()
{
  triadic_colors[1] = color;
  HslColor hsl = HslColor(color);
  hsl.H = fmod(hsl.H + (120.0 / 360.0), 1.0);
  triadic_colors[0] = hsl;
  hsl = HslColor(color);
  hsl.H = fmod(hsl.H + (240.0 / 360.0), 1.0);
  triadic_colors[2] = hsl;
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup()
{
  triadic();

  GPF(SELECT_1) = GPFFS(GPFFS_GPIO(SELECT_1));
  GPF(SELECT_2) = GPFFS(GPFFS_GPIO(SELECT_2));

  GPC(SELECT_1) = GPC(SELECT_1) & (0x0F << GPCI);
  GPC(SELECT_2) = GPC(SELECT_2) & (0x0F << GPCI);

  GPES = (1 << SELECT_1);
  GPES = (1 << SELECT_2);

  Serial.begin(115200);
  Serial.flush();
  while (!Serial)
    ;

  Serial.println("Started...");

  Serial.setDebugOutput(true);

  strip.Begin();
  delay(5000);

  utils.SelectOutput(output_strip);
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();

  WiFi.setOutputPower(10);
  wifi_setup();
  Serial.println(WiFi.localIP());
  smart_config();
  WiFi.printDiag(Serial);

  mdns_setup();

  initWebSocket();

  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              AsyncResponseStream *response = request->beginResponseStream("application/json");
              DynamicJsonDocument jsonDocument(256);
              jsonDocument["r"] = color.R;
              jsonDocument["g"] = color.G;
              jsonDocument["b"] = color.B;
              serializeJson(jsonDocument, *response);
              request->send(response); //
            });

  server.on("/partyMode", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("enable"))
              {
                if (strncmp(request->getParam("enable")->value().c_str(), "true", strlen("true")) == 0)
                {
                  party_mode = true;
                  triadic();
                  request->send(200);
                  return;
                }
                else if (strncmp(request->getParam("enable")->value().c_str(), "false", strlen("false")) == 0)
                {
                  party_mode = false;
                  strip.ClearTo(ColorWithBrightness(color));
                  update = true;
                  request->send(200);
                  return;
                }
              }
              AsyncResponseStream *response = request->beginResponseStream("application/json");
              DynamicJsonDocument jsonDocument(64);
              jsonDocument["status"] = party_mode ? "true" : "false";
              serializeJson(jsonDocument, *response);
              request->send(response); //
            });
  server.on(
      "/fadeInOut", HTTP_GET, [](AsyncWebServerRequest *request)
      {
        if (request->hasParam("enable"))
        {
          if (strncmp(request->getParam("enable")->value().c_str(), "true", strlen("true")) == 0)
          {
            fadeinout_mode = true;
            triadic();
            request->send(200);
            return;
          }
          else if (strncmp(request->getParam("enable")->value().c_str(), "false", strlen("false")) == 0)
          {
            fadeinout_mode = false;
            strip.ClearTo(ColorWithBrightness(color));
            update = true;
            request->send(200);
            return;
          }
        }

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument jsonDocument(64);
        jsonDocument["status"] = fadeinout_mode ? "true" : "false";
        serializeJson(jsonDocument, *response);
        request->send(response); //
      });

  utils.SelectOutput(output_ring);
  strip.ClearTo(ColorWithBrightness(RgbColor(0, 255, 0)));
  strip.Show();
  delay(2500);
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
  delay(1000);

  server.begin();
  Serial.println("Started HTTP Server.");

  utils.SelectOutput(output_strip);
  strip.ClearTo(ColorWithBrightness(RgbColor(255, 255, 255)));
  strip.Show();
  delay(100);
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
  delay(100);
  strip.ClearTo(color);
  strip.Show();
}

void loop()
{
  unsigned long long current_millis_connection_check = millis();
  if (current_millis_connection_check - previous_millis_connection_check >= 1500)
  {
    previous_millis_connection_check = current_millis_connection_check;
    if (!WiFi.isConnected())
      WiFi.begin();
  }

  if (party_mode)
  {
    unsigned long long current_millis_party = millis();
    if (current_millis_party - previous_millis_party >= 150)
    {
      previous_millis_party = current_millis_party;
      if (triadic_color_idx >= 3)
      {
        triadic_color_idx = 0;
      }

      strip.ClearTo(ColorWithBrightness(triadic_colors[triadic_color_idx]));
      triadic_color_idx++;
    }

    update = true;
  }
  if (fadeinout_mode)
  {
    brightness = CIEL_10_12[fadeinout_cie_idx];
    fadeinout_cie_idx += fadeinout_dir;

    if (triadic_color_idx >= 3)
    {
      triadic_color_idx = 0;
    }

    strip.ClearTo(ColorWithBrightness(RgbColor(0).LinearBlend(triadic_colors[triadic_color_idx], triadic_colors[triadic_color_idx >= 2 ? 0 : triadic_color_idx + 1], (uint8_t)map(fadeinout_cie_idx, 0, 1024, 255, 0))));

    if (fadeinout_cie_idx < 256)
    {
      fadeinout_cie_idx = 256;
      fadeinout_dir = 12;
      triadic_color_idx++;
    }
    else if (fadeinout_cie_idx >= 1024)
    {
      fadeinout_cie_idx = 1024;
      fadeinout_dir = -12;
    }

    update = true;
  }

  if (flashing_mode)
  {
    unsigned long long current_millis_flashing = millis();
    if (previous_millis_flashing - current_millis_flashing >= 250)
    {
      previous_millis_flashing = current_millis_flashing;
      flashing_blackout = true;
    }
    else
    {
      if (triadic_color_idx >= 3)
      {
        triadic_color_idx = 0;
      }

      strip.ClearTo(ColorWithBrightness(triadic_colors[triadic_color_idx]));
      triadic_color_idx++;
    }

    update = true;
  }

  if (update)
  {
    update = false;
    strip.Show();
  }

  unsigned long long current_millis_cleanup = millis();
  if (current_millis_cleanup - previous_millis_cleanup >= 1 * 60 * 1'000)
  {
    previous_millis_cleanup = current_millis_cleanup;
    ws.cleanupClients();
  }

  MDNS.update();
  ESP.wdtFeed();
}

void mdns_setup()
{
  if (MDNS.begin(WiFi.BSSIDstr() + F("_LUX")))
  {
    Serial.println(F("MDNS started"));
  }

  MDNS.addService(F("http"), F("tcp"), 80);

  MDNS.addServiceTxt(F("http"), F("tcp"), F("ID"), F("ImperiumNode"));
  MDNS.addServiceTxt(F("http"), F("tcp"), F("Product"), F("Imperium Lux RGB"));
  MDNS.addServiceTxt(F("http"), F("tcp"), F("MAC"), WiFi.macAddress());

  char hostname[128] = {0};
  WiFi.hostname().toCharArray(hostname, 128);
  Serial.printf("MDNS:\n\thostname: %s\n\t_http._tcp : 80\n", hostname);
}

void smart_config()
{
  utils.SelectOutput(output_ring);
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.beginSmartConfig();
  }

  int rad = M_PI * 2.5;
  while (WiFi.status() != WL_CONNECTED)
  {
    yield();

    unsigned long long current_millis_begin = millis();

    if (current_millis_begin - previous_millis_begin >= 5'000)
    {
      previous_millis_begin = current_millis_begin;
      WiFi.beginSmartConfig();
    }

    while (1)
    {
      yield();

      unsigned long long current_millis_connecting = millis();

      if (current_millis_connecting - previous_millis_connecting >= 250)
      {
        previous_millis_connecting = current_millis_connecting;

        connecting_LED_state = !connecting_LED_state;
        strip.ClearTo(RgbColor(connecting_LED_state * 255,
                               connecting_LED_state * 255, 0));
        strip.Show();
      }

      if (WiFi.smartConfigDone())
      {
        WiFi.begin();
        strip.ClearTo(RgbColor(0, 0, 0));

        while (WiFi.status() != WL_CONNECTED)
        {
          yield();
          unsigned long long current_millis_timeout = millis();

          if (current_millis_timeout - previous_millis_timeout >= 60'000)
          {
            previous_millis_timeout = current_millis_timeout;
            break;
          }

          unsigned long long current_millis_fadeinout = millis();

          if (current_millis_fadeinout - previous_millis_fadeinout >= 5)
          {
            previous_millis_fadeinout = current_millis_fadeinout;
            rad += rad_increment;
            if (rad >= 2.0 * M_PI)
            {
              rad -= 2.0 * M_PI;
            }

            int val = (255.5 / 9.0 * (pow(10, (0.5 * sin(rad)) + 0.5) - 1.0));
            strip.ClearTo(0, 0, val);

            strip.Show();
            yield();
          }
        }

        break;
      }
    }

    strip.ClearTo(RgbColor(0, 0, 0));
    strip.Show();
  }

  Serial.println(F(""));
}

void wifi_setup()
{
  utils.SelectOutput(output_ring);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.forceSleepWake();

  WiFi.setAutoConnect(true);
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA);

  if (WiFi.SSID().length() > 0 || WiFi.psk().length() > 0)
  {
    Serial.printf("Previous SSID: %s\n Previous psk: %s\n", WiFi.SSID().c_str(),
                  WiFi.psk().c_str());

    Serial.println(F("Connecting to WiFi..."));

    WiFi.begin();

    float rad = 1.5 * M_PI;

    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      yield();

      for (int i = 0; i <= 255; i++)
      {
        yield();
        rad += rad_increment;
        if (rad >= 2.0 * M_PI)
        {
          rad -= 2.0 * M_PI;
        }

        int val = (255.5 / 9.0 * (pow(10, (0.5 * sin(rad)) + 0.5) - 1.0));

        yield();
        strip.ClearTo(RgbColor(val, val, 0));
        strip.Show();

        delay(10);
      }

      if (cnt++ >= 15)
        break;
    }

    strip.ClearTo(RgbColor(0, 0, 0));
    strip.Show();
  }
}
