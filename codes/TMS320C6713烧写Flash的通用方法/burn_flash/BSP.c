/*
 * FileName : BSP.c
 * Author   : xiahouzuoxin
 * Date     : 2013.09.28
 * Version  : v1.0
 * Brief    :  
 */
#include "BSP.h"
#include "csl.h"
#include "csl_emif.h"
#include "csl_pll.h"

/* EMIF Registers */
#define EMIF_GCTL       0x01800000
#define EMIF_CE1        0x01800004
#define EMIF_CE0        0x01800008
#define EMIF_CE2        0x01800010
#define EMIF_CE3        0x01800014
#define EMIF_SDRAMCTL   0x01800018
#define EMIF_SDRAMTIM   0x0180001C
#define EMIF_SDRAMEXT   0x01800020
#define EMIF_CCFG       0x01840000
#define SDRAM_BASE_ADDRESS_CE0    0x80000000	
#define SDRAM_SIZE_BYTE           0x01000000	//256Mb SDRAM

#define PLLCSR 0x01B7C100
#define PLLMULT 0x01B7C110
#define PLLDIV0 0x01B7C114
#define PLLDIV1 0x01B7C118
#define PLLDIV2 0x01B7C11C
#define PLLDIV3 0x01B7C120
#define PLLOSCDIV1 0x01B7C124

#define CSR_PLLEN          0x00000001
#define CSR_PLLPWRDN       0x00000002
#define CSR_PLLRST         0x00000008
#define CSR_PLLSTABLE      0x00000040

#define DIV_ENABLE         0x00008000

extern far void vectors();   /* _init_c00 */

/*Local software delay function*/
//static void PLLDelay(int Count)
//{
//    volatile int i = Count;
//    while(i--);
//}

void plldelay(Uint32 count)
{
   Uint32 i = count;
   while(i--)
   {
     asm(" NOP 1");
   }
}

/*
 * @brief
 * @param  None
 * @retval None
 */
void InitEmif( void )
{
	*(int *)EMIF_GCTL     = 0x3068;
	*(int *)EMIF_CE0      = 0x30;  // CE0:SDRAM 16-bit
	*(int *)EMIF_CE1      = 0xffffff13;  // CE1:Flash 16-bit 
	 		
	*(int *)EMIF_SDRAMCTL = 0x54126000;  // SDRAM control
	*(int *)EMIF_SDRAMTIM = 0x061a;  // SDRAM timing (refresh)
	*(int *)EMIF_SDRAMEXT = 0x54529;  // SDRAM Extension register
}

/*
 * @brief  Init Board, BSP means Board Support Package.
 *         Place configuration for SDRAM and PLL here.
 * @param  
 * @retval 
 */
