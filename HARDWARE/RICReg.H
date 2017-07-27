//////////////////////////////////////////////////////////////////////////////
//    Copyright (c), Philips Semiconductors Gratkorn
//
//                     (C)PHILIPS Electronics N.V. 2000
//                          All rights are reserved. 
//  Philips reserves the right to make changes without notice at any time.
// Philips makes no warranty, expressed, implied or statutory, including but
// not limited to any implied warranty of merchantibility or fitness for any
//particular purpose, or that the use will not infringe any third party patent,
// copyright or trademark. Philips must not be liable for any loss or damage
//                          arising from its use.
//////////////////////////////////////////////////////////////////////////////
/*! \file RICReg.h
*
* Register Setting of the reader IC
*/
#ifndef RICREG_H
#define RICREG_H

#ifdef __cplusplus
extern "C"
{
#endif
#define	 RC500BASE				0X80000000
// PAGE 0      Command and Status
#define     RegPage               			RC500BASE + 0x0000   /* Page Select Register */
#define     RegCommand            	 	RC500BASE + 0x0001   /* Command Register     */
#define     RegFIFOData           	  	RC500BASE + 0x0002   /* FiFo Register        */
#define     RegPrimaryStatus         	RC500BASE + 0x0003   /* Modem State/IRQ/ERR/LoHiAlert Reg  */
#define     RegFIFOLength            		RC500BASE + 0x0004   /* Buffer length Register   */
#define     RegSecondaryStatus    		RC500BASE + 0x0005   /* diverse status flags     */
#define     RegInterruptEn            		RC500BASE + 0x0006   /* IRQ enable Register      */
#define     RegInterruptRq            		RC500BASE + 0x0007   /* IRQ bits Register        */
// PAGE 1      Control and Status
#define     RegPage1                   		RC500BASE + 0x0008   /* Page Select Register */
#define     RegControl                 		RC500BASE + 0x0009   /* processor control        */
#define     RegErrorFlag               		RC500BASE + 0x000A   /* error flags showing the error
																  status of the last command executed */
#define     RegCollPos            			RC500BASE + 0x000B   /*  bit position of the first bit
																  collision detected on the RF-interface */
#define     RegTimerValue         		RC500BASE + 0x000C   /* preload value of the timer */
#define     RegCRCResultLSB       		RC500BASE + 0x000D   /* LSB of the CRC Coprocessor register */
#define     RegCRCResultMSB       		RC500BASE + 0x000E   /* MSB of the CRC Coprocessor register */
#define     RegBitFraming         		RC500BASE + 0x000F   /* Adjustments for bit oriented frames */
// PAGE 2      Transmitter and Coder Control
#define     RegPage2              		RC500BASE + 0x0010   /* Page Select Register */
#define     RegTxControl          		RC500BASE + 0x0011   /* controls the logical behaviour of
																  the antenna driver pins TX1 and TX2 */
#define     RegCwConductance      		RC500BASE + 0x0012   /* selects the conductance of the
																  antenna driver pins TX1 and TX2 */
#define     RegModConductance     	RC500BASE + 0x0013   /* RFU                 */
#define     RegCoderControl       		RC500BASE + 0x0014   /* selects coder rate  */
#define     RegModWidth           		RC500BASE + 0x0015   /* selects the width of the modulation pulse */
#define     RFU16                 			RC500BASE + 0x0016   /* RFU                 */
#define     RegTypeBFraming       		RC500BASE + 0x0017   /* RFU                 */
// PAGE 3      Receiver and Decoder Control
#define     RegPage3              		RC500BASE + 0x0018   /* Page Select Register */
#define     RegRxControl1         		RC500BASE + 0x0019   /* controls receiver behaviour      */
#define     RegDecoderControl     		RC500BASE + 0x001A   /* controls decoder behaviour       */
#define     RegBitPhase           		RC500BASE + 0x001B   /* selets the bit phase between
																  transmitter and receiver clock */
#define     RegRxThreshold        		RC500BASE + 0x001C   /* selects thresholds for the bit decoder */
#define     RegBPSKDemControl     	RC500BASE + 0x001D   /* RFU                */
#define     RegRxControl2         		RC500BASE + 0x001E   /* controls decoder behaviour and
																  defines the input source for the receiver */
#define     RegClockQControl      		RC500BASE + 0x001F   /* controls clock generation for the
																  90° phase shifted Q-channel clock */
// PAGE 4      RF-Timing and Channel Redundancy
#define     RegPage4              		RC500BASE + 0x0020   /* Page Select Register */
#define     RegRxWait             		RC500BASE + 0x0021   /* selects the time interval after
																  transmission, before receiver starts */
#define     RegChannelRedundancy  	RC500BASE + 0x0022   /* selects the kind and mode of
																  checking the data integrity on the RF-channel */
#define     RegCRCPresetLSB       		RC500BASE + 0x0023   /* LSB of the pre-set value for the
																  CRC register */
#define     RegCRCPresetMSB       		RC500BASE + 0x0024   /* MSB of the pre-set value for the
																  CRC register */
#define     RFU25                 			RC500BASE + 0x0025   /* RFU                    */
#define     RegMfOutSelect        		RC500BASE + 0x0026   /* selects internal signal applied to
																  pin MfOut */
#define     RFU27                 			RC500BASE + 0x0027   /* RFU                    */
// PAGE 5      FIFO, Timer and IRQ-Pin Configuration
#define     RegPage5              		RC500BASE + 0x0028   /* Page Select Register */
#define     RegFIFOLevel          		RC500BASE + 0x0029   /* Defines level for FIFO over- and
																  underflow warning */
#define     RegTimerClock         		RC500BASE + 0x002A   /* selects the divider for the timer clock */
#define     RegTimerControl       		RC500BASE + 0x002B   /* selects start and stop conditions
																  for the timer */
#define     RegTimerReload        		RC500BASE + 0x002C   /* defines the pre-set value for the
																  timer */
#define     RegIRqPinConfig       		RC500BASE + 0x002D   /* configures the output stage of pin IRq */
#define     RFU2E                 			RC500BASE + 0x002E   /* RFU  */
#define     RFU2F                 			RC500BASE + 0x002F   /* RFU  */
// PAGE 6      RFU
#define     RegPage6              		RC500BASE + 0x0030   /* Page Select Register */
#define     RFU31                 			RC500BASE + 0x0031   /* RFU  */
#define     RFU32                 			RC500BASE + 0x0032   /* RFU  */
#define     RFU33                 			RC500BASE + 0x0033   /* RFU  */
#define     RFU34                 			RC500BASE + 0x0034   /* RFU  */
#define     RFU35                 			RC500BASE + 0x0035   /* RFU  */
#define     RFU36                 			RC500BASE + 0x0036   /* RFU  */
#define     RFU37                 			RC500BASE + 0x0037   /* RFU  */
// PAGE 7      Test Control
#define     RegPage7              		RC500BASE + 0x0038   /* Page Select Register */
#define     RFU39                 			RC500BASE + 0x0039   /* RFU  */
#define     RegTestAnaSelect      		RC500BASE + 0x003A   /* selects analog test mode  */
#define     RFU3B                 			RC500BASE + 0x003B   /* RFU  */
#define     RFU3C                 			RC500BASE + 0x003C   /* RFU  */
#define     RegTestDigiSelect     		RC500BASE + 0x003D   /* selects digital test mode */
#define     RFU3E                 			RC500BASE + 0x003E   /* RFU  */
#define     RegTestDigiAccess     		RC500BASE + 0x003F

#define  	DEF_FIFO_LENGTH    		64                    /* default FIFO size   */

//                   P C D - C O M M A N D S
#define 	PCD_IDLE           			0x00 /* No action: cancel current command
										  or home state */
#define 	PCD_WRITEE2        		0x01 /* Get data from FIFO and write it to the E2PROM  */
#define 	PCD_READE2         			0x03 /* Read data from E2PROM and put it into the
										  FIFO */
#define 	PCD_LOADCONFIG     		0x07 /* Read data from E2PROM and initialise the
										  registers */
#define 	PCD_LOADKEYE2      		0x0B /* Read a master key from the E2PROM and put
                                it into the master key buffer */
#define 	PCD_AUTHENT1       		0x0C /* Perform the first part of the card
                                authentication using the Crypto1 algorithm.
                                
                                Remark: The master key is automatically taken 
                                from the master key buffer. this implies, 
                                that the command LoadKeyE2 has to be executed
                                before using a certain key for card 
                                authentication */
#define 	PCD_CALCCRC        		0x12 /* Activate the CRC-Coprocessor

                                Remark: The result of the CRC calculation can
                                be read from the register CRCResultXXX */
#define 	PCD_AUTHENT2       		0x14 /* Perform the second part of the card
                                authentication using the Crypto1 algorithm. */
#define 	PCD_RECEIVE        			0x16 /* Activate Receiver Circuitry. Before the
                                receiver actually starts, the state machine 
                                waits until the time configured in the 
                                register RxWait has passed.
                                
                                Remark: It is possible to read any received 
                                data from the FIFO while the Receive command
                                is active. Thus it is possible to receive an 
                                unlimited number of bytes by reading them 
                                from the FIFO in timer. */
#define 	PCD_LOADKEY        		0x19 /* Read a master key from the FIFO and put it
                                into the master key buffer
                                
                                Remark: The master key has to be prepared in
                                a certain format. Thus, 12 byte have to be 
                                passed to load a 6 byte master key */
#define 	PCD_TRANSMIT       		0x1A /* Transmit data from FIFO to the card

                                Remark: If data is already in the FIFO when 
                                the command is activated, this data is 
                                transmitted immediately. It is possible to 
                                write data to the FIFO while the Transmit 
                                command is active. Thus it is possible to 
                                transmit an unlimited number of bytes in one
                                stream by writting them to the FIFO in time.*/
#define 	PCD_TRANSCEIVE     		0x1E /* Transmits data from FIFO to the card and
                                after that automatically activates the 
                                receiver. Before the receiver actually 
                                starts,the state machine waits until the 
                                time configured in the register RxWait has 
                                passed. 
                                
                                Remark: This command is the combination of 
                                Transmit and Receive.*/
#define 	PCD_RESETPHASE     		0x3F /* Runs the Reset- and Initialisation Phase
                                Remark: This command can not be activated by 
                                software, but only by a Power-On or 
										  Hard Reset */

// #define TCLFSDSNDMAX   16   /* max. frame size send   */
// #define TCLFSDRECMAX   16   /* max. frame size rcv    */
// #define TCLDSMAX       3   /* max. baudrate divider PICC --> PCD   */
// #define TCLDRMAX       3   /* max. baudrate divider PCD --> PICC   */
//
// #define TCLDSDFLT      0   /* default baudrate divider PICC --> PCD */
// #define TCLDRDFLT      0   /* default baudrate divider PCD --> PICC */
// #define MFIFOLength    64

#ifdef __cplusplus
}
#endif

#endif //RICREG_H
//////////////////////////////////////////////////////////////////////////////
// End of File
//////////////////////////////////////////////////////////////////////////////
