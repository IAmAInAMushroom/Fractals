/* Helena Yan
   Date: 25.04.2019
   Summary: Explore the Mandelbrot Set
 */
/*
 *  bmpServer.c
 *  1917 serve that 3x3 bmp from lab3 Image activity
 *
 *  Created by Tim Lambert on 02/04/12.
 *  Containing code created by Richard Buckland on 28/01/11.
 *  Copyright 2012 Licensed under Creative Commons SA-BY-NC 3.0.
 *
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
 
#include "mandelbrot.h"
#include "pixelColor.h"
#include "pixelColor.c"
 
/* ===================== *
 * Server defines        *
 * ===================== */
 
#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
 
// after serving this many pages the server will halt
#define NUMBER_OF_PAGES_TO_SERVE 100000000
 
/* ===================== *
 * BMP header defines    *
 * ===================== */
 
#define BYTES_PER_PIXEL 3
#define BITS_PER_PIXEL (BYTES_PER_PIXEL*8)
#define NUMBER_PLANES 1
#define PIX_PER_METRE 2835
#define MAGIC_NUMBER 0x4d42
#define NO_COMPRESSION 0
#define OFFSET 54
#define DIB_HEADER_SIZE 40
#define NUM_COLORS 0
 
// Must be multiples of 4
#define WIDTH 512
#define HEIGHT 512
 
// Some stuff for the actual set
#define DISTANCE_FROM_ORIGIN 2 //per definition of the Mandelbrot set   
#define MAX_STEPS 255 //I think we can change this, but it was the
   //default for the escapeSteps activity
 
//And some stuff for humans
#define TRUE 1
#define FALSE 0
 
typedef unsigned char  bits8;
typedef unsigned short bits16;
typedef unsigned int   bits32;
 
struct pixel {
   bits8 red;
   bits8 green;
   bits8 blue;
};
 
typedef struct pixel pixel;
 
/* ===================== *
 * Mandelbrot defines    *
 * ===================== */
 
struct complex {
   double re; // x coordinate
   double im; // y coordinate
};
 
typedef struct complex complex;
 
typedef struct triordinate {
   //Adding this typedef to make extract easier
   //Stole from parsing activity
   double x;
   double y;
   int z;
   int allOK; //I mean, it's technically still a triordinate since
      //allOK isn't an ordinate???
} triordinate;
 
 
/* ===================== *
 * Function declarations *
 * ===================== */
 
int waitForConnection(int serverSocket);
int makeServerSocket(int portno);
triordinate extractCoordinates(char *request);
void writeHTML(FILE *stream);
void writeBMP(FILE *stream, complex centre, int zoom);
void writeBMPHeader(FILE *file);
void writePixels(FILE *file, complex centre, int zoom);
 
//Testing prototypes
void testAllFunctions (void);
void testExtractCoordinates (void);
void testEscapeSteps (void);
void testTenRaisedTo(void);
void testTwoRaisedTo(void);
 
/* ======================== *
 * Function implementations *
 * ======================== */
 
