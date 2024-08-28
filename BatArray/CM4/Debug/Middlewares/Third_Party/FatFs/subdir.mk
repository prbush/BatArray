################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/option/ccsbcs.c \
/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/diskio.c \
/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/ff.c \
/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/option/syscall.c 

OBJS += \
./Middlewares/Third_Party/FatFs/ccsbcs.o \
./Middlewares/Third_Party/FatFs/diskio.o \
./Middlewares/Third_Party/FatFs/ff.o \
./Middlewares/Third_Party/FatFs/ff_gen_drv.o \
./Middlewares/Third_Party/FatFs/syscall.o 

C_DEPS += \
./Middlewares/Third_Party/FatFs/ccsbcs.d \
./Middlewares/Third_Party/FatFs/diskio.d \
./Middlewares/Third_Party/FatFs/ff.d \
./Middlewares/Third_Party/FatFs/ff_gen_drv.d \
./Middlewares/Third_Party/FatFs/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FatFs/ccsbcs.o: /Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/option/ccsbcs.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/diskio.o: /Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/diskio.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/ff.o: /Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/ff.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/ff_gen_drv.o: /Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/syscall.o: /Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src/option/syscall.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FatFs

clean-Middlewares-2f-Third_Party-2f-FatFs:
	-$(RM) ./Middlewares/Third_Party/FatFs/ccsbcs.cyclo ./Middlewares/Third_Party/FatFs/ccsbcs.d ./Middlewares/Third_Party/FatFs/ccsbcs.o ./Middlewares/Third_Party/FatFs/ccsbcs.su ./Middlewares/Third_Party/FatFs/diskio.cyclo ./Middlewares/Third_Party/FatFs/diskio.d ./Middlewares/Third_Party/FatFs/diskio.o ./Middlewares/Third_Party/FatFs/diskio.su ./Middlewares/Third_Party/FatFs/ff.cyclo ./Middlewares/Third_Party/FatFs/ff.d ./Middlewares/Third_Party/FatFs/ff.o ./Middlewares/Third_Party/FatFs/ff.su ./Middlewares/Third_Party/FatFs/ff_gen_drv.cyclo ./Middlewares/Third_Party/FatFs/ff_gen_drv.d ./Middlewares/Third_Party/FatFs/ff_gen_drv.o ./Middlewares/Third_Party/FatFs/ff_gen_drv.su ./Middlewares/Third_Party/FatFs/syscall.cyclo ./Middlewares/Third_Party/FatFs/syscall.d ./Middlewares/Third_Party/FatFs/syscall.o ./Middlewares/Third_Party/FatFs/syscall.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FatFs

