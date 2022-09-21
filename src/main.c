#include <stdio.h>
#include <string.h>
#include "profile.h"
#include "ingsoc.h"
#include "platform_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "trace.h"
#include "peripheral_ir.h"
#include "user_ir_remote.h"
#include "key_board_sample.h"
static uint32_t cb_hard_fault(hard_fault_info_t *info, void *_)
{
    platform_printf("HARDFAULT:\nPC : 0x%08X\nLR : 0x%08X\nPSR: 0x%08X\n"
                    "R0 : 0x%08X\nR1 : 0x%08X\nR2 : 0x%08X\nP3 : 0x%08X\n"
                    "R12: 0x%08X\n",
                    info->pc, info->lr, info->psr,
                    info->r0, info->r1, info->r2, info->r3, info->r12);
    for (;;);
}

static uint32_t cb_assertion(assertion_info_t *info, void *_)
{
    platform_printf("[ASSERTION] @ %s:%d\n",
                    info->file_name,
                    info->line_no);
    for (;;);
}

static uint32_t cb_heap_out_of_mem(uint32_t tag, void *_)
{
    platform_printf("[OOM] @ %d\n", tag);
    for (;;);
}

#define PRINT_PORT    APB_UART0

uint32_t cb_putc(char *c, void *dummy)
{
    while (apUART_Check_TXFIFO_FULL(PRINT_PORT) == 1);
    UART_SendData(PRINT_PORT, (uint8_t)*c);
    return 0;
}

int fputc(int ch, FILE *f)
{
    cb_putc((char *)&ch, NULL);
    return ch;
}

void config_uart(uint32_t freq, uint32_t baud)
{
    UART_sStateStruct config;

    config.word_length       = UART_WLEN_8_BITS;
    config.parity            = UART_PARITY_NOT_CHECK;
    config.fifo_enable       = 1;
    config.two_stop_bits     = 0;
    config.receive_en        = 1;
    config.transmit_en       = 1;
    config.UART_en           = 1;
    config.cts_en            = 0;
    config.rts_en            = 0;
    config.rxfifo_waterlevel = 1;
    config.txfifo_waterlevel = 1;
    config.ClockFrequency    = freq;
    config.BaudRate          = baud;

    apUART_Initialize(PRINT_PORT, &config, 0);
}

/* USER CODE BEGIN 0 */
static int test_id1,test_id2,test_id3,test_id14,test_id4;

const struct key_combine_t test_combine1[] = {
    { .id = KB_HARD_K1,   .state = KEY_RELEASE },
    { .id = KB_HARD_K2,   .state = KEY_RELEASE },
};
const struct key_combine_t test_combine2[] = {
    { .id = KB_HARD_K8,   .state = KEY_PRESS },
    { .id = KB_HARD_K9,   .state = KEY_PRESS },
};
const struct key_combine_t test_combine3[] = {
    { .id = KB_HARD_K11,   .state = KEY_PRESS },
    { .id = KB_HARD_K12,   .state = KEY_PRESS },
};
const struct key_combine_t test_combine4[] = {
    { .id = KB_HARD_K4,   .state = KEY_PRESS_LONG },
    { .id = KB_HARD_K5,   .state = KEY_PRESS_LONG },
};
const struct key_combine_t test_combine14[] = {
    { .id = KB_HARD_K14,   .state = KEY_PRESS },
    { .id = KB_HARD_K15,   .state = KEY_PRESS },
};
/* USER CODE END 0 */

