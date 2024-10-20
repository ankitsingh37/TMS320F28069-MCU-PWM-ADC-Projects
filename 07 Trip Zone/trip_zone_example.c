/**
 * Trip zone configured to switch off gating signals
 * Trip zone TZ1 is used (GPIO12)
 */

#include "DSP28x_Project.h"

__interrupt void epwm1_ISR(void);
__interrupt void epwm2_ISR(void);
__interrupt void tz1_epwm1_ISR(void);
__interrupt void tz1_epwm2_ISR(void);

Uint16 pwm1_period = 22500;
Uint16 pwm2_period = 11250;
double duty_ratio1 = 0.5;
double duty_ratio2 = 0.5;
Uint16 duty_ratio_cmpa1 = 22500/2;
Uint16 duty_ratio_cmpa2 = 11250/2;
double modulation_signal1 = 0.5;
double modulation_signal2 = 0.5;
Uint16 modulation_signal_cmpa1 = 6000;
Uint16 modulation_signal_cmpa2 = 3000;

void main(void)
{
	// Initialize system
    InitSysCtrl();

    // Disable interrupts
    DINT;

    // Clear interrupts
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize peripherals
    InitPieCtrl();

    // Initialize PIE vector table
    InitPieVectTable();

    // Assign our ISR addresses to the elements in vector table
    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_ISR;
    PieVectTable.EPWM2_INT = &epwm2_ISR;
    PieVectTable.EPWM1_TZINT = &tz1_epwm1_ISR;
    PieVectTable.EPWM2_TZINT = &tz1_epwm2_ISR;
    EDIS;

    // Configure ePWM1
    // Enable ePWM1 clock
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
    EDIS;

    // Setting ePWM1 carrier as sawtooth waveform
    // EPwm1Regs.TBCTL.bit.CTRMODE = 0;
    // Setting ePWM1 carrier as triangular waveform
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    // Making timer free fun
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 2;

    // Default 45 MHz ePWM1 clock
    // Setting 2000 Hz time period for sawtooth
    // EPwm1Regs.TBPRD = pwm1_period-1;
    // Setting 2000 Hz time period for triangular
    EPwm1Regs.TBPRD = pwm1_period/2;
    // Enable ePWM1 interrupt
    EPwm1Regs.ETSEL.bit.INTEN = 1;
    // Enable interrupt on counter 0 (end/beginning of PWM cycle)
    EPwm1Regs.ETSEL.bit.INTSEL = 1;
    // Generate interrupt in every PWM cycle
    EPwm1Regs.ETPS.bit.INTPRD =1;

    // Counter Compare
    // Load mirror CMPAM into CMPA at timer 0
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0;


    // Action Qualifier - for dc-dc converter
    // ePWM1 A - lower device of converter leg
    // EPwm1Regs.AQCTLA.bit.CAU = 2;
    // EPwm1Regs.AQCTLA.bit.ZRO = 1;
    // ePWM1 B - lower device of converter leg
    // EPwm1Regs.AQCTLB.bit.CAU = 1;
    // EPwm1Regs.AQCTLB.bit.ZRO = 2;

    // Action Qualifier - for dc-ac converter
    // ePWM1 A - lower device of converter leg
    EPwm1Regs.AQCTLA.bit.CAU = 1;
    EPwm1Regs.AQCTLA.bit.CAD = 2;
    // ePWM1 B - lower device of converter leg
    EPwm1Regs.AQCTLB.bit.CAU = 1;
    EPwm1Regs.AQCTLB.bit.CAD = 2;

    // Dead band configuration
    // Action qualifier output A is falling edge delay & B is rising edge delay
    EPwm1Regs.DBCTL.bit.IN_MODE = 1;
    // Inverting falling edge delay i.e. signal A
    EPwm1Regs.DBCTL.bit.POLSEL = 2;
    // Both delays selected to go to pins
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;

    // Dead band (always choose max + safety factor)
    // IGBT : 3 micro secs turn off time
    // 5 micro seconds dead time
    EPwm1Regs.DBRED = 225;
    EPwm1Regs.DBFED = 225;

    // Trip zone configuration
    EALLOW;
    // Selecting TZ1 as OSHT1 - one shot trip
    EPwm1Regs.TZSEL.bit.OSHT1 = 1;
    // Turning off pin A and pin B on OSHT1
    EPwm1Regs.TZCTL.bit.TZA = 2;
    EPwm1Regs.TZCTL.bit.TZB = 2;
    // Enabling interrupt on TZ1 fault
    EPwm1Regs.TZEINT.bit.OST = 1;
    EDIS;

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
    // EPwm2Regs.TBPRD = pwm2_period-1;
    // Setting 4000 Hz time period for triangular
    EPwm2Regs.TBPRD = pwm2_period/2;
    // Enable ePWM2 interrupt
    EPwm2Regs.ETSEL.bit.INTEN = 1;
    // Enable interrupt on counter 0 (end/beginning of PWM cycle)
    EPwm2Regs.ETSEL.bit.INTSEL = 1;
    // Generate interrupt in every PWM cycle
    EPwm2Regs.ETPS.bit.INTPRD =1;

    // Counter Compare
    // Load mirror CMPAM into CMPA at timer 0
    EPwm2Regs.CMPCTL.bit.LOADAMODE = 0;


    // Action Qualifier for dc-dc converter
    // ePWM2 A - lower device of converter leg
    // EPwm2Regs.AQCTLA.bit.CAU = 2;
    // EPwm2Regs.AQCTLA.bit.ZRO = 1;
    // ePWM2 B - lower device of converter leg
    // EPwm2Regs.AQCTLB.bit.CAU = 1;
    // EPwm2Regs.AQCTLB.bit.ZRO = 2;

    // Action Qualifier - for dc-ac converter
    // ePWM1 A - lower device of converter leg
    EPwm2Regs.AQCTLA.bit.CAU = 1;
    EPwm2Regs.AQCTLA.bit.CAD = 2;
    // ePWM1 B - lower device of converter leg
    EPwm2Regs.AQCTLB.bit.CAU = 1;
    EPwm2Regs.AQCTLB.bit.CAD = 2;

    // Dead band configuration
    // Action qualifier output A is falling edge delay & B is rising edge delay
    EPwm2Regs.DBCTL.bit.IN_MODE = 1;
    // Inverting falling edge delay i.e. signal A
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    // Both delays selected to go to pins
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;


    // Dead band (always choose max + safety factor)
    // IGBT : 3 micro secs turn off time
    // 5 micro seconds dead time
    EPwm2Regs.DBRED = 225;
    EPwm2Regs.DBFED = 225;

    // Trip zone configuration
    EALLOW;
    // Selecting TZ1 as OSHT1 - one shot trip
    EPwm2Regs.TZSEL.bit.OSHT1 = 1;
    // Turning off pin A and pin B on OSHT1
    EPwm2Regs.TZCTL.bit.TZA = 2;
    EPwm2Regs.TZCTL.bit.TZB = 2;
    // Enabling interrupt on TZ1 fault
    EPwm2Regs.TZEINT.bit.OST = 1;
    EDIS;


    // Enable PIE Module interrupts
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    // Trip zone 1 for ePWM1 and ePWM2
    PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx2 = 1;

    // Enable group interrupts in IER
    IER |= 0x0004;
    IER |= 0x0002;

    // Enable global interrupts
    EINT;

    // Setup GPIO pins
    // Setting up GPIO0, GPIO1, GPIO2 and GPIO3 as ePWM1 and ePWM2
    // GPIO 12 - TZ1 input
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
    EDIS;

    // Initial duty ratio for dc-dc converter
    // ePWM1
    // EPwm1Regs.CMPA.half.CMPA = pwm1_period/2;   // if high resolution then select all
    // ePWM2
    // EPwm2Regs.CMPA.half.CMPA = pwm2_period/2;

    // Initial modulation signal for dc-ac converter
    // ePWM1
    EPwm1Regs.CMPA.half.CMPA = pwm1_period/4;   // if high resolution then select all
    // ePWM2
    EPwm2Regs.CMPA.half.CMPA = pwm2_period/4;


    // Infinite loop
    for(;;);

}

