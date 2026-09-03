#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define RED_LED GPIO_NUM_1
#define GREEN_LED GPIO_NUM_3
#define BLUE_LED GPIO_NUM_4

static const char *TAG = "blinkers";

void blink_red(void *args) {
  int on = 1;
  char *state = "ON";
  int delay = *(int *)args;

  while (1) {
    // Alternate between on and off
    on = on ? 0 : 1;
    state = on ? "ON" : "OFF";

    gpio_set_level(RED_LED, on);
    ESP_LOGI(TAG, "RED %s", state);
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}

void blink_green(void *args) {
  int on = 1;
  char *state = "ON";
  int delay = *(int *)args;
  while (1) {
    // Alternate between on and off
    on = on ? 0 : 1;
    state = on ? "ON" : "OFF";

    gpio_set_level(GREEN_LED, on);
    ESP_LOGI(TAG, "GREEN %s", state);
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}

void blink_blue(void *args) {
  int on = 1;
  char *state = "ON";
  int delay = *(int *)args;
  while (1) {
    // Alternate between on and off
    on = on ? 0 : 1;
    state = on ? "ON" : "OFF";

    gpio_set_level(BLUE_LED, on);
    ESP_LOGI(TAG, "BLUE %s", state);
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}

void app_main(void) {
  gpio_reset_pin(RED_LED);
  gpio_reset_pin(GREEN_LED);
  gpio_reset_pin(BLUE_LED);

  gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT);

  static int red_delay = 125;
  static int green_delay = 250;
  static int blue_delay = 500;

  // (void *) cast is optional as C can convert any object pointer to void *
  // automatically
  xTaskCreate(blink_red, "Blink Red", 4096, (void *)&red_delay, 2, NULL);
  xTaskCreate(blink_green, "Blink Green", 4096, (void *)&green_delay, 2, NULL);
  xTaskCreate(blink_blue, "Blink Blue", 4096, (void *)&blue_delay, 2, NULL);
}
