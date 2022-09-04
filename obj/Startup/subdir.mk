################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Startup/startup_ch32v30x.S 

OBJS += \
./Startup/startup_ch32v30x.o 

S_UPPER_DEPS += \
./Startup/startup_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.S
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\Startup" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\drivers" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\include" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\include\libc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\libcpu" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\libcpu\risc-v\common" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\src" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\include" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\misc" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\drivers\serial" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread\components\finsh" -I"C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\rtthread" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

