#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include <stdio.h>

#define A GPIO_NUM_1
#define B GPIO_NUM_2
#define C GPIO_NUM_3
#define D GPIO_NUM_4
#define E GPIO_NUM_5
#define F GPIO_NUM_6
#define G GPIO_NUM_7
#define DOT GPIO_NUM_8

void app_main(void) {

  // int pattern = 0b00000111;
  int patterns[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111,
                      0b01100110, 0b01101101, 0b01111101, 0b00000111,
                      0b01111111, 0b01101111};

  gpio_reset_pin(A);
  gpio_reset_pin(B);
  gpio_reset_pin(C);
  gpio_reset_pin(D);
  gpio_reset_pin(E);
  gpio_reset_pin(F);
  gpio_reset_pin(G);
  gpio_reset_pin(DOT);

  gpio_set_direction(A, GPIO_MODE_OUTPUT);
  gpio_set_direction(B, GPIO_MODE_OUTPUT);
  gpio_set_direction(C, GPIO_MODE_OUTPUT);
  gpio_set_direction(D, GPIO_MODE_OUTPUT);
  gpio_set_direction(E, GPIO_MODE_OUTPUT);
  gpio_set_direction(F, GPIO_MODE_OUTPUT);
  gpio_set_direction(G, GPIO_MODE_OUTPUT);
  gpio_set_direction(DOT, GPIO_MODE_OUTPUT);

  while (1) {

    for (int i = 0; i <= 9; i++) {
      ESP_ERROR_CHECK(gpio_set_level(A, (patterns[i] >> 0) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(B, (patterns[i] >> 1) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(C, (patterns[i] >> 2) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(D, (patterns[i] >> 3) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(E, (patterns[i] >> 4) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(F, (patterns[i] >> 5) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(G, (patterns[i] >> 6) & 0x01));
      ESP_ERROR_CHECK(gpio_set_level(DOT, (patterns[i] >> 7) & 0x01));
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}
