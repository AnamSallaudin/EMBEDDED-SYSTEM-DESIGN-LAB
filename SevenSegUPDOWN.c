#include <LPC17xx.h>

int seven_seg[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
};

void delay(int count) {
    volatile int j;
    for(j = 0; j < count; j++);
}

void displayNumber(int value) {
    LPC_GPIO0->FIOPIN = (seven_seg[value] << 4); // Shift the segment data to P0.4 to P0.11
}

int main() {
    int count = 0;
    int last_switch_state = 1; // Assume switch is not pressed (HIGH)
    
    SystemInit();
    SystemCoreClockUpdate();

    // Configure P0.4 to P0.11 as output for the data lines
    LPC_PINCON->PINSEL0 &= ~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14) | (3 << 16) | (3 << 18) | (3 << 20) | (3 << 22));
    LPC_GPIO0->FIODIR |= (0xFF << 4); // Set P0.4 to P0.11 as output

    // Configure P0.0 as input for the switch
    LPC_GPIO0->FIODIR &= ~(1 << 0); // Set P0.0 as input
    LPC_GPIO0->FIOMASK |= (1 << 0); // Mask other bits to ensure reading P0.0

    while (1) {
        int switch_state = (LPC_GPIO0->FIOPIN & (1 << 0)) == 0; // Read switch state (active LOW)

        // Check for a falling edge (switch pressed)
        if (switch_state == 1 && last_switch_state == 0) {
            // Increment or decrement count
            count++;
            if (count > 9) {
                count = 0; // Wrap around
            }
        } else if (switch_state == 0 && last_switch_state == 1) {
            // Decrement count
            count--;
            if (count < 0) {
                count = 9; // Wrap around
            }
        }

        displayNumber(count); // Display the current count
        last_switch_state = switch_state; // Store the current switch state
        delay(100000); // Delay for debouncing
    }
}
