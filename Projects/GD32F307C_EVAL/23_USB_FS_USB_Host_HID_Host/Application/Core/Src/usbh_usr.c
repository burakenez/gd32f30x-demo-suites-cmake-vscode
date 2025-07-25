/*!
    \file    usbh_usr.c
    \brief   some user routines

    \version 2024-12-20, V3.0.1, firmware for GD32F30x
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

#include "lcd_log.h"
#include "usbh_usr.h"
#include "usbh_standard_hid.h"
#include "usb_lcd_conf.h"
#include "drv_usb_hw.h"
#include <string.h>

extern int16_t XLoc, YLoc;
extern __IO int16_t PrevX, PrevY;

extern usbh_host usb_host;
extern usb_core_driver usb_hid_core;

uint16_t KeybrdCharXpos = 0U;
uint16_t KeybrdCharYpos = 0U;

const uint8_t MSG_HOST_HEADER[]        = "USB OTG FS HID Host";
const uint8_t MSG_HOST_FOOTER[]        = "USB Host Library v3.0.0";

/* Points to the DEVICE_PROP structure of current device */
usbh_user_cb usr_cb = {
    usbh_user_init,
    usbh_user_deinit,
    usbh_user_device_connected,
    usbh_user_device_reset,
    usbh_user_device_disconnected,
    usbh_user_over_current_detected,
    usbh_user_device_speed_detected,
    usbh_user_device_desc_available,
    usbh_user_device_address_assigned,
    usbh_user_configuration_descavailable,
    usbh_user_manufacturer_string,
    usbh_user_product_string,
    usbh_user_serialnum_string,
    usbh_user_enumeration_finish,
    usbh_user_userinput,
    NULL,
    usbh_user_device_not_supported,
    usbh_user_unrecovered_error
};

/*!
    \brief      user operation for host-mode initialization
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_init(void)
{
    static uint8_t startup = 0U;

#if USB_LOW_POWER
    if(usb_host.wakeup_mode) {
        startup = 0U;
    }
#endif /* USB_LOW_POWER */

    if(0U == startup) {
        startup = 1U;

        /* configure the User key */
        gd_eval_key_init(KEY_USER, KEY_MODE_GPIO);
        gd_eval_key_init(KEY_WAKEUP, KEY_MODE_EXTI);

        gd_eval_lcd_init();

        lcd_log_init();

        lcd_log_header_set((uint8_t *)MSG_HOST_HEADER, 60U);

        LCD_UsrLog("USB host library started\n");

        lcd_log_footer_set((uint8_t *)MSG_HOST_FOOTER, 40U);
    }
}

/*!
    \brief      deinitialize user state and associated variables
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_deinit(void)
{
}

/*!
    \brief      user operation for device attached
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_device_connected(void)
{
    LCD_UsrLog("> Device Attached.\n");
}

/*!
    \brief      user operation for unrecovered error happens
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_unrecovered_error(void)
{
    LCD_ErrLog("> UNRECOVERED ERROR STATE.\n");
}

/*!
    \brief      user operation for device disconnect event
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_device_disconnected(void)
{
    lcd_background_color_set(LCD_COLOR_BLACK);
    lcd_text_color_set(LCD_COLOR_BLACK);

    lcd_log_textzone_clear(LCD_TEXT_ZONE_X, \
                           LCD_TEXT_ZONE_Y, \
                           LCD_TEXT_ZONE_WIDTH, \
                           LCD_TEXT_ZONE_HEIGHT);

    LCD_UsrLog("> Device Disconnected.\n");
}

/*!
    \brief      user operation for reset USB Device
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_device_reset(void)
{
    /* users can do their application actions here for the USB-Reset */
    LCD_UsrLog("> Reset the USB device.\n");
}

/*!
    \brief      user operation for detecting device speed
    \param[in]  device_speed: device speed
    \param[out] none
    \retval     none
*/
void usbh_user_device_speed_detected(uint32_t device_speed)
{
    if(PORT_SPEED_HIGH == device_speed) {
        LCD_UsrLog("> High speed device detected.\r\n");
    } else if(PORT_SPEED_FULL == device_speed) {
        LCD_UsrLog("> Full speed device detected.\r\n");
    } else if(PORT_SPEED_LOW == device_speed) {
        LCD_UsrLog("> Low speed device detected.\r\n");
    } else {
        LCD_ErrLog("> Device Fault.\r\n");
    }
}

/*!
    \brief      user operation when device descriptor is available
    \param[in]  device_desc: device descriptor
    \param[out] none
    \retval     none
*/
void usbh_user_device_desc_available(void *device_desc)
{
    usb_desc_dev *pDevStr = device_desc;

    LCD_DevInformation("VID: %04Xh\n", (uint32_t)pDevStr->idVendor);
    LCD_DevInformation("PID: %04Xh\n", (uint32_t)pDevStr->idProduct);
}

