// Helena Yan
// Date: 28.04.2019
// Summary: Color my Mandelbrot Set


#include "pixelColor.h"



unsigned char stepsToRed (int steps) {
   if ( (steps>=1) && (steps<=4) ) { //escapes in 1 to 4
      steps = 255;
   }
   else if ( (steps>=5) && (steps<=8) ) { //escapes in 5 to 8
      steps = 255;
   }
   else if ( (steps>=9) && (steps<=12) ) { //escapes in 9 to 12
      steps = 255;
   }
   else if ( (steps>=13) && (steps<=16) ) { //escapes in 13 to 16
      steps = 0;
   }
   else {
      steps = 177;
   }

   return steps;
}

unsigned char stepsToGreen (int steps) {
   if ( (steps>=1) && (steps<=4) ) { //escapes in 1 to 4
      steps = 255;
   }
   else if ( (steps>=5) && (steps<=8) ) { //escapes in 5 to 8
      steps = 127;
   }
   else if ( (steps>=9) && (steps<=12) ) { //escapes in 9 to 12
      steps = 0;
   }
   else if ( (steps>=13) && (steps<=16) ) { //escapes in 13 to 16
      steps = 255;
   }
   else {
      steps = 0;
   }

   return steps;
}


unsigned char stepsToBlue (int steps) {
   if ( (steps>=1) && (steps<=4) ) { //escapes in 1 to 4
      steps = 0;
   }
   else if ( (steps>=5) && (steps<=8) ) { //escapes in 5 to 8
      steps = 0;
   }
   else if ( (steps>=9) && (steps<=12) ) { //escapes in 9 to 12
      steps = 0;
   }
   else if ( (steps>=13) && (steps<=16) ) { //escapes in 13 to 16
      steps = 0;
   }
   else {
      steps = 255;
   }

   return steps;
}
