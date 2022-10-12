
/*--------------------------------------------------------------
                      INCLUDES
---------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "component_b.h"
#include "esp_log.h"
#include "esp_system.h"

/*--------------------------------------------------------------
                      PRIVATE CONSTANTS
---------------------------------------------------------------*/

static const char *TAG = "component_b";

/*--------------------------------------------------------------
                      PRIVATE DATA
---------------------------------------------------------------*/

DECLARE_EVENT(component_notify_message_evt, COMPONENT_NOTIFY_MESSAGE, 1)

/*--------------------------------------------------------------
                      PRIVATE FUNCTIONS
---------------------------------------------------------------*/

static void component_notify_message_callback(void *data)
{
    EVENT_DATA_COMPONENT_NOTIFY_MESSAGE *message = (EVENT_DATA_COMPONENT_NOTIFY_MESSAGE*)data;
    ESP_LOGI(TAG, "Start message received from \"%s\" with data \"%s\"", message->component_name, message->notification_message);
}

/*--------------------------------------------------------------
                      PUBLIC FUNCTIONS
---------------------------------------------------------------*/

void component_b(void *pv_parameters)
{
    ESP_LOGI(TAG, "Component B started");
    event_register(&component_notify_message_evt, component_notify_message_callback);

    int cycles = 0;
    while(1)
    {
        event_poll(&component_notify_message_evt, 0);
        vTaskDelay(100/portTICK_PERIOD_MS);

        cycles++;
        if(cycles == 50)
        {
            EVENT_DATA_COMPONENT_START_MESSAGE start_message;
            sprintf(start_message.component_name, "Component B started");
            start_message.magic_number = 1234;
            event_publish(COMPONENT_START_MESSAGE, &start_message, sizeof(start_message));
        }
        if(cycles == 70)
        {
            EVENT_DATA_COMPONENT_UPDATE_MESSAGE update_message;
            sprintf(update_message.component_name, "Component B updated");
            update_message.array[0] = 5;
            update_message.array[1] = 6;
            update_message.array[2] = 7;
            update_message.array[3] = 8;
            event_publish(COMPONENT_UPDATE_MESSAGE, &update_message, sizeof(update_message));
        }
    }
}