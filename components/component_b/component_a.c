
/*--------------------------------------------------------------
                      INCLUDES
---------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "component_a.h"
#include "esp_log.h"
#include "esp_system.h"

/*--------------------------------------------------------------
                      PRIVATE CONSTANTS
---------------------------------------------------------------*/

static const char *TAG = "component_a";

/*--------------------------------------------------------------
                      PRIVATE DATA
---------------------------------------------------------------*/

DECLARE_EVENT(component_start_message_evt, COMPONENT_START_MESSAGE, 1)
DECLARE_EVENT(component_update_message_evt, COMPONENT_UPDATE_MESSAGE, 1)

/*--------------------------------------------------------------
                      PRIVATE FUNCTIONS
---------------------------------------------------------------*/

static void component_start_message_callback(void *data)
{
    EVENT_DATA_COMPONENT_START_MESSAGE *message = (EVENT_DATA_COMPONENT_START_MESSAGE*)data;
    ESP_LOGI(TAG, "Start message received from \"%s\" with data %d", message->component_name, message->magic_number);
}

static void component_update_message_callback(void *data)
{
    EVENT_DATA_COMPONENT_UPDATE_MESSAGE *message = (EVENT_DATA_COMPONENT_UPDATE_MESSAGE*)data;
    ESP_LOGI(TAG, "Update message received from \"%s\" with data %d %d %d %d", message->component_name, message->array[0], message->array[1], message->array[2], message->array[3]);
}

/*--------------------------------------------------------------
                      PUBLIC FUNCTIONS
---------------------------------------------------------------*/

void component_a(void *pv_parameters)
{
    ESP_LOGI(TAG, "Component A started");

    event_register(&component_start_message_evt, component_start_message_callback);
    event_register(&component_update_message_evt, component_update_message_callback);

    int cycles = 0;
    while(1)
    {
        event_poll(&component_start_message_evt, 0);
        event_poll(&component_update_message_evt, 0);
        vTaskDelay(100/portTICK_PERIOD_MS);

        cycles++;
        if(cycles == 50)
        {
            EVENT_DATA_COMPONENT_NOTIFY_MESSAGE notify_message;
            sprintf(notify_message.component_name, "Component A");
            sprintf(notify_message.notification_message, "Notification from A");
            event_publish(COMPONENT_NOTIFY_MESSAGE, &notify_message, sizeof(notify_message));
        }
    }
}