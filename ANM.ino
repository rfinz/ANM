/****************************Library Dependencies****************/
#include <SD.h>

#define SWITCH 0
#define DELIM 151
#define FREQ 500

/*****************************SD Variables***********************/
const int CHIP_SELECT = 10;              //IO pin for SD card
const int RED = 3, GREEN = 4;            //status LEDs
File file;      //file objects for input and output                                       

/*************************Serial Variables***********************/
const int COM_BAUD = 1200;    //edit me for connectivity with other platforms
const int DEV_BAUD = 9600;    //edit me for connectivity with other platforms

/************************Arduino Main****************************/
void setup(){
  //Initialize SD card
  pinMode(CHIP_SELECT,OUTPUT);
  SD_safe_begin(CHIP_SELECT);
  
  /****Mode Switch****/
  switch(SWITCH){
    case 0: //write to SD
      file = SD.open("RAW.dat", O_WRITE|O_CREAT);
      Serial.begin(DEV_BAUD);
      break;
      
    case 1: //write to COM port
      file = SD.open("RAW.dat", O_READ);
      Serial.begin(COM_BAUD);
      break;
      
    default:
      break;
  }
  /****END Mode Switch****/
  
}


void loop(){
  switch(SWITCH){
    case 0:
      delay(FREQ);
      sendSingle(byte(DELIM)); // may need more logic depending on
                               // depending on the recorded signal
      break;
      
    case 1:
      break;
      
    default:
      break;
  }
}

/************************SD and Serial Functions***************************/

void SD_safe_begin(int pin){
  if(!SD.begin(pin)){
    digitalWrite(RED,HIGH);    //turn on red LED for error
    while(1);
  }
}

void serialEvent(){
  switch(SWITCH){ //if in playback mode
    case 0:
      break;
      
    case 1:
      byte in = '\0';      //null byte
      byte buffer[128];    //space for bytes from serial buffer
      int index = 0;       //next available index in the buffer
      while(Serial.available()){
        in = (byte)Serial.read();
        buffer[index++] = in;
      }                    //while data available, caste it to bytes
                           //and store in buffer  
      for(int i = 0; i<index; i++){
        digitalWrite(GREEN, HIGH);
        file.write(buffer[i]); //write is used instead of print
                                  //for the sake of efficiency.
        digitalWrite(GREEN, LOW);
      }
      digitalWrite(RED, HIGH);
      file.flush();            //write to the SD
      digitalWrite(RED, LOW);
      break;
      
    default:
      break;
  }//endswitch
}

void sendSingle(byte delim){
	byte in = 0;
        byte toSend[128];
	int index = 0;
	
	//toSend.reserve(128);
	while((in = playback.read()) != delim ){
		toSend[index++] = in;
		if(!playback.available()) playback.seek(0);
	}
	toSend[index] = in;
        Serial.print("Incoming!");
	for(int i = 1; i<index; i++){
		Serial.write(toSend[i]);
        }
}



