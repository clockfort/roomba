/**
 *                                                 ROOMBA (MiniDIN-8) Connector      
 * INTERFACING HARDWARE INFORMATION                           _     _
 * Arduino/Roomba pinout:                                    / |___| \
 * Pin 0 ---> Roomba Tx (Roomba Pin #4)                     / 7  6  5 \
 * Pin 1 ---> Roomba Rx (Roomba Pin #3)                    |    ___    |
 * Pin 2 ---> Roomba Device Detect (Roomba Pin #5)         | 4 |___| 3 |
 *                                                         |           |
 * WARNING: Because of the use of the Arduino's natural     \__2___1__/
 * serial rx/tx pins (pins 0 & 1) I _strongly_ encourage you
 * to unplug the Roomba from the Arduino while reprogramming 
 * the Arduino.
 *
 *
 * 
 *
 * @author Chris Lockfort
 **/
#include "WProgram.h"
#include "Roomba.h"


/**
 * Default (and only!) constructor for a new Roomba object.
 **/
Roomba::Roomba(){
  Serial.begin(57600);//due to high baud rate, SoftwareSerial can't be used :-(
  
  initialize();

  _wakePin=2;// You can change this pin if you'd like
  pinMode(_wakePin, OUTPUT);
   . 
  _defaultDuration=64;//whole note = X/64ths of a second
}

/**
 * Wake up the roomba and put it under control of the serial interface.
 **/
void Roomba::initialize(){
  /*digitalWrite(_wakePin, HIGH);
  delay(100);
  digitalWrite(_wakePin, LOW);
  delay(500);*/
  wake();
  digitalWrite(_wakePin, HIGH);
  delay(2000);

  Serial.print(128, BYTE); //START (Mode--> Passive)
  delay(50);
  Serial.print(130, BYTE); //CONTROL (Mode--> Safe)
  delay(50);
}

void Roomba::goForward(int velocity=200) {
  drive(velocity, 32768);
}

void Roomba::goBackward(int velocity=200) {
  drive( -velocity, 32768);
}

void Roomba::goLeft(int degrees = 90) {
  drive(200,1);
  delay(degrees*4);//just guessing for now, will add magic value later
}
void Roomba::goRight(int degrees = 90) {
  drive(200,-1);
  delay(degrees*4);
}

/**
 * Use the roomba's motors to move at the specified speed in the specified direction.
 * Please note that "dead straight" corresponds to the special radius value of 32768.
 * For convenience, the programmer can call this as:
 * <code>roomba.drive(500,STRAIGHT);</code>
 * To mean that the roomba should drive forward full speed.
 *
 * @param velocity  The velocity, in mm/s. Range: -500mm/s to 500mm/s. 
 *                  Negative is taken to mean "backwards".
 * @param radius    The radius of curvature, in mm. Range: -2000mm to 2000mm, or 32768.
 *                  Negative is taken to mean right/clockwise, and positive means left/counter-clockwise.
 **/
void Roomba::drive(int velocity, int radius){
  //I put this if(...) sanity check in here because I have honestly no idea what the roomba would do
  //outside of this range of values. It's not even documented :-(
  if(velocity>=-500 && velocity<=500 && ((radius>=-2000 && radius<=2000) || radius==32768)){
    Serial.print(137,BYTE); //DRIVE
    Serial.print((velocity>>8 & 0xFF), BYTE); //Split 16-bit velocity into 2 bytes and send
    Serial.print((velocity & 0xFF), BYTE);
    Serial.print((radius>>8 & 0xFF), BYTE);
    Serial.print((radius & 0xFF), BYTE);
  } 
}

void Roomba::disco(){
led('s');
delay(150);
led('c');
delay(150);
led('m');
delay(150);
led('d');
delay(150);
}

void Roomba::led(char type){
  Serial.print(139, BYTE);
  if(type=='s')
    Serial.print(B00001000, BYTE);
  else if (type == 'c')
    Serial.print(B00000100, BYTE);
  else if (type == 'm')
    Serial.print(B00000010, BYTE);
  else if (type == 'd')
    Serial.print(B00000001, BYTE);
  Serial.print(0, BYTE);
  Serial.print(0xff, BYTE);
}


/**
 * Download from the Roomba all the sensor values.
 * However, the act of downloading this information resets some of this information on the Roomba.
 * For instance, 'distance' refers to the distance (in mm) traveled since the last time this sensor 
 * data was downloaded from the Roomba.
 **/
