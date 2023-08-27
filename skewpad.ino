#define HID_CUSTOM_LAYOUT
#define LAYOUT_FRENCH
#include <HID-Project.h>

#define NBCOLS 4
#define NBROWS 5
#define ROTARYPIN A3

const byte colPins [NBCOLS] = {2, 3, 4, 5};
const byte rowPins [NBROWS] = {18, 15, 14, 16, 10};

const KeyboardKeycode keys [NBCOLS - 1][NBROWS - 1] = {{KEY_5, KEY_1, KEY_8, KEY_4}, {KEY_2, KEY_9, KEY_6, KEY_3}, {KEY_7, KEY_Q, KEY_B, KEY_C}};

KeyboardKeycode tosend [1024] = {};
int sendindex = 0;

bool keyboard [NBCOLS][NBROWS] = {{false, false}};

void setup() {
  Serial.begin(9600);
  
  pinMode(ROTARYPIN, INPUT);
  for(int i = 0; i < NBROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  
  Keyboard.begin();
  Consumer.begin();
}

void loop() {
  int rotary_value = analogRead(ROTARYPIN);
  
  for(int i = 0; i < NBCOLS; i++) {
    pinMode (colPins[i], OUTPUT);
    digitalWrite(colPins[i], LOW);

    for(int j = 0; j < NBROWS; j++) {
      bool pressed = digitalRead(rowPins[j]) == LOW;
      if (pressed && !keyboard[i][j]){

        if (i == NBCOLS - 1) {
          //if (j > 0) {
          //}
        }
        else if (j < NBROWS - 1){
          tosend[sendindex++] = keys[i][j];
        }
        else {
          switch(i) {
            case 0 :
              break;
            case 1 :
              if (sendindex > 0) sendindex--;
              break;
            case 2 :
              if (sendindex > 0) {
                for(int i = 0; i < sendindex; i++)
                {                
                  Keyboard.press(KEY_LEFT_SHIFT);
                  Keyboard.write(tosend[i]);
                  Keyboard.release(KEY_LEFT_SHIFT);
                }
                Keyboard.write(KEY_COMMA);
                
                char buffer [4];
                String poten = itoa(rotary_value, buffer, 10);
                
                Keyboard.press(KEY_LEFT_SHIFT);
                Keyboard.print(poten);
                Keyboard.release(KEY_LEFT_SHIFT);
                
                Keyboard.write(KEY_ENTER);
                
                sendindex = 0;
              }
              break;
          }
        }
        
        keyboard[i][j] = true;
      }
      if (!pressed) keyboard[i][j] = false;
    }
    pinMode (colPins[i], INPUT);
  }
  
  delay(100);
}
