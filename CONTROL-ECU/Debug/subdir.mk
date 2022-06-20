################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../buzzer.c \
../controlecu.c \
../external_eeprom.c \
../gpio.c \
../motor.c \
../timers.c \
../twi.c \
../uart.c 

OBJS += \
./buzzer.o \
./controlecu.o \
./external_eeprom.o \
./gpio.o \
./motor.o \
./timers.o \
./twi.o \
./uart.o 

C_DEPS += \
./buzzer.d \
./controlecu.d \
./external_eeprom.d \
./gpio.d \
./motor.d \
./timers.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


