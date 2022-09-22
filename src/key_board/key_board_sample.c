#include "key_board_sample.h"
#include "platform_api.h"
#include "btstack_event.h"
#include "../profile.h"

typedef uint8_t keyFunc_t;

typedef struct
{
    keyFunc_t ketHardId;
    enum key_state_t state;
    keyFunc_t keyId;
    eClickState multiType;
    void (*cbFun)(void);
} keyFuncMap_t;

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
typedef struct
{
    int combineKeyId;
    const struct key_combine_t *pCombineTab;
    uint8_t size;
    keyFunc_t keyId;
    void (*cbFun)(void);
} keyCombineMap_t;

static const struct key_combine_t key_ir_learn[] = {
    { .id = KB_HARD_K1,   .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K5,   .state = KEY_PRESS_LONG },
};
static const struct key_combine_t key_adv_start[] = {
    { .id = KB_HARD_K9,   .state = KEY_PRESS },
    { .id = KB_HARD_K9,  .state = KEY_RELEASE },
};
static const struct key_combine_t key_adv_stop[] = {
    { .id = KB_HARD_K10,   .state = KEY_PRESS },
    { .id = KB_HARD_K10,   .state = KEY_RELEASE },
};
static const struct key_combine_t key_power_on[] = {
    { .id = KB_HARD_K11,  .state = KEY_PRESS },
    { .id = KB_HARD_K11,  .state = KEY_RELEASE },
};
static const struct key_combine_t key_power_off[] = {
    { .id = KB_HARD_K12,  .state = KEY_PRESS },
    { .id = KB_HARD_K12,  .state = KEY_RELEASE },
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
    {0,  key_ir_learn,  GET_ARRAY_SIZE(key_ir_learn),    KB_NULL,   cmbinetest1}, // combine1
    {0,  key_adv_start, GET_ARRAY_SIZE(key_adv_start),   KB_NULL,   cmbinetest2}, // combine2
    {0,  key_adv_stop,  GET_ARRAY_SIZE(key_adv_stop),    KB_NULL,   cmbinetest3}, // combine3
    {0,  key_power_on,  GET_ARRAY_SIZE(key_power_on),    KB_NULL,   cmbinetest4}, // combine4
    {0,  key_power_off, GET_ARRAY_SIZE(key_power_off),   KB_NULL,   cmbinetest5}, // combine5
};
#endif
static void test16_press_long(void)
{
    platform_printf("k16 press_long\n");
}
static void test16_triple_click_press(void)
{
    platform_printf("k16 triple_click_press\n");
}
static const keyFuncMap_t keyFuncMap[] = {
   //hard id       triger state   key_event id          key describe         
    {KB_HARD_K1,    KEY_PRESS,      KB_LEFT        ,      MULTI_CLICK_NONE,  NULL     }, // J1
    {KB_HARD_K2,    KEY_PRESS,      KB_RIGHT       ,      MULTI_CLICK_NONE,  NULL     }, // J2
    {KB_HARD_K3,    KEY_PRESS,      KB_UP          ,      MULTI_CLICK_NONE,  NULL     }, // J3
    {KB_HARD_K4,    KEY_PRESS,      KB_DOWN        ,      MULTI_CLICK_NONE,  NULL     }, // J4
    {KB_HARD_K5,    KEY_PRESS,      KB_ENTER       ,      MULTI_CLICK_NONE,  NULL     }, // J5
    {KB_HARD_K6,    KEY_PRESS,      KB_EXIT        ,      MULTI_CLICK_NONE,  NULL     }, // J6
    {KB_HARD_K7,    KEY_PRESS,      KB_POWER       ,      MULTI_CLICK_NONE,  NULL     }, // J7
    {KB_HARD_K8,    KEY_PRESS,      KB_MENU        ,      MULTI_CLICK_NONE,  NULL     }, // J8
    {KB_HARD_K9,    KEY_PRESS,      KB_HOME        ,      MULTI_CLICK_NONE,  NULL     }, // J9
    {KB_HARD_K10,   KEY_PRESS,      KB_VOICE       ,      MULTI_CLICK_NONE,  NULL     }, // J10
    {KB_HARD_K11,   KEY_PRESS,      KB_VOLUME_MUTE ,      MULTI_CLICK_NONE,  NULL     }, // J11
    {KB_HARD_K12,   KEY_PRESS,      KB_VOLUME_UP   ,      MULTI_CLICK_NONE,  NULL     }, // J12
    {KB_HARD_K13,   KEY_PRESS,      KB_VOLUME_DOWN ,      MULTI_CLICK_NONE,  NULL     }, // J13
    {KB_HARD_K14,   KEY_PRESS,      KB_VOICE_STOP  ,      MULTI_CLICK_NONE,  NULL     }, // J14
    {KB_HARD_K15,   KEY_PRESS,      KB_PAGE_UP     ,      MULTI_CLICK_NONE,  NULL     }, // J15
    {KB_HARD_K16,   KEY_PRESS,      KB_PAGE_DOWN   ,      MULTI_CLICK_NONE,  NULL     },  // J16
    {KB_HARD_K16,   KEY_PRESS_LONG, KB_NULL        ,      MULTI_CLICK_NONE,  test16_press_long   },  // J16
    {KB_HARD_K16,   KEY_PRESS_MULTI,KB_NULL        ,      TRIPLE_CLICK    ,  test16_triple_click_press   },  // J16
    
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
    KEY_PUBLIC_SIG_DEF(KB_HARD_K1, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K5, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K9, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KB_HARD_K13, &key_pin_sig[3], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
#if (USER_KEY_BOARD_MATRIX)
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
static void kb_check_event_callback(void)
{
    bool bFlag = false;
    for(int i = 0;i < GET_ARRAY_SIZE(keyFuncMap);i++)
    {
        if(KEY_PRESS_MULTI == keyFuncMap[i].state)
        {
            if(keyFuncMap[i].multiType == key_check_state(keyFuncMap[i].ketHardId, keyFuncMap[i].state))
            {
                bFlag = true;
            }
        }
        else
        {
            if(key_check_state(keyFuncMap[i].ketHardId, keyFuncMap[i].state))
            {
                bFlag = true;                
            }
        }
       if(bFlag)
       {
            if(NULL != keyFuncMap[i].cbFun)
                keyFuncMap[i].cbFun();
            if(KB_NULL != keyFuncMap[i].keyId)
                btstack_push_user_msg(USER_MSG_ID_HARD_KEY, &keyFuncMap[i].keyId,sizeof(keyFuncMap[i].keyId));
            bFlag = false;
       }
    
    }
    for(int i = 0;i < GET_ARRAY_SIZE(keyCombineMap);i++)
    {
        if(keyCombineMap[i].combineKeyId && key_check_combine_state(keyCombineMap[i].combineKeyId))
        {
            if(NULL != keyCombineMap[i].cbFun)keyCombineMap[i].cbFun();
            if(KB_NULL != keyCombineMap[i].keyId) 
                btstack_push_user_msg(USER_MSG_ID_HARD_KEY, &keyCombineMap[i].keyId,sizeof(keyCombineMap[i].keyId));
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

static void ComplexKeyCallback(TimerHandle_t xTimer)
{
    kb_check_event_callback();
}
#if USER_KEY_DEBUG
void key_print_debug_callback(const char *str)
{
    platform_printf("%s\n", str);
}
#endif
/*key GPIO init*/
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
#if (USER_KEY_BOARD_MATRIX)
    key_board_register(KEY_BOARD_MATRIX, key_public_sig, GET_ARRAY_SIZE(key_public_sig), key_public_ctrl, GET_ARRAY_SIZE(key_public_ctrl));
#else
    key_board_register(KEY_BOARD_NORMAL, key_public_sig, GET_ARRAY_SIZE(key_public_sig), NULL, 0);
#endif
#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    combine_register();
#endif    
#if USER_KEY_DEBUG    
    key_board_debug_register(key_print_debug_callback);
#endif    
    
    complexKeyTimer = xTimerCreate("Complex Key",
                        pdMS_TO_TICKS(COMPLEX_TIMER_INTERVAL),
                        pdTRUE,
                        NULL,
                        ComplexKeyCallback);
    xTimerStart(complexKeyTimer, portMAX_DELAY);
}
