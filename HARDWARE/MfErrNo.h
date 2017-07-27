///////////////////////////////////////////////////////////////////////////////
//    Copyright (c), Philips Semiconductors Gratkorn
//
//                     (C)PHILIPS Electronics N.V. 2000
//                        All rights are reserved. 
//  Philips reserves the right to make changes without notice at any time.
// Philips makes no warranty, expressed, implied or statutory, including but
// not limited to any implied warranty of merchantibility or fitness for any
//particular purpose, or that the use will not infringe any third party patent,
// copyright or trademark. Philips must not be liable for any loss or damage
//                          arising from its use.
///////////////////////////////////////////////////////////////////////////////
#ifndef MFERRNO_H
#define MFERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
// Reader Error Codes                   Base Address Start:      0000
//                                      Base Address End:        -999
//////////////////////////////////////////////////////////////////////////////
#define READER_ERR_BASE_START           (0)
#define MI_OK                           (0)
#define MI_CHK_OK                       (0)
#define MI_CRC_ZERO                     (0)
// ICODE1 Error   Codes
#define  I1_OK                          (0)
#define  I1_NO_ERR                      (0)

#define MI_NOTAGERR                     (-1)
#define MI_CHK_FAILED                   (-1)
#define MI_CRCERR                       (-2)
#define MI_CHK_COMPERR                  (-2)
#define MI_EMPTY                        (-3)
#define MI_AUTHERR                      (-4)
#define MI_PARITYERR                    (-5)
#define MI_CODEERR                      (-6)

#define MI_SERNRERR                     (-8)
#define MI_KEYERR                       (-9)
#define MI_NOTAUTHERR                   (-10)
#define MI_BITCOUNTERR                  (-11)
#define MI_BYTECOUNTERR                 (-12)
#define MI_IDLE                         (-13)
#define MI_TRANSERR                     (-14)
#define MI_WRITEERR                     (-15)
#define MI_INCRERR                      (-16)
#define MI_DECRERR                      (-17)
#define MI_READERR                      (-18)
#define MI_OVFLERR                      (-19)
#define MI_POLLING                      (-20)
#define MI_FRAMINGERR                   (-21)
#define MI_ACCESSERR                    (-22)
#define MI_UNKNOWN_COMMAND              (-23)
#define MI_COLLERR                      (-24)
#define MI_RESETERR                     (-25)
#define MI_INITERR                      (-25)
#define MI_INTERFACEERR                 (-26)
#define MI_ACCESSTIMEOUT                (-27)
#define MI_NOBITWISEANTICOLL            (-28)
#define MI_QUIT                         (-30)
#define MI_CODINGERR                    (-31)
#define MI_SENDBYTENR                   (-51)
#define MI_CASCLEVEX                    (-52)    
#define MI_SENDBUF_OVERFLOW             (-53)
#define MI_BAUDRATE_NOT_SUPPORTED       (-54)
#define MI_SAME_BAUDRATE_REQUIRED       (-55)

#define MI_WRONG_PARAMETER_VALUE        (-60)

// ICODE1 Error   Codes
#define  I1_WRONGPARAM                  (-61)
#define  I1_NYIMPLEMENTED               (-62)
#define  I1_TSREADY                     (-63)

#define  I1_TIMEOUT                     (-70)
#define  I1_NOWRITE                     (-71)
#define  I1_NOHALT                      (-72)
#define  I1_MISS_ANTICOLL               (-73) 

#define  I1_COMM_ABORT                  (-82)

#define MI_BREAK                        (-99)
#define MI_NY_IMPLEMENTED               (-100)
#define MI_NO_MFRC                      (-101)
#define MI_MFRC_NOTAUTH                 (-102)
#define MI_WRONG_DES_MODE               (-103)
#define MI_HOST_AUTH_FAILED             (-104)

#define MI_WRONG_LOAD_MODE              (-106)
#define MI_WRONG_DESKEY                 (-107)
#define MI_MKLOAD_FAILED                (-108)
#define MI_FIFOERR                      (-109)
#define MI_WRONG_ADDR                   (-110)
#define MI_DESKEYLOAD_FAILED            (-111)
#define MI_RECBUF_OVERFLOW              (-112)
#define MI_WRONG_SEL_CNT                (-114)

#define MI_WRONG_TEST_MODE              (-117)
#define MI_TEST_FAILED                  (-118)
#define MI_TOC_ERROR                    (-119)
#define MI_COMM_ABORT                   (-120)
#define MI_INVALID_BASE                 (-121)
#define MI_MFRC_RESET                   (-122)
#define MI_WRONG_VALUE                  (-123)
#define MI_VALERR                       (-124)

//add by xuyan
#define MI_INIT_ERR						(-125)

#ifdef __cplusplus
}
#endif

#endif // MFERRNO_H

