#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include <stdint.h>
#include <stdio.h>

#define SD GPIO_NUM_1
#define SH GPIO_NUM_2
#define ST GPIO_NUM_42
#define D1 GPIO_NUM_4
#define D2 GPIO_NUM_5
#define D3 GPIO_NUM_6
#define D4 GPIO_NUM_7

gpio_num_t PIN_LIST[7] = {SD, SH, ST, D1, D2, D3, D4};
int PIN_NUM = sizeof(PIN_LIST) / sizeof(PIN_LIST[0]);

void gpio_init() {
  uint64_t pin_mask = 0;
  for (int i = 0; i < PIN_NUM; i++) {
    pin_mask = pin_mask | (1ULL << PIN_LIST[i]);
  }

  gpio_config_t cfg = {.pin_bit_mask = pin_mask,
                       .mode = GPIO_MODE_OUTPUT,
                       .pull_down_en = GPIO_PULLDOWN_DISABLE,
                       .pull_up_en = GPIO_PULLUP_DISABLE,
                       .intr_type = GPIO_INTR_DISABLE};

  gpio_config(&cfg);
}

// Test print output number 5 to display
void test(void *args) {

  uint8_t value = 0b10010010; // segment representation of number 5

  while (1) {
    for (int i = 7; i >= 0; i--) {
      gpio_set_level(SD, (value >> i) & 0x01);
      gpio_set_level(SH, 1);
      esp_rom_delay_us(1);
      gpio_set_level(SH, 0);
    }

    // latch output
    gpio_set_level(ST, 1);
    esp_rom_delay_us(1);
    gpio_set_level(ST, 0);
    gpio_set_level(D1, 1);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_main(void) {
  gpio_init();
  xTaskCreate(test, "test", 2048, NULL, 2, NULL);
}
