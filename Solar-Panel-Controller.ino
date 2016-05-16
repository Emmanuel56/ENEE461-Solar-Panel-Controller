// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
// Motor with 200 steps per rev (1.8 degree)
// to motor port #1 (M1 and M2)
AF_Stepper motorTop(200, 1);   //top motor
// to motor port #2 (M3 and M4)
AF_Stepper motorBottom(48, 2);   //bottom motor

int lightPinLeft = 0;  //define a pin for Photo resistor
int lightPinRight = 1; //Analog 1
int lightPinUp = 2; //Analog 1
int lightPinDown = 3; //Analog 1
int pos= 0;
int posTop = 0;
int bestMax = 0;



void bottomF()  { motorBottom.onestep(FORWARD, INTERLEAVE); }
void bottomB() { motorBottom.onestep(BACKWARD, INTERLEAVE);}
// wrappers for the second motor!
void topF()  { motorTop.onestep(FORWARD, INTERLEAVE); }
void topB() { motorTop.onestep(BACKWARD, INTERLEAVE);}


void bottomF10(){
  int i = 0;
  while (i<10){
    bottomF();
    delay(50);
    i++;
  }
}
void bottomB10(){
  int i = 0;
  while (i<10){
    bottomB();
    delay(50);
    i++;
  }
 }
void topF10(){
    int i = 0;
    while (i<10){
      topF();
      delay(20);
      i++;
    }
 }  
void topB10(){
    int i = 0;
    while (i<10){
      topB();
      delay(20);
      i++;
    }
 }   

 int sweepTop(int prevMax){
      int pinLeft = analogRead(lightPinLeft); //read all values
       int pinRight = analogRead(lightPinRight);
       int pinUp= analogRead(lightPinUp);
       int pinDown = analogRead(lightPinDown);
       int i=0;
       int side=0; 
       int bestTotal=0;
       int totalAll;
       int bestPos=0;
       while (i<100){
        i=i+10;
        posTop = posTop+10;
        topF10();
       }
       i = 0;
       while(i<200){
      
        pinLeft = analogRead(lightPinLeft); //read all values
        pinRight = analogRead(lightPinRight);
        pinUp= analogRead(lightPinUp);
        pinDown = analogRead(lightPinDown);

        totalAll = pinLeft+pinRight+pinUp+pinDown;
        if(bestTotal<totalAll){
          bestTotal= totalAll;
          bestPos= posTop;
          Serial.print("Best total  : ");
          Serial.print(bestTotal); //Write the value of the photoresistor to the serial monitor.
          Serial.print(" bestPos Top ");
          Serial.println(bestPos); //Write the value of the photoresistor to the serial monitor.
        }
        posTop = posTop-10;
        i= i+10;
        topB10();
        delay(20);
      }
      bestMax = bestTotal;
      while(bestPos!= posTop && posTop < 100){
       Serial.print(" bestPos ");
       Serial.println(bestPos); //Write the value of the photoresistor to the serial monitor.
        posTop = posTop+10;
        Serial.print(" top Pos  is");
        Serial.println(posTop); //Write the value of the photoresistor to the serial monitor.
        topF10();
      }
}

void setup() {
    Serial.begin(9600);  //Begin serial communcation
}