/*
 * For dc-dc converter
__interrupt void epwm1_ISR(void){
    // Action
    // Controller - PI, PID, PR
    // Controller produces duty ratio - 0 and 1
    // Load a value into CMPA that corresponds to this duty ratio - 0 to TBPRD (pwm1_period)

    // Saturation
    if (duty_ratio1 > 0.98) {
        duty_ratio1 = 0.98;
    }
    if (duty_ratio1 < 0.02){
        duty_ratio1 = 0.02;
    }

    // Scale our duty ratio to fit the CMPA register
    duty_ratio_cmpa1 = (Uint16) (duty_ratio1 * pwm1_period);

    // Load CMPA register
    EPwm1Regs.CMPA.half.CMPA = duty_ratio_cmpa1;

    // Clean-up
    // Resetting the flag bit in ePWM1 interrupt register
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Acknowledge group 3 interrupt in PIEACK
    PieCtrlRegs.PIEACK.bit.ACK3 =1;

}

__interrupt void epwm2_ISR(void){
    // Action
    // Controller - PI, PID, PR
    // Controller produces duty ratio - 0 and 1
    // Load a value into CMPA that corresponds to this duty ratio - 0 to TBPRD (pwm2_period)

    // Saturation
    if (duty_ratio2 > 0.98) {
        duty_ratio2 = 0.98;
    }
    if (duty_ratio2 < 0.02){
        duty_ratio2 = 0.02;
    }

    // Scale our duty ratio to fit the CMPA register
    duty_ratio_cmpa2 = (Uint16) (duty_ratio2 * pwm2_period);

    // Load CMPA register
    EPwm2Regs.CMPA.half.CMPA = duty_ratio_cmpa2;


    // Clean-up
    // Resetting the flag bit in ePWM2 interrupt register
    EPwm2Regs.ETCLR.bit.INT = 1;
    // Acknowledge group 3 interrupt in PIEACK
    PieCtrlRegs.PIEACK.bit.ACK3 =1;

}
*/

    // For dc-ac converter

