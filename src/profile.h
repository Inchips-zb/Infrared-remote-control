#ifndef _PROFILESTASK_H_
#define _PROFILESTASK_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "timers.h"

#define USER_MSG_ID_HARD_KEY    (0x0000000A)
#define USER_MSG_ID_IR_KEY      (0x0000000B)
uint32_t setup_profile(void *data, void *user_data);

#endif