/*!
    \brief      USB device is successfully assigned the address
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_device_address_assigned(void)
{
}

/*!
    \brief      user operation when configuration descriptor is available
    \param[in]  cfg_desc: pointer to configuration descriptor
    \param[in]  itf_desc: pointer to interface descriptor
    \param[in]  ep_desc: pointer to endpoint descriptor
    \param[out] none
    \retval     none
*/
void usbh_user_configuration_descavailable(usb_desc_config *cfg_desc, \
                                           usb_desc_itf *itf_desc, \
                                           usb_desc_ep *ep_desc)
{
    usb_desc_itf *id = itf_desc;

    if(0x08U == (*id).bInterfaceClass) {
        LCD_UsrLog("> Mass storage device connected.\n");
    } else if(0x03U == (*id).bInterfaceClass) {
        LCD_UsrLog("> HID device connected.\n");
    }
}

/*!
    \brief      user operation when manufacturer string exists
    \param[in]  manufacturer_string: manufacturer string of USB device
    \param[out] none
    \retval     none
*/
void usbh_user_manufacturer_string(void *manufacturer_string)
{
    LCD_DevInformation("Manufacturer: %s\r\n", (char *)manufacturer_string);
}

/*!
    \brief      user operation when product string exists
    \param[in]  product_string: product string of USB device
    \param[out] none
    \retval     none
*/
void usbh_user_product_string(void *product_string)
{
    LCD_DevInformation("Product: %s\r\n", (char *)product_string);
}

/*!
    \brief      user operation when serialNum string exists
    \param[in]  serial_num_string: serialNum string of USB device
    \param[out] none
    \retval     none
*/
void usbh_user_serialnum_string(void *serial_num_string)
{
    LCD_DevInformation("Serial Number: %s\r\n", (char *)serial_num_string);
}

/*!
    \brief      user response request is displayed to ask for application jump to class
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_enumeration_finish(void)
{
    LCD_UsrLog("> Enumeration completed.\n");

    lcd_text_color_set(LCD_COLOR_RED);
    lcd_vertical_string_display(LCD_HINT_LINE0, 0U, (uint8_t *)"---------------------------------------");
    lcd_text_color_set(LCD_COLOR_GREEN);
    lcd_vertical_string_display(LCD_HINT_LINE1, 0U, (uint8_t *)"To start the HID class operations:  ");
    lcd_vertical_string_display(LCD_HINT_LINE2, 0U, (uint8_t *)"Press User Key...             ");
}

/*!
    \brief      user operation when device is not supported
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_device_not_supported(void)
{
    LCD_ErrLog("> Device not supported.\n");
}

/*!
    \brief      user action for application state entry
    \param[in]  none
    \param[out] none
    \retval     user response for user key
*/
usbh_user_status usbh_user_userinput(void)
{
    usbh_user_status usbh_usr_status = USR_IN_NO_RESP;

#if USB_LOW_POWER

    if(usb_host.suspend_flag){
        LCD_UsrLog("> Host in suspend status.\n");
        LCD_UsrLog("> Pls press KEY_WAKEUP key (General wakeup).\n");
        LCD_UsrLog("> Or operate device (Remote wakeup).\n");
    }else{
        if(GENERAL_WAKEUP == usb_host.wakeup_mode) {
            usb_host.wakeup_mode = NORMAL_WORK;

            LCD_UsrLog("> General wakeup success.\n");
         } else if(REMOTE_WAKEUP == usb_host.wakeup_mode) {
            usb_host.wakeup_mode = NORMAL_WORK;
            LCD_UsrLog("> Remote wakeup success.\n");
        }else{
        }
    }
#endif /* USB_LOW_POWER */

    /*Key USER is in polling mode to detect user action */
    if(RESET == gd_eval_key_state_get(KEY_USER)) {
        usbh_usr_status = USR_IN_RESP_OK;
    }

    return usbh_usr_status;
}

