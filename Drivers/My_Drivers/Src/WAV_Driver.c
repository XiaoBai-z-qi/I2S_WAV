#include "wav_driver.h"
#include <stdio.h>
#include <string.h>
/*=========================================*/
#include "dma.h"
#include "fatfs.h"
#include "i2s.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"
/*=========================================*/

//测试函数
void WAV_Disp(void)
{
    HAL_Delay(1000);
    if(f_mount(&SDFatFS, (const TCHAR*)&SDPath, 1) == FR_OK){
        printf("SD卡挂载成功\r\n");
    }else{
        printf("SD卡挂载失败\r\n");
    }
}
