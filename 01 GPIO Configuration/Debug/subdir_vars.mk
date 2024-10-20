################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../28069_RAM_lnk.cmd \
../F2806x_Headers_nonBIOS.cmd 

ASM_SRCS += \
../F2806x_CodeStartBranch.asm \
../F2806x_usDelay.asm 

C_SRCS += \
../F2806x_GlobalVariableDefs.c \
../F2806x_SysCtrl.c \
../gpio_example.c 

C_DEPS += \
./F2806x_GlobalVariableDefs.d \
./F2806x_SysCtrl.d \
./gpio_example.d 

OBJS += \
./F2806x_CodeStartBranch.obj \
./F2806x_GlobalVariableDefs.obj \
./F2806x_SysCtrl.obj \
./F2806x_usDelay.obj \
./gpio_example.obj 

ASM_DEPS += \
./F2806x_CodeStartBranch.d \
./F2806x_usDelay.d 

OBJS__QUOTED += \
"F2806x_CodeStartBranch.obj" \
"F2806x_GlobalVariableDefs.obj" \
"F2806x_SysCtrl.obj" \
"F2806x_usDelay.obj" \
"gpio_example.obj" 

C_DEPS__QUOTED += \
"F2806x_GlobalVariableDefs.d" \
"F2806x_SysCtrl.d" \
"gpio_example.d" 

ASM_DEPS__QUOTED += \
"F2806x_CodeStartBranch.d" \
"F2806x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../F2806x_CodeStartBranch.asm" \
"../F2806x_usDelay.asm" 

C_SRCS__QUOTED += \
"../F2806x_GlobalVariableDefs.c" \
"../F2806x_SysCtrl.c" \
"../gpio_example.c" 


