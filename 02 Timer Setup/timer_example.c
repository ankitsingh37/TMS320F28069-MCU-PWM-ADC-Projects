/**
 * Example for configuring and using Timer 0,1 and 2
 * Timer 0 - 0.01s timer
 * Timer 1 - 1.50s timer
 * Timer 2 - 60.0s timer
 * With timer 0, toggle GPIO12
 * With timer 1, toggle GPIO20
 * With timer 2, toggle GPIO32
 */

#include "DSP28x_Project.h"

__interrupt void timer0_ISR(void);
__interrupt void timer1_ISR(void);
__interrupt void timer2_ISR(void);

void main(void)
{
	// Initializing the system
    InitSysCtrl();

    // Disable global interrupt mask bit
    DINT;

    // Disable all interrupts and clear all flags
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize PIE module
    InitPieCtrl();

    // Initialize PIE vecotr table
    InitPieVectTable();

    EALLOW;
    PieVectTable.TINT0 = &timer0_ISR;
    PieVectTable.TINT1 = &timer1_ISR;
    PieVectTable.TINT2 = &timer2_ISR;
    EDIS;

    // Configure Timer 0
    // Enable Timer 0 clock
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
    EDIS;
    // Stop the timer
    CpuTimer0Regs.TCR.bit.TSS = 1;
    // Load period corresponding to 0.01s
    CpuTimer0Regs.PRD.all = 900000;
    // Load period into the timer counter
    CpuTimer0Regs.TCR.bit.TRB = 1;
    // Enable Timer 0 interrupt
    CpuTimer0Regs.TCR.bit.TIE = 1;
    // Enable Free run mode
    CpuTimer0Regs.TCR.bit.FREE = 1;
    // Set pre-scale to 0 (disable)
    CpuTimer0Regs.TPR.bit.TDDR = 0;
    CpuTimer0Regs.TPRH.bit.TDDRH = 0;


    // Configure Timer 1
    // Enable Timer 1 clock
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1;
    EDIS;
    // Stop the timer
    CpuTimer1Regs.TCR.bit.TSS = 1;
    // Load period corresponding to 1.50s
    CpuTimer1Regs.PRD.all = 135000000;
    // Load period into the timer counter
    CpuTimer1Regs.TCR.bit.TRB = 1;
    // Enable Timer 0 interrupt
    CpuTimer1Regs.TCR.bit.TIE = 1;
    // Enable Free run mode
    CpuTimer1Regs.TCR.bit.FREE = 1;
    // Set pre-scale to 0 (disable)
    CpuTimer1Regs.TPR.bit.TDDR = 0;
    CpuTimer1Regs.TPRH.bit.TDDRH = 0;

    // Configure Timer 2
    // Enable Timer 2 clock
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1;
    EDIS;
    // Stop the timer
    CpuTimer2Regs.TCR.bit.TSS = 1;
    // Load period corresponding to 30.0s
    CpuTimer2Regs.PRD.all = 2700000002;
    // Load period into the timer counter
    CpuTimer2Regs.TCR.bit.TRB = 1;
    // Enable Timer 0 interrupt
    CpuTimer2Regs.TCR.bit.TIE = 1;
    // Enable Free run mode
    CpuTimer2Regs.TCR.bit.FREE = 1;
    // Set pre-scale to 0 (disable)
    CpuTimer2Regs.TPR.bit.TDDR = 1;
    CpuTimer2Regs.TPRH.bit.TDDRH = 0;


    // Configure GPIO pins
    // Setup our GPIO pins - GPIO12, GPIO20, GPIO32
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    EDIS;


    // Setup interrupts in PIE module
    // Enable timer 0 TINT0 in group 1
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    // Enable interrupts in IER
    IER |= M_INT1;
    //IER |= 0x0001;   // Bit 0 (INT1) is enabled
    IER |= M_INT13;    // For Timer 1
    IER |= M_INT14;    // For Timer 2

    // Enable global interrupt
    EINT;
    ERTM;

    // Start timers
    CpuTimer0Regs.TCR.bit.TSS = 0;
    CpuTimer1Regs.TCR.bit.TSS = 0;
    CpuTimer2Regs.TCR.bit.TSS = 0;

    // Infinite loop
    for (;;);

}


__interrupt void timer0_ISR(void){
    // Action
    GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;

    // Clean-up
    CpuTimer0Regs.TCR.bit.TIF = 1;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;  // Reset Group 1 of PIE module
}

__interrupt void timer1_ISR(void){
    // Action
    GpioDataRegs.GPATOGGLE.bit.GPIO20 = 1;

    // Clean-up
    CpuTimer1Regs.TCR.bit.TIF = 1;
}

__interrupt void timer2_ISR(void){
    // Action
    GpioDataRegs.GPBTOGGLE.bit.GPIO32 = 1;

    // Clean-up
    CpuTimer2Regs.TCR.bit.TIF = 1;
}





