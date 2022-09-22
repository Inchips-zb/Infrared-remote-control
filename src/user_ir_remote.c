#include <stdlib.h>
#include "btstack_event.h"
#include "btstack_defines.h"
#include "platform_api.h"
#include "user_ir_remote.h"
#include "profile.h"
#define FCLK            32000
#define NEC_WAVE_FREQ   38000
#define RC5_WAVE_FREQ   36000
#if 1
#define IR_WAVE_NEC_TC9012_FREQ (OSC_CLK_FREQ/NEC_WAVE_FREQ)    
#define IR_WAVE_RC5_FREQ (OSC_CLK_FREQ/RC5_WAVE_FREQ)   
#define NEC_UINT    (FCLK*560/1000000+1)
#define TC9012_UINT (FCLK*260/1000000+1)
#define RC5_UINT    (FCLK*889/1000000+1)
#else
#define IR_WAVE_FREQ (35)    
#define UINT    (1)
#endif
#define INESSENTIAL     0
typedef struct
{
    uint16_t timer1;
    uint16_t timer2;
    uint16_t timer3;
    uint16_t timer4;
    uint16_t timer5;
    uint16_t btimer1;
    uint16_t btimer2;
    uint16_t bit_cycle;
    uint16_t carry_low;
    uint16_t carry_high;
}Ir_mode_param_t;

typedef struct{
    Ir_mode_param_t param_tx;
    Ir_mode_param_t param_rx;
}Ir_type_param_t;
//
const static Ir_type_param_t t_ir_type_param_table[] = 
{
    {//NEC param
        {  //TX
            16*NEC_UINT-1,4*NEC_UINT-1,8*NEC_UINT-1,1*NEC_UINT-1,INESSENTIAL,
            2*NEC_UINT-1, 4*NEC_UINT-1,1*NEC_UINT-1,IR_WAVE_NEC_TC9012_FREQ*2/3,IR_WAVE_NEC_TC9012_FREQ*1/3},
        {   //RX
            14*NEC_UINT-1,18*NEC_UINT-1,22*NEC_UINT-1,26*NEC_UINT-1,0xfff,
            INESSENTIAL,2*NEC_UINT-1,0x7f,INESSENTIAL,INESSENTIAL},    
    },
    {//TC9012 param
        {   //TX
            16*TC9012_UINT-1,8*TC9012_UINT-1,16*TC9012_UINT-1,2*TC9012_UINT-1,INESSENTIAL,
            4*TC9012_UINT-1,6*TC9012_UINT-1,2*TC9012_UINT-1,IR_WAVE_NEC_TC9012_FREQ*2/3,IR_WAVE_NEC_TC9012_FREQ*1/3},
        {    //RX
            28*TC9012_UINT-1,36*TC9012_UINT-1,44*TC9012_UINT-1,32*TC9012_UINT-1,0xfff,INESSENTIAL,
            4*TC9012_UINT-1,0x7f,INESSENTIAL,INESSENTIAL},    
    },    
    {//RC5 param
        {   //TX
            2*RC5_UINT-1,2*RC5_UINT-1,INESSENTIAL,INESSENTIAL,INESSENTIAL,2*RC5_UINT-1,
            INESSENTIAL,1*RC5_UINT,IR_WAVE_RC5_FREQ*2/3,IR_WAVE_RC5_FREQ*1/3},
        
        {   //RX
            1*RC5_UINT-2,1*RC5_UINT,3*RC5_UINT-1,5*RC5_UINT-1,INESSENTIAL,1*RC5_UINT-3,
            1*RC5_UINT-1,2*RC5_UINT-1,INESSENTIAL,INESSENTIAL},    
    }
};
ir_t t_ir;
static ir_key_t t_ir_key;
//
static ir_key_t get_ir_key();
static int ir_transmit_repeat(void);
static int ir_transmit_fun(uint16_t addr,uint16_t data);
static void user_ir_device_init(IR_IrMode_e mode,IR_TxRxMode_e tx_rx_mode,GIO_Index_t irpin);

#if !IR_RX_HARD
static ir_rx_data_struct_t t_ir_data_struct;
static void ir_soft_rxpin_init(GIO_Index_t channel_index);
#endif

 #if INGCHIPS_FAMILY == INGCHIPS_FAMILY_916   