void loop() {
  if(Serial.available()>0){
    int i,j, bestPos, lowestH, lowestV, solarH, solarV;;
    int pinLeft, pinRight, pinUp, pinDown, pinDiffH, pinDiffV, totalH;
    int prevPinDiffH, prevPinDiffV, prevTotalH;
    String in;
    in = Serial.readString();
    if(in.equals("reset")){
      Serial.println("It is resetting");

      i=0;
      j=0;
      if(pos<0){
        while (pos<0){
          bottomF();
          pos++;
          Serial.print(" Pos  is");
          Serial.println(pos); //Write the value of the photoresistor to the serial monitor.
          delay(50);
        }
      }else if(pos>0){
        while(pos>0){
          bottomB();
          pos--; 
          Serial.print(" Pos  is");
          Serial.println(pos); //Write the value of the photoresistor to the serial monitor.
          delay(50);
        }
      }
      i=0;
      j=0;
      if(posTop<0){
        while (posTop<0){
          topF();
          posTop++;
          Serial.print(" Pos  is");
          Serial.println(posTop); //Write the value of the photoresistor to the serial monitor.
          delay(50);
        }
      }else if(posTop>0){
        while(posTop>0){
          topB();
          posTop--; 
          Serial.print(" Top Pos  is");
          Serial.println(posTop); //Write the value of the photoresistor to the serial monitor.
          delay(50);
        }
      }  
    }else if(in.equals("start")){
      Serial.println("It is starting");
      i= 200;
      bestPos=0;   //position with most sunlight
      lowestH =999;  //lowest difference between Horizontal sensors
      lowestV=999 ;  //lowest difference between Vertical sensors
      
      /*while (i>0){  // re-locate 180 degrees Back
        i= i-10;
        pos= pos-10;
        bottomB10();
      }*/
      i=0;
      pinLeft = analogRead(lightPinLeft); //read all values
      pinRight = analogRead(lightPinRight);
      pinUp= analogRead(lightPinUp);
      pinDown = analogRead(lightPinDown);
      prevPinDiffH = abs(pinLeft - pinRight);
      prevPinDiffV = abs(pinUp-pinDown);
      prevTotalH = pinLeft+pinRight; 
      Serial.print(" prev diff   Horizontal: ");
      Serial.print(prevPinDiffH); //Write the value of the photoresistor to the serial monitor.
      Serial.print("      prev total horizontal: ");
      Serial.println(prevTotalH); //Write the value of the photoresistor to the serial monitor.
      delay(20);
      pos = pos+10;
      bottomF10();
      pinLeft = analogRead(lightPinLeft); //read all values
      pinRight = analogRead(lightPinRight);
      pinUp= analogRead(lightPinUp);
      pinDown = analogRead(lightPinDown);
      pinDiffH = abs(pinLeft - pinRight);
      pinDiffV = abs(pinUp-pinDown);
      totalH = pinLeft+pinRight; 
      Serial.print("diff   Horizontal: ");
      Serial.print(pinDiffH); //Write the value of the photoresistor to the serial monitor.
      Serial.print("   total horizontal: ");
      Serial.println(totalH); //Write the value of the photoresistor to the serial monitor.
      while(totalH >= prevTotalH && pinDiffH <= prevPinDiffH){
        prevPinDiffH = pinDiffH;
        prevTotalH = totalH;
        pos= pos+10; 
        bottomF10();
        pinLeft = analogRead(lightPinLeft); //read all values
        pinRight = analogRead(lightPinRight);
        pinUp= analogRead(lightPinUp);
        pinDown = analogRead(lightPinDown);
        pinDiffH = abs(pinLeft - pinRight);
        pinDiffV = abs(pinUp-pinDown);
        totalH = pinLeft+pinRight; 
        if(pos>200){
          break;
        }
        Serial.print("diff   Horizontal: ");
        Serial.print(pinDiffH); //Write the value of the photoresistor to the serial monitor.
        Serial.print("    total horizontal: ");
        Serial.println(totalH); //Write the value of the photoresistor to the serial monitor.
     }
        Serial.println("Forward is not the correct way either, going backwardnow");
        Serial.print(" prev diff   Horizontal: ");
        Serial.print(prevPinDiffH); //Write the value of the photoresistor to the serial monitor.
        Serial.print("      prev total horizontal: ");
        Serial.println(prevTotalH); //Write the value of the photoresistor to the serial monitor.
        pos = pos-10;
        bottomB10();
        pinLeft = analogRead(lightPinLeft); //read all values
        pinRight = analogRead(lightPinRight);
        pinUp= analogRead(lightPinUp);
        pinDown = analogRead(lightPinDown);
        pinDiffH = abs(pinLeft - pinRight);
        pinDiffV = abs(pinUp-pinDown);
        totalH = pinLeft+pinRight; 
        Serial.print("diff   Horizontal: ");
        Serial.print(pinDiffH); //Write the value of the photoresistor to the serial monitor.
        Serial.print("     total horizontal: ");
        Serial.println(totalH); //Write the value of the photoresistor to the serial monitor.
     while(totalH >= prevTotalH && pinDiffH <= prevPinDiffH){
        prevPinDiffH = pinDiffH;
        prevTotalH = totalH;
        pos= pos-10; 
        bottomB10();
        pinLeft = analogRead(lightPinLeft); //read all values
        pinRight = analogRead(lightPinRight);
        pinUp= analogRead(lightPinUp);
        pinDown = analogRead(lightPinDown);
        pinDiffH = abs(pinLeft - pinRight);
        pinDiffV = abs(pinUp-pinDown);
        totalH = pinLeft+pinRight; 
        if(pos<200){
          break;
        }
        Serial.print("diff   Horizontal: ");
        Serial.print(pinDiffH); //Write the value of the photoresistor to the serial monitor.
        Serial.print("total horizontal: ");
        Serial.println(totalH); //Write the value of the photoresistor to the serial monitor.
     }
     Serial.println("Backward is not the correct way either, giving up");
    }else if(in.equals("s")){
      i = 0;
      while (i<200){
        bottomF();
        i++;
        pos++;
        delay(20);
      }
      int bestTotal=0;
      int totalAll;
      bestPos=0;
      i=0;
      while(i<400){
        pinLeft = analogRead(lightPinLeft); //read all values
        pinRight = analogRead(lightPinRight);
        pinUp= analogRead(lightPinUp);
        pinDown = analogRead(lightPinDown);
        pinDiffH = abs(pinLeft - pinRight);
        pinDiffV = abs(pinUp-pinDown);
        totalAll = pinLeft+pinRight+pinUp+pinDown;
        if(bestTotal<totalAll){
          bestTotal= totalAll;
          bestPos= pos;
          Serial.print("Best total  : ");
          Serial.print(bestTotal); //Write the value of the photoresistor to the serial monitor.
          Serial.print(" bestPos ");
          Serial.println(bestPos); //Write the value of the photoresistor to the serial monitor.
        }
        pos = pos-10;
        i= i+10;
        bottomB10();
        delay(20);
      }
      bestMax = bestTotal;
      while(pos!= bestPos && pos < 200){
       Serial.print(" bestPos ");
       Serial.println(bestPos); //Write the value of the photoresistor to the serial monitor.
        pos = pos+10;
        Serial.print(" Pos  is");
        Serial.println(pos); //Write the value of the photoresistor to the serial monitor.
        bottomF10();
      }
      
    
    
    
    }else if(in.equals("test")){
      while (!(in.equals("stop"))){
        int pinLeft = analogRead(lightPinLeft); //read all values
        int pinRight = analogRead(lightPinRight);
        int pinUp= analogRead(lightPinUp);
        int pinDown = analogRead(lightPinDown);
        int pinDiffH = abs(pinLeft - pinRight);
        int pinDiffV = abs(pinUp-pinDown);
        int total = pinLeft+pinRight;
        int totalV = pinUp+pinDown;

         Serial.print("right: ");
         Serial.println( pinRight); //Write the value of the photoresistor to the serial monitor.
    
         Serial.print("left: ");
         Serial.println(pinLeft); //Write the value of the photoresistor to the serial monitor.
   
         Serial.print("up: ");
         Serial.println(pinUp); //Write the value of the photoresistor to the serial monitor.
    
         Serial.print("down: ");
         Serial.println(pinDown); //Write the value of the photoresistor to the serial monitor.

    
         Serial.print("diff   Horizontal: ");
         Serial.print(pinDiffH); //Write the value of the photoresistor to the serial monitor.

         Serial.print("   vs   diff Verticaltal: ");
         Serial.println(pinDiffV); //Write the value of the photoresistor to the serial monitor.
    
         Serial.print("total horizontal: ");
         Serial.print(total); //Write the value of the photoresistor to the serial monitor.
         Serial.print("      vs   total vertical: ");
         Serial.println(totalV); //Write the value of the photoresistor to the serial monitor.
         Serial.print("      vs   total all: ");
         Serial.println(totalV+total); //Write the value of the photoresistor to the serial monitor.
    
         Serial.println("    ");
         Serial.println("    ");
                                                //with a 10k resistor divide the value by 2, for 100k resistor divide by 4.
          in = Serial.readString();

         delay(2000); //short delay for faster response to light.
      }
      
    }else if(in.equals("s2")){   //foward is left 
      delay(50);
       int pinLeft = analogRead(lightPinLeft); //read all values
       int pinRight = analogRead(lightPinRight);
       int pinUp= analogRead(lightPinUp);
       int pinDown = analogRead(lightPinDown);
       int prevPos = pos;
       int prevMax = bestMax;
       int i=0;
       int side=0; 
       if(pinUp>=pinDown){
        while(i<100){
            //pinUp= analogRead(lightPinUp);
            //pinDown = analogRead(lightPinDown);
            bottomF10();
            pos= pos+10;
            i= i+10;
            delay(20);
            Serial.print("pos is: ");
            Serial.print(pos); //Write the value of the photoresistor to the serial monitor.
        }
        side = 0;
       }else if(pinUp<pinDown){
        i=0;
        while(i<100){
            //pinUp= analogRead(lightPinUp);
            //pinDown = analogRead(lightPinDown);
            bottomB10();
            pos = pos-10;
            i= i+10;
            delay(20);
            Serial.print("pos is: ");
            Serial.print(pos); //Write the value of the photoresistor to the serial monitor.
        }
        side = 1;
       }

       int ret = sweepTop(bestMax);
      
    }else if(in.equals("side")){   //foward is left
      int i = 0;

      while(i<100){
        topF10(); //foward is left
        i= i+10;
        
      }
      i=0;
      while(i<100){
        topB10();
        i = i+10;
      }
       
    }


  }
          

}
          /*Serial.print("pin diffH is ");
          Serial.println(pinDiffH);
          Serial.print("pinDiffV is ");
          Serial.println(pinDiffV);
          Serial.print("lowest H is ");
          Serial.println(lowestH);
          Serial.print("best pos is ");
          Serial.println(bestPos);
          Serial.println("        ");
      
        }
        pos++;  
        i++;
        bottomF();
        delay(20); 
      }
      j= 200-pos;
      Serial.print("best pos is ");
      Serial.println(bestPos);
      Serial.print("pos is ");
      Serial.println(pos);
      Serial.print("j is ");
      Serial.println(j);
      Serial.println("    ");
      */
      /*while (j>0){
        bottomB();
        j--;
        pos--;
      }
      Serial.print("pos is ");
      Serial.println(pos);
      Serial.println("    ");
      
*/
