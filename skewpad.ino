#define HID_CUSTOM_LAYOUT
#define LAYOUT_FRENCH
#include <HID-Project.h>

#define NBCOLS 4
#define NBROWS 5
#define ROTARYPIN A3

const byte colPins [NBCOLS] = {2, 3, 4, 5};
const byte rowPins [NBROWS] = {18, 15, 14, 16, 10};

const KeyboardKeycode keys [NBCOLS - 1][NBROWS - 1] = {{KEY_F13, KEY_F14, KEY_F15, KEY_F16}, {KEY_F17, KEY_F18, KEY_F19, KEY_F20}, {KEY_F21, KEY_F22, KEY_F23, KEY_F24}};
const ConsumerKeycode consumers [NBROWS - 1] = {MEDIA_VOLUME_MUTE, MEDIA_PLAY_PAUSE, MEDIA_NEXT, MEDIA_PREVIOUS};

bool keyboard [NBCOLS][NBROWS] = {{false, false}};
int rotary_value = 0;

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
  int rotary_value_new = map(analogRead(ROTARYPIN), 0, 1023, 0, 65);
  while (rotary_value != rotary_value_new){
    Consumer.write(rotary_value < rotary_value_new ? MEDIA_VOLUME_UP : MEDIA_VOLUME_DOWN);
    rotary_value += rotary_value < rotary_value_new ? 1 : -1;
  }
  
  for(int i = 0; i < NBCOLS; i++) {
    pinMode (colPins[i], OUTPUT);
    digitalWrite(colPins[i], LOW);

    for(int j = 0; j < NBROWS; j++) {
      bool pressed = digitalRead(rowPins[j]) == LOW;
      if (pressed && !keyboard[i][j]){

        if (i == NBCOLS - 1) {
          if (j > 0) {
            Consumer.write(consumers[j - 1]); 
          }
        }
        else if (j < NBROWS - 1){
          Keyboard.write(keys[i][j]);
        }
        else {
          switch(i) {
            case 0 :
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.write(KEY_J);
              Keyboard.release(KEY_LEFT_CTRL);
              Keyboard.release(KEY_LEFT_SHIFT);
              break;
            case 1 :
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.write(KEY_Z);
              Keyboard.release(KEY_LEFT_CTRL);
              Keyboard.release(KEY_LEFT_SHIFT);
              break;
            case 2 :
              Keyboard.press(KEY_LEFT_GUI);
              Keyboard.write(KEY_L);
              Keyboard.release(KEY_LEFT_GUI);
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
