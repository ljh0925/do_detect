/*
*         Copyright (c), Philips Semiconductors Gratkorn / Austria
*
*                     (C)PHILIPS Electronics N.V.2000
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  Philips reserves the right to make changes without notice at any time.
* Philips makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. Philips must not be liable for any loss or damage
*                          arising from its use.
*/

/*! \file PICCCmdConst.h
*
* Projekt: MF EV X00 Firmware
*
* $Workfile:: PICCCmdConst.h                                                 $ 
* $Modtime:: 5.12.01 6:52                                               $ 
* $Author:: Hb                                                          $
* $Revision:: 1                                                         $
*
*/
#ifndef PICCCMDCONST_H
#define PICCCMDCONST_H


/** \name PICC Commands
* \ingroup mifare
* Commands which are handled by the tag
*
* Each tag command is written to the reader IC and transfered via RF
*/
 #define PICC_REQIDL        0x26         //!< request idle
 #define PICC_REQALL        0x52         //!< request all
 #define PICC_ANTICOLL1     0x93         //!< anticollision level 1 106 kBaud
 #define PICC_ANTICOLL11    0x92         //!< anticollision level 1 212 kBaud
 #define PICC_ANTICOLL12    0x94         //!< anticollision level 1 424 kBaud
 #define PICC_ANTICOLL13    0x98         //!< anticollision level 1 848 kBaud
 #define PICC_ANTICOLL2     0x95         //!< anticollision level 2
 #define PICC_ANTICOLL3     0x97         //!< anticollision level 3
 #define PICC_AUTHENT1A     0x60         //!< authentication using key A
 #define PICC_AUTHENT1B     0x61         //!< authentication using key B
 #define PICC_READ16        0x30         //!< read 16 byte block
 #define PICC_WRITE16       0xA0         //!< write 16 byte block
 #define PICC_WRITE4        0xA2         //!< write 4 byte block
 #define PICC_DECREMENT     0xC0         //!< decrement value
 #define PICC_INCREMENT     0xC1         //!< increment value
 #define PICC_RESTORE       0xC2         //!< restore command code
 #define PICC_TRANSFER      0xB0         //!< transfer command code
 #define PICC_HALT          0x50         //!< halt


#define     REQIDL        0x26         /* request idle  */
#define     REQALL        0x52         /* request all   */
#define     ANTICOLL1     0x93         /* anticollision level 1 106 kBaud   */
#define     ANTICOLL2     0x95         /* anticollision level 2             */
#define     ANTICOLL3     0x97         /* anticollision level 3             */
#define     AUTHENT1A     0x60         /* authentication using key A        */
#define     AUTHENT1B     0x61         /* authentication using key B        */
#define     READ          		0x30         /* read block                        */
#define     WRITE         		0xA0         /* write block                       */
#define     DECREMENT     0xC0         /* decrement value                   */
#define     INCREMENT     0xC1         /* increment value                   */
#define     RESTORE       0xC2         /* restore command code              */
#define     TRANSFER      0xB0         /* transfer command code             */
#define     HALT          		0x50         /* halt                              */


#endif // PICCCMDCONST_H

