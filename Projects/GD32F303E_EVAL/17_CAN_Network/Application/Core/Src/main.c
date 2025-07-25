/*!
    \file    main.c
    \brief   CAN network communication demo

    \version 2024-12-20, V3.0.1, demo for GD32F30x
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include <stdio.h>
#include "gd32f303e_eval.h"

extern FlagStatus receive_flag;
extern can_receive_message_struct receive_message;
can_trasnmit_message_struct transmit_message;
    
void nvic_config(void);
void led_config(void);
void gpio_config(void);
ErrStatus can_networking(void);
void can_networking_init(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint8_t i = 0;
    uint32_t timeout = 0xFFFF;
    uint8_t transmit_mailbox = 0;
    receive_flag = RESET;
    /* configure Tamper key */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_GPIO);
    /* configure GPIO */
    gpio_config();
    /* configure USART */
    gd_eval_com_init(EVAL_COM1);
    /* configure NVIC */
    nvic_config();
    /* configure leds */
    led_config();
    /* set all leds off */
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    gd_eval_led_off(LED5);
    /* initialize CAN and CAN filter */
    can_networking_init();
    
    /* enable CAN receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INTEN_RFNEIE0);
    
    /* initialize transmit message */
    transmit_message.tx_sfid = 0x7ab;
    transmit_message.tx_efid = 0x00;
    transmit_message.tx_ft = CAN_FT_DATA;
    transmit_message.tx_ff = CAN_FF_STANDARD;
    transmit_message.tx_dlen = 8;
    printf("\r\nplease press the Tamper key to transmit data!\r\n");
    while(1){
        /* waiting for the Tamper key pressed */
        while(0 == gd_eval_key_state_get(KEY_TAMPER)){
            transmit_message.tx_data[0] = 0xA0;
            transmit_message.tx_data[1] = 0xA1;
            transmit_message.tx_data[2] = 0xA2;
            transmit_message.tx_data[3] = 0xA3;
            transmit_message.tx_data[4] = 0xA4;
            transmit_message.tx_data[5] = 0xA5;
            transmit_message.tx_data[6] = 0xA6;
            transmit_message.tx_data[7] = 0xA7;
            printf("\r\n can0 transmit data:");
            for(i = 0; i < transmit_message.tx_dlen; i++){
                printf(" %02x", transmit_message.tx_data[i]);
            }
            
            /* transmit message */
            transmit_mailbox = can_message_transmit(CAN0, &transmit_message);
            /* waiting for transmit completed */
            timeout = 0xFFFF;
            while((CAN_TRANSMIT_OK != can_transmit_states(CAN0, transmit_mailbox)) && (0 != timeout)){
                timeout--;
            }
            /* waiting for Tamper key up */
            while(0 == gd_eval_key_state_get(KEY_TAMPER));
        }
        if(SET == receive_flag){
            gd_eval_led_toggle(LED2);
            receive_flag = RESET;
            printf("\r\n can0 receive data:");
            for(i = 0; i < receive_message.rx_dlen; i++){
                printf(" %02x", receive_message.rx_data[i]);
            }
        }
    }
}

/*!
    \brief      initialize CAN and filter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void can_networking_init(void)
{
    can_parameter_struct can_parameter;
    can_filter_parameter_struct can_filter;
    /* initialize CAN structures */
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
    /* initialize CAN register */
    can_deinit(CAN0);
    
    /* initialize CAN */
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = ENABLE;
    can_parameter.auto_wake_up = DISABLE;
    can_parameter.auto_retrans = ENABLE;
    can_parameter.rec_fifo_overwrite = DISABLE;
    can_parameter.trans_fifo_order = DISABLE;
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_7TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_2TQ;
    /* baudrate 1Mbps */
    can_parameter.prescaler = 6;
    can_init(CAN0, &can_parameter);

    /* initialize filter */
    /* CAN0 filter number */
    can_filter.filter_number = 0;

    /* initialize filter */    
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x0000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x0000;
    can_filter.filter_mask_low = 0x0000;  
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    /* configure CAN0 NVIC */
    nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn,0,0);
}

/*!
    \brief      configure the leds
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);
    gd_eval_led_init(LED5);
}

/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* enable can clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOD);
    
    /* configure CAN0 GPIO, CAN0_TX(PD1) and CAN0_RX(PD0) */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    
    gpio_init(GPIOD, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    
    gpio_pin_remap_config(GPIO_CAN_FULL_REMAP,ENABLE);
}

/* retarget the C library printf function to the usart */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t) ch);
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
    return ch;
}
