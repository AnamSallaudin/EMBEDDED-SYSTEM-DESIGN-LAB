#include <LPC17xx.h>  // Include the LPC17xx header
#include "lcd.c"      // Include your LCD library

#define THRESHOLD 400 // Example threshold value
#define LED_PIN (1 << 22) // Assuming LED is connected to P0.22

void ADC_Init(void) {
    LPC_SC->PCONP |= (1 << 12); // Power up ADC
    LPC_PINCON->PINSEL1 |= (1 << 14); // Select ADC channel 1 (P0.25)
    LPC_ADC->ADCR = (1 << 1) | (1 << 21) | (4 << 8); // Select channel, 4 ADC clocks, enable ADC
}

uint16_t ADC_Read(void) {
    LPC_ADC->ADCR |= (1 << 24); // Start conversion
    while (!(LPC_ADC->ADGDR & (1 << 31))); // Wait for completion
    return (LPC_ADC->ADGDR >> 4) & 0xFFF; // Return the ADC value
}

void LED_Control(uint8_t status) {
    if (status) {
        LPC_GPIO0->FIOSET = LED_PIN; // Turn ON LED
    } else {
        LPC_GPIO0->FIOCLR = LED_PIN; // Turn OFF LED
    }
	}
void delay(uint32_t count) {
    volatile uint32_t i;
    for (i = 0; i < count; i++); // Using the defined variable i
}

int main(void) {
    uint16_t moistureValue;

    // Initialize ADC
    ADC_Init();
    
    // Initialize the GPIO for the LED
    LPC_GPIO0->FIODIR |= LED_PIN; // Set LED pin as output
    
    // Initialize LCD
    LCD_Init();
    
    while (1) {
        moistureValue = ADC_Read(); // Read moisture value
        
        // Display moisture value on LCD
        LCD_Clear();
        LCD_SetCursor(0, 0);
        LCD_Print("Moisture: ");
        LCD_PrintInt(moistureValue);
        
        // Compare with threshold
        if (moistureValue < THRESHOLD) {
            LED_Control(1); // Turn ON LED (pump ON)
        } else {
            LED_Control(0); // Turn OFF LED (pump OFF)
        }
        
        // Simple delay
        delay(1000000); // Call the delay function
    }
		
    
    return 0;
}