int main(int argc, char *argv[]) {
   testAllFunctions();

   printf ("************************************\n");
   printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
   printf ("Serving bmps since 2012\n");
 
   int serverSocket = makeServerSocket (DEFAULT_PORT);
   printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
   printf ("************************************\n");
 
   char request[REQUEST_BUFFER_SIZE];
 
   int numberServed = 0;
   while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
 
      printf ("*** So far served %d pages ***\n", numberServed);
 
      int connectionSocket = waitForConnection (serverSocket);
      // wait for a request to be sent from a web browser, open a new
      // connection for this conversation
 
      // read the first line of the request sent by the browser
      int bytesRead;
      bytesRead = read (connectionSocket, request, (sizeof request)-1);
      //ssize_t read(int fildes, void *buf, size_t nbyte);
      //The read() function shall attempt to read nbyte bytes from the file
      //associated with the open file descriptor, fildes, into the buffer
      //pointed to by buf.
      assert (bytesRead >= 0);
      // were we able to read any data from the connection?
 
      // print entire request to the console
      printf (" *** Received http request ***\n \"%s\"\n", request);
   //request for http://localhost:1917/
      //GET / HTTP/1.1
   //request for http://localhost:1917/mandelbrot/2/8/-1.0/0.5/tile.bmp
      //GET /mandelbrot/2/8/-1.0/0.5/tile.bmp HTTP/1.1
      // Investigate using sscanf to extract information from the request!
      printf (" *** Sending http response ***\n");
      FILE *stream = fdopen(connectionSocket, "wb");
 
      /* ===================================================== *
       * TODO: Extract centre and zoom level from the request. *
       * and pass the data to writeBMP(stream, centre, zoom).  *
       * for an image to be generated with those inputs.       *
       * If none given, call writeHTML(stream) to display the  *
       * set using the almondbread viewer instead.             *
       * You should determine this, as well as set values for  *
       * centre & zoom by passing their addresses into the     *
       * extractCoordinates() function, an extracting to those *
       * addresses appropriately                                *
       * ===================================================== */
      
      //Deja vu? Taking some stuff from parsing.
      triordinate data = extractCoordinates(request);
      printf ("%f, %f, %d", data.x, data.y, data.z);
      complex centre;
         centre.re = data.x;
         centre.im = data.y;
      int zoom;
         zoom = data.z;
      
      /* writeBMP(stream, centre, zoom); OR
       * writeHTML(stream);
       */
      if (data.allOK == TRUE) {
         writeBMP(stream, centre, zoom);
         printf ("TRUE\n");
      } else if (data.allOK == FALSE) {
         writeHTML(stream);
         printf ("FALSE\n");
      }
 
      /* ===================================================== *
       * END of TODO                                           *
       * ===================================================== */
 
      //close the connection after sending the page- keep aust beautiful
      fclose(stream);
 
      numberServed++;
   }
 
   //close the server connection after we are done- keep aust beautiful
   printf ("** shutting down the server **\n");
   close (serverSocket);
 
   return EXIT_SUCCESS;
}
 
 
 
