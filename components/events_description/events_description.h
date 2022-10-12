#ifndef __EVENTS_DESCRIPTION_H__
#define __EVENTS_DESCRIPTION_H__

/* NOTE: This file should be placed localy at the application level of the project */

/*--------------------------------------------------------------
                      INCLUDES
---------------------------------------------------------------*/

#include <stdint.h>

/*--------------------------------------------------------------
                      PUBLIC TYPES
---------------------------------------------------------------*/

typedef enum
{
    COMPONENT_START_MESSAGE = 0,
    COMPONENT_UPDATE_MESSAGE,
    COMPONENT_NOTIFY_MESSAGE,
    NUMBER_OF_EVENT_CODES
} event_codes_t;

static const char* events_code_str[] = 
{
    [COMPONENT_START_MESSAGE] = "COMPONENT_START_MESSAGE",
    [COMPONENT_UPDATE_MESSAGE] = "COMPONENT_UPDATE_MESSAGE",
    [COMPONENT_NOTIFY_MESSAGE] = "COMPONENT_NOTIFY_MESSAGE"
};
_Static_assert(sizeof(events_code_str)/sizeof(events_code_str[0]) == NUMBER_OF_EVENT_CODES, "event strings are not defined correctly");

typedef struct 
{
    char component_name[32];
    int magic_number;
} EVENT_DATA_COMPONENT_START_MESSAGE;

typedef struct 
{
    char component_name[32];
    uint32_t array[4];
} EVENT_DATA_COMPONENT_UPDATE_MESSAGE;

typedef struct 
{
    char component_name[32];
    char notification_message[32];
} EVENT_DATA_COMPONENT_NOTIFY_MESSAGE;

#endif /*__EVENTS_DESCRIPTION_H__*/