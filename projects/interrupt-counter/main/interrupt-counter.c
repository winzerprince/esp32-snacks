#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define BTN GPIO_NUM_4

// Volatile so compiler doesn't setup use of cached value
volatile int counter = 0;
volatile bool counter_changed = false;

// Store this in iram not flash for speed and availabilty as flash can be
// unavailable
void IRAM_ATTR counter_isr(void *arg) {
  counter++;
  counter_changed = true;
}

void app_main(void) {
  // Step 1: Configure pin direction and pull mode
  gpio_set_direction(BTN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(
      BTN, GPIO_PULLUP_ONLY); // Set this for active low setup of button

  // Step 2: Set trigger mode
  gpio_set_intr_type(BTN,
                     GPIO_INTR_NEGEDGE); // Hight to low when button is pressed

  // Step 3: install global ISR service
  gpio_install_isr_service(0);

  // step 4: Attach pins handler to ISR service
  gpio_isr_handler_add(BTN, counter_isr, NULL);

  while (1) {
    if (counter_changed) {
      counter_changed = false;
      printf("Counter: %d\n", counter);
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
