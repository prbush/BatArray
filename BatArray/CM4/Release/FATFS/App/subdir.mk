################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/App/fatfs.c 

OBJS += \
./FATFS/App/fatfs.o 

C_DEPS += \
./FATFS/App/fatfs.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/App/%.o FATFS/App/%.su FATFS/App/%.cyclo: ../FATFS/App/%.c FATFS/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FATFS-2f-App

clean-FATFS-2f-App:
	-$(RM) ./FATFS/App/fatfs.cyclo ./FATFS/App/fatfs.d ./FATFS/App/fatfs.o ./FATFS/App/fatfs.su

.PHONY: clean-FATFS-2f-App

