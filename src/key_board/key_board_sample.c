#include "key_board_sample.h"
#include "platform_api.h"
#include "btstack_event.h"
#include "../profile.h"


typedef struct
{
    key_id_hard_e keyHardId;
    enum key_state_t state;
    key_t key;
    eClickState multiType;  //if KEY_PRESS_CONTINUOUS state
    void (*cbFun)(void);
} keyFuncMap_t;

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
typedef struct
{
    int combineKeyId;
    const struct key_combine_t *pCombineTab;
    uint8_t size;
    key_t key;
    void (*cbFun)(void);
} keyCombineMap_t;

static const struct key_combine_t key_ir_learn[] = {
    { .id = KB_HARD_K1,   .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K5,   .state = KEY_PRESS_LONG },
};
static const struct key_combine_t key_adv_start[] = {
    { .id = KB_HARD_K8,   .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K9,  .state = KEY_PRESS_LONG },
};
static const struct key_combine_t key_adv_stop[] = {
    { .id = KB_HARD_K9,   .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K10,   .state = KEY_PRESS_LONG },
};
static const struct key_combine_t key_power_on[] = {
    { .id = KB_HARD_K11,  .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K12,  .state = KEY_PRESS_LONG },
};
static const struct key_combine_t key_power_off[] = {
    { .id = KB_HARD_K12,  .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K13,  .state = KEY_PRESS_LONG },
};

