#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char num_of_rows = 4;
const char num_of_columns = 4;

int i = 0;
char question[20] = {};  // Array to store the current input

// Check for addition ('A')
bool isAdd(char arr[]) {
    int checker = 0;
    for (int n = 0; n < 20; n++) {
        if (arr[n] == 'A') {
            checker += 1;
        }
    }
    return (checker > 0);
}

// Find the previous number in the array based on the operation ('A' or 'C')
void prevNumber(char operation, char *num1) {
    int j = 0;
    for (int i = 0; i < 20; i++) {
        if (question[i] == operation) {
            break;
        }
        num1[j] = question[i];
        j++;
    }
    num1[j] = '\0';  // Null-terminate the string
}

// Find the next number in the array after the operation ('A' or 'C')
void nextNumber(char operation, char *num2) {
    int j = 0;
    bool found = false;
    for (int i = 0; i < 20; i++) {
        if (found) {
            num2[j] = question[i];
            j++;
        }
        if (question[i] == operation) {
            found = true;
        }
    }
    num2[j] = '\0';  // Null-terminate the string
}

// Clear the question array after processing
void clearArray() {
    i = 0;  // Reset index to 0
    for (int j = 0; j < 20; j++) {
        question[j] = '\0';  // Clear the array
    }
}

// Check for subtraction ('C')
bool isSub(char arr[]) {
    int checker = 0;
    for (int n = 0; n < 20; n++) {
        if (arr[n] == 'C') {
            checker += 1;
        }
    }
    return (checker > 0);
}

char row_pins[num_of_rows] = {9, 8, 7, 6};
char col_pins[num_of_columns] = {5, 4, 3, 2};

char key_array[num_of_rows][num_of_columns] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad k = Keypad(makeKeymap(key_array), row_pins, col_pins, num_of_rows, num_of_columns);

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
}

void loop() {
    char key_pressed = k.getKey();
    
    if (key_pressed) {
        lcd.setCursor(0,0);
        lcd.clear();
        if (key_pressed != 'D') {
            // Store the key pressed in the question array
            if(key_pressed == 'A'){
                lcd.print('+');
            } else if(key_pressed == 'C'){
                  lcd.print('-');
            } else {
              lcd.print(key_pressed);
            }
            
            question[i] = key_pressed;
            i++;
        } else {
            if (isAdd(question)) {
                char num1[20];
                char num2[20];

                prevNumber('A', num1);  // Get the number before 'A'
                nextNumber('A', num2);  // Get the number after 'A'

                int sum = atoi(num1) + atoi(num2);  // Convert to integer and add
                lcd.setCursor(0, 1);
                lcd.print("Sum: ");
                lcd.print(sum);  // Display the sum on the LCD
                Serial.println(sum);

            } else if (isSub(question)) {
                char num1[20];
                char num2[20];

                prevNumber('C', num1);  // Get the number before 'C'
                nextNumber('C', num2);  // Get the number after 'C'

                int sub = atoi(num1) - atoi(num2);  // Convert to integer and subtract
                lcd.setCursor(0, 1);
                lcd.print("Sub: ");
                lcd.print(sub);  // Display the subtraction result
                Serial.println(sub);
            } else {
              lcd.setCursor(0, 1);
              lcd.print("Sorry,Try Again");
            }

            // Reset the input array for the next calculation
            clearArray();
        }
    }
}
