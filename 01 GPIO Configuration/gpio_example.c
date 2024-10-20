/**
 * example to configure the GPIO pins
 * Will set, toggle and clear some sample GPIO pins
 */

#include "DSP28x_Project.h"

void main(void)
{
    // Initialize the system
    InitSysCtrl();

    // Setup our GPIO pins - GPIO12, GPIO20, GPIO32
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    EDIS;

    // Manage the state of our GPIO pins
    GpioDataRegs.GPASET.bit.GPIO12 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
    GpioDataRegs.GPBSET.bit.GPIO32 = 1;

    // Stay idle - infinite loop
    for(;;);
}
