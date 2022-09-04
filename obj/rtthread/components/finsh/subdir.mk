################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtthread/components/finsh/cmd.c \
../rtthread/components/finsh/msh.c \
../rtthread/components/finsh/msh_cmd.c \
../rtthread/components/finsh/msh_file.c \
../rtthread/components/finsh/shell.c \
../rtthread/components/finsh/symbol.c 

OBJS += \
./rtthread/components/finsh/cmd.o \
./rtthread/components/finsh/msh.o \
./rtthread/components/finsh/msh_cmd.o \
./rtthread/components/finsh/msh_file.o \
./rtthread/components/finsh/shell.o \
./rtthread/components/finsh/symbol.o 

C_DEPS += \
./rtthread/components/finsh/cmd.d \
./rtthread/components/finsh/msh.d \
./rtthread/components/finsh/msh_cmd.d \
./rtthread/components/finsh/msh_file.d \
./rtthread/components/finsh/shell.d \
./rtthread/components/finsh/symbol.d 


# Each subdirectory must supply rules for building sources it contributes
rtthread/components/finsh/%.o: ../rtthread/components/finsh/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\SRC\Debug" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\Core" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\User" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\SRC\Peripheral\inc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\drivers" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\include" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\include\libc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\libcpu\risc-v" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\libcpu\risc-v\common" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\src" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\include" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\misc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\serial" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