void key_print_debug_callback(const char *str)
{
    platform_printf("%s\n", str);
}
static uint32_t timer2_isr(void *user_data);
static void init_timer2(void)
{
    SYSCTRL_ClearClkGateMulti(0
                                | (1 << SYSCTRL_ClkGate_APB_TMR2));
#if (INGCHIPS_FAMILY == INGCHIPS_FAMILY_918)
    // setup timer 1: 40us (25kHz)
    TMR_SetCMP(APB_TMR2, TMR_CLK_FREQ / 25000);
    TMR_SetOpMode(APB_TMR2, TMR_CTL_OP_MODE_WRAPPING);
    TMR_IntEnable(APB_TMR2);
    TMR_Reload(APB_TMR2);
    TMR_Enable(APB_TMR2);
#elif (INGCHIPS_FAMILY == INGCHIPS_FAMILY_916)
    // setup channel 0 timer 1: 0.5s (2Hz)
    SYSCTRL_SelectTimerClk(TMR_PORT_2, SYSCTRL_TMR_CLK_32k); 
    TMR_SetOpMode(APB_TMR2, 0, TMR_CTL_OP_MODE_32BIT_TIMER_x1, TMR_CLK_MODE_EXTERNAL, 0);
    TMR_SetReload(APB_TMR2, 0, TMR_GetClk(APB_TMR2, 0) / 2);
    TMR_Enable(APB_TMR2, 0, 0xf);
    *(uint32_t *)(0x40003014) = 0x1;
    *(uint32_t *)(0x40003018) = 0x1;
    *(uint32_t *)(0x40003020) = 0x9;
    *(uint32_t *)(0x40003024) = (1000000*128/20);
    *(uint32_t *)(0x4000301c) = 0x1;

#else
    #error unknown or unsupported chip family
#endif
	platform_set_irq_callback(PLATFORM_CB_IRQ_TIMER1, timer2_isr, NULL);
}
static uint32_t timer2_isr(void *user_data)
{
    key_check();
    //platform_printf("t1\n");
#if (INGCHIPS_FAMILY == INGCHIPS_FAMILY_918)
    TMR_IntClr(APB_TMR2);
#elif (INGCHIPS_FAMILY == INGCHIPS_FAMILY_916)
    TMR_IntClr(APB_TMR2, 0, 0xf);
#else
    #error unknown or unsupported chip family
#endif

    return 0;
}
//extern int ir_transmit_fun(uint16_t addr,uint16_t data);
void setup_peripherals(void)
{
    SYSCTRL_ClearClkGateMulti(0
                            | (1 << SYSCTRL_ClkGate_APB_WDT));
    config_uart(OSC_CLK_FREQ, 115200);

    t_ir.init(IR_IR_MODE_IR_NEC,IR_txrx_mode_tx_mode,GIO_GPIO_32);
    // t_ir.init(IR_IR_MODE_IR_NEC,IR_MODE_TX,GIO_GPIO_9);
    GPIO_Key_Board_Init();
    key_board_debug_register(key_print_debug_callback);

 //   test_id2 = key_combine_register(test_combine2, GET_ARRAY_SIZE(test_combine2));
    //init_timer2();
    if (!IS_DEBUGGER_ATTACHED())
    {
        // Watchdog will timeout after ~20sec
      //  TMR_WatchDogEnable(TMR_CLK_FREQ * 10);
    }

}


uint32_t on_deep_sleep_wakeup(void *dummy, void *user_data)
{
    (void)(dummy);
    (void)(user_data);
    setup_peripherals();
    return 0;
}

uint32_t query_deep_sleep_allowed(void *dummy, void *user_data)
{
    (void)(dummy);
    (void)(user_data);
    // TODO: return 0 if deep sleep is not allowed now; else deep sleep is allowed
    return 0;
}
static void timer_task(void *pdata)
{
    for(;;)
    {
        key_check();
        //platform_printf("t0\n");
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
void xtimer_call(void);
static void watchdog_task(void *pdata)
{

    unsigned int res = 0;
    for (;;)
    {
        TMR_WatchDogRestart();
        vTaskDelay(pdMS_TO_TICKS(9000));
     }
}

trace_rtt_t trace_ctx = {0};


int app_main()
{
    static uint8_t sendflag = 0;
    uint32_t reg,count;
    platform_set_evt_callback(PLATFORM_CB_EVT_PROFILE_INIT, setup_profile, NULL);
    // setup handlers
    platform_set_evt_callback(PLATFORM_CB_EVT_HARD_FAULT, (f_platform_evt_cb)cb_hard_fault, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_ASSERTION, (f_platform_evt_cb)cb_assertion, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_HEAP_OOM, (f_platform_evt_cb)cb_heap_out_of_mem, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_ON_DEEP_SLEEP_WAKEUP, on_deep_sleep_wakeup, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_QUERY_DEEP_SLEEP_ALLOWED, query_deep_sleep_allowed, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_PUTC, (f_platform_evt_cb)cb_putc, NULL);
    setup_peripherals();

    xTaskCreate(watchdog_task,
           "w",
           configMINIMAL_STACK_SIZE,
           NULL,
           (configMAX_PRIORITIES - 1),
           NULL);
     xTaskCreate(timer_task,
           "t",
           configMINIMAL_STACK_SIZE,
           NULL,
           (configMAX_PRIORITIES - 1),
           NULL);   
    trace_rtt_init(&trace_ctx);
    platform_set_evt_callback(PLATFORM_CB_EVT_TRACE, (f_platform_evt_cb)cb_trace_rtt, &trace_ctx);
    // TODO: config trace mask
    platform_config(PLATFORM_CFG_TRACE_MASK, 0);
    return 0;
}
