// Imports
#include <Adafruit_NeoPixel.h>

// declare neopixel strip
int stripLength = 120;
int stripPin = 7;
Adafruit_NeoPixel strip(stripLength, stripPin);

// button vars
boolean buttonState = false;
boolean prevButtonState = false;
int buttonPin = 8;

//timer vars
unsigned long startTime = millis();
int interval = 40;
unsigned long endTime;


//finite state Machine state var
int state = 0;

void setup()
{
  // set up the Serial port
  Serial.begin(9600);
  
  // set up the neopixel strip
  // clear it out at the beginning
  strip.begin();
  strip.clear();
  strip.show();
}

void loop(){
  prevButtonState = buttonState;
  buttonState = debounce(buttonPin, prevButtonState);
  Serial.println(buttonState);
  
  //toggle if statement
  if (buttonState == HIGH && prevButtonState == LOW){
    state += 1;
    
    //state = state%3; is the same as the following
    if (state > 1){
      state = 0;
    }
  }
  
  Serial.println(state);
  
  //finite state machine
  switch(state){
    case 0:
    strip.clear();
    strip.show();
    break;
   
    case 1:
    if(millis() - startTime > interval){
      rainbowTimer();
    }
    strip.show();
    break;   
  }
  
}

/*
Debounce function for buttons
*/
boolean debounce(int aButPin, boolean aPrevState){
  // get the bouncy reading of the button
  boolean aButtonState = digitalRead(aButPin);

  // if the button state is different from what it
  // was last frame, then delay so it doesn't read
  // while the button is bouncing
  if (aButtonState != aPrevState){
    delay(15);
  }

  return aButtonState;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void rainbowTimer() { // modified from Adafruit example to make it a state machine
  static uint16_t j = 0;
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((i + j) & 255));
  }
  strip.show();
  j++;
  if (j >= 256) j = 0;
  startTime = millis(); // time for next change to the display
}
