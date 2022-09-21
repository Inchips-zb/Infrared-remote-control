#ifndef _USER_IR_REMOTE_H_
#define _USER_IR_REMOTE_H_

#include <stdint.h>
#include "peripheral_ir.h"
#include "peripheral_pinctrl.h"

#define IR_RX_HARD          0

#define KEY_STATE_NONE      (0x00)
#define KEY_STATE_PRESS     (0xFA)
#define KEY_STATE_REPEAT    (0xFB)

typedef struct ir_key_s
{
    uint32_t value;
    int state;
}ir_key_t;

typedef struct ir_rx_symbol_s
{
    uint16_t mark_period;
    uint16_t symbol_period;
} ir_rx_symbol_t;

typedef struct ir_rx_data_struct_s
{
    ir_rx_symbol_t t_ir_rx_symbol[34];
    int symbol_num;
    uint8_t rev_complete;
    GIO_Index_t rx_io_pin;
}ir_rx_data_struct_t;

typedef struct ir_struct_s{
    ir_key_t (*key_get) (void);
    int (*transmit_repeat) (void);
    int (*transmit_data) (uint16_t,uint16_t);
    void (*init) (IR_IrMode_e,IR_TxRxMode_e,GIO_Index_t);
}ir_t;
extern ir_t t_ir;
#endif