static uint32_t IRQHandler_IR_INT(void *user_data)
{
    uint16_t addr,data;
    if(IR_FsmGetIrTransmitOk(APB_IR))
        ;//platform_printf("int ir send ok\n");
    if(IR_FsmGetIrTxRepeat(APB_IR))
        ;//platform_printf("int ir repeat ok\n");
#if IR_RX_HARD
    if(IR_FsmGetIrReceivedOk(APB_IR))
    {
        t_ir_key.value = IR_RxCodeGetIrRxUsercode(APB_IR) <<16 | IR_RxCodeGetIrRxDatacode(APB_IR);
        t_ir_key.state = KEY_STATE_PRESS;
        btstack_push_user_msg(USER_MSG_ID_IR_KEY, NULL,0);
        platform_printf("Received:0x%x,0x%x\n",t_ir_key.value,t_ir_key.state);
    }
    
    if(IR_FsmGetIrRepeat(APB_IR))
    {
        t_ir_key.state = KEY_STATE_REPEAT;
        btstack_push_user_msg(USER_MSG_ID_IR_KEY, NULL,0);
    }
#endif
    return 0;
}
#endif

static void user_ir_device_init(IR_IrMode_e mode,IR_TxRxMode_e tx_rx_mode,GIO_Index_t irpin)
{
    uint8_t sysctrl_clkgate_gpio = SYSCTRL_ClkGate_APB_GPIO0;
#if 0 == IR_RX_HARD
    if(IR_txrx_mode_rx_mode == tx_rx_mode)
    {
        platform_printf("Ir rx soft\n");
        t_ir_data_struct.symbol_num  = 0;
        ir_soft_rxpin_init(irpin);
        return;
    }
#endif
#if INGCHIPS_FAMILY == INGCHIPS_FAMILY_916   
  //  platform_printf("UNIT=%d,APB_38K=%d\n",UINT,IR_WAVE_FREQ);
    IR_CtrlSetIrMode(APB_IR,mode);
    IR_CtrlSetTxRxMode(APB_IR,tx_rx_mode);
    IR_CtrlSetIrIntEn(APB_IR);
    if(irpin > GIO_GPIO_17)
    {
        sysctrl_clkgate_gpio = SYSCTRL_ClkGate_APB_GPIO1;
    }
    SYSCTRL_ClearClkGateMulti(0                                    
                                | (1 << sysctrl_clkgate_gpio)
                                | (1 << SYSCTRL_ITEM_APB_IR)
                                | (1 << SYSCTRL_ClkGate_APB_PinCtrl));
    if(IR_txrx_mode_tx_mode == tx_rx_mode)
    {
        platform_printf("IR mode TX\n");
        IR_TxConfigIrTxPol(APB_IR);
        IR_TxConfigCarrierCntClr(APB_IR);
        IR_TxConfigIrIntEn(APB_IR);
        IR_CarryConfigSetIrCarryLow(APB_IR,t_ir_type_param_table[mode].param_tx.carry_low);   //30% pwm
        IR_CarryConfigSetIrCarryHigh(APB_IR,t_ir_type_param_table[mode].param_tx.carry_high);
        IR_TimeSetIrTime1(APB_IR,t_ir_type_param_table[mode].param_tx.timer1);
        IR_TimeSetIrTime2(APB_IR,t_ir_type_param_table[mode].param_tx.timer2);
        IR_TimeSetIrTime3(APB_IR,t_ir_type_param_table[mode].param_tx.timer3 );
        IR_TimeSetIrTime4(APB_IR,t_ir_type_param_table[mode].param_tx.timer4); 
        IR_CtrlIrSetBitTime1(APB_IR,t_ir_type_param_table[mode].param_tx.btimer1);
        IR_CtrlIrSetBitTime2(APB_IR,t_ir_type_param_table[mode].param_tx.btimer2);    
        IR_CtrlIrSetIrBitCycle(APB_IR,t_ir_type_param_table[mode].param_tx.bit_cycle);        
        PINCTRL_SetPadMux(irpin,IO_SOURCE_IR_DATA_OUT);
    }
    else{
        platform_printf("IR mode RX\n");
        IR_CtrlSetIrEndDetectEn(APB_IR);//end code detect en
        IR_CtrlSetIrIntVerifyEn(APB_IR);
        IR_CtrlIrUsercodeVerify(APB_IR);
        IR_CtrlIrDatacodeVerify(APB_IR);
        IR_TimeSetIrTime1(APB_IR,t_ir_type_param_table[mode].param_rx.timer1);
        IR_TimeSetIrTime2(APB_IR,t_ir_type_param_table[mode].param_rx.timer2);
        IR_TimeSetIrTime3(APB_IR,t_ir_type_param_table[mode].param_rx.timer3);
        IR_TimeSetIrTime4(APB_IR,t_ir_type_param_table[mode].param_rx.timer4); 
        IR_TimeSetIrTime5(APB_IR,t_ir_type_param_table[mode].param_rx.timer5); 
        IR_CtrlIrSetBitTime1(APB_IR,t_ir_type_param_table[mode].param_rx.btimer1);
        IR_CtrlIrSetBitTime2(APB_IR,t_ir_type_param_table[mode].param_rx.btimer2);    
        IR_CtrlIrSetIrBitCycle(APB_IR,t_ir_type_param_table[mode].param_rx.bit_cycle);   
        PINCTRL_SetPadMux(irpin,IO_SOURCE_IR_DATA_IN);
    }
    IR_CtrlEnable(APB_IR);
    platform_set_irq_callback(PLATFORM_CB_IRQ_IR_INT, IRQHandler_IR_INT, NULL);
 #endif
}


