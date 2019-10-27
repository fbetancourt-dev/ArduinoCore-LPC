#include "Arduino.h"
//#include "ArduinoLog.h"
//#include "EONLog.h"
#include "HardwareSerial.h"

#include "globals.h"

static CTL_TASK_t mainTask, task1Task, task2Task;
static unsigned task1Stack[64];
static unsigned task2Stack[128];

//String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

void task1(void *p) {

  Serial3.begin(115200);

  pinMode(P1_22, INPUT_PULLDOWN);
  pinMode(P1_25, INPUT_PULLDOWN);
  while (1) {
    delay(1000);
    
     Serial3.printf("HWID0:%s",digitalRead(P1_22)==HIGH?"HIGH, ":"LOW, ");
     Serial3.printf("HWID1:%s",digitalRead(P1_25)==HIGH?"HIGH\r\n":"LOW\r\n");
  }
}

/*
void delayMicro(unsigned int a_microseconds)
{

loop:
         MOVS   R1, R0
         SUB    R0, R0, #1
         BNE    loop
                 BX     LR
                 
}
*/

// PIN Number
// #define PINNUMBER ""

// APN data
// #define GPRS_APN "GPRS_APN"      // replace your GPRS APN
// #define GPRS_LOGIN "login"       // replace with your GPRS login
// #define GPRS_PASSWORD "password" // replace with your GPRS password

// initialize the library instance
//GSMClient client;
// GPRS gprs;
// GSM gsmAccess;

// URL, path & port (for example: arduino.cc)
// char server[] = "arduino.cc";
// char path[] = "/asciilogo.txt";
// int port = 80; // port 80 is the default for HTTP

void task2(void *p) {
  // task code; on return, the task will be terminated.
  Serial.begin(115200, SERIAL_8N1, GPIO_gsmRTS, GPIO_gsmCTS);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);

  // Initialize with log level and log output.
  // Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  //pinMode(GPIO_UL_GL, INPUT);
  //pinMode(GPIO_HL, INPUT);

  //pinMode(GPIO_VmDoorOpen, OUTPUT);
  pinMode(GPIO_busy, OUTPUT);
  
  int reset = P0_31;
  int pwr_3_7 = P0_13;
  int pwr_1_8 = P0_19;
  int pwr_mon = P0_18;

  pinMode(reset, OUTPUT);
  pinMode(pwr_1_8, OUTPUT);
  pinMode(pwr_3_7, OUTPUT);
  pinMode(pwr_mon, INPUT);

  // Turn on the HL
  digitalWrite(reset, LOW);
  digitalWrite(pwr_1_8, LOW);
  digitalWrite(pwr_3_7, LOW);
  delay(1000);

  digitalWrite(pwr_3_7, HIGH);
  delay(2000);
  digitalWrite(pwr_1_8, HIGH);
  delay(100);
  digitalWrite(reset, HIGH);
  delay(100);
  digitalWrite(reset, LOW);
  delay(7000);

  while (1) {
    if(digitalRead(pwr_mon)==HIGH) {
      debug_printf("ON");
    }
    else
    {
      debug_printf("OFF");
    }
    Serial.println("UART0");
    debug_printf("Sending AT\n");
    Serial.println("AT");
    delay(500);
    while(Serial.available()!=0)
      debug_printf("%c",Serial.read());

    delay(500);
    digitalWrite(GPIO_busy, HIGH);
    delay(500);
    digitalWrite(GPIO_busy, LOW);
  }

  /*digitalWrite(GPIO_UL_GL, HIGH);
    digitalWrite(GPIO_HL, HIGH);
    delay(100);
    digitalWrite(GPIO_UL_GL, LOW);
    digitalWrite(GPIO_HL, LOW);
    delay(100);*/
  /*
    digitalWrite(GPIO_HL, LOW);
    digitalWrite(GPIO_UL_GL, LOW);
    delay(100);
    digitalWrite(GPIO_HL, HIGH);
    digitalWrite(GPIO_UL_GL, HIGH);
    delay(100);
    */
  /*
    delay(100);
    digitalWrite(GPIO_VmDoorOpen, HIGH);
    delay(100);
    digitalWrite(GPIO_VmDoorOpen, LOW);
    */
  /*
    if(digitalRead(GPIO_VmDoorOpen) == LOW) {
      Serial.println("Door Open");
    }
    else
    {
      Serial.println("Door Closed");
    }
    delay(500);
  */
  /*
    if (digitalRead(GPIO_HL) == HIGH) {
      Serial.print("HL");
    } else {
      Serial.print("None");
    }

    if (digitalRead(GPIO_UL_GL) == HIGH) {
      Serial.print("UL");
    } else {
      Serial.print("GL");
    }
    Serial.println();
  */
  //}

  /*
    Serial.println("Hello 0");
    Serial1.println("Hello 1");
    Serial2.println("Hello 2");
    Serial3.println("Hello 3");
    */

  /*
  if (Serial1.available())
      Serial1.write(Serial1.read());
    */

  /*
  if (serialEventRun)
      serialEventRun();

    // print the string when a newline arrives:
    if (stringComplete) {
      Serial.print(inputString);
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
    */

  // Start logging text and formatted values
  //Log.error("Log as Error   with binary values             : %b, %B" CR, 23, 345808);
  //Log.warning(F("Log as Warning with integer values from Flash : %d, %d" CR), 34, 799870);
  //Serial.raw_puts("Hello World 0\x0d\x0a");
  //Serial.print("Hello World!");
  //Serial.__putchar("Hello");
  //Serial.printf("Hello");
  //Serial2.raw_puts("Hello World 2\x0d\x0a");
  //Serial3.raw_puts("Hello World 3\x0d\x0a");

  //Debug.printfX(debug_prog_flow,"Data");
  //Serial.__putchar('A');
  /*
    digitalWrite(P0_7, HIGH);
    delay(100);
    digitalWrite(P0_7, LOW);
    delay(100);
    */
  //Serial.begin(9600);
}

