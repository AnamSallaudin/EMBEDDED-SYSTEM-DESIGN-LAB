#include <LPC17xx.h>

int seven_seg[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

// delay function
void delay() {
    volatile int j;
    for(j = 0; j < 1000; j++);
}

void displayHex(int value) {
    LPC_GPIO0->FIOPIN = (seven_seg[value] << 4); // Shift the segment data to P0.4 to P0.11
}

int main(void) {
    int i;
    SystemInit();
    SystemCoreClockUpdate();

    // Configure P0.4 to P0.11 as output for the data lines
    LPC_PINCON->PINSEL0 &= ~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14) | (3 << 16) | (3 << 18) | (3 << 20) | (3 << 22));
    LPC_GPIO0->FIODIR |= (0xFF << 4); // Set P0.4 to P0.11 as output

    // Configure P0.5 to P0.18 as outputs for the decoder control
    LPC_GPIO0->FIODIR |= (0xFF << 5); // Set P0.5 to P0.12 as output

    while (1) {
        for (i = 0; i < 16; i++) { // Count from 0 to F
            displayHex(i); // Display the hex value
            delay(); // Delay for visibility
        }
    
		}
		
}