triordinate extractCoordinates(char *request) {
   //TODO: Set values for centre and zoom according do the msg request
    
   double myAtoD (char *string);
   long myAtoL (char *string);
 
 
   triordinate functionData;
   int tempInt;
   //To use for stuff without effecting our iterator
  
 
   //Just testing some stuff
   /*int hi = 0;
   printf ("\nRequest is:");
   while (request[hi] != '\0') {
      printf ("%c", request[hi]);
      hi++;
   }
   printf ("\n\n");*/
  
   functionData.allOK = TRUE;
   functionData.x = 0;
   functionData.y = 0;
   functionData.z = 0;
  
   //Get through the humble jumble at the beginning
   int i = 4; //pass GET [space]

   while (request[i] != '2') {
      if (request[i] == ' ') {//If string terminates without
         //fulfilling the triordinate stuff
         functionData.allOK = FALSE;
         //printf ("two");
         return functionData;

      }
      i = i + 1;
   }
   i = i + 2; //Skip past the slash
   tempInt = i;
  
   //Get the zoom (z) value
      //Use temp variable to determine size of zoom to malloc
   int zoomLength = 0;
   while (request[tempInt] != '/') {
      //printf ("%c", request[i]);
      if (request[i] == 't') {//If string terminates without
         //fulfilling the triordinate stuff
         //printf ("zoom");
         functionData.allOK = FALSE;
         return functionData;
      }
      zoomLength++;
      tempInt++;
   }
      //Get the actual value
   char *zoom;
   zoom = malloc((zoomLength+1)*sizeof(char));
   int j = 0;
   while (request[i] != '/') {
      zoom[j] = request[i];
      j++;
      i++;
   }
   zoom[j] = '\0';
   //When you reach the slash, teminate it
   functionData.z = myAtoL(zoom); //Convert our string to long
   i++;
   tempInt = i;
     
   //Get the x-coordinate (x) value
      //Use temp variable to determine size of x-coordinate
   int xCoordinateLength = 0;
   while (request[tempInt] != '/') {
      if (request[i] == 't') {//If string terminates without
         //fulfilling the triordinate stuff
         //printf ("x");
         functionData.allOK = FALSE;
         return functionData;
      }
      xCoordinateLength++;
      tempInt++;
   }
      //Get the actual value. Works if it's not been terminated
   char *xCoordinateValue;
   xCoordinateValue = malloc((xCoordinateLength+1)*sizeof(char));
   int k = 0;
   while (request[i] != '/') {
      xCoordinateValue[k] = request[i];
      k++;
      i++;
   }
   xCoordinateValue[k] = '\0';
   //When you reach the slash, terminate
   functionData.x = myAtoD(xCoordinateValue);
   i++;
   tempInt = i;
  
   //Get the y-coordinate (y) value
      //Use temp variable to determine size of y-coordinate
   int yCoordinateLength = 0;
   while (request[tempInt] != '/') {
      if (request[i] == 't') {//If string terminates without
         //fulfilling the triordinate stuff
         //printf ("y");
         functionData.allOK = FALSE;
         return functionData;
      }
      yCoordinateLength++;
      tempInt++;
   }
      //Get the actual value
   char *yCoordinateValue;
   yCoordinateValue = malloc((yCoordinateLength+1)*sizeof(char));
   int l = 0;
   while (request[i] != '/') {
      yCoordinateValue[l] = request[i];
      l++;
      i++;
   }
   yCoordinateValue[l] = '\0';
   //When you reach the slash, terminate
   functionData.y = myAtoD(yCoordinateValue);
 
   i++;
   
   //Free the zoom, x-coordinate and y-xoordinate
   //Keep aus beautiful
   free(zoom);
   free(xCoordinateValue);
   free(yCoordinateValue);        
  
  
   return functionData;
}
 
   double myAtoD (char *string) {
      //Convert the string to a double, for x and y
      int tenRaisedTo (int iterator);
    
      int digits = strlen(string);
      //Doesn't include null
      double number = 0;
      int n = 0;
      int placeValue = 0;
      int ignoreTheDotn = 0;
      int isNegative = 0; //Set to 1 if negative.
      //If still 0, then coordinate is positive
      int decimalPlaceFromRight = 0;
     
      //Determine if the coordinate has a decimal point
      while (string[n] != '\0') {
         if (string[n] == '.') {
           ignoreTheDotn++;
         }
         n++;
      }
      n = 0;
     
      while (string[n] != '\0') {
         if (string[n] == '-') {
            isNegative = 1;
            ignoreTheDotn++;
            n++; //move past first char if it is minus symbol
         }
         if (string[n] == '.') {
            decimalPlaceFromRight = digits - 1 - n;
            n++; //Pretend dp isn't there for now, and move on
         }
         placeValue = digits - 1 - ignoreTheDotn;
         int currentDigit = string[n] - '0';
         number = number + currentDigit * tenRaisedTo(placeValue);
         n++;
         ignoreTheDotn++;
      }
      number = number / tenRaisedTo(decimalPlaceFromRight);
      if (isNegative == 1) {
         number = number * (-1);
      }
      return number;
   }
 
   long myAtoL (char *string) {
      //Convert the string to a long. for zoom
      int tenRaisedTo (int iterator);
    
      int digits = strlen(string);
      int number = 0;
      int m = 0;
      int placeValue;
     
      while (string[m] != '\0') {
         int currentDigit = string[m] - '0';
         placeValue = digits - 1 - m;
         number = number + currentDigit * tenRaisedTo(placeValue);
         m++;
      }
      return number;
   }
      //Find powers of 10
      int tenRaisedTo (int iterator) {
         //Yeah, so math.h is a thing, but I wasn't sure if we could
         //use it yet. Also I don't know how to so...
         //Exponent of 10 isn't gonna be smaller than 1, because we
         //have to have A value, so iterator has to be >=0
        
         int value = 1;
         //If 10^0, value stays 1
         if (iterator > 0) {
            while (iterator > 0) {
               value = value * 10;
            iterator = iterator - 1;
            }
         }
         return value;
      }
 
void writeHTML(FILE *stream) {
   char * message =
      "HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
      "<script src=\"http://almondbread.cse.unsw.edu.au/tiles.js\"></script>";
   printf("about to send=> %s\n", message);
   fwrite(message, strlen(message), 1, stream);
}

 
/* ======================== *
 * BMP functions            *
 * ======================== */
 
