/**
 * Configure ePWM1 and ePWM2
 * ePWM1 = 2000 Hz switching frequency
 * ePWM1 produce an interrupt toggle GPIO12
 * ePWM2 = 4000 Hz switching frequency
 * ePWM2 produce an interrput toggle GPIO20
 */

#include "DSP28x_Project.h"

__interrupt void epwm1_ISR(void);
__interrupt void epwm2_ISR(void);

void main(void)
{
    // Initialize System
    InitSysCtrl();

    // Disable global interrupts
    DINT;

    // Disable interrupts in IER and clear IFR
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize peripherals
    InitPieCtrl();

    // Initialize PIE vector table
    InitPieVectTable();

    // Assign our ISR addresses to elements in the vector table
    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_ISR;
    PieVectTable.EPWM2_INT = &epwm2_ISR;
    EDIS;

    // Configure ePWM1
    // Enable ePWM1 clock
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
    EDIS;

    // Setting ePWM1 carrier as sawtooth waveform
    //EPwm1Regs.TBCTL.bit.CTRMODE = 0;
    // Setting ePWM1 carrier as triangular waveform
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    // Making timer free fun
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 2;

    // Default 45 MHz ePWM1 clock
    // Setting 2000 Hz time period for sawtooth
    //EPwm1Regs.TBPRD = 22499;
    // Setting 2000 Hz time period for triangular
    EPwm1Regs.TBPRD = 11250;
    // Enable ePWM1 interrupt
    EPwm1Regs.ETSEL.bit.INTEN = 1;
    // Enable interrupt on counter 0 (end/beginning of PWM cycle)
    EPwm1Regs.ETSEL.bit.INTSEL = 1;
    // Generate interrupt in every PWM cycle
    EPwm1Regs.ETPS.bit.INTPRD =1;

    // Configure ePWM2
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;
    EDIS;

    // Setting ePWM2 carrier as sawtooth waveform
    // EPwm2Regs.TBCTL.bit.CTRMODE = 0;
    // Setting ePWM2 carrier as triangular waveform
    EPwm2Regs.TBCTL.bit.CTRMODE = 2;
    // Making timer free fun
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 2;

    // Default 45 MHz ePWM1 clock
    // Setting 4000 Hz time period for sawtooth
    // EPwm2Regs.TBPRD = 11249;
    // Setting 4000 Hz time period for triangular
    EPwm2Regs.TBPRD = 5625;
    // Enable ePWM2 interrupt
    EPwm2Regs.ETSEL.bit.INTEN = 1;
    // Enable interrupt on counter 0 (end/beginning of PWM cycle)
    EPwm2Regs.ETSEL.bit.INTSEL = 1;
    // Generate interrupt in every PWM cycle
    EPwm2Regs.ETPS.bit.INTPRD =1;


    // Enable appropriate interrupts in the PIE module
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;

    // Enable interrupt for PIE Group in the IER
    IER |= 0x0004;

    // Enable the global interrupt (INTM = 0)
    EINT;

    // Start clocks (optional)


    // Setup GPIO12 and GPIO20
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;
    EDIS;

    // Infinite Loop
    for(;;);

}


__interrupt void epwm1_ISR(void){
    // Action
    GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
    // Clean-up
    // Resetting the flag bit in ePWM1 interrupt register
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Acknowledge group 3 interrupt in PIEACK
    PieCtrlRegs.PIEACK.bit.ACK3 =1;

}

__interrupt void epwm2_ISR(void){
    // Action
    GpioDataRegs.GPATOGGLE.bit.GPIO20 = 1;
    // Clean-up
    // Resetting the flag bit in ePWM2 interrupt register
    EPwm2Regs.ETCLR.bit.INT = 1;
    // Acknowledge group 3 interrupt in PIEACK
    PieCtrlRegs.PIEACK.bit.ACK3 =1;

}
