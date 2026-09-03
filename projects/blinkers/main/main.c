#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#define RED_LED GPIO_NUM_1
#define GREEN_LED GPIO_NUM_3
#define BLUE_LED GPIO_NUM_4


void blink_red(void *args){
  while(1){
    gpio_set_level(RED_LED, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(RED_LED,0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  
}

void blink_green(void *args){
  while(1){
    gpio_set_level(GREEN_LED,1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(GREEN_LED,0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }

}

void blink_blue(void *args){
  while(1){
    gpio_set_level(BLUE_LED,1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(BLUE_LED,0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }

}

void app_main(void){
  gpio_reset_pin(RED_LED);
  gpio_reset_pin(GREEN_LED);
  gpio_reset_pin(BLUE_LED);

  gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT);

  xTaskCreate(blink_red,"Blink Red",4096,NULL,2,NULL);
  xTaskCreate(blink_green, "Blink Green", 4096, NULL, 2,NULL);
  xTaskCreate(blink_blue,"Blink Blue", 4096, NULL, 2, NULL);


}
