#include <esp_sleep.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include <driver/rtc_io.h>

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH_GPIO_NUM 4
#define LIGHT_IO 33
#define PIR_IO 12
const char * networkName = YOUR_WIFI_SSID
const char * networkPswd = YOUR_WIFI_PASSWORD
const char * server_ip = YOUR_TCP_SERVER_IP;
const int server_port = YOUR_TCP_SERVER_PORT

bool connected;
int senddur;
bool stoptcp = true;
unsigned long recorddur = 10000;
unsigned long maxrecorddur = 60000;
unsigned long startmillis, start;
unsigned long timebetweenframes;
int kol_frames = 0;
int kol_times = 0;
bool PIR_VAL;

WiFiClient client;

void setup() {  

  Serial.begin(115200);  

  pinMode(PIR_IO, INPUT);

  pinMode(FLASH_GPIO_NUM, OUTPUT);
  digitalWrite(FLASH_GPIO_NUM, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);
  gpio_deep_sleep_hold_en();
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIR_IO, HIGH);

  setupCamera();

  WiFi.begin(networkName, networkPswd); 
  int wifinumtry = 0;
  while (WiFi.status() != WL_CONNECTED && wifinumtry < 50) {
    wifinumtry = wifinumtry + 1;
    delay(100);
    Serial.println("Connecting..");
    Serial.println(WiFi.status());    
  }
  if (WiFi.status() != WL_CONNECTED){
    Serial.print("WiFi.status() != WL_CONNECTED");
    esp_deep_sleep_start();
  }

  Serial.print("Пробуждение заняло: ");
  Serial.print(millis());
  Serial.println(" мс.");      

  start = millis();
  startmillis = millis();
  while((millis() - startmillis) < recorddur and (millis() - start) < maxrecorddur){    

    PIR_VAL = digitalRead(PIR_IO);
    if (PIR_VAL == 1) {
      startmillis = millis();
    }
    camera_fb_t * fb = NULL;    
    fb = esp_camera_fb_get();   
    if(fb) {
      if (!client.connected()){
        if (!client.connect(server_ip, server_port)) {           
          esp_camera_fb_return(fb);
          fb = NULL;             
          continue;
        }
      }

      client.write(fb->buf, fb->len);      

      Serial.print("FPS: ");
      if ((millis() - timebetweenframes) != 0){
        Serial.print(1000/(millis() - timebetweenframes));
        Serial.println(" кадров в секунду");
        kol_frames = kol_frames + 1;
        kol_times = kol_times + 1000/(millis() - timebetweenframes);
        timebetweenframes = millis();   
      } else {
        Serial.println("деление на ноль");
      }
      Serial.print("Кадр: ");
      Serial.println(kol_frames); 
    }
    esp_camera_fb_return(fb);
    fb = NULL;
  }
  if (!client.connected()){
    if (!client.connect(server_ip, server_port)) {
      Serial.println("Connection to server failed 2");
      //esp_wifi_stop();
      //esp_camera_deinit();
      esp_deep_sleep_start();
    }
  }
  client.stop();
  Serial.print("Прошло время: ");
  Serial.println((millis() - start)/1000);  
  Serial.print("Avg. FPS: ");
  Serial.println(kol_times/kol_frames);  
  esp_deep_sleep_start();
}

void setupCamera(){
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG; //PIXFORMAT_YUV422

    //sensor_t * s = esp_camera_sensor_get();    
    //s->set_hmirror(s, 1);
    //s->set_vflip(s, 1);
    
    if(psramFound()){
      Serial.println("psramFound");
      config.frame_size = FRAMESIZE_VGA;
      //FRAMESIZE_CIF; // 352 x 288 FRAMESIZE_VGA 640x480 FRAMESIZE_SVGA (800 x 600) FRAMESIZE_XGA (1024 x 768)
      config.jpeg_quality = 10;
      config.fb_count = 2;
    }
      
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      esp_deep_sleep_start();
      return;
    } 
}

void loop() {}
