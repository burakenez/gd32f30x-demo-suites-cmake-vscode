/*!
    \file    main.c
    \brief   EXMC NandFlash demo

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
#include "gd32f303e_eval.h"
#include "exmc_nandflash.h"
#include <stdio.h>

/* NAND */
#define NAND_HY_MAKERID             (0xADU)
#define NAND_HY_DEVICEID            (0xF1U)
#define BUFFER_SIZE                 ((uint32_t)0x0400U)

nand_id_struct nand_id;
uint8_t txbuffer[BUFFER_SIZE], rxbuffer[BUFFER_SIZE];
__IO uint32_t writereadstatus = 0, status= 0;
uint32_t writereadaddr;
uint16_t zone, block, page, pageoffset;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t j = 0,k = 0;

    /* LED initialize */
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED4);

    /* config the USART */
    gd_eval_com_init(EVAL_COM1);

    /* config the EXMC access mode */
    exmc_nandflash_init(EXMC_BANK1_NAND);

    printf("\r\nNAND flash initialized!");

    /* read NAND ID */
    nand_read_id(&nand_id);

    printf("\r\nRead NAND ID!");

    /* print NAND ID */
    printf("\r\nNand flash ID:0x%X 0x%X 0x%X 0x%X\r\n",nand_id.maker_id,nand_id.device_id,
                                                       nand_id.third_id,nand_id.fourth_id);

    if((NAND_HY_MAKERID == nand_id.maker_id) && (NAND_HY_DEVICEID == nand_id.device_id)){
        /* set the read and write the address */
        zone = 0;
        block = 10;
        page = 0;
        pageoffset = 1200;
        writereadaddr = ((zone * NAND_ZONE_SIZE + block) * NAND_BLOCK_SIZE + page)
                        * NAND_PAGE_SIZE + pageoffset;

        /* whether address cross-border */
        if((writereadaddr + BUFFER_SIZE ) > NAND_MAX_ADDRESS){
            printf("\r\nAddress cross-border!");

            /* failure, light on LED4 */
            gd_eval_led_on(LED4);
            while(1);
        }

        /* fill writebuffer with 0x00.. */
        fill_buffer_nand(txbuffer, BUFFER_SIZE , 0x00);

        /* write data to nand flash */
        status = nand_write(writereadaddr, txbuffer, BUFFER_SIZE);
        if(NAND_OK == status){
            printf("\r\nWrite data successfully!");
        }else{
            printf("\r\nWrite data failure!");

            /* failure, light on LED4 */
            gd_eval_led_on(LED4);
            while(1);
        }

        /* read data from nand flash */
        status = nand_read(writereadaddr, rxbuffer, BUFFER_SIZE);
        if(NAND_OK == status){
            printf("\r\nRead data successfully!");
        }else{
            printf("\r\nRead data failure!");

            /* failure, light on LED4 */
            gd_eval_led_on(LED4);
            while(1);
        }

        printf("\r\nCheck the data!");
        /* read and write data comparison for equality */
        writereadstatus = 0;
        for(j = 0; j < BUFFER_SIZE; j++){
            if(txbuffer[j] != rxbuffer[j]){
                writereadstatus++;
                break;
            }
        }

        if (writereadstatus == 0){
            printf("\r\nAccess NAND flash successfully!");
            gd_eval_led_on(LED2);
        }else{
            printf("\r\nAccess NAND flash failure!");
            /* failure, light on LED4 */
            gd_eval_led_on(LED4);
            while(1);
        }
        printf("\r\nThe data to be read:\r\n");
        for(k = 0; k < BUFFER_SIZE; k ++){
            printf("0x%02X ",rxbuffer[k]);
            if(((k+1)%16) == 0){
                printf("\r\n");
            }
        }
    }else{
        printf("\r\nRead NAND ID failure!");

        /* failure, light on LED4 */
        gd_eval_led_on(LED4);
        while(1);
    }

    while(1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM1,USART_FLAG_TBE));
    return ch;
}