#if !IR_RX_HARD

// NEC soft decode
static int NEC_Decode(ir_rx_symbol_t symbol_buf[], int symbol_num,ir_key_t *ir_key)
{
    unsigned int key_value;
    static uint8_t decode_complete_flag = 0;
    if(symbol_num == 33) 
    {
	    key_value = 0;
	    if ((symbol_buf[0].symbol_period > (4500 - 300)) &&  //symbol_buf[0] storage Preamble Code
            (symbol_buf[0].symbol_period <(4500 + 300))&&
            (symbol_buf[0].mark_period >(9000 - 300))&&
            (symbol_buf[0].mark_period <(9000 + 300))
        )
        {
            for(int i = (symbol_num-1); i >= 1; i--)
            {
                if( (symbol_buf[i].symbol_period <(560-200)) ||  // error symbol return -1
                    (symbol_buf[i].symbol_period > (1680+200)))
                {
                    platform_printf("symbol %d is invalid,%d \n", i,symbol_buf[i].symbol_period);
                    return -1;
                }
                else
                {
                    key_value <<= 1;
                    if( (symbol_buf[i].symbol_period > (1680-200)) && 
                        (symbol_buf[i].symbol_period <= (1680+200)) )
                    {
                        // get bit 1
                        key_value |= 1;
                    }
                 }
               }
            //if verify
      //  if((((key_value&0xff)^((key_value>>8)&0xff)) == 0xff)&&((((key_value>>16)&0xff)^((key_value>>24)&0xff)) == 0xff))
            {
                ir_key->value = key_value;
                ir_key->state = KEY_STATE_PRESS;
                decode_complete_flag = 1;
            }
        }
    }
    else if(symbol_num == 1) //is repeat code
    {
       // ir_key->value = 0;
        ir_key->state = KEY_STATE_REPEAT;
        decode_complete_flag = 1;
    }
    else // null
    {
       // ir_key->value = 0;
        ir_key->state = KEY_STATE_NONE;
        return 0;
    }
    //platform_printf("num:%d \n",symbol_num);
    if(decode_complete_flag)
    {
        btstack_push_user_msg(USER_MSG_ID_IR_KEY, NULL,0);
    }
    return 1;
}

