#define FILENAME "sensors.txt"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <CoolSatBaro.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MCP9808.h>
#include <Adafruit_BNO055.h>
#include <uCamII.h>
#include <TinyGPS++.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include "sensor.h"
#include "messages.h" // Defines incoming data header

// define tasks
void TaskBlink( void *pvParameters ); //TODO remove this test task
void TaskAnalogRead( void *pvParameters ); //TODO remove this test task
void TaskSensorRead(void *pvParameters);

// define semaphores
SemaphoreHandle_t xSerialSemaphore;
SemaphoreHandle_t xRadioSemaphore;

// define data log
File allSensors;
//File file_baro; 
//File file_temp;
//File file_light;

/**
 *Global setup should occur here
 */
void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);

  Serial.println("Initializing SD Card");
  if(!SD.begin(46)){
    Serial.println("SD card failed to initialize!");
  }
  Serial.println("SD Initialized");
  allSensors = SD.open("sensors.csv",FILE_WRITE);
 // file_baro = SD.open("baro.csv",FILE_WRITE);
 // file_temp = SD.open("temp.csv",FILE_WRITE);
 // file_light = SD.open("light.csv",FILE_WRITE);

  if (xSerialSemaphore == NULL) {
    xSerialSemaphore = xSemaphoreCreateMutex(); 
    if(xSerialSemaphore){ xSemaphoreGive(xSerialSemaphore);}
  }
 
 if (xRadioSemaphore == NULL) {
    xRadioSemaphore = xSemaphoreCreateMutex(); 
    if(xRadioSemaphore){ xSemaphoreGive(xRadioSemaphore);}
  }
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *) "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

 xTaskCreate(
   TaskAnalogRead
   ,  (const portCHAR *) "AnalogRead"
   ,  128  // Stack size
   ,  NULL
   ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
   ,  NULL );

xTaskCreate(
    TaskSensorRead
    ,  (const portCHAR *) "ReadSensors"
    ,  4048  // Stack size
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

xTaskCreate(
    TaskCamera
    ,  (const portCHAR *) "Take Photos"
    ,  512  // Stack size
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/


void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {

    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      //Serial.println("Analog read Test Task Read");
      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}


void TaskSensorRead(void *pvParameters){
  (void) pvParameters;

  // Task Setup
  Wire.begin(); //Begining everying on our I2C Bus

  // Create sensor instances
  Adafruit_MCP9808 sensor_temp_ex = Adafruit_MCP9808();
  Adafruit_MCP9808 sensor_temp_in = Adafruit_MCP9808();
  Adafruit_BNO055  sensor_gyro = Adafruit_BNO055();
  CoolSatBaro sensor_baro;
  TinyGPSPlus sensor_gps;

  // Initialze sensors
  // These functions should be defined in sensor.h
  initialize_temp_ex(&sensor_temp_ex, Serial);
  initialize_temp_in(&sensor_temp_in, Serial);
  initialize_baro(&sensor_baro, Serial);
  initialize_gyro(&sensor_gyro, Serial);

  int readIntervals[] = {1000,10}; // How often to execute in milliseconds
  unsigned int lastRead[2]; // To store last read time

  /*
    File has to be open when task starts in order to write data to log. We will
    close it for now, and have each sensor open and close it to ensure we don't
    corrupt our filesystem.
   */
  allSensors.close();
 // file_baro.close();
 // file_temp.close();
 // file_light.close();

  for(;;){
    unsigned int now = millis();

    // Runs once a second
    if(now - lastRead[0] > readIntervals[0]){
      lastRead[0] = now;

       if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ){
        // Safe to use serial print here
//        File all_sensors = SD.open(FILENAME, FILE_WRITE);
 //       File file_baro = SD.open("baro.csv", FILE_WRITE);
 //       File file_temp = SD.open("temp.csv", FILE_WRITE);
 //       File file_light = SD.open("light.csv", FILE_WRITE);

        
//        Stream* outputs[] = {&Serial, &all_sensors, (Stream*) NULL};
//        print_sensor(&sensor_temp_ex, read_temp, 'T', outputs);
 //       print_sensor(&sensor_temp_in, read_temp, 't', outputs);
//        print_sensor(&sensor_baro, read_baro, 'B', outputs );  
//        print_sensor((void*) NULL, read_light, 'L', outputs);  
//        print_sensor((void*) NULL, read_uv, 'V', outputs);
//        print_sensor(&sensor_gps, read_gps, 'G',  outputs); 
  
        Stream* out[] = {&Serial, &Serial3};
	int numOut = 2;
	
        sensor_out(&sensor_temp_ex, read_temp,"temp_ex.csv" , out, numOut);
/*	sensor_out(&sensor_temp_in, read_temp,"temp_in.csv" , out, numOut);
	sensor_out(&sensor_baro, read_baro,"baro.csv" , out, numOut);
	sensor_out((void*) NULL, read_light,"light.csv" , out, numOut);
	sensor_out((void*) NULL, read_uv,"uv.csv" , out, numOut);
	sensor_out(&sensor_gps, read_gps,"gsp.csv" , out, numOut);
*/
 //       all_sensors.close();


        xSemaphoreGive( xSerialSemaphore );
      }
      // Runs once every 10 milliseconds
    } else if(now - lastRead[1] > readIntervals[1]) {
      lastRead[1] = now;

      if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ){
        // Safe to use serial print here

        File file = SD.open(FILENAME, FILE_WRITE);
     	Stream* outputs[] = {&file, (Stream*) NULL};       
        print_sensor(&sensor_gyro, read_gyro, 'Y', outputs);

        file.close();
        xSemaphoreGive( xSerialSemaphore );
      }
    }
  }
}

void TaskCamera(void *pvParameters){
  (void) pvParameters;

  Serial1.begin(115200);

  UCAMII camera(Serial1, &Serial);
  short x = 0;
  int bytes;
  bool taken = false;
  for(;;){

    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ){
      // Safe to use serial print here
       if(Serial.peek() == TAKE_PHOTO){
        if (camera.init()) {
          camera.takePicture();
          Serial.print("Image size: ");
          Serial.println(camera.imageSize, DEC);
          Serial.print("number of packages: ");
          Serial.println(camera.numberOfPackages(), DEC);

          while ( bytes = camera.getData() ) {
            for (x = 0; x < bytes; x++) {
              Serial.print("0x");
              Serial.print(camera.imgBuffer[x], HEX);
              Serial.print(" ");
            }
            Serial.println("");
          }
          Serial.println("done downloading");

        }
      }
    xSemaphoreGive( xSerialSemaphore );
   }
  }
}