void writeBMP(FILE *stream, complex centre, int zoom) {
   char* message;
 
   // Send the http response header
   message =
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: image/bmp\r\n"
    "\r\n";
 
   printf ("about to send=> %s\n", message);
   fwrite (message, strlen(message), 1, stream);
 
   writeBMPHeader(stream);
   writePixels(stream, centre, zoom);
}
 
// Send BMP pixel array
void writePixels(FILE *stream, complex centre, int zoom) {
   /* ====================================================== *
    * TODO: Print the pixel array for a mandelbrot set using *
    * escapeSteps and stepsToRed, stepsToGreen, stepsToBlue  *
    * ====================================================== */
   //See: chessboard
   double twoRaisedTo (int zoom); 
   //cUz ThErE's No PoWeR fUnCtIoN iNbUiLt In C
   //Distance between two pixels is 2^(-z), where z is the zoom level
 
   //Decision that the coordinate is to the right and up of the centre
   // ___________       If these are the 4 middle pixels of the 512x512
   //|     |/////|      image, then the coordinate of (x,y) will be that
   //|_____|/////|      shaded in square in the top right
   //|     |     |
   //|_____|_____|
   pixel p;

   double distanceBetweenPixels = twoRaisedTo(zoom);  
   double xCentre = centre.re;
   double yCentre = centre.im;
  
   
   //in format (col, row)
   int row = 0; //From bottom left to top right
   while (row < HEIGHT) {
      int col = 0;
      while (col < WIDTH) {
	   // this will write a pixel to the server at (col, row)
       // you will have to colour this pixel based on (col, row)
	   // your code should go here
         double xCoordinate;
         xCoordinate = xCentre + (col - (HEIGHT/2)) * distanceBetweenPixels;
            //Because we want to start on the bottom, which is the negative axis
         double yCoordinate;
         yCoordinate = yCentre + (row - (HEIGHT/2)) * distanceBetweenPixels;
         int numberOfSteps = escapeSteps(xCoordinate, yCoordinate);
         
            //p.red = numberOfSteps;
            //p.green = numberOfSteps;
            //p.blue = numberOfSteps;

            if (numberOfSteps == MAX_STEPS) {
               p.red = 0;
               p.green = 0;
               p.blue = 0;
            } else if (numberOfSteps == 1) {
               p.red = 127;
               p.green = 127;
               p.blue = 127;
            } else {
               p.red = stepsToRed(numberOfSteps);
               p.green = stepsToGreen(numberOfSteps);
               p.blue = stepsToBlue(numberOfSteps);
            }

         fwrite (&p, sizeof p, 1, stream);
         col++;
      }
      row++;
   }
}
   double twoRaisedTo (int zoom) {
      double result = 1;
     
      int z = (-1)*zoom;
      
      if (z > 0) {
         while (z > 0) {
           result = result * 2; 
            z = z - 1;
         }
      } else if (z < 0) {
         z = (-1) * z;
         while (z > 0) {
            result = result * 2;
            z = z - 1;
         }
         result = 1 / result;
      } else {
         result = 1;
      }
     
      return result;
   }
 
