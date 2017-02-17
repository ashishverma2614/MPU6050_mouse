/* 
 * Arduino: 1.6.13 (Linux), Board: "Arduino/Genuino Micro"
 *  Code to control the mouse pointer
 * through the movement of the head
 * Change values at vx and vy (+300 and -100 in my case)
 * using the TEST code to make your project work.
 * 
 * Gabry295
 * keycode 113 press
  keycode 113 release // MUTE for ubuntu 
  keycode 114 press
  keycode 114 release // volume DOwn
  keycode 115 press
  keycode 115 release // Volume up 



 */
 // Keyboard.press(byte)......Keyboard.release(byte)
 //Keyboard.print(string) .......Keyboard.println(string)
 //Keyboard.write(char).....Keyboard.write(char)

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

#include "HID-Project.h"

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int16_t vx, vy, vx_prec , vy_prec;  //, gx, gy, gx_prec , gy_prec ;
double angle_pitch_output, angle_roll_output , prev_angle_roll;
int enable_cursor = 16;
int left_click = 9;
int mouseClickFlag =0;
int counting = 0;
boolean  Increase;
void setup() {
  pinMode(enable_cursor, INPUT_PULLUP);
  pinMode(left_click , INPUT_PULLUP);
  Serial.begin(9600);
  Wire.begin();
  Mouse.begin();
  Consumer.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
    }
    Consumer.write(MEDIA_VOLUME_MUTE);
}

void loop() 
{


  if (digitalRead(enable_cursor) == 0)
  {
     
         if ( (vx_prec-2)<=vx && vx<=vx_prec+2 && (vy_prec-2)<=vy && vy<=vy_prec+2) 
         {
             mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
                vx = (gx+300)/200;
                vy = -(gz-100)/200;
              Mouse.move(vy, vx);
              Serial.println("MOUSE MOVE ");
         }
         
       {  
         
             if ((digitalRead(left_click) == 0) && (!mouseClickFlag)) 
                {
                  mouseClickFlag = 1;
                  Mouse.press(MOUSE_LEFT);  
                }
                
              else if ((digitalRead(left_click))&&(mouseClickFlag))
                {
                  mouseClickFlag = 0;
                  Mouse.release(MOUSE_LEFT);  
                }

       }
          
      vx_prec = vx; 
      vy_prec = vy;


  }

   //   if ((digitalRead(left_click) == 0) && (!mouseClickFlag) ) 
   if ((digitalRead(left_click) == 0)&& (digitalRead(enable_cursor) == 1) ) 
                {
                     //Roll = atan2 (Y, Z) * (180 / PI)
                         angle_roll_output = atan2(double(ay) , double(az)) *double(180/3.14159);
                    // Pitch = atan2 (X, sqrt((Y*Y) + (Z*Z))) * (180 / PI
                    if (abs((prev_angle_roll - angle_roll_output)) >= 10)
                    {
                          if((prev_angle_roll) < (angle_roll_output) )
                              {
                                Consumer.write(MEDIA_VOLUME_DOWN);
                                delayMicroseconds(30);
                                Consumer.write(MEDIA_VOLUME_DOWN);  // Volume Up`````
                                   
                                        
                              }
                          if((prev_angle_roll) > (angle_roll_output))
                           {
                               Consumer.write(MEDIA_VOLUME_UP);
                               delayMicroseconds(30);
                               Consumer.write(MEDIA_VOLUME_UP);  // Volume Up`````
                                         
                           }
                      prev_angle_roll = angle_roll_output;
                    }
                }
                
  
             
    else if (digitalRead(left_click) == 1)
                {
                }
                
  if ((digitalRead(left_click))&&(mouseClickFlag))
              {
                mouseClickFlag = 0;
                Mouse.release(MOUSE_LEFT);  
              }
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      angle_roll_output = atan2(double(ay), double(az)) *double(180/3.14159);
      Serial.println(abs(angle_roll_output ));

}
//
//  if (digitalRead(enable_cursor) == 0)
//  {
//     
//         if ( (vx_prec-2)<=vx && vx<=vx_prec+2 && (vy_prec-2)<=vy && vy<=vy_prec+2) 
//         {
//             mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//      
//               vx = (gx+300)/200;  // "+300" because the x axis of gyroscope give values about -350 while it's not moving. Change this value if you get something different using the TEST code, chacking if there are values far from zero.
//               vy = -(gz-100)/200; // same here about "-100"
//      
//              Mouse.move(vy, vx);
//         }
//         
//       {  
//         
//             if ((digitalRead(left_click) == 0) && (!mouseClickFlag)) 
//                {
//                  mouseClickFlag = 1;
//                  Mouse.press(MOUSE_LEFT);  
//                }
//                
//              else if ((digitalRead(left_click))&&(mouseClickFlag))
//                {
//                  mouseClickFlag = 0;
//                  Mouse.release(MOUSE_LEFT);  
//                }
//
//       }
//          
//      vx_prec = vx; 
//      vy_prec = vy;
//
//
//  }
//   
//   if ((digitalRead(left_click) == 0) && (!mouseClickFlag) ) 
//                {
//
//                        mouseClickFlag = 1;
//                       // Mouse.press(MOUSE_LEFT);
//                        Consumer.write(MEDIA_VOLUME_UP);
//                        delay(10);
//                        Consumer.write(MEDIA_VOLUME_UP);// Volume Up``````
//                       //Keyboard.releaseAll();
//                       
//
//                }
//                
//  
//                
//    else if ((digitalRead(left_click))&&(mouseClickFlag))
//                {
//                  mouseClickFlag = 0;
//                 //Keyboard.releaseAll();
//                  //Mouse.release(MOUSE_LEFT);  
//                }