int main(void) {

  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);

  //pinMode(P0_7, OUTPUT);

  pinMode(P0_27, OUTPUT);
  digitalWrite(P0_27, LOW);

  ctl_events_init(&event_set_isr, 0); // use these events to trigger tasks from ISRs

  // Turn myself into a task running at the highest priority.
  ctl_task_init(&mainTask, 255, "main");
  ctl_start_timer(ctl_increment_tick_from_isr); // start the timer

  // Initialize the stack of task1.
  memset(task1Stack, 0xba, sizeof(task1Stack));

  // Prepare another task to run.
  ctl_task_run(&task1Task, 1, task1, 0, "task1", sizeof(task1Stack) / sizeof(unsigned), task1Stack, 0);
  //ctl_task_run(&task2Task, 2, task2, 0, "task2", sizeof(task2Stack) / sizeof(unsigned), task2Stack, 0);

  // Now that all the tasks have been created, go to the lowest priority task.
  ctl_task_set_priority(&mainTask, 0);

  // Main task, if activated because task1 is suspended, just
  // enters low-power mode and waits for task1 to run again
  // (for example, because an interrupt wakes it).
  for (;;) {
  }
  return 0;
}

void serialEvent() {
/*
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  */
}

void ctl_handle_error(CTL_ERROR_CODE_t e) {
  static int iNoTaskToRunErrs = 0;
  static int iMutexUnlockErrs = 0;
  // If the tasking stuff can't find a task to run we don't want to lock up...
  if (e == CTL_ERROR_NO_TASKS_TO_RUN) {
    ++iNoTaskToRunErrs;
    return;
  } else if (e == CTL_MUTEX_UNLOCK_CALL_ERROR) {
    ++iMutexUnlockErrs;
    return;
  }
  while (1)
    ;
}

// The __cxa_pure_virtual function is an error handler that is invoked
// when a pure virtual function is called.
// If you are writing a C++ application that has pure virtual functions
// you must supply your own __cxa_pure_virtual error handler function
// If anywhere in the runtime of your program an object is created
// with a virtual function pointer not filled in, and when the
// corresponding function is called, you will be calling a
// 'pure virtual function'

extern "C" void __cxa_pure_virtual() {
  while (1)
    ;
}