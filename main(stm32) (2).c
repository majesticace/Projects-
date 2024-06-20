#include "stm32l476xx.h"
#include "core_cm4.h"

void LED_init(void);
void pushButton_init(void);
void keypad_init(void);
void delay(int);
int scanKeypad(void);
int isValidCode(char* code);

// Global variables
char correctCode[] = "123456";
char inputCode[6];
int inputIndex = 0;
int locked = 0;

int main(void) {
    char key;
    LED_init();
    pushButton_init();
    keypad_init();

    // Sample infinite while loop
    while (1) {
        // Arm the lock when the user button is pressed
        if ((GPIOC->IDR & GPIO_IDR_ID13) == 0) {
            locked = 1;
            GPIOA->ODR |= (1UL << 5); // Turn on LED when locked
            while ((GPIOC->IDR & GPIO_IDR_ID13) == 0)
                ; // Wait until the button is released
        }

        // When the lock is unlocked, accept user input from the keypad
        if (!locked) {
            key = scanKeypad();
            if (key == 0)
                continue;
            else {
                inputCode[inputIndex++] = key;
                if (inputIndex == 6) {
                    inputCode[inputIndex] = '\0'; // Null-terminate the input code
                    if (isValidCode(inputCode)) {
                        // Unlock the lock
                        GPIOA->ODR &= ~(1UL << 5); // Turn off LED
                        locked = 0;
                        inputIndex = 0;
                    } else {
                        // Invalid code, blink the LED and reset inputCode
                        for (int i = 0; i < 3; i++) {
                            GPIOA->ODR ^= (1UL << 5); // Toggle LED
                            delay(1000); // Delay for 1 second
                        }
                        inputIndex = 0;
                    }
                }
            }
        }
    }
}

void LED_init(void) {
    // Same as before...
}

void pushButton_init(void) {
    // Same as before...
}

void keypad_init(void) {
    // Same as before...
}

void delay(int a) {
    // Same as before...
}

int scanKeypad(void) {
    // Same as before...
}

int isValidCode(char* code) {
    for (int i = 0; i < 6; i++) {
        if (code[i] != correctCode[i])
            return 0; // Code is invalid
    }
    return 1; // Code is valid
}
what does this code accomplish?