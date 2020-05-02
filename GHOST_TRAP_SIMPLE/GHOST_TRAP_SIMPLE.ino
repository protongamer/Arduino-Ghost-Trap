//////////////////////////////////////////////////////////
//Ghost trap with Mp3 module - Protongamer 2020

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Servo.h>

#include "parameters.h"
#include "files_names.h"
#include "pins.h"


#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial mySerial(RX_PIN, TX_PIN); // RX, TX
#endif

Servo motor_1, motor_2;




//Pins
uint8_t pinsOut[] = { PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, PIN_C1, PIN_C2, RED_LED, SMOKE_PIN};
uint8_t pinsIn[] = { /*BUT1,*/ BUT2 };

//Bargraph Pins
uint8_t bargraphPins[] = { PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, }; //pour le bargraph

uint8_t lvl = 0;
uint8_t counterCom = 0;

bool flag = false;
bool trapOpen = false;
bool bargraphCom = 0;

//int moy[100];

uint32_t ledTimer;

//Prototypes

void bargraphLevel(uint8_t l);
//void displayBargraph(void);
void trapStuff(void);
bool mp3IsPlaying(void);




void setup() {

  Serial.begin (9600);
  mySerial.begin (9600);
  /*Serial.println("hi");
    Serial.println(TCCR1A,BIN);
    Serial.println(TCCR1B,BIN);
    Serial.println(TIMSK1,BIN);*/

  //Set servo motor
  motor_1.attach(SERVO_MOTOR_1);
  motor_2.attach(SERVO_MOTOR_2);

  //motor_1.write(MIN_DEGREE_1);
  //motor_2.write(MIN_DEGREE_2);

#ifdef USE_SOFTWARE_SERIAL
  mp3_set_serial (mySerial);  //set Serial for DFPlayer-mini mp3 module
#else
  mp3_set_serial (Serial);  //set softwareSerial for DFPlayer-mini mp3 module
#endif

  for (uint8_t i = 0; i < sizeof(pinsOut); i++) {
    pinMode(pinsOut[i], OUTPUT);
  }

  for (uint8_t i = 0; i < sizeof(pinsIn); i++) {
    pinMode(pinsIn[i], INPUT_PULLUP);
  }

  digitalWrite(SMOKE_PIN, LOW);

  pinMode(MP3_PLAYING_PIN, INPUT);


  mp3_set_volume (VOLUME); //set mp3 volume

  mp3_play(1); //boot sound
  delay(100);




}



//Bargraph display
ISR(TIMER1_OVF_vect) {


  bargraphCom = !bargraphCom;

  digitalWrite(PIN_C1, bargraphCom);
  digitalWrite(PIN_C2, !bargraphCom);



  for (uint8_t i = 0; i < sizeof(bargraphPins); i++) {

    if (lvl == 0) {
      digitalWrite(bargraphPins[i], LOW);
    }

    if (bargraphCom) {

      if (i < lvl) {
        digitalWrite(bargraphPins[i], HIGH);
      }
      if (i >= lvl) {
        digitalWrite(bargraphPins[i], LOW);
      }

    } else {

      if (i + 5 < lvl) {
        digitalWrite(bargraphPins[i], HIGH);
      }

      if (i + 5 >= lvl) {
        digitalWrite(bargraphPins[i], LOW);
      }

    }

  }


  //TCNT1 = TIMER_DISPLAY;

}




void loop() {


  trapStuff();//call function

}



void bargraphLevel(uint8_t l) { //trap level

  lvl = l;

}