void Roomba::updateSensors() {
/*
  Serial.print(142, BYTE);
  Serial.print(0,   BYTE);  // sensor packet 1, 10 bytes
  delay(100); // wait for sensors 
  char i = 0;
  while(Serial.available()) {
    int c = Serial.read();
    if( c==-1 ) {
    //ERROR
    }
    sensorbytes[i++] = c;
  }*/   
}

/**
 * Sets the baud rate of the Arduino and of the Roomba. Ignores you
 * if you choose an invalid baud rate.
 * Not so sure why you'd want to do this, except maybe to pop it up to
 * 115200 baud for some fine-grained control.
 *
 * WARNING: The Arduino will slip back into its original default baud rate
 * (57600 baud) upon loss of battery power.
 *
 * Code note: This is pretty much why switch statements are awesome.
 **/
void Roomba::setBaud(int baudRate){
  char i=0;
  switch(baudRate){
  case 115200:
    i++;
  case 57600:
    i++;
  case 38400:
    i++;
  case 28800:
    i++;
  case 19200:
    i++;
  case 14400:
    i++;
  case 9600:
    i++;
  case 4800:
    i++;
  case 2400:
    i++;
  case 1200:
    i++;
  case 600:
    i++;
  case 300:
    Serial.print(129, BYTE); //BAUD (mode-->passive)
    Serial.print(i, BYTE);
    Serial.begin(baudRate);
    delay(100);//wait 100ms as per manufacturer specs
  default:
    break;
  }
}
  /**
   * Puts the roomba into sleep mode. This is exactly equivalent 
   * to pressing the "power" button when the device isn't being 
   * controlled by some moron with an Arduino.
   * 
   * I guess you could use this if you wanted to save power, or,
   * even better, if you to use the hardware serial lines for something else,
   * like sending debug data to the computer from the Arduino.
   * (i.e. roomba.sleep(); Serial.println("Debug message"); roomba.wake();)
   *
   * @see wake()
   */
  void Roomba::sleep(){
    digitalWrite(_wakePin, LOW); //set DD pin low just in case
    Serial.print(133, BYTE); //POWER
  }

  /**
   * Wake the roomba from sleep mode.
   * @see sleep()
   */
  void Roomba::wake(){
    digitalWrite(_wakePin, HIGH);
    delay(500);
    digitalWrite(_wakePin, LOW);
  }

  /**
   * Puts the roomba into spot-cleaning mode, where it spirals out
   * from its current location, cleaning the floor.
   * This command is exactly equivalent to pressing the "spot" button 
   * when the device isn't being controlled by some moron with an Arduino.
   **/
  void Roomba::spotClean(){
    Serial.print(134, BYTE); //SPOT (mode-->passive)
  }
  /**
   * Puts the roomba into normal cleaning mode.
   *
   * This command is exactly equivalent to pressing the "clean" button 
   * when the device isn't being controlled by some moron with an Arduino.
   **/
  void Roomba::clean(){
    Serial.print(135, BYTE); //CLEAN (mode-->passive)
  }

  /**
   * Puts the roomba into maximum time cleaning mode. 
   * (MAX = go until battery is dead)
   *
   * This command is exactly equivalent to pressing the "clean" button 
   * when the device isn't being controlled by some moron with an Arduino.
   **/
  void Roomba::maxClean(){
    Serial.print(136, BYTE); //MAX (mode-->passive)
  }


  /**
   * Well, the Roomba's default way of doing songs is fairly deficient from a 
   * user-interface perspective. This way is much more user-friendly, at the 
   * cost of some processing power/ memory. My floor (Computer Science House) 
   * owns a vintage IBM PCJr, and this is essentially it's "play" command
   * from its BASIC interpreter.
   *
   * Code note: I'm fairly sure my roommate owes me a Jolt Cola for this.
   * I mean, PCJr? Really?
   */
  void Roomba::PCJrPlay(char song[]){
    int arrayLength=0;
    for(int i=0; song[i]!='\0'; i++){arrayLength=i+1;}
    
    char temp[32]={0};
    int temp_index=0;
    char thisNote=0;
    char thisOctave=3;
    char thisDuration=64;

    for(int i=0; song[i]!='\0'; i++){
      //if(note)
      if((song[i]>='A' && song[i]<='G') || (song[i]>='a' && song[i]<='g')){

	//Set note to it's fundamental frequency
	if(song[i]=='A' || song[i]=='a'){thisNote=33;}
	if(song[i]=='B' || song[i]=='b'){thisNote=35;}
	if(song[i]=='C' || song[i]=='c'){thisNote=36;}
	if(song[i]=='D' || song[i]=='d'){thisNote=38;}
	if(song[i]=='E' || song[i]=='e'){thisNote=40;}
	if(song[i]=='F' || song[i]=='f'){thisNote=41;}
	if(song[i]=='G' || song[i]=='g'){thisNote=43;}

	//Didn't want to use multiplication, too much memory usage
	switch(thisOctave){
	case 5:
	  thisNote=thisNote+12;
	case 4:
	  thisNote=thisNote+12;
	case 3:
	  thisNote=thisNote+12;
	case 2:
	  thisNote=thisNote+12;
	}
	
	  thisNote=(thisNote-1)*12+thisNote;

	//WRITE OUT NOTE BYTE
	temp[temp_index]=thisNote;

	//if(whole note through eight note)
	if(song[i+1]>='1' && song[i+1]<='8'){ //Eventually I will use tempo here
	  i++;
	  if(song[i]=='1'){
	    if(i+1<=arrayLength)
	      if(song[++i]==6)
		temp[temp_index++]=4; //sixteenth note
	    temp[temp_index++]=64; //whole note
	  }
	  if(song[i]=='2'){ temp[temp_index++]=32;}//half note
	  if(song[i]=='4'){ temp[temp_index++]=16;}//quarter note
	  if(song[i]=='8'){ temp[temp_index++]=8; } //eighth note
	  if(song[i]=='3' && i+1<=arrayLength)
	    if(song[++i]=='2')
	      temp[temp_index++]=2; //32nd note
	  if(song[i]=='6' && i+1<=arrayLength)
	    if(song[++i]=='4')
	      temp[temp_index++]=1; //64th note
	}
	//else, no special duration specified, use current
	else{
	  temp[temp_index++]=thisDuration;
	}
  }
  
      //Octave change, Ox notation
  if(song[i]=='o' || song[i]=='O'){
	//Octave zero doesn't exist on the roomba
	  if(song[i+1]>='1' && song[i+1]<='9'){
	    thisOctave=song[i+1]-48;
	  }
	i++;
  }

      //Octave change, arrow notation
      if(song[i]=='>' && thisOctave !=5)
	thisOctave++;
      if(song[i]=='<' && thisOctave!=1)
	thisOctave--;

      //Note length change, L notation
      if(song[i]=='L' || song[i]=='l'){
	if(song[i+1]>='1' && song[i+1]<='8')
	  i++;
	  if(song[i]=='1'){
	    if(i+1<=arrayLength)
	      if(song[++i]==6){
		thisDuration=4; //sixteenth note
	      }
	    thisDuration=64; //whole note
	  }
	  else if(song[i]=='2'){ thisDuration=32;}//half note
	  else if(song[i]=='4'){ thisDuration=16;}//quarter note
	  else if(song[i]=='8'){ thisDuration=8; } //eighth note
	  else if(song[i]=='3' && i+1<=arrayLength){
	    if(song[++i]=='2')
	      thisDuration=2; //32nd note
	  }
	  else if(song[i]=='6' && i+1<=arrayLength)
	    if(song[++i]=='4')
	      thisDuration=1; //64th note
	}
    }//for

    setSong(temp, temp_index+1, 15);
    playSong(15);
  }

  
  /**
   * This will work much nicer if you choose nice perfect-square numbers
   * as a tempo.
   **/
  void Roomba::PCJrTempo(int tempo){
 
    _defaultDuration=3840/tempo; //convert BPM to # of 1/64sec periods
  }

  /**
   * Set the song in the roomba's memory.
   *
   * Format:
   * song[0]=  Note number (MIDI format, aka 60=C, 61=C#)
   *           Valid range is 31(G) to 127(G)
   * song[1]=  Duration of note in 1/64 second periods
   * song[2]=  Note number...
   * song[3]=  Duration...
   * (etc)
   * 
   * Songs can only be 16 notes long, due to internal roomba limitations.
   **/
  void Roomba::setSong(char song[], char songLength, char songNumber){
    if(songLength>0 && songLength<17){
      Serial.print(140, BYTE); //SONG
      Serial.print(songNumber, BYTE);
      Serial.print(songLength, BYTE);
      //apologies if the bitshift is confusing, I'm trying to avoid
      // libraries for multiplication,etc being drawn into the compile
      // and taking up extra memory space
      for(int i=0; song[i]<(songLength << 1); i++){Serial.print(song[i]);}
    }
  }

  void Roomba::playSong(char songNumber){
  }