static void cmbinetest1(void)
{
    platform_printf("key_ir_learn\n");
}
static void cmbinetest2(void)
{
    platform_printf("key_adv_start\n");
}
static void cmbinetest3(void)
{
    platform_printf("key_adv_stop\n");
}
static void cmbinetest4(void)
{
    platform_printf("key_power_on\n");
}
static void cmbinetest5(void)
{
    platform_printf("key_power_off\n");
}
// Number of members limited to the macro definition KEY_COMBINE_NUM.
static keyCombineMap_t keyCombineMap[] = 
{
    {0,  key_ir_learn,  GET_ARRAY_SIZE(key_ir_learn),    KB_NULL,     KB_IR_NULL ,       cmbinetest1}, // combine1
    {0,  key_adv_start, GET_ARRAY_SIZE(key_adv_start),   KB_NULL,     KB_IR_NULL ,       cmbinetest2}, // combine2
    {0,  key_adv_stop,  GET_ARRAY_SIZE(key_adv_stop),    KB_NULL,     KB_IR_NULL ,       cmbinetest3}, // combine3
    {0,  key_power_on,  GET_ARRAY_SIZE(key_power_on),    KB_NULL,     KB_IR_NULL ,       cmbinetest4}, // combine4
    {0,  key_power_off, GET_ARRAY_SIZE(key_power_off),   KB_NULL,     KB_IR_NULL ,       cmbinetest5}, // combine5
};
#endif
static void test14_press_continou(void)
{
    platform_printf("k14 press_continou\n");
}
static void test15_press_long(void)
{
    platform_printf("k15 press_long\n");
}
static void test16_triple_click_press(void)
{
    platform_printf("k16 triple_click_press\n");
}
static const keyFuncMap_t keyFuncMap[] = {
   //hard id       trigger state   key_event id      ir key id            multi key type   event callback     
    {KB_HARD_K1,    KEY_PRESS,      KB_VOLUME_MUTE ,KB_IR_VOLUME_MUTE , MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J1
    {KB_HARD_K2,    KEY_PRESS,      KB_POWER       ,KB_IR_POWER ,       MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J2
    {KB_HARD_K3,    KEY_PRESS,      KB_UP          ,KB_IR_UP ,          MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J3
    {KB_HARD_K4,    KEY_PRESS,      KB_RIGHT       ,KB_IR_RIGHT ,       MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J4
    {KB_HARD_K5,    KEY_PRESS,      KB_ENTER       ,KB_IR_ENTER ,       MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J5
    {KB_HARD_K6,    KEY_PRESS,      KB_LEFT        ,KB_IR_LEFT ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J6
    {KB_HARD_K7,    KEY_PRESS,      KB_DOWN        ,KB_IR_DOWN ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J7
    {KB_HARD_K8,    KEY_PRESS,      KB_MENU        ,KB_IR_MENU ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J8
    {KB_HARD_K9,    KEY_PRESS,      KB_VOICE       ,KB_IR_NULL ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J9
    {KB_HARD_K10,   KEY_PRESS,      KB_EXIT        ,KB_IR_EXIT ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J10
    {KB_HARD_K11,   KEY_PRESS,      KB_VOLUME_UP   ,KB_IR_VOLUME_UP ,   MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J11
    {KB_HARD_K12,   KEY_PRESS,      KB_HOME        ,KB_IR_HOME ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J12
    {KB_HARD_K13,   KEY_PRESS,      KB_PAGE_UP     ,KB_IR_PAGE_UP ,     MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J13
    {KB_HARD_K14,   KEY_PRESS,      KB_VOLUME_DOWN ,KB_IR_VOLUME_DOWN , MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J14
    {KB_HARD_K15,   KEY_PRESS,      KB_NULL        ,KB_IR_NULL ,        MULTI_CLICK_NONE,  CB_FUN_NULL     }, // J15
    {KB_HARD_K16,   KEY_PRESS,      KB_PAGE_DOWN   ,KB_IR_PAGE_DOWN ,   MULTI_CLICK_NONE,  CB_FUN_NULL     },  // J16
    
//    {KB_HARD_K14,   KEY_PRESS_CONTINUOUS,   KB_NULL     , KB_IR_NULL ,     MULTI_CLICK_NONE,  test14_press_continou   },  // J16
//    {KB_HARD_K15,   KEY_PRESS_LONG,         KB_NULL     , KB_IR_NULL ,     MULTI_CLICK_NONE,  test15_press_long   },  // J16
//    {KB_HARD_K16,   KEY_PRESS_MULTI,        KB_NULL     , KB_IR_NULL ,     TRIPLE_CLICK    ,  test16_triple_click_press   },  // J16   
};

const struct key_pin_t key_pin_sig[] = {
    {
        .pin = GIO_GPIO_11,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },
    {
        .pin = GIO_GPIO_12,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },
    {
        .pin = GIO_GPIO_13,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },
    {
        .pin = GIO_GPIO_14,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },
};

#if (USER_KEY_BOARD_MATRIX)
const struct key_pin_t key_pin_ctrl[] = {
    {
        .pin = GIO_GPIO_8,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },
    {
        .pin = GIO_GPIO_9,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },
    {
        .pin = GIO_GPIO_10,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    }, 
    {
        .pin = GIO_GPIO_34,
        .valid = GPIO_PIN_SET,
        .invalid = GPIO_PIN_RESET
    },    
};
#endif

static inline bool pin_level_get(const void *desc)
{
    struct key_pin_t *pdesc;

    pdesc = (struct key_pin_t *)desc;
    return GIO_ReadValue(pdesc->pin) == pdesc->valid;
}

#if (USER_KEY_BOARD_MATRIX)
static inline void pin_level_set(const void *desc, bool flag)
{
    struct key_pin_t *pdesc;

    pdesc = (struct key_pin_t *)desc;
    //GIO_SetDirection(pdesc->pin, GIO_DIR_OUTPUT);
    GIO_WriteValue(pdesc->pin, flag ? pdesc->valid : pdesc->invalid);
}
#endif

const struct key_public_sig_t key_public_sig[] = {
#if (USER_KEY_BOARD_MATRIX)    
    KEY_PUBLIC_SIG_DEF(KB_HARD_K1, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K5, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K9, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K13, &key_pin_sig[3], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),

    KEY_PUBLIC_SIG_DEF(KB_HARD_K2, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K6, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K10, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K14, &key_pin_sig[3], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),

    KEY_PUBLIC_SIG_DEF(KB_HARD_K3, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K7, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K11, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K15, &key_pin_sig[3], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),

    KEY_PUBLIC_SIG_DEF(KB_HARD_K4, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K8, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K12, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K16, &key_pin_sig[3], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
#else
    KEY_PUBLIC_SIG_DEF(KB_HARD_K1, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K2, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K3, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K4, &key_pin_sig[3], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),

#endif
};

#if (USER_KEY_BOARD_MATRIX)
const struct key_public_ctrl_t key_public_ctrl[] = {
    KEY_PUBLIC_CTRL_DEF(&key_pin_ctrl[0], pin_level_set),
    KEY_PUBLIC_CTRL_DEF(&key_pin_ctrl[1], pin_level_set),
    KEY_PUBLIC_CTRL_DEF(&key_pin_ctrl[2], pin_level_set),
    KEY_PUBLIC_CTRL_DEF(&key_pin_ctrl[3], pin_level_set),    
};
#endif

#define KEY_CB_QUEUE_LENGTH 10

typedef struct t_keyEventCb
{
    void (*cbFun)(void);
} keyEventCb_t;

// static queue struct 
typedef struct t_queue{
    keyEventCb_t * pBase;      //memory pool pointer
    int front;                 //queue  front
    int rare;                  //queue  rare
} queue_t,*pQ;  

static keyEventCb_t KeyCbMapList[KEY_CB_QUEUE_LENGTH];//Create a static queue memory pool

static queue_t keyCbQueue = //Create a static queue object
{
    .pBase = KeyCbMapList,
    .front = 0,
    .rare = 0,
};

static void keyCbMapListInit(void)
{
    for(int i = 0;i<GET_ARRAY_SIZE(KeyCbMapList)-1;i++)
    {
        KeyCbMapList[i].cbFun = (void*) NULL;
    }
    
}
static bool Is_Full(void) //Queue full judge
{
    if ( (keyCbQueue.rare + 1) % GET_ARRAY_SIZE(KeyCbMapList) == keyCbQueue.front )
        return true;
    else
        return false;
}

static bool Is_Empty(void) //Queue empty judge
{
    if (keyCbQueue.front == keyCbQueue.rare)
        return true;
    else
        return false;
}


static int KeyCbFunQueuePush(void (*cbFun)(void))
{
    if (Is_Full()) //Queue over full dont push
    {
        platform_printf("F\n");
        return -1;
    }
    keyCbQueue.pBase[keyCbQueue.rare].cbFun = cbFun;
    keyCbQueue.rare = (keyCbQueue.rare + 1) % GET_ARRAY_SIZE(KeyCbMapList);
    
    return 1;
}
static int KeyCbFunQueuePop()
{
    if(Is_Empty())
    {
        return (-1);
    }
    keyCbQueue.pBase[ keyCbQueue.front].cbFun();
    keyCbQueue.front = (keyCbQueue.front + 1) %  GET_ARRAY_SIZE(KeyCbMapList);
    return 1;
}
static bool repeat_flag = 0;
static void kb_check_event_callback(void)
{
    bool bFlag = false;
    static uint8_t com_count = 0;
    BaseType_t xReturn = pdPASS;
    for(int i = 0;i < GET_ARRAY_SIZE(keyFuncMap);i++)
    {
        if(KEY_PRESS_MULTI == keyFuncMap[i].state)
        {
            if(keyFuncMap[i].multiType == key_check_state(keyFuncMap[i].keyHardId, keyFuncMap[i].state))
            {
                bFlag = true;
            }
        }
        else
        {
            if(key_check_state(keyFuncMap[i].keyHardId,KEY_RELEASE))
            {
                com_count--;
            }
            if(key_check_state(keyFuncMap[i].keyHardId, keyFuncMap[i].state))
            {
                bFlag = true;  
                if(KEY_PRESS == keyFuncMap[i].state) com_count++;
            }
        }
       if(bFlag)
       {
            if(CB_FUN_NULL != keyFuncMap[i].cbFun)
            {
                KeyCbFunQueuePush(keyFuncMap[i].cbFun);
            }   
            if(KB_NULL != keyFuncMap[i].key.keyId)
            {
                if((NULL != xKeyQueue))
                {
                  xReturn = xQueueSendToBackFromISR( xKeyQueue, 
                                        &keyFuncMap[i].key,
                                        NULL );        
                  if(pdPASS != xReturn)
                      platform_printf("send error\n");
                }
            }
            bFlag = false;
       }
    
    }
    
    repeat_flag = (1 == com_count) ?  true : false; 

    for(int i = 0;i < GET_ARRAY_SIZE(keyCombineMap);i++)
    {
        if(keyCombineMap[i].combineKeyId && key_check_combine_state(keyCombineMap[i].combineKeyId))
        {
            if(CB_FUN_NULL != keyCombineMap[i].cbFun)
            {
                KeyCbFunQueuePush(keyCombineMap[i].cbFun);
            }
            if(KB_NULL != keyCombineMap[i].key.keyId) 
            {           
                if((NULL != xKeyQueue))
                {
                  xReturn = xQueueSendToBackFromISR( xKeyQueue, 
                                        &keyCombineMap[i].key,
                                        NULL );        
                  if(pdPASS != xReturn)
                      platform_printf("send error\n");
                }
            }    
        }
    
    }
}
static void combine_register(void)
{
    for(int i = 0;i < GET_ARRAY_SIZE(keyCombineMap);i++)
    {
        if(NULL != keyCombineMap[i].pCombineTab)
        {
             keyCombineMap[i].combineKeyId = key_combine_register(keyCombineMap[i].pCombineTab, keyCombineMap[i].size);
        }
    }
}
static TimerHandle_t complexKeyTimer = 0;
#if(KEY_TRIG_QUERY == KEY_EVENT_TRIG_MODE)   
void (*cbFunSwTimer) (void);
#endif
#include "../user_ir_remote.h"
static void ComplexKeyCallback(TimerHandle_t xTimer)
{
    static uint16_t repeat_tick = 0;
#if(KEY_TRIG_QUERY == KEY_EVENT_TRIG_MODE)      
    if(cbFunSwTimer)
        cbFunSwTimer();
#endif    
    KeyCbFunQueuePop();
    if(repeat_flag)
    {
        if(!(++repeat_tick % 7))
        {
            t_ir.transmit_repeat();
        }
    
    }
    else{
        repeat_tick = 0;
    }
}

#if USER_KEY_DEBUG
void key_print_debug_callback(const char *str)
{
    platform_printf("%s\n", str);
}
#endif

#define QUEUE_LENGTH 10
 
#define ITEM_SIZE sizeof( key_t )
 
static StaticQueue_t xStaticQueue;
 
static uint8_t ucKeyQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ]; 
 
QueueHandle_t xKeyQueue = NULL;
static void KeyMsgQueueCreate(void)
{
    xKeyQueue = xQueueCreateStatic( QUEUE_LENGTH, 
                                 ITEM_SIZE,
                                 ucKeyQueueStorageArea,
                                 &xStaticQueue );    
    if(NULL == xKeyQueue)
    {
        platform_printf("error create\n");
    }
    xQueueReset(xKeyQueue);
}

key_t key_id_get(void)
{
    BaseType_t xReturn = pdTRUE;
    key_t r_key;
    xReturn = xQueueReceive( xKeyQueue,&r_key, portMAX_DELAY); 
    if(pdTRUE != xReturn)
    {
        r_key.keyId = KB_NULL;
        r_key.IrkeyId = KB_IR_NULL;
    }        
    return r_key;
}


/*key timer init*/
static uint32_t timer1_isr(void *user_data);
static void init_timer1(void)
{
    SYSCTRL_ClearClkGateMulti(0
                                | (1 << SYSCTRL_ClkGate_APB_TMR1));
#if (INGCHIPS_FAMILY == INGCHIPS_FAMILY_918)
    // setup timer 1: 40us (25kHz)
    TMR_SetCMP(APB_TMR1, TMR_CLK_FREQ / 25000);
    TMR_SetOpMode(APB_TMR1, TMR_CTL_OP_MODE_WRAPPING);
    TMR_IntEnable(APB_TMR1);
    TMR_Reload(APB_TMR1);
    TMR_Enable(APB_TMR1);
#elif (INGCHIPS_FAMILY == INGCHIPS_FAMILY_916)
    SYSCTRL_SelectTimerClk(TMR_PORT_1, SYSCTRL_TMR_CLK_OSC_DIV_4); 
    TMR_SetOpMode(APB_TMR1, 0, TMR_CTL_OP_MODE_32BIT_TIMER_x1, TMR_CLK_MODE_EXTERNAL, 0);
    TMR_SetReload(APB_TMR1, 0, TMR_GetClk(APB_TMR1,0) / KEY_SCAN_RATE);
    TMR_IntEnable(APB_TMR1, 0, 0x1);
    TMR_Enable(APB_TMR1, 0, 0x1);
#else
    #error unknown or unsupported chip family
#endif
	platform_set_irq_callback(PLATFORM_CB_IRQ_TIMER1, timer1_isr, NULL);
}

static uint32_t timer1_isr(void *user_data)
{
    key_check();
#if (INGCHIPS_FAMILY == INGCHIPS_FAMILY_918)
    TMR_IntClr(APB_TMR1);
#elif (INGCHIPS_FAMILY == INGCHIPS_FAMILY_916)
    TMR_IntClr(APB_TMR1, 0, 0x1);
#else
    #error unknown or unsupported chip family
#endif
    return 0;
}

void GPIO_Key_Board_Init(void)
{
    unsigned int i;
    SYSCTRL_ClearClkGateMulti(0 | (1 << SYSCTRL_ClkGate_APB_GPIO0)
                                | (1 << SYSCTRL_ClkGate_APB_GPIO1)
                                | (1 << SYSCTRL_ClkGate_APB_PinCtrl));

    for(i = 0;i < GET_ARRAY_SIZE(key_pin_sig);i++)
    {
        PINCTRL_SetPadMux(key_pin_sig[i].pin, IO_SOURCE_GPIO);
        GIO_SetDirection(key_pin_sig[i].pin, GIO_DIR_INPUT);        
    }

#if (USER_KEY_BOARD_MATRIX)
    for(i = 0;i < GET_ARRAY_SIZE(key_pin_ctrl);i++)
    {
        PINCTRL_SetPadMux(key_pin_ctrl[i].pin, IO_SOURCE_GPIO);        
        GIO_SetDirection(key_pin_ctrl[i].pin, GIO_DIR_OUTPUT);
        GIO_WriteValue(key_pin_ctrl[i].pin, 1);
    }
#endif
    key_board_init();
    init_timer1();
#if (USER_KEY_BOARD_MATRIX)
    #if (KEY_EVENT_TRIG_MODE == KEY_TRIG_REPORT)
    key_board_register(KEY_BOARD_MATRIX, key_public_sig, GET_ARRAY_SIZE(key_public_sig), key_public_ctrl, GET_ARRAY_SIZE(key_public_ctrl),kb_check_event_callback);
    #else
    key_board_register(KEY_BOARD_MATRIX, key_public_sig, GET_ARRAY_SIZE(key_public_sig), key_public_ctrl, GET_ARRAY_SIZE(key_public_ctrl),CB_FUN_NULL);
    #endif
#else
    key_board_register(KEY_BOARD_NORMAL, key_public_sig, GET_ARRAY_SIZE(key_public_sig), NULL, 0,CB_FUN_NULL);
#endif
#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    combine_register();
#endif    
#if USER_KEY_DEBUG    
    key_board_debug_register(key_print_debug_callback);
#endif    
    KeyMsgQueueCreate();
    keyCbMapListInit();
#if(KEY_TRIG_QUERY == KEY_EVENT_TRIG_MODE)  
    cbFunSwTimer = kb_check_event_callback;   
#endif
    complexKeyTimer = xTimerCreate("Complex Key",
                        pdMS_TO_TICKS(COMPLEX_TIMER_INTERVAL),
                        pdTRUE,
                        NULL,
                        ComplexKeyCallback);
    xTimerStart(complexKeyTimer, portMAX_DELAY);

}
