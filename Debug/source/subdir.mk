################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/button.cpp \
../source/flowsensor.cpp \
../source/heartrate.cpp \
../source/main.cpp \
../source/motorcontrol.cpp \
../source/pressuresensor.cpp \
../source/solenoid.cpp \
../source/spi_proto_slave.cpp 

OBJS += \
./source/button.o \
./source/flowsensor.o \
./source/heartrate.o \
./source/main.o \
./source/motorcontrol.o \
./source/pressuresensor.o \
./source/solenoid.o \
./source/spi_proto_slave.o 

CPP_DEPS += \
./source/button.d \
./source/flowsensor.d \
./source/heartrate.d \
./source/main.d \
./source/motorcontrol.d \
./source/pressuresensor.d \
./source/solenoid.d \
./source/spi_proto_slave.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK66FN2M0VMD18" -I../freertos/Source/include -I../startup -I../board -I../utilities -I../freertos/Source/portable/GCC/ARM_CM4F -I../CMSIS -I../source -I../drivers -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