/*!
    \brief      user action for device over current detection event
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbh_user_over_current_detected(void)
{
    LCD_ErrLog("> Over current detected.\n");
}

/*!
    \brief      initialize mouse window
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usr_mouse_init(void)
{
    LCD_UsrLog("> HID Demo Device : Mouse.\n");

    lcd_text_color_set(LCD_COLOR_BLACK);

    lcd_log_textzone_clear(LCD_TEXT_ZONE_X, \
                           LCD_TEXT_ZONE_Y, \
                           LCD_TEXT_ZONE_WIDTH, \
                           LCD_TEXT_ZONE_HEIGHT);

    lcd_text_color_set(LCD_COLOR_GREY);
    lcd_rectangle_fill(MOUSE_WINDOW_X, MOUSE_WINDOW_Y, MOUSE_WINDOW_WIDTH, MOUSE_WINDOW_HEIGHT);

    lcd_rectangle_fill(MOUSE_BUTTON_XCHORD, MOUSE_BUTTON1_YCHORD, MOUSE_BUTTON_WIDTH, MOUSE_BUTTON_HEIGHT);
    lcd_rectangle_fill(MOUSE_BUTTON_XCHORD, MOUSE_BUTTON3_YCHORD, MOUSE_BUTTON_WIDTH, MOUSE_BUTTON_HEIGHT);
    lcd_rectangle_fill(MOUSE_BUTTON_XCHORD, MOUSE_BUTTON2_YCHORD, MOUSE_BUTTON_WIDTH, MOUSE_BUTTON_HEIGHT);

    lcd_text_color_set(LCD_COLOR_BLACK);
    lcd_background_color_set(LCD_COLOR_WHITE);
    lcd_vertical_char_display(MOUSE_SPL_X, MOUSE_SPL_Y, 'x');

    hid_mouse_button_released(0U);
    hid_mouse_button_released(1U);
    hid_mouse_button_released(2U);

    XLoc  = 0U;
    YLoc  = 0U; 
    PrevX = 0U;
    PrevY = 0U;
}

/*!
    \brief      process mouse data
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usr_mouse_process_data(mouse_report_data *data)
{
    if((0U != data->x) && (0U != data->y)) {
        hid_mouse_update_position(data->x, data->y);
    }

    for(uint8_t index = 0U; index < 3U; index++) {
        if(data->buttons[index]) {
            hid_mouse_button_pressed(index);
        } else {
            hid_mouse_button_released(index);
        }
    }
}

/*!
    \brief      initialize keyboard window
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usr_keyboard_init(void)
{
    LCD_UsrLog("> HID Demo Device : Keyboard.\n");
    LCD_UsrLog("> Use Keyboard to tape characters: \n");

    lcd_text_color_set(LCD_COLOR_BLACK);

    lcd_log_textzone_clear(LCD_TEXT_ZONE_X, \
                           LCD_TEXT_ZONE_Y, \
                           LCD_TEXT_ZONE_WIDTH, \
                           LCD_TEXT_ZONE_HEIGHT);

    lcd_text_color_set(LCD_COLOR_GREY);
    lcd_rectangle_fill(KYBRD_WINDOW_X, KYBRD_WINDOW_Y, KYBRD_WINDOW_WIDTH, KYBRD_WINDOW_HEIGHT);

    KeybrdCharXpos = KYBRD_FIRST_LINE;
    KeybrdCharYpos = KYBRD_FIRST_COLUMN;
}

/*!
    \brief      process keyboard data
    \param[in]  data: keyboard data to be displayed
    \param[out] none
    \retval     none
*/
void usr_keybrd_process_data(uint8_t data)
{
    if('\n' == data){
        KeybrdCharYpos = KYBRD_FIRST_COLUMN;

        /* increment char X position */
        UPDATE_XP(KeybrdCharXpos);
    }else if('\r' == data){
        /* manage deletion of character and update cursor location */
        if(KYBRD_FIRST_COLUMN == KeybrdCharYpos){
            /* first character of first line to be deleted */
            if(KYBRD_FIRST_LINE == KeybrdCharXpos){
                KeybrdCharYpos = KYBRD_FIRST_COLUMN;
            } else {
                UPDATE_XP(KeybrdCharXpos);
                KeybrdCharYpos =(KYBRD_LAST_COLUMN + SMALL_FONT_COLUMN_WIDTH); 
            }
        } else {
            UPDATE_YP(KeybrdCharYpos);
        }

        lcd_text_color_set(LCD_COLOR_BLACK);
        lcd_background_color_set(LCD_COLOR_WHITE);
        lcd_vertical_char_display(CHAR_CURSOR(KeybrdCharXpos, KeybrdCharYpos), ' ');
    } else {
        lcd_text_color_set(LCD_COLOR_BLACK);
        lcd_background_color_set(LCD_COLOR_WHITE);
        lcd_vertical_char_display(CHAR_CURSOR(KeybrdCharXpos, KeybrdCharYpos), data);

        /* update the cursor position on LCD */

        /* increment char Y position */
        UPDATE_YP(KeybrdCharYpos);

        /*check if the Y position has reached the last column*/
        if(KYBRD_LAST_COLUMN == KeybrdCharYpos){
            KeybrdCharYpos = KYBRD_FIRST_COLUMN;

            /*increment char X position*/
            UPDATE_XP(KeybrdCharXpos);
        }
    }

    if(KYBRD_LINE_LIMIT(KeybrdCharXpos)){
        lcd_text_color_set(LCD_COLOR_GREY);
        lcd_rectangle_fill(KYBRD_WINDOW_X, KYBRD_WINDOW_Y, KYBRD_WINDOW_WIDTH, KYBRD_WINDOW_HEIGHT);

        KeybrdCharXpos = KYBRD_FIRST_LINE;
    }
}
