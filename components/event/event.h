#ifndef __EVENT_H__
#define __EVENT_H__

/*--------------------------------------------------------------
                      INCLUDES
---------------------------------------------------------------*/

#include "events_description.h" /* The file with the description of events data, should be specific for the application and located the the application level */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/*--------------------------------------------------------------
                      PUBLIC TYPES
---------------------------------------------------------------*/

typedef struct
{
    int queue_length;
    size_t item_size;
    uint8_t* queue_storage;
    StaticQueue_t* queue_static_queue;
    event_codes_t code;
    size_t registered_id;
} event_t;

typedef void (*MESSAGE_CALLBACK)(void *);

/*--------------------------------------------------------------
                      PUBLIC MACRO
---------------------------------------------------------------*/

#define DECLARE_EVENT(EVENT, EVENT_ID, QUEUE_LENGTH) \
    static StaticQueue_t xStaticQueue##EVENT_ID; \
    static uint8_t ucQueueStorageArea##EVENT_ID[ QUEUE_LENGTH * sizeof( EVENT_DATA_##EVENT_ID ) ]; \
    static event_t EVENT = { \
        .queue_length = QUEUE_LENGTH, \
        .item_size = sizeof( EVENT_DATA_##EVENT_ID ), \
        .queue_static_queue = &xStaticQueue##EVENT_ID, \
        .queue_storage = ucQueueStorageArea##EVENT_ID, \
        .code = EVENT_ID \
    };

/*--------------------------------------------------------------
                      PUBLIC FUNCTIONS
---------------------------------------------------------------*/

void event_init(void);
int event_register(event_t * event, MESSAGE_CALLBACK callback);
int event_poll(event_t * event, TickType_t wait_ticks);
int event_publish(event_codes_t code, void* data, size_t data_size);

#endif /*__EVENT_H__*/