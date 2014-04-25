

int current_pixel = 0;
int current_pixel2 = 0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete



#include <Adafruit_NeoPixel.h>

#define PIN 6
#define TOTAL_PIXELS 60 //46 ou 60
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  Serial.begin(9600);
  
  Serial.println("****************");
  Serial.println("* votaBrasil v0");
  Serial.print("* ");
  Serial.print(strip.numPixels());
  Serial.println(" leds");
  Serial.println("****************");

  inputString.reserve(200);
  
  
  
}

void loop() {
  
  if (stringComplete) {
    //Serial.print(inputString); 
    //Serial.print(" ");
    //Serial.print(inputString.length());
    //Serial.print(" ");
    //Serial.println(inputString.toInt()-1);

    if (inputString.equals("rainbow")) {
      rainbow(30);
      return;
    } else if (inputString.equals("blue")) {
      colorWipe(strip.Color(0, 0, 255), 0);
      return;
    } else if (inputString.equals("red")) {
      colorWipe(strip.Color(255, 0, 0), 0);
      return;
    } else if (inputString.equals("green")) {
      colorWipe(strip.Color(0, 255, 0), 0);
      return;
    } else if (inputString.equals("yellow")) {
      colorWipe(strip.Color(255, 255, 0), 0);
      return;
    } else if (inputString.equals("black")) {
      colorWipe(strip.Color(0, 0, 0), 0);
      return;
    }
    
    
    if (inputString.length() != 9) {
      clearInputString();
      Serial.println("INVALID INPUT");
      return;
    }
    
    int green = map(inputString.substring(0, 3).toInt(), 0, 100, 0, TOTAL_PIXELS);
    int yellow = map(inputString.substring(3, 6).toInt(), 0, 100, 0, TOTAL_PIXELS);
    int red = map(inputString.substring(6).toInt(), 0, 100, 0, TOTAL_PIXELS);

    clearInputString();
    
    // jogar pixels de sobra no amarelo
    if (green + yellow + red != TOTAL_PIXELS) {
      if (green + yellow + red == 0) {
        Serial.println("zero");
        colorWipe(strip.Color(0, 0, 0), 0);
        return;
      } else {
        Serial.println("recalculando...");
        yellow = TOTAL_PIXELS - green - red;
      }
    }

    //Serial.print(green); Serial.print("\t");
    //Serial.print(yellow); Serial.print("\t");
    //Serial.print(red); Serial.println();
    
    //int green = random(0, TOTAL_PIXELS);
    //int yellow = random(0, TOTAL_PIXELS - green);
    //int red = TOTAL_PIXELS - green - yellow;
    
    barcolor(green, yellow, red);
  }
  
  

}


void clearInputString() {
  stringComplete = false; inputString = "";
}


void serialEvent() {
  if (stringComplete) {
    Serial.println("zerando...");
    clearInputString();
  }
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      // add it to the inputString:
      inputString += inChar;

    }
  }
}








void barcolor(int green, int yellow, int red) {
  
  if (green + yellow + red != strip.numPixels()) {
    return colorWipe(strip.Color(255, 255, 255), 0); // Red
  }
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (green > 0) {
      green--;
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    } else if (yellow > 0) {
      yellow--;
      strip.setPixelColor(i, strip.Color(255, 255, 0));
    } else if (red > 0) {
      red--;
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    
  }
  strip.show();
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) { //strip.numPixels()
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

