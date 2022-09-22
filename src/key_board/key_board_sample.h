#ifndef __KEY_BOARD_SAMPLE_H
#define __KEY_BOARD_SAMPLE_H

#include "ingsoc.h"
#include "key_board.h"

//Enable this macro if it is a matrix keyboard
#define USER_KEY_BOARD_MATRIX 1

#define USER_KEY_DEBUG        0

#define COMPLEX_TIMER_INTERVAL     20

#define KB_NULL         0
#define KB_POWER        0x66
#define KB_PAGE_UP      0x4B
#define KB_PAGE_DOWN    0x4E
#define KB_MENU         0x76
#define KB_HOME         0x4A
#define KB_VOICE        0x3E
#define KB_ENTER        0x28        
#define KB_EXIT         0x29
#define KB_LEFT         0x50     
#define KB_RIGHT        0x4F
#define KB_UP           0x52
#define KB_DOWN         0x51
#define KB_VOLUME_MUTE  0x7F
#define KB_VOLUME_UP    0x80
#define KB_VOLUME_DOWN  0x81
#define KB_VOICE_STOP   0x3F

typedef enum
{   
    GPIO_PIN_RESET = 0u,
    GPIO_PIN_SET = 1u,    
}GPIO_PinState;
typedef enum
{   
    MULTI_CLICK_NONE = 0,
    DOUBLE_CLICK = 2u   ,
    TRIPLE_CLICK        , 
    FOUR_CLICK          ,   
    FIVE_CLICK          ,
    SIX_CLICK           ,
    SEVEN_CLICK         ,
    EIGHT,CLICK    
} eClickState;
enum key_id_hard_e {
    KB_HARD_K1 = 0, 
    KB_HARD_K2    , 
    KB_HARD_K3    , 
    KB_HARD_K4    , 
    KB_HARD_K5    , 
    KB_HARD_K6    , 
    KB_HARD_K7    , 
    KB_HARD_K8    , 
    KB_HARD_K9    , 
    KB_HARD_K10   , 
    KB_HARD_K11   , 
    KB_HARD_K12   ,
    KB_HARD_K13   , 
    KB_HARD_K14   ,
    KB_HARD_K15   , 
    KB_HARD_K16   , 
    KB_HARD_END
};
struct key_pin_t {
    GIO_Index_t pin;
    GPIO_PinState valid;
    GPIO_PinState invalid;
};
/*key bord init*/
void GPIO_Key_Board_Init(void);

#endif/*__KEY_BOARD_SAMPLE_H*/
