
/*--------------------------------------------------------------
                      INCLUDES
---------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"

#include "component_a.h"
#include "component_b.h"
#include "event.h"

/*--------------------------------------------------------------
                      PRIVATE CONSTANTS
---------------------------------------------------------------*/

static const char *TAG = "main"; /**< Logger tag for the module */

/*--------------------------------------------------------------
                      PUBLIC FUNCTIONS
---------------------------------------------------------------*/

/**
 * @brief Main application function: initialisation, WiFi connection
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Event test");
    event_init();

    xTaskCreate(&component_a, "component_a", 10000, NULL, 5, NULL);
    xTaskCreate(&component_b, "component_b", 10000, NULL, 5, NULL);

    while(1)
    {
        vTaskDelay(10000);
    }
}