void trapStuff(void) {

  //Serial.println(digitalRead(BUT2));

  if (millis() - ledTimer > BLINK_TIME_SLEEP) {
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    ledTimer = millis();
  }


  if (!digitalRead(BUT2)) {
    delay(50); //debounce filter
    while (!digitalRead(BUT2)) {
      #ifdef USE_SOFTWARE_SERIAL
      Serial.println(digitalRead(BUT2));
      #endif
    };//wait user to unpush button
    trapOpen = true;
    digitalWrite(RED_LED, HIGH);
    //play trap open/idle sequence
    //(here)
    if (mp3IsPlaying()) { //no sound played ?
    mp3_play(OPEN); //play it ♪♪♪
    delay(500);
    }
    motor_1.write(MAX_DEGREE_1);
    motor_2.write(MAX_DEGREE_2);

    analogWrite(FLASH1, 50);
    analogWrite(FLASH2, 50);
    #ifdef USE_SOFTWARE_SERIAL
    Serial.println("Flashers");
    #endif
    //open servo motor

    while (trapOpen) { //while trapOpen is true don't leave sequence effect
      #ifdef USE_SOFTWARE_SERIAL
      Serial.println("idle");
      #endif
      if (mp3IsPlaying()) { //if no sound played
        mp3_play(IDLE_SEQ);
        delay(500);
      }
      if (millis() - ledTimer > BLINK_TIME_SLEEP / 2) {
        digitalWrite(RED_LED, !digitalRead(RED_LED));
        ledTimer = millis();
      }

      if (!digitalRead(BUT2)) { //start capture ghost sequence
        while(!mp3IsPlaying()) { //if is playing
          mp3_stop(); //stop it
          delay(100);
        }
        #ifdef USE_SOFTWARE_SERIAL
        Serial.println("capture");
        #endif
        //play capture ghost sound
        while(mp3IsPlaying()){ //not playing ?
        mp3_play(CAPT_SEQ); //play it ( o) (o )
        delay(500);
        }

        for (uint8_t i = 0; i < 255; i += 5) {
          analogWrite(FLASH1, i);
          analogWrite(FLASH2, i);
          delay(5);
        }
        trapOpen = false;
      }

    }

    digitalWrite(RED_LED, HIGH);

    delay(10);

    while (!mp3IsPlaying()) { //while sound playing
      //add unstable flicker effect here(on flashers)
      analogWrite(FLASH1, random(FLICKER_MIN, FLICKER_MAX));
      analogWrite(FLASH2, random(FLICKER_MIN, FLICKER_MAX));
      delay(5);
    }
    //once sound is played
    //play close trap sound (here)
    mp3_play(CLOSE);
    delay(50);
    motor_1.write(MIN_DEGREE_1);
    motor_2.write(MIN_DEGREE_2);
    digitalWrite(RED_LED, LOW);
    for (uint8_t i = 255; i > 0; i -= 5) {
      analogWrite(FLASH1, i);
      analogWrite(FLASH2, i);
      delay(5);
    }
    analogWrite(FLASH1, 0);
    analogWrite(FLASH2, 0);
    #ifdef USE_SOFTWARE_SERIAL
    Serial.println("close");
    #endif
    delay(2000);
    //disable motor for a moment
    //motor_1.detach();
    //motor_2.detach();

    //Prefer use this method(true disable servo motor process)
    TCCR1B |= _BV(CS10);
    TCCR1A = 1;
    TIFR1 &= ~_BV(OCF1A); //detach function forgot this line, why ?
    TIMSK1 &=  ~_BV(OCIE1A) ;
    //add level


    //play beep sound(here)
    mp3_play(BEEP1);
    delay(10);
    TIMSK1 |= _BV(TOIE1); //enable timer interrupt
    #ifdef USE_SOFTWARE_SERIAL
    Serial.println("beep");
    #endif
    for (uint8_t i = 1; i <= sizeof(bargraphPins) * 2; i++) { //fill bargraph
      bargraphLevel(i);
      delay(50);
    }

    //add smoke
    digitalWrite(SMOKE_PIN, HIGH);
    delay(SMOKE_TIME);

    //stop smoke
    digitalWrite(SMOKE_PIN, LOW);
    //start spark sound(here)
    #ifdef USE_SOFTWARE_SERIAL
    Serial.println("spark seq");
    #endif
    int max = 0;
    int min = 32000;
    uint32_t timer;
    timer = millis();

    while (digitalRead(BUT2)) {


      //if sound finished -> play it again
      if (mp3IsPlaying()) {
        mp3_play(SPARK_SEQ);
        delay(500);
      }

      int val = analogRead(SOUND_READ);

      if (millis() - ledTimer > BLINK_TIME_CAPT) {

        digitalWrite(RED_LED, HIGH);
        delay(25);
        digitalWrite(RED_LED, LOW);
        delay(25);

        ledTimer = millis();
      }

      if (millis() - timer < 3000) {
        if (max < 20 * log10(val)) {
          max = 20 * log10(val) + OFFSET;
        }
        if (min > 20 * log10(val)) {
          min = 20 * log10(val) - OFFSET;
        }
      }

      //add spark effect(analogRead ?)
      #ifdef USE_SOFTWARE_SERIAL
      Serial.print(20 * log10(val));
      Serial.print("\t");
      Serial.print(min);
      Serial.print("\t");
      Serial.println(max);
      #endif

      if (20 * log10(val) > max) {
        analogWrite(FLASH3, map(analogRead(SOUND_READ), max, max + MAX_SPARK, 0, 255));
      }
      if (20 * log10(val) < min) {
        analogWrite(FLASH3, map(analogRead(SOUND_READ), min - MAX_SPARK, min, 255, 0));
      }
      if (20 * log10(val) < max && 20 * log10(val) > min) {
        analogWrite(FLASH3, 0);
      }
    }
    TIMSK1 &= ~_BV(TOIE1); //disable timer interrupt
    //play down sound (here)
    mp3_play(STOP);
    delay(10);
    TIMSK1 |= _BV(TOIE1);
    bargraphLevel(0);

    delay(500);
    TIMSK1 &= ~_BV(TOIE1);

    //enable servo motor again

    //motor_1.attach(SERVO_MOTOR_1);
    //motor_2.attach(SERVO_MOTOR_2);

    //reconfigure registers for servo motor
    TCCR1B &= ~_BV(CS10);
    TCCR1A = 0;
    TIFR1 |= _BV(OCF1A); //detach function forgot this line, why ?
    TIMSK1 |=  _BV(OCIE1A) ;
    digitalWrite(RED_LED, LOW);
  }



}






bool mp3IsPlaying(void) {
  return digitalRead(MP3_PLAYING_PIN);
}
