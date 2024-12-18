/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pca9555.h"
#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio_ex.h"
#include "iot_gpio.h"
#include "iot_i2c.h"
#include "iot_errno.h"
#include "hi_errno.h"
#include "hi_i2c.h"

#define PCA9555_I2C_IDX              0
#define IOT_I2C_IDX_BAUDRATE         400000 // 400k

// 初始化指定地址的PCA9555器件
void PCA9555Init(void)
{
    // 初始化I2C设备0，并指定波特率为400k
    IoTI2cInit(PCA9555_I2C_IDX, IOT_I2C_IDX_BAUDRATE);
    // 设置I2C设备0的波特率为400k
    IoTI2cSetBaudrate(PCA9555_I2C_IDX, IOT_I2C_IDX_BAUDRATE);
    // 设置GPIO13的管脚复用关系为I2C0_SDA
    IoSetFunc(IOT_IO_NAME_GPIO_13, IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    // 设置GPIO14的管脚复用关系为I2C0_SCL
    IoSetFunc(IOT_IO_NAME_GPIO_14, IOT_IO_FUNC_GPIO_14_I2C0_SCL);

    // PCA555扩展IO的中断脚为GPIO11
    // 初始化GPIO11
    IoTGpioInit(IOT_IO_NAME_GPIO_11);
    // 初始化GPIO11管脚复用为GPIO
    IoSetFunc(IOT_IO_NAME_GPIO_11, IOT_IO_FUNC_GPIO_11_GPIO);
    // 初始化GPIO11管脚方向为输入
    IoTGpioSetDir(IOT_IO_NAME_GPIO_11, IOT_GPIO_DIR_IN);
    // 设置GPIO11为上拉功能
    IoSetPull(IOT_IO_NAME_GPIO_11, IOT_IO_PULL_UP);
}

// 从指定地址的PCA9555器件的指定寄存器读一字节数据
uint32_t PCA9555I2CReadByte(uint8_t *rec_byte)
{
    uint32_t retval;
    uint8_t cmd = 0x00;
    retval = IoTI2cWrite(PCA9555_I2C_IDX, PCA9555_WRITE, &cmd, 1);
    if (retval != IOT_SUCCESS) {
        return retval;
    }
    retval = IoTI2cRead(PCA9555_I2C_IDX, PCA9555_READ, rec_byte, 1);
    return retval; 
}

// 向指定地址的PCA9555器件的指定寄存器写一字节数据
uint32_t PCA9555I2CWriteByte(uint8_t* buffer, uint32_t buffLen)
{
    uint32_t retval = IoTI2cWrite(PCA9555_I2C_IDX, PCA9555_WRITE, buffer, buffLen);
    if (retval != IOT_SUCCESS) {
        printf("IoTI2cWrite(0x%x) failed, 0x%x!\n", buffer[0], retval);
        return retval;
    }
    return IOT_SUCCESS;
}

uint32_t SetPCA9555GpioValue(uint8_t addr, uint8_t buffer)
{
    uint8_t write[2] = {addr, buffer};
    PCA9555I2CWriteByte(write, 2); // 2 len
    return IOT_SUCCESS;
}