#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "stdint.h"
#include <stdint.h>

#define RED_LED GPIO_NUM_1
#define GREEN_LED GPIO_NUM_3
#define BLUE_LED GPIO_NUM_4

// Type to encapsulate led configs to pass to functions
typedef struct {
  gpio_num_t led;
  char *name;
  uint32_t delay;
} led_config_t;

static const char *TAG = "blinkers";
void blink(void *args) {
  uint32_t on = 1;
  char *state;

  led_config_t *cfg = (led_config_t *)args;

  while (1) {
    on = on ? 0 : 1;
    state = on ? "ON" : "OFF";
    ESP_LOGI(TAG, "name: %s, on: %d", cfg->name, on);
    ESP_ERROR_CHECK(gpio_set_level(cfg->led, on));
    ESP_LOGI(TAG, "%s %s", cfg->name, state);
    vTaskDelay(pdMS_TO_TICKS(cfg->delay));
  }
}

void app_main(void) {
  char task_name[100];

  // Ensure leds survives app_main since its used by tasks even after app_main
  // returns
  static led_config_t leds[3] = {
      {RED_LED, "RED", 125}, {GREEN_LED, "GREEN", 250}, {BLUE_LED, "BLUE", 500}

  };

  // Reset pins, set levels and create tasks for each gpio pin
  for (int i = 0; i < 3; i++) {
    ESP_LOGI(TAG, "leds[%d]: pin=%d name=%s delay=%lu", i, leds[i].led,
             leds[i].name, leds[i].delay);
    // Rename the task for each iteration based on value of i
    snprintf(task_name, sizeof(task_name), "%s-%d", "task", i);

    ESP_ERROR_CHECK(gpio_reset_pin(leds[i].led));
    ESP_ERROR_CHECK(gpio_set_direction(leds[i].led, GPIO_MODE_OUTPUT));

    // (void *) cast is optional as C can convert any object pointer to void *
    // automatically
    // ESP_ERROR_CHECK(
    //     xTaskCreate(blink, task_name, 4096, (void *)&leds[i], 2, NULL));

    BaseType_t result =
        xTaskCreate(blink, task_name, 4096, (void *)&leds[i], 2, NULL);
    ESP_LOGI(TAG, "Task %d created result: %d (pdPASS=%d)", i, result, pdPASS);
  }
}