void BSP_init(void)
{
	/* Initealize the board APIs */
	/*
	EMIF_Config MyEMIFcfg0={
		0x30								|
		EMIF_FMKS(GBLCTL,NOHOLD,DISABLE)	|
		EMIF_FMKS(GBLCTL,EKEN,DEFAULT)		|
		EMIF_FMKS(GBLCTL,CLK1EN,DISABLE)	|
		EMIF_FMKS(GBLCTL,CLK2EN,DISABLE),
		
		EMIF_FMKS(CECTL,WRSETUP,DEFAULT)	|
		EMIF_FMKS(CECTL,WRSTRB,DEFAULT)		|
		EMIF_FMKS(CECTL,WRHLD,DEFAULT)		|
		EMIF_FMKS(CECTL,RDSETUP,DEFAULT)	|
		EMIF_FMKS(CECTL,TA,OF(2))			|
		EMIF_FMKS(CECTL,RDSTRB,DEFAULT)		|
		EMIF_FMKS(CECTL,MTYPE,SDRAM32)		|
		EMIF_FMKS(CECTL,RDHLD,DEFAULT),
		
		EMIF_FMKS(CECTL, WRSETUP, OF(0))    |
        EMIF_FMKS(CECTL, WRSTRB, OF(8))     |
        EMIF_FMKS(CECTL, WRHLD, OF(2))      |
        EMIF_FMKS(CECTL, RDSETUP, OF(0))    |
        EMIF_FMKS(CECTL, TA, OF(2))         |
        EMIF_FMKS(CECTL, RDSTRB, OF(8))     |
        EMIF_FMKS(CECTL, MTYPE, ASYNC16)    |
        EMIF_FMKS(CECTL, RDHLD, OF(2)),
        
        EMIF_FMKS(CECTL, WRSETUP, OF(0))    |
        EMIF_FMKS(CECTL, WRSTRB, OF(8))     |
        EMIF_FMKS(CECTL, WRHLD, OF(2))      |
        EMIF_FMKS(CECTL, RDSETUP, OF(0))    |
        EMIF_FMKS(CECTL, TA, OF(2))         |
        EMIF_FMKS(CECTL, RDSTRB, OF(8))     |
        EMIF_FMKS(CECTL, MTYPE, ASYNC8)    |
        EMIF_FMKS(CECTL, RDHLD, OF(2)),
		
		EMIF_FMKS(CECTL, WRSETUP, OF(2))    |
        EMIF_FMKS(CECTL, WRSTRB, OF(10))    |
        EMIF_FMKS(CECTL, WRHLD, OF(2))      |
        EMIF_FMKS(CECTL, RDSETUP, OF(2))    |
        EMIF_FMKS(CECTL, TA, OF(2))         |
        EMIF_FMKS(CECTL, RDSTRB, OF(10))    |
        EMIF_FMKS(CECTL, MTYPE, ASYNC32)    |
        EMIF_FMKS(CECTL, RDHLD, OF(2)),
        
        EMIF_FMKS(SDCTL,SDBSZ,4BANKS)		|
        EMIF_FMKS(SDCTL,SDRSZ,12ROW)		|
        EMIF_FMKS(SDCTL,SDCSZ,8COL)			|
        EMIF_FMKS(SDCTL,RFEN,ENABLE)		|
        EMIF_FMKS(SDCTL,INIT,YES)			|
        EMIF_FMKS(SDCTL,TRCD,OF(2))			|
        EMIF_FMKS(SDCTL,TRP,OF(2))			|
        EMIF_FMKS(SDCTL,TRC,OF(7)),
        
        //EMIF_FMKS(SDTIM,XRFR,OF(1))			|
        EMIF_FMKS(SDTIM, CNTR, OF(0))       |
        EMIF_FMKS(SDTIM, PERIOD, OF(1400)),
        
		EMIF_FMKS(SDEXT, WR2RD, OF(0))      |
        EMIF_FMKS(SDEXT, WR2DEAC, OF(2))    |
        EMIF_FMKS(SDEXT, WR2WR, OF(0))      |
        EMIF_FMKS(SDEXT, R2WDQM, OF(1))     |
        EMIF_FMKS(SDEXT, RD2WR, OF(0))      |
        EMIF_FMKS(SDEXT, RD2DEAC, OF(1))    |
        EMIF_FMKS(SDEXT, RD2RD, OF(0))      |
        EMIF_FMKS(SDEXT, THZP, OF(2))       |
        EMIF_FMKS(SDEXT, TWR, OF(2))        |
        EMIF_FMKS(SDEXT, TRRD, OF(0))       |
        EMIF_FMKS(SDEXT, TRAS, OF(5))       |
        EMIF_FMKS(SDEXT, TCL, OF(1))
        };
    */    
	/* Initialize PLL Registers */
    /* Crystal 20MHz */
    *(volatile unsigned int *)PLLCSR =0X00000000;/*disable the PLL,in bypass status*/
    plldelay(20);
    *(volatile unsigned int *)PLLCSR |=CSR_PLLRST;/*PLL in the reset Status*/
    plldelay(20);
    *(volatile unsigned int *)PLLDIV0=DIV_ENABLE;/*divide 1,output the same frequence*/
    *(volatile unsigned int *)PLLMULT= 0x0000000f;/*Multiply 15 and the pll frequence is 300Mhz */
    plldelay(20);
    *(volatile unsigned int *)PLLOSCDIV1=DIV_ENABLE+0;/*clkout 3 output,10MHz*/
    *(volatile unsigned int *)PLLDIV3=DIV_ENABLE+2;/*300Mhz/3=100Mhz for the EMIF port*/
    *(volatile unsigned int *)PLLDIV2=DIV_ENABLE+2;/*300Mhz/3=100Mhz for Peripherals */
    plldelay(20);
    *(volatile unsigned int *)PLLDIV1=DIV_ENABLE+0;/*300Mhz for cpu core frequence*/
    *(volatile unsigned int *)PLLCSR = 0x00000000 | DIV_ENABLE;/*Reset release*/
    plldelay(1500);
    *(volatile unsigned int *)PLLCSR |=CSR_PLLEN;/*enable the PLL*/
    plldelay(20);
   
   	/* Initialize EMIF */
    //EMIF_config(&MyEMIFcfg0);
    InitEmif();   

    /* Init chip support library */
    CSL_init();
        
    /**************************** 
     * Set vector table pointor 
     ***************************/
	IRQ_setVecs(vectors);
    IRQ_nmiEnable();
    IRQ_globalEnable();     
}

