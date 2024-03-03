#define HID_CUSTOM_LAYOUT
#define LAYOUT_FRENCH
#include <HID-Project.h>

#define NBROWS 5
#define NBCOLS 4

const byte colPins [NBCOLS] = {15, 14, 16, 10};
const byte rowPins [NBROWS] = {2, 3, 4, 5, 6};

const KeyboardKeycode keys [NBROWS][NBCOLS] = {{KEY_A, KEY_B, KEY_C, KEY_D}, {KEY_E, KEY_F, KEY_G, KEY_H}, {KEY_I, KEY_J, KEY_K, KEY_L}, {KEY_M, KEY_N, KEY_O, KEY_P}, {KEY_Q, KEY_R, KEY_S, KEY_T}};

bool keyboard [NBROWS][NBCOLS] = {{false, false}};

void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < NBCOLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  
  Keyboard.begin();
}

void loop() {
  
  for(int i = 0; i < NBROWS; i++) {
    pinMode (rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);

    for(int j = 0; j < NBCOLS; j++) {
      bool pressed = digitalRead(colPins[j]) == LOW;
      if (pressed && !keyboard[i][j]){

          Keyboard.write(keys[i][j]);
        }
        
        keyboard[i][j] = true;
      if (!pressed) keyboard[i][j] = false;
    }
    pinMode (rowPins[i], INPUT);
  }
  
  delay(100);
}
