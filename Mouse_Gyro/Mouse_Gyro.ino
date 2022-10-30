#define TRUE true
#define FALSE false

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include "HID-Project.h" //by NicoHood V2.8.2 from arduino library manager 

#define DEBUG TRUE //make TRUE to enable serial output and FALSE to disable

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int16_t vx, vy;
double angle_pitch_output, angle_roll_output , prev_angle_roll;
int enable_cursor = 16;
int left_click = 9;
int Volume_CONTROL =10;
int mouseClickFlag =0;
void setup() {
  pinMode(enable_cursor, INPUT_PULLUP);
  pinMode(left_click , INPUT_PULLUP);
  pinMode(Volume_CONTROL , INPUT_PULLUP);
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
  if (digitalRead(enable_cursor) == LOW)
    {    
       mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
       vx = -(gx+350)/300;  
       vy = -(gz-100)/250; 
       if(vy >= -1 && vy <= (1)) { vy = 0; }
       if(vx >= -1 && vx <= (1)) { vx = 0; }
       #if(DEBUG == TRUE)
       Serial.print("  vx : ");
       Serial.print(vx);
       Serial.print("  vy : ");
       Serial.println(vy);
       #endif
       Mouse.move(vy, vx);  
    }
  else
    {
      /*Do nothing */
    }
  if ((digitalRead(left_click) == LOW)) 
    {   
       Mouse.press(MOUSE_LEFT); 
       delayMicroseconds(10); 
    }
  else
    {
        Mouse.release(MOUSE_LEFT);
       delayMicroseconds(10);  
    } 
  if (digitalRead(Volume_CONTROL) == LOW)
    {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      angle_roll_output = atan2(double(ay) , double(az)) *double(180/3.14159);
      if (abs((prev_angle_roll - angle_roll_output)) >= (10))
         {
           if((prev_angle_roll) < (angle_roll_output) )
             {
               Consumer.write(MEDIA_VOLUME_DOWN);
               delayMicroseconds(20);
               Consumer.write(MEDIA_VOLUME_DOWN);  // Volume DOWN
               delayMicroseconds(20);
             }
           else if((prev_angle_roll) > (angle_roll_output))
             {
               Consumer.write(MEDIA_VOLUME_UP);
               delayMicroseconds(20);
               Consumer.write(MEDIA_VOLUME_UP);  // Volume UP
               delayMicroseconds(20);
             }
             else
             {
              /*Do nothing */
             }
            prev_angle_roll = angle_roll_output;
         }
    }
  else
    {
      /*Do nothing */                
    }
    delay(5);
}