static uint32_t gpio_isr(void *user_data)
{
    uint64_t current_tick = platform_get_us_time();//record into times
    static uint64_t last_tick = 0;
    uint32_t current = GIO_GetAllIntStatus(); //get io interrupt status
    if (current & (1 << t_ir_data_struct.rx_io_pin)) //if ir rx int
    {            
        if(GIO_ReadValue(t_ir_data_struct.rx_io_pin)) //Rising edge
        {
             //get mark peroid
            t_ir_data_struct.t_ir_rx_symbol[t_ir_data_struct.symbol_num].mark_period = (uint16_t)( current_tick-last_tick);
            if( (t_ir_data_struct.t_ir_rx_symbol[t_ir_data_struct.symbol_num].mark_period >= 7000)&&
                (t_ir_data_struct.t_ir_rx_symbol[t_ir_data_struct.symbol_num].mark_period <= 11000))
            { // receive 9000ms Preamble Code
                t_ir_data_struct.t_ir_rx_symbol[0].mark_period = t_ir_data_struct.t_ir_rx_symbol[t_ir_data_struct.symbol_num].mark_period ;
                t_ir_data_struct.symbol_num = 0;
            }
        }
        else //Falling edge
        {
             //get symbol peroid            
            t_ir_data_struct.t_ir_rx_symbol[t_ir_data_struct.symbol_num].symbol_period = (uint16_t)( current_tick-last_tick) ;           
            //if symbol peroid between 4000 and 5000ms, considered is start signal,or between 2000 and 2500ms,considered is repeat code
            if((((4000 < t_ir_data_struct.t_ir_rx_symbol[0].symbol_period)  &&
                (5000 > t_ir_data_struct.t_ir_rx_symbol[0].symbol_period))  ||
                ((t_ir_data_struct.t_ir_rx_symbol[0].symbol_period > 2000)  &&
                (t_ir_data_struct.t_ir_rx_symbol[0].symbol_period < 2500))) &&
                (0 == t_ir_data_struct.symbol_num))    
            {
                t_ir_data_struct.symbol_num = 1;
                
                if( (t_ir_data_struct.t_ir_rx_symbol[0].symbol_period >= 2000)&&
                    (t_ir_data_struct.t_ir_rx_symbol[0].symbol_period <= 2500))
                {
                    t_ir_data_struct.rev_complete = 1;
                }
            }
            else if(t_ir_data_struct.symbol_num) // data receive
            {
                if(++t_ir_data_struct.symbol_num == 33)
                {
                    t_ir_data_struct.t_ir_rx_symbol[t_ir_data_struct.symbol_num].symbol_period = 0;
                    t_ir_data_struct.rev_complete = 1;
                }
            }
        }
        if(1 == t_ir_data_struct.rev_complete) //receive is completed
        {
            NEC_Decode(&t_ir_data_struct.t_ir_rx_symbol[0],t_ir_data_struct.symbol_num,&t_ir_key);
            t_ir_data_struct.symbol_num = 0;
            t_ir_data_struct.rev_complete = 0;
        }
        last_tick = current_tick;
    }
    GIO_ClearAllIntStatus();
    return 0;
}
static void ir_soft_rxpin_init(GIO_Index_t channel_index)
{
    uint8_t sysctrl_clkgate_gpio = SYSCTRL_ClkGate_APB_GPIO0;
    platform_irq_callback_type_t gpio_irq = PLATFORM_CB_IRQ_GPIO0;
    t_ir_data_struct.rx_io_pin = channel_index;
#if INGCHIPS_FAMILY == INGCHIPS_FAMILY_916
    if(t_ir_data_struct.rx_io_pin > GIO_GPIO_17)
    {
        sysctrl_clkgate_gpio = SYSCTRL_ClkGate_APB_GPIO1;
        gpio_irq = PLATFORM_CB_IRQ_GPIO1;
    }
#endif
	SYSCTRL_ClearClkGateMulti(0 | (1 << sysctrl_clkgate_gpio)
                                | (1 << SYSCTRL_ClkGate_APB_PinCtrl));
    PINCTRL_SetPadMux(t_ir_data_struct.rx_io_pin, IO_SOURCE_GPIO);
    GIO_SetDirection(t_ir_data_struct.rx_io_pin, GIO_DIR_INPUT);
    GIO_ConfigIntSource(t_ir_data_struct.rx_io_pin,
                        GIO_INT_EN_LOGIC_LOW_OR_FALLING_EDGE | GIO_INT_EN_LOGIC_HIGH_OR_RISING_EDGE,
                        GIO_INT_EDGE);
    platform_set_irq_callback(gpio_irq, gpio_isr, NULL);
}
#endif

static ir_key_t get_ir_key(void)
{
    return t_ir_key;
}

static int ir_transmit_fun(uint16_t addr,uint16_t data) //ir hard transmit data
{
    IR_TxCodeSetIrTxUsercode(APB_IR,addr);
    IR_TxCodeSetIrTxDatacode(APB_IR,data);
    IR_CleanIrTxRepeatMode(APB_IR);//must clearn the repeat mode reg
    IR_TxConfigTxStart(APB_IR);
    //while(!IR_FsmGetIrTransmitOk(APB_IR));
    return 0;
}
static int ir_transmit_repeat(void) //ir hard transmit repeat
{
    IR_CtrlIrTxRepeatMode(APB_IR);   
    IR_TxConfigTxStart(APB_IR);
 //   while(!IR_FsmGetIrTransmitOk(APB_IR)); 
    return 0;
}

ir_t t_ir = 
{
    .init = user_ir_device_init,
    .key_get = get_ir_key,
    .transmit_data = ir_transmit_fun,
    .transmit_repeat = ir_transmit_repeat,
};