// Send BMP header
void writeBMPHeader(FILE *file) {
   assert(sizeof (bits8) == 1);
   assert(sizeof (bits16) == 2);
   assert(sizeof (bits32) == 4);
 
   bits16 magicNumber = MAGIC_NUMBER;
   fwrite (&magicNumber, sizeof magicNumber, 1, file);
 
   bits32 fileSize = OFFSET + (WIDTH * HEIGHT * BYTES_PER_PIXEL);
   fwrite (&fileSize, sizeof fileSize, 1, file);
 
   bits32 reserved = 0;
   fwrite (&reserved, sizeof reserved, 1, file);
 
   bits32 offset = OFFSET;
   fwrite (&offset, sizeof offset, 1, file);
 
   bits32 dibHeaderSize = DIB_HEADER_SIZE;
   fwrite (&dibHeaderSize, sizeof dibHeaderSize, 1, file);
 
   bits32 width = WIDTH;
   fwrite (&width, sizeof width, 1, file);
 
   bits32 height = HEIGHT;
   fwrite (&height, sizeof height, 1, file);
 
   bits16 planes = NUMBER_PLANES;
   fwrite (&planes, sizeof planes, 1, file);
 
   bits16 bitsPerPixel = BITS_PER_PIXEL;
   fwrite (&bitsPerPixel, sizeof bitsPerPixel, 1, file);
 
   bits32 compression = NO_COMPRESSION;
   fwrite (&compression, sizeof compression, 1, file);
 
   bits32 imageSize = (WIDTH * HEIGHT * BYTES_PER_PIXEL);
   fwrite (&imageSize, sizeof imageSize, 1, file);
 
   bits32 hResolution = PIX_PER_METRE;
   fwrite (&hResolution, sizeof hResolution, 1, file);
 
   bits32 vResolution = PIX_PER_METRE;
   fwrite (&vResolution, sizeof vResolution, 1, file);
 
   bits32 numColors = NUM_COLORS;
   fwrite (&numColors, sizeof numColors, 1, file);
 
   bits32 importantColors = NUM_COLORS;
   fwrite (&importantColors, sizeof importantColors, 1, file);
}
 
/* ======================== *
 * Escape steps functions   *
 * ======================== */
//Find the number of iterations/steps a point takes to escape
int escapeSteps (double x, double y) {
   // TODO
   int numberOfSteps = 0;
   double realPart = 0;
      double realTemp = 0;
   double complexPart = 0;
      double complexTemp = 0;
   double d = 0; //d for distance
  
   //Distance formula uses a square root, but idk how to use math.h
      //so I'm leaving everything squared. It doesn't effect the number
      //of steps though
   while (d <= DISTANCE_FROM_ORIGIN*DISTANCE_FROM_ORIGIN) {
      if (numberOfSteps > MAX_STEPS) {
         numberOfSteps = MAX_STEPS; //Screw it, we max it out anyway
         d = DISTANCE_FROM_ORIGIN*DISTANCE_FROM_ORIGIN + 1; //and
            //just pretend we eventually got a result where it
            //escaped, even though we really, really didn't D:
      } else {
         numberOfSteps++;
         /* (a + bi)^2 - (x + yi)
          * = a^2 + 2abi + (bi)^2 - x - yi
          * = [a^2 + b^2(-1) - x] + [2abi - yi]
          */
         realTemp = realPart*realPart - complexPart*complexPart + x;
         complexTemp = 2*realPart*complexPart + y;
         realPart = realTemp;
         complexPart = complexTemp;
         d = realPart*realPart + complexPart*complexPart;
      }
   }
  
   return numberOfSteps;
}
 
/* ======================== *
 * Server functions         *
 * ======================== */
 
// start the server listening on the specified port number
int makeServerSocket (int portNumber) {
 
   // create socket
   int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
   assert (serverSocket >= 0);
   // error opening socket
 
   // bind socket to listening port
   struct sockaddr_in serverAddress;
   memset ((char *) &serverAddress, 0,sizeof (serverAddress));
 
   serverAddress.sin_family      = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY;
   serverAddress.sin_port        = htons (portNumber);
 
   // let the server start immediately after a previous shutdown
   int optionValue = 1;
   setsockopt (
      serverSocket,
      SOL_SOCKET,
      SO_REUSEADDR,
      &optionValue,
      sizeof(int)
   );
 
   int bindSuccess =
      bind (
         serverSocket,
         (struct sockaddr *) &serverAddress,
         sizeof (serverAddress)
      );
 
   assert (bindSuccess >= 0);
   // if this assert fails wait a short while to let the operating
   // system clear the port before trying again
 
   return serverSocket;
}
 
// wait for a browser to request a connection,
// returns the socket on which the conversation will take place
int waitForConnection (int serverSocket) {
   // listen for a connection
   const int serverMaxBacklog = 10;
   listen (serverSocket, serverMaxBacklog);
 
   // accept the connection
   struct sockaddr_in clientAddress;
   socklen_t clientLen = sizeof (clientAddress);
   int connectionSocket =
      accept (
         serverSocket,
         (struct sockaddr *) &clientAddress,
         &clientLen
      );
 
   assert (connectionSocket >= 0);
   // error on accept
 
   return (connectionSocket);
}
 