__interrupt void epwm1_ISR(void){
    // Action
    // Controller - PI, PID, PR
    // Controller produces modulation signal : -1 and 1
    // Load a value into CMPA that corresponds to this modulation signal - 0 to TBPRD (pwm1_period)

    // Saturation
    if (modulation_signal1 > 0.98) {
        modulation_signal1 = 0.98;
    }
    if (modulation_signal1 < -0.98){
        modulation_signal1 = -0.98;
    }

    // (modulation_signal1 + 1)/2


    // Scale our duty ratio to fit the CMPA register
    modulation_signal_cmpa1 = (Uint16) (((modulation_signal1 + 1)/2) * (pwm1_period/2));

    // Load CMPA register
    EPwm1Regs.CMPA.half.CMPA = modulation_signal_cmpa1;

    // Clean-up
    // Resetting the flag bit in ePWM1 interrupt register
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Acknowledge group 3 interrupt in PIEACK
    PieCtrlRegs.PIEACK.bit.ACK3 =1;

}

__interrupt void epwm2_ISR(void){
    // Action
    // Controller - PI, PID, PR
    // Controller produces modulation signal : -1 and 1
    // Load a value into CMPA that corresponds to this modulation signal - 0 to TBPRD (pwm1_period)

    // Saturation
    if (modulation_signal2 > 0.98) {
        modulation_signal2 = 0.98;
    }
    if (modulation_signal2 < -0.98){
        modulation_signal2 = -0.98;
    }

    // (modulation_signal2 + 1)/2


    // Scale our duty ratio to fit the CMPA register
    modulation_signal_cmpa2 = (Uint16) (((modulation_signal2 + 1)/2) * (pwm2_period/2));

    // Load CMPA register
    EPwm2Regs.CMPA.half.CMPA = modulation_signal_cmpa2;


    // Clean-up
    // Resetting the flag bit in ePWM2 interrupt register
    EPwm2Regs.ETCLR.bit.INT = 1;
    // Acknowledge group 3 interrupt in PIEACK
    PieCtrlRegs.PIEACK.bit.ACK3 =1;

}

__interrupt void tz1_epwm1_ISR(void){
    // Action
    // Start a timer - turn off the converter until timer expires
    // System specific

    // Clean-up
    EALLOW;
    EPwm1Regs.TZCLR.bit.INT = 1;
    EPwm1Regs.TZCLR.bit.OST = 1;
    EDIS;
    PieCtrlRegs.PIEACK.bit.ACK2 =1;
}
__interrupt void tz1_epwm2_ISR(void){
    // Action
    // Start a timer - turn off the converter until timer expires
    // System specific

    // Clean-up
    EALLOW;
    EPwm2Regs.TZCLR.bit.INT = 1;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EDIS;
    PieCtrlRegs.PIEACK.bit.ACK2 =1;
}
