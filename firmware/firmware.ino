/*
 *  Projeto Curricular Articulador
 *       UnigranRio - 2018.2
 *       
 *       
 *  Tornozeleira detectora de obstáculos
 */


#include <Thread.h>   
#include <ThreadController.h>

// Arduino Uno Pinage
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define PIN_MOTOR 5
  #define PIN_TRIG 4
  #define PIN_ECHO 3
  #define PIN_LED 13

// Digispark AtTiny85 Pinage
#else
  #define PIN_MOTOR 0
  #define PIN_TRIG 2
  #define PIN_ECHO 4
  #define PIN_LED 1
  
#endif

#define DIST_FAR 150
#define DIST_WARN 100
#define DIST_ALERT 75
#define DIST_DANGER 25

#define THREAD_SENSOR_DELAY 100

//HCSR04 sensor(PIN_TRIG, PIN_ECHO);

ThreadController controll_task = ThreadController();
Thread thread_sensor = Thread();

int distance = 0;
int blink_level = 0;

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_MOTOR, LOW);

  // Sensor Thread config
  thread_sensor.onRun(read_distance);
  thread_sensor.setInterval(THREAD_SENSOR_DELAY);
  
  // Add thread controll
  controll_task.add(&thread_sensor);
}

void loop() {
  controll_task.run();
  
  if(distance > DIST_FAR){
    analogWrite(PIN_MOTOR, 0);
    blink_level = 16;
  } else
  if(distance > DIST_WARN && distance <= DIST_FAR){
    analogWrite(PIN_MOTOR, 64);
    blink_level = 8;
  } else
  if(distance > DIST_ALERT && distance <= DIST_WARN){
    analogWrite(PIN_MOTOR, 96);
    blink_level = 4;
  } else
  if(distance > DIST_DANGER && distance <= DIST_ALERT){
    analogWrite(PIN_MOTOR, 128);
    blink_level = 2;
  } else
  if(distance <= DIST_DANGER){
    analogWrite(PIN_MOTOR, 255);
    blink_level = 1;
  }

  digitalWrite(PIN_LED, millis()/(64 * blink_level)%2 == 0);
}

// Threads
void read_distance(){
  // Pulse
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Echo
  distance = pulseIn(PIN_ECHO, HIGH)/58.82;
}
