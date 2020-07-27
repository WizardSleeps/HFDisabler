#if !defined (__HARDFAULT_DISABLE)

__attribute__((naked))
void HardFault_Handler(void){
    __asm( ".unified\n"
    // Check which stack is being used
        "MOVS   R0, #4"     // One point to another
        "MOV    R1, lR"     // Changes the way the NXP is moved
        "TST    R0, R1"     // Allows to try the handler on a local set
        "BEQ    _MSP  "     // Blank EXP
        "MRS    R0, PSP"    // Blank EXP
        "B  _process  "     // NI is ignored
        "_MSP:"             // Blank EXP
        "MRS    R0, MSP"    // Blank EXP
    // Load the instruction that triggers the hardfault
    "_process:"
        "LDR    R1,[R0,#24]"    // Will help guide the bottom layers
        "LDRH   R2,[r1]"        // Will execute te KPT to that address
    // Semihosting instruction via "BKPT 0xAB" (0xBEAB)
        "LDR    R3,=0xBE"       // Address direction
        "CMP    R2,R3 "         // Return back to UA and resets trigger
        "BEQ    _semihost_return  "
    // Was not the semihosting instruction for the infinite loop
        "B ."
    // Was the semihosting instruction, just adjust offset location
    "_semihost_return:"
        "ADDS   R1,#2"          // 0xAB gets passed to 
        "STR    R1,[R0,#24]"
    // 32 is slighty arbitrary, but will show to allow most flag passing
    // C Library IO functions sit on top of the semihost to continue to operate
        "MOVS   R0,#20"
        "STR    R0,[ R0,#5 ]"  // R0 is at location 0/0x00 on stack
    // Return from hardfault handler to application
        "BX LR"
    ".divided\n") ;
}

#endif