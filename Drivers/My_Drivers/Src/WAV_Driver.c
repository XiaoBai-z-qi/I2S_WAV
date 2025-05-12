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

#define WAV_BUFFER_SIZE 4096*2
#define HALF_BUFFER_SIZE (WAV_BUFFER_SIZE/2)

uint16_t WAV_Buffer[2][HALF_BUFFER_SIZE];
UINT br,bw;
volatile uint8_t WAV_Flag = 0;
volatile uint32_t WAV_Count = 0;

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    // 处理传输完成后的数据准备
    if(WAV_Flag == 0){
        WAV_Flag = 1;
        WAV_Count++;
        
        // 传输当前的第二块数据缓冲区
        HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[1], HALF_BUFFER_SIZE/2);

        // 读取SD卡数据到第一块缓冲区
         f_read(&SDFile, WAV_Buffer[0], HALF_BUFFER_SIZE, &br);
        // if(br < HALF_BUFFER_SIZE){
        //     return; // 如果读取的数据不足半缓冲区，退出
        // }
    }else{
        WAV_Flag = 0;
        WAV_Count++;
        
        // 传输当前的第一块数据缓冲区
        HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[0], HALF_BUFFER_SIZE/2);

        // 读取SD卡数据到第二块缓冲区
        f_read(&SDFile, WAV_Buffer[1], HALF_BUFFER_SIZE, &br);
        // if(br < HALF_BUFFER_SIZE){
        //     return; // 如果读取的数据不足半缓冲区，退出
        // }
    }
}



// void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
// {
//     if(WAV_Flag == 0){
//         WAV_Flag = 1;
//         WAV_Count++;
        
//         HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[1], HALF_BUFFER_SIZE);
// 		f_read(&SDFile, WAV_Buffer[0], HALF_BUFFER_SIZE, &br);
//         if(br < HALF_BUFFER_SIZE){
//             return;
//         }
//     }else{
//         WAV_Flag = 0;
//         WAV_Count++;
        
//         HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[0], HALF_BUFFER_SIZE);
// 		f_read(&SDFile, WAV_Buffer[1], HALF_BUFFER_SIZE, &br);
//         if(br < HALF_BUFFER_SIZE){
//             return;
//         }
//     }

    

// }

// void HAL_I2S_HalfTxCpltCallback(I2S_HandleTypeDef *hi2s)
// {
//     if(WAV_Flag == 0){
//         WAV_Flag = 1;
//         WAV_Count++;
        
//         HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[1], HALF_BUFFER_SIZE);
// 		f_read(&SDFile, WAV_Buffer[0], HALF_BUFFER_SIZE, &br);
//         if(br < HALF_BUFFER_SIZE){
//             return;
//         }
//     }else{
//         WAV_Flag = 0;
//         WAV_Count++;
        
//         HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[0], HALF_BUFFER_SIZE);
// 		f_read(&SDFile, WAV_Buffer[1], HALF_BUFFER_SIZE, &br);
//         if(br < HALF_BUFFER_SIZE){
//             return;
//         }
//     }
// }

//测试函数
void WAV_Disp(void)
{
    HAL_Delay(1000);
    if(f_mount(&SDFatFS, (const TCHAR*)&SDPath, 1) == FR_OK){
        printf("SD卡挂载成功\r\n");
    }else{
        printf("SD卡挂载失败\r\n");
    }

    f_open(&SDFile, "music1.wav", FA_OPEN_EXISTING | FA_READ);
    f_lseek(&SDFile, 44);

    f_read(&SDFile, WAV_Buffer[0], WAV_BUFFER_SIZE, &br);

    HAL_I2S_Transmit_DMA(&hi2s2, WAV_Buffer[0], HALF_BUFFER_SIZE/2);
}
