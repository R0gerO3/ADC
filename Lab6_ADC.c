#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <stdbool.h>

void delayMs(int n);

///////////////////////////////////////////////////////////////////////////////////////////Pre Lab side

// PORTS_INIT FUNCTION
void PORTS_init() {
    SYSCTL->RCGCGPIO |= 0x01; // enable clk for port A
    SYSCTL->RCGCGPIO |= 0x10; // enable clk for port E
    SYSCTL->RCGCGPIO |= 0x04; // enable clk for port C

    GPIOA->AMSEL &= ~0x3C; // uses pins 2-5 for the data pins D4-D7 0011 1100
    GPIOA->DATA &= ~0x3C;
    GPIOA->DIR |= 0x3C;
    GPIOA->DEN |= 0x3C; 

    GPIOE->AMSEL &= ~0x01; // for SS SPI for R/S of LCD pin 0 0000 0001
    GPIOE->DIR |= 0x01; // Register select pin
    GPIOE->DEN |= 0x01;
    GPIOE->DATA |= 0x01;

    GPIOC->AMSEL &= ~0x40; // CS or EN for LCD pin 4 0001 0000
    GPIOC->DIR |= 0x40; // enable pin for LCD
    GPIOC->DEN |= 0x40;
    GPIOC->DATA &= ~0x40;
}

// LCD write data/command function
void LCD_nibble_write(char data, unsigned char control) {
    GPIOA->DIR |= 0x3C; // setting PA2-PA5 as outputs (interface to LCD)
    GPIOA->DATA &= ~0x3C; // clearing the line
    GPIOA->DATA |= (data & 0xF0) >> 2; // extracting the upper nibble
    /* set RS bit */
    if (control & 1) // control & RS
        GPIOE->DATA |= 1; // check if LCD data is data
    else
        GPIOE->DATA &= ~1; // check if LCD data is command

    /* sending a high to low transition pulse on LCD enable pin (PC6) */
    GPIOC->DATA |= 1 << 6;
    delayMs(0);
    GPIOC->DATA &= ~(1 << 6);
    GPIOA->DIR &= ~0x3C; // clearing the line
}

void LCD_command(unsigned char command) { // 8 bit commands function
    LCD_nibble_write(command & 0xF0, 0); // upper nibble
    LCD_nibble_write(command << 4, 0); // lower nibble
    if (command < 3)
        delayMs(2); // maximum delay of 2 ms for the first two instructions
    else
        delayMs(1); // maximum delay of 1 ms for all other instructions
}

void LCD_init(void) {
    PORTS_init(); // CONTAINS CODE FROM LCD CONFIGURATION SECTION
    delayMs(20);
    LCD_nibble_write(0x30, 0);
    delayMs(5);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x20, 0); // use 4-bit data mode
    delayMs(1);
    LCD_command(0x28); // set 4-bit data, 2-line, 5x7 font
    LCD_command(0x06); // move cursor right
    LCD_command(0x01); // clear screen, move cursor to home
    LCD_command(0x0F); // turn on display, cursor blinking
}

// LCD write character data function
void LCD_data(char data) {
    LCD_nibble_write(data & 0xF0, 1); // upper nibble
    LCD_nibble_write(data << 4, 1); // lower nibble
    delayMs(1);
}

