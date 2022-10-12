
/*--------------------------------------------------------------
                      INCLUDES
---------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"

/*--------------------------------------------------------------
                      PRIVATE CONSTANTS
---------------------------------------------------------------*/

static const char *TAG = "event";

#define MAXIMUM_NUMBER_OF_SUBSCRIBERS (10)

/*--------------------------------------------------------------
                      PRIVATE DATA
---------------------------------------------------------------*/

typedef struct
{
    QueueHandle_t xQueue[MAXIMUM_NUMBER_OF_SUBSCRIBERS];
    int queue_length[MAXIMUM_NUMBER_OF_SUBSCRIBERS];
    MESSAGE_CALLBACK callbacks[MAXIMUM_NUMBER_OF_SUBSCRIBERS];
} events_hub_element_t;

static events_hub_element_t events_hub[NUMBER_OF_EVENT_CODES] = {0};

static SemaphoreHandle_t mutex = NULL;
static StaticSemaphore_t xMutexBuffer;

/*--------------------------------------------------------------
                      PRIVATE FUNCTIONS
---------------------------------------------------------------*/


/*--------------------------------------------------------------
                      PUBLIC FUNCTIONS
---------------------------------------------------------------*/

void event_init(void)
{
    mutex = xSemaphoreCreateMutexStatic(&xMutexBuffer);
}

/**
 * @brief Register a callback for an event in the subscriver thread, subscribe to an event
*/
int event_register(event_t * event, MESSAGE_CALLBACK callback)
{
    assert(mutex);
    if( xSemaphoreTake( mutex, portMAX_DELAY ) == pdTRUE )
    {
        for(int i=0; i<MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
        {
            if(events_hub[event->code].xQueue[i] == 0)
            {
                events_hub[event->code].xQueue[i] = xQueueCreateStatic(event->queue_length,
                                                                    event->item_size,
                                                                    event->queue_storage,
                                                                    event->queue_static_queue);
                events_hub[event->code].queue_length[i] = event->queue_length;
                events_hub[event->code].callbacks[i] = callback;
                event->registered_id = i;
                ESP_LOGI(TAG, "Event registered for code %d (%s)", event->code, events_code_str[event->code]);
                xSemaphoreGive( mutex );
                return 0;
            }
        }
        xSemaphoreGive( mutex );
    }
    else
    {
        ESP_LOGE(TAG, "Mutex error");
    }
    return -1;
}

/**
 * @brief Check for new events in the subscriver thread
*/
int event_poll(event_t * event, TickType_t wait_ticks)
{
    assert(mutex);
    uint8_t data_pointer[event->item_size];
    if( xSemaphoreTake( mutex, portMAX_DELAY ) == pdTRUE )
    {
        if( xQueueReceive( events_hub[event->code].xQueue[event->registered_id],
                        data_pointer,
                        wait_ticks) == pdPASS )
        {
            ESP_LOGI(TAG, "Event received for code %d (%s)", event->code, events_code_str[event->code]);
            if(events_hub[event->code].callbacks[event->registered_id])
            {
                events_hub[event->code].callbacks[event->registered_id](data_pointer);
            }
            xSemaphoreGive( mutex );
            return 0;
        }
        xSemaphoreGive( mutex );
    }
    else
    {
        ESP_LOGE(TAG, "Mutex error");
    }
    return -1;
}

/**
 * @brief Publish an event from a publisher thread
*/
int event_publish(event_codes_t code, void* data, size_t data_size)
{
    assert(mutex);
    if( xSemaphoreTake( mutex, portMAX_DELAY ) == pdTRUE )
    {
        for(int i=0; i<MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
        {
            if(events_hub[code].xQueue[i] != 0)
            {
                if(events_hub[code].queue_length[i] > 1)
                {
                    xQueueSend(
                        events_hub[code].xQueue[i],
                        data,
                        portMAX_DELAY);
                }
                else
                {
                    xQueueOverwrite(
                        events_hub[code].xQueue[i],
                        data);
                }
                ESP_LOGI(TAG, "Event published for code %d (%s) for subscriber %d", code, events_code_str[code], i);
            }
        }
        xSemaphoreGive( mutex );
        return 0;
    }
    else
    {
        ESP_LOGE(TAG, "Mutex error");
    }
    return -1;
}