//Call all written test cases
void testAllFunctions (void) {
   testExtractCoordinates();
   testEscapeSteps();
   testTenRaisedTo();
   testTwoRaisedTo();
}

   void testExtractCoordinates (void) {//tEC
      //Test 1
         char *tEC1 = "http://almondbread.cse.unsw.edu.au/mandelbrot/2/"
                     "99999/3.14/0.141/tile.bmp";
         triordinate test1 = extractCoordinates(tEC1);
         assert (test1.x == 3.14);
         assert (test1.y == 0.141);
         assert (test1.z == 99999);
         assert (test1.allOK == TRUE);
      //Test 2
         char *tEC2 = "GET /mandelbrot/2/8/-1.0/0.5/tile.bmp HTTP/1.1";
         triordinate test2 = extractCoordinates(tEC2);
         assert (test2.x == -1);
         assert (test2.y == 0.5);
         assert (test2.z == 8);
         assert (test2.allOK == TRUE);
      //Test 3
         char *tEC3 = "http://havesumfunnames.hi/2/5/6/7/8";
         triordinate test3 = extractCoordinates(tEC3);
         assert (test3.x == 6);
         assert (test3.y == 7);
         assert (test3.z == 5);
         assert (test3.allOK == TRUE);
      //Test 4
         char *tEC4 = "GET mandelbrot/2/8/-0.14324/02.43";
         triordinate test4 = extractCoordinates(tEC4);
         assert (test4.x == -0.14324);
         assert (test4.y == 2.43);
         assert (test4.z == 8);
         assert (test4.allOK == TRUE);
      //Test 5
         char *tEC5 = "hello";
         triordinate test5 = extractCoordinates(tEC5);
         assert (test5.allOK == FALSE);
      //Test 6
         char *tEC6 = "GET / HTTP/1.1";
         triordinate test6 = extractCoordinates(tEC6);
         assert (test6.allOK == FALSE);
      //Test 7
         char *tEC7 = "GET /mandelbrot/2/tile.bmp HTTP/1.1";
         triordinate test7 = extractCoordinates(tEC7);
         assert (test7.allOK == FALSE);
      //Test 8
         char *tEC8 = "GET /mandelbrot/2/8/tile.bmp HTTP/1.1";
         triordinate test8 = extractCoordinates(tEC8);
         assert (test8.allOK == FALSE);
      //Test 9
         char *tEC9 = "GET /mandelbrot/2/8/-1.0/tile.bmp HTTP/1.1";
         triordinate test9 = extractCoordinates(tEC9);
         assert (test9.allOK == FALSE);
       printf ("extractCoordinates passed!\n");
}
 
void testEscapeSteps (void) {
      //Test 1
         int test1 = escapeSteps(5, 6);
         assert (test1 == 1);
      //Test 2
         int test2 = escapeSteps(-0.14, 0.5);
         assert (test2 == MAX_STEPS);   
      //Test 3
         int test3 = escapeSteps(1, 1);
         assert (test3 == 2);
      printf ("escapeSteps passed!\n");
   }

    void testTenRaisedTo (void) {
   //All valid input are positive integers or 0
      //Test 1
         int ten1 = tenRaisedTo(0);
         assert (ten1 == 1);
      //Test 2
         int ten2 = tenRaisedTo(1);
         assert (ten2 == 10);
      //Test 3
         int ten3 = tenRaisedTo(9);
         assert (ten3 == 1000000000);
      //Test 4
         int ten4 = tenRaisedTo(7);
         assert (ten4 == 10000000);
      printf ("tenRaisedTo passed!\n");
   }

   void testTwoRaisedTo (void) {
   //All valid inputs are positive integers or 0
     //Test 1
         int test1 = twoRaisedTo(0);
         assert (test1 == 1);
      //Test 2
         int test2 = twoRaisedTo(1);
         assert (test2 == 1/2);
      //Test 3
         int test3 = twoRaisedTo(-9);
         assert (test3 == 512);
      //Test 4
         int test4 = twoRaisedTo(16);
         assert (test4 == 1/65536);
      printf ("twoRaisedTo passed!\n");
   }
