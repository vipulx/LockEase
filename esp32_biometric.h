#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "esp_http_server.h"
#include "fb_gfx.h"
#include "img_converters.h"
#include "fd_forward.h"
#include "fr_forward.h"

// Define camera model pins for AI-Thinker module
#define PWDN_GPIO_NUM     -1
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

// Initialize face recognition variables
mtmn_config_t mtmn_config = {0};
face_id_list id_list = {0};

// Function to initialize the SD card
bool initSDCard() {
  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    return false;
  }
  if (SD_MMC.cardType() == CARD_NONE) {
    Serial.println("No SD card attached");
    return false;
  }
  Serial.println("SD Card initialized.");
  return true;
}

// Function to save image to SD card
void saveImageToSD(camera_fb_t *fb, String path) {
  File file = SD_MMC.open("/" + path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file in write mode");
    return;
  }
  file.write(fb->buf, fb->len);
  file.close();
  Serial.println("Image saved to SD card: " + path);
}

// Function to start the camera server (web interface)
void startCameraServer();

// Setup function
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configure camera settings
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Set frame size and quality based on PSRAM availability
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Initialize camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  // Initialize SD card
  if (!initSDCard()) {
    Serial.println("SD Card initialization failed");
    return;
  }

  // Start the camera server
  startCameraServer();
}

// Main loop
void loop() {
  // All operations are handled via the web interface
}
