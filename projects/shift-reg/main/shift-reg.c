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
gpio_num_t DIG_LIST[4] = {D4, D3, D2, D1};
int our_num = 0;

uint8_t NUM[10] = {
    0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
    0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10001000,
};

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

// print num on digit
void disp_dig(int num, gpio_num_t digit) {
  uint8_t num_val = NUM[num];
  for (int i = 3; i < 7; i++) {
    gpio_set_level(PIN_LIST[i], 0);
  }

  for (int i = 7; i >= 0; i--) {
    gpio_set_level(SD, (num_val >> i) & 0x01);
    gpio_set_level(SH, 1);
    esp_rom_delay_us(1);
    gpio_set_level(SH, 0);
  }

  // latch output and display on digit
  gpio_set_level(ST, 1);
  esp_rom_delay_us(1);
  gpio_set_level(ST, 0);
  gpio_set_level(digit, 1);
}

// Display a 4 digit number
void disp_num(int *value) {

  int pv = 1; // Place value of a number
  int num;
  int dig_count = 0;

  while (1) {

    dig_count %= 4;
    if (pv > 1000) {
      pv = 1;
    }

    num = (*value % (pv * 10)) / pv; // extract digit from place value

    disp_dig(num, DIG_LIST[dig_count]);

    pv *= 10;
    ++dig_count;
  }
}

// Test print output a number to display
void test(void *args) {
  disp_num(&our_num);

  printf("Okay");
}

// Test print output a number to display
void test_1(void *args) {
  while (1) {

    printf("%d ", our_num);
    vTaskDelay(pdMS_TO_TICKS(1000));
    ++our_num;
  }
}

void app_main(void) {
  gpio_init();
  xTaskCreate(test, "test", 2048, NULL, 2, NULL);

  xTaskCreate(test_1, "test_1", 2048, NULL, 2, NULL);
}
