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
    const char *intro;
    uint8_t count;
    void (*cbFun)(void);
} keyFuncMap_t;
static keyFuncMap_t keyFuncMap[] = {
   //hard id     triger state  key_event id   key describe         
    {KB_HARD_K1,    KEY_PRESS, KB_LEFT        , "KB_LEFT"        ,0,NULL     }, // J1
    {KB_HARD_K2,    KEY_PRESS, KB_RIGHT       , "KB_RIGHT"       ,0,NULL     }, // J2
    {KB_HARD_K3,    KEY_PRESS, KB_UP          , "KB_UP"          ,0,NULL     }, // J3
    {KB_HARD_K4,    KEY_PRESS, KB_DOWN        , "KB_DOWN"        ,0,NULL     }, // J4
    {KB_HARD_K5,    KEY_PRESS, KB_ENTER       , "KB_ENTER"       ,0,NULL     }, // J5
    {KB_HARD_K6,    KEY_PRESS, KB_EXIT        , "KB_EXIT"        ,0,NULL     }, // J6
    {KB_HARD_K7,    KEY_PRESS, KB_POWER       , "KB_POWER"       ,0,NULL     }, // J7
    {KB_HARD_K8,    KEY_PRESS, KB_MENU        , "KB_MENU"        ,0,NULL     }, // J8
    {KB_HARD_K9,    KEY_PRESS, KB_HOME        , "KB_HOME"        ,0,NULL     }, // J9
    {KB_HARD_K10,   KEY_PRESS, KB_VOICE       , "KB_VOICE"       ,0,NULL     }, // J10
    {KB_HARD_K11,   KEY_PRESS, KB_VOLUME_MUTE , "KB_VOLUME_MUTE" ,0,NULL     }, // J11
    {KB_HARD_K12,   KEY_PRESS, KB_VOLUME_UP   , "KB_VOLUME_UP"   ,0,NULL     }, // J12
    {KB_HARD_K13,   KEY_PRESS, KB_VOLUME_DOWN , "KB_VOLUME_DOWN" ,0,NULL     }, // J13
    {KB_HARD_K14,   KEY_PRESS, KB_VOICE_STOP  , "KB_VOICE_STOP"  ,0,NULL     }, // J14
    {KB_HARD_K15,   KEY_PRESS, KB_PAGE_UP     , "KB_PAGE_UP"     ,0,NULL     }, // J15
    {KB_HARD_K16,   KEY_PRESS, KB_PAGE_DOWN   , "KB_PAGE_DOWN"   ,0,NULL     }  // J16
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
            if(keyFuncMap[i].count == key_check_state(keyFuncMap[i].ketHardId, keyFuncMap[i].state))
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
            if(keyFuncMap[i].cbFun)
                keyFuncMap[i].cbFun();
            btstack_push_user_msg(USER_MSG_ID_HARD_KEY, &keyFuncMap[i].keyId,sizeof(keyFuncMap[i].keyId));
            bFlag = false;
       }
    
    }

}
TimerHandle_t complexKeyTimer = 0;

static void ComplexKeyCallback(TimerHandle_t xTimer)
{
    kb_check_event_callback();
}

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
    complexKeyTimer = xTimerCreate("Complex Key",
                        pdMS_TO_TICKS(COMPLEX_TIMER_INTERVAL),
                        pdTRUE,
                        NULL,
                        ComplexKeyCallback);
    xTimerStart(complexKeyTimer, portMAX_DELAY);
}
