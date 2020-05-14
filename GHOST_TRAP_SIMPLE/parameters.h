//Parameters


#define USE_SOFTWARE_SERIAL


#define VOLUME            30



//PWM FLICKER EFFECT
#define FLICKER_MAX       255
#define FLICKER_MIN       100

//Time in ms
#define SMOKE_TIME             2000
#define BLINK_TIME_CAPT        250
#define BLINK_TIME_SLEEP       1000

//Sound offset threshold for spark
#define OFFSET                  2
//spark offset compare value for bright(lower value -> more shiny    /!\  Overflow value -> back to 0 bright -> dark )
#define MAX_SPARK               20



//For servo motor

#define MIN_DEGREE_1          0
#define MAX_DEGREE_1          120
#define MIN_DEGREE_2          120
#define MAX_DEGREE_2          0

//If spirit halloween, --> no servo motor
//#define SPIRIT_HALLOWEEN_MOD

//if ghost trap with removable cartridge
#define REMOVABLE_CARTRIDGE

//for ISR
//#define TIMER_DISPLAY     0
