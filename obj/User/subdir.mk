################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\SRC\Debug" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\Core" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\User" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\SRC\Peripheral\inc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\drivers" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\include" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\include\libc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\libcpu\risc-v" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\libcpu\risc-v\common" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\src" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\include" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\misc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\serial" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

