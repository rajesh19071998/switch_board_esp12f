#ifndef _PIN_CONFIG_H
#define _PIN_CONFIG_H


#define SWITCH1        4
#define SWITCH2        5  
#define SWITCH3       13

#define RELAY1       12
#define RELAY2        0
#define RELAY3       14


/*********************************************  ESP12F PINS *************************************************************************/
#if 0
/*
INPUT / OUTPUT PINS = 5
OUTPUT PINS         = 3
                    = 8 PINS
*/
#define ESP_D0 D0 // 16 HIGH at BOOT (No interuppts) (WAKUP PIN) Mostly don't use
#define ESP_D1 D1 // 5   SCL (INPUT / OUTPUT) (I2C) Safe
#define ESP_D2 D2 // 4   SDA (INPUT / OUTPUT) (I2C) Safe
#define ESP_D3 D3 // 0  OUTPUT only (BOOT fails if pulled Input LOW)
#define ESP_D4 D4 // 2  OUTPUT only (BOOT fails if pulled Input LOW) (ESP BOARD LED)
#define ESP_D5 D5 // 14 CLK  (INPUT / OUTPUT) (SPI) Safe
#define ESP_D6 D6 // 12 MISO (INPUT / OUTPUT) (SPI) Safe
#define ESP_D7 D7 // 13 MOSI (INPUT / OUTPUT) (SPI) Safe
#define ESP_D8 D8 // 15 CS  OUTPUT only (BOOT fails if pulled HIGH ) (SPI chip select)

#define ESP_RX 3  // don't use this pin (HIGH at BOOT)
#define ESP_TX 1  // don't use this pin (HIGH at BOOT)

#endif
/**************************************************************************************************************************/

#endif //_PIN_CONFIG_H