// Prelab
int main(void) {
    SYSCTL->RCGCADC |= (1 << 0);//enable clk adc mod 0
    SYSCTL->RCGCGPIO |= 0x10; // enable clock to GPIOE
    GPIOE->AFSEL |= 0x26;
    GPIOE->DEN &= ~0x26;
    GPIOE->AMSEL |= 0x26;
    ADC0->ACTSS &= ~1;//disable sampler sequence
    ADC0->EMUX &= ~0x000F;//enable software trigger by clearing lower 4 bits
    ADC0->SSMUX0 &= ~0xFFFFFFFF;//clear all bits
    ADC0->SSMUX0 |= 0x00000821;//channel 1 for 1st sample, chan 2 for second, chan 8 for 3rd sample
    ADC0->SSCTL0 |= 0x00000600;//end sequence at 3rd sample
    ADC0->ACTSS |= 1;//enable sampler sequence

    LCD_init();

    SYSCTL->RCGCGPIO |= 0x08; // enable clock to GPIOD
    GPIOD->DIR &= ~0x08; // set pin 3-0 as input
    GPIOD->AMSEL &= ~0x08; // disable analog functions
    GPIOD->DEN |= 0x08; // set pin 3-0 as digital 

    // Conversion results
    int potent_dig;
    int photosensor_dig;
    int temperature_dig;
    double analog_reading;
    bool buttonState = false;
    bool lastButtonState = false;
    bool format = false;
    int sensorIndex = 0;

    char reading_str[9] = {' '}; // storage location for analog readings
 // Initial display setup
  
    while (1) {
        ADC0->PSSI |= 1; // starting the conversion
        while ((ADC0->RIS & 1) == 0); // wait for conversion to be completed
        potent_dig = ADC0->SSFIFO0; // potentiometer
        photosensor_dig = ADC0->SSFIFO0; // light sensor
        temperature_dig = ADC0->SSFIFO0; // temperature sensor
        ADC0->ISC = 1; // clear interrupt flag
        buttonState = (GPIOD->DATA & 0x08) >> 3;

			
			//Temperature sensor F and C
			
//			
//        if (buttonState != lastButtonState) {
//            if (buttonState == 1) {
//                sensorIndex++;
//                sensorIndex %= 3;
//                format = !format; // F->C
//                delayMs(50);
//            }
//        }

        // Switch statement for displaying temperature in the correct format
//        analog_reading = (temperature_dig * 3.3) / 4095;
//        analog_reading = analog_reading * 100;
//        if (format) { // Fahrenheit
//            analog_reading = (analog_reading * 9.0 / 5.0) + 32.0;
//        }
//        sprintf(reading_str, "%.6f", analog_reading);
//        LCD_command(1); // clear display of LCD
//        LCD_data('T');
//        LCD_data('E');
//        LCD_data('M');
//        LCD_data('P');
//        LCD_data('E');
//        LCD_data('R');
//        LCD_data('A');
//        LCD_data('T');
//        LCD_data('U');
//        LCD_data('R');
//        LCD_data('E');
//        LCD_command(0xC0); // next line
//        for (unsigned int i = 0; i < sizeof(reading_str) / sizeof(reading_str[0]); i++) {
//            LCD_data(reading_str[i]);
//        }
//        if (format) {
//            LCD_data('F');
//        } else {
//            LCD_data('C');
//        }
//        delayMs(100);
//			}
//		}
//			
    








//Potentiometer


//   LCD_command(1); // clear display of LCD
//    LCD_data('P');
//    LCD_data('O');
//    LCD_data('T');
//    LCD_data('E');
//    LCD_data('N');
//    LCD_data('T');
//    LCD_data('I');
//    LCD_data('O');
//    LCD_data('M');
//    LCD_data('E');
//    LCD_data('T');
//    LCD_data('E');
//    LCD_data('R');
//    LCD_command(0xC0); // move to next line
//         analog_reading = (potent_dig * 3.3) / 4095;
//        sprintf(reading_str, "%.6f", analog_reading);

//        // Update only the necessary part of the display
//        for (unsigned int i = 0; i < sizeof(reading_str) / sizeof(reading_str[0]); i++) {
//            LCD_data(reading_str[i]);
//        }
//        LCD_data('V');

//        delayMs(500); 
//			}
//		}


		
		
               
// Light sensor 


//LCD_command(1); // clear display of LCD
//LCD_data('L');
//LCD_data('I');
//LCD_data('G');
//LCD_data('H');
//LCD_data('T');
//LCD_data(' ');
//LCD_data('S');
//LCD_data('E');
//LCD_data('N');
//LCD_data('S');
//LCD_data('O');
//LCD_data('R');
//LCD_command(0xC0); // next line

//analog_reading = (photosensor_dig * 3.3) / 4095;
//sprintf(reading_str, "%.6f", analog_reading);

//for (unsigned int i = 0; i < sizeof(reading_str) / sizeof(reading_str[0]); i++) {
//    LCD_data(reading_str[i]);
//}

//LCD_data('V'); // This is where you correctly display 'V' for Volts
//delayMs(500);
//}
//		}



