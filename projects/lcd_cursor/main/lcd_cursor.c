#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include <stdint.h>
#include <stdio.h>

#define RS GPIO_NUM_1
#define RW GPIO_NUM_2
#define D0 GPIO_NUM_4
#define D1 GPIO_NUM_5
#define D2 GPIO_NUM_6
#define D3 GPIO_NUM_7
#define D4 GPIO_NUM_15
#define D5 GPIO_NUM_16
#define D6 GPIO_NUM_17
#define D7 GPIO_NUM_18
#define E GPIO_NUM_8

gpio_num_t PIN_LIST[11] = {RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7};
int NUM_PINS = sizeof(PIN_LIST) / sizeof(PIN_LIST[0]);

void gpio_init(void) {
  uint64_t pin_mask = 0;
  for (int i = 0; i < NUM_PINS; i++) {
    pin_mask = pin_mask | (1ULL << PIN_LIST[i]);
  }

  gpio_config_t cfg = {.pin_bit_mask = pin_mask,
                       .mode = GPIO_MODE_OUTPUT,
                       .pull_up_en = GPIO_PULLUP_DISABLE,
                       .pull_down_en = GPIO_PULLDOWN_DISABLE,
                       .intr_type = GPIO_INTR_DISABLE};

  gpio_config(&cfg);
}

void command(uint8_t cmd, int rs, int rw) {
  gpio_set_level(RS, rs);
  gpio_set_level(RW, rw);

  gpio_set_level(D0, (cmd >> 0) & 0x01);
  gpio_set_level(D1, (cmd >> 1) & 0x01);
  gpio_set_level(D2, (cmd >> 2) & 0x01);
  gpio_set_level(D3, (cmd >> 3) & 0x01);
  gpio_set_level(D4, (cmd >> 4) & 0x01);
  gpio_set_level(D5, (cmd >> 5) & 0x01);
  gpio_set_level(D6, (cmd >> 6) & 0x01);
  gpio_set_level(D7, (cmd >> 7) & 0x01);

  gpio_set_level(E, 1);
  vTaskDelay(pdMS_TO_TICKS(15));
  gpio_set_level(E, 0);
  vTaskDelay(pdMS_TO_TICKS(15));
}

void cursor_blink(void *args) {
  // clear display
  command(0b00000001, 0, 0);
  // display on off cursor after dealay
  while (1) {
    command(0b00001111, 0, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
    command(0b00001101, 0, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void app_main() {
  gpio_init();

  xTaskCreate(cursor_blink, "blink cursor", 2048, NULL, 2, NULL);
}
