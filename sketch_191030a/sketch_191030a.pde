                                                                                                                                                                                                                                                                                                                                                                                                                                                    import processing.sound.*;
import processing.serial.*; //import the Serial library

 Serial myPort;  //the Serial port object
 String val;
 SoundFile file1;
 SoundFile file2;
 SoundFile cheerSFX;
 LowPass lowpass1;// = new LowPass(this);
 LowPass lowpass2;// = new LowPass(this);
 HighPass highpass1;// = new HighPass(this);
 HighPass highpass2;// = new HighPass(this);
 int savedTime;
 // set index (base 1), for starting song and set for amount of songs
 int currentSong = 1;
 int songCount = 17;
//int passedTime;
// adjust the true numbers as necessary to match sensor max and min
float min = 2.0;
float max = 27.94 - min;
float midpoint = (min + ((max)/2));

// variables for attribute control
float gain1 = 0.5;
float gain2 = 0.5;
float lowpassLimit1 = 4000.0;
float highpassLimit1 = 1;
float lowpassLimit2 = 4000.0;
float highpassLimit2 = 1;
float playrate = 1;

boolean setupComplete = false;
boolean lastE = false;




void setup() {
  size(640, 360); //make our canvas 200 x 200 pixels big
  background(25);
  //  initialize your serial port and set the baud rate to 9600
  println(Serial.list().length);
  print(Serial.list()[2]);
  myPort = new Serial(this, Serial.list()[2], 9600);
  myPort.bufferUntil('\n'); 
  
  //pre-cache all songs
  for (int i = 1; i < songCount - 1; i++) {
    file1 = new SoundFile(this, "instr-"+i+".mp3"); 
    file2 = new SoundFile(this, "voc-"+i+".mp3");
    println("loaded" + i);
  }
  cheerSFX = new SoundFile(this, "switch-1.mp3");
  println("songs loaded");
  setupComplete = true;
  
  //savedTime = millis();
}

void draw() {
}

// catch for fade
void serialEvent(Serial myPort) {
    try {
      //println(myPort);
      //put the incoming data into a String - 
      //the '\n' is our end delimiter indicating the end of a complete packet
      val = myPort.readStringUntil('\n');
      //println(val);
      //make sure our data isn't empty before continuing
      if (val != null && val != " out of range " && setupComplete == true) {
        char interaction = trim(val).charAt(0);
       //print(interaction +" first one \n ");
        //trim whitespace and formatting characters (like carriage return) and remove first char
        float f = float(trim(val.substring(1)));
        // shift f to give true 0 value
        f = f - min;
        
        // to avoid false positive on E
        if (interaction == 'E') {
          if (f < 12.0 && f >= min) {
            lastE = true;
          } else {
            lastE = false;
          }
        }
        
        
        // check if it is within range
        if (f <= max && f >= min) {
          //starts songs if nothing playing and C or D are detected
          if( !file1.isPlaying() && !file2.isPlaying()){
            if (interaction == 'C' || interaction == 'D') {         
              startSong();
            }
          } else {
            
            // A == crossfade sensor
            if (interaction == 'A') {
              gain2 = (f-min)/max;
              gain1 = 1 - gain2;
            // B == rate/BPM
            } else if (interaction == 'B') {
              playrate =  1 + (((f/midpoint) - 1)/6);
              file1.rate(playrate);
              file2.rate(playrate);
            // C == filter for file1
            } else if (interaction == 'C') {
              file1.amp(gain1);
              if (f == midpoint) {
                highpass1.stop();
                lowpass1.stop();
              } else if (f < midpoint) {
                lowpassLimit1 = 4000 - (3800*(1-f/midpoint));
                highpass1.stop();
                lowpass1.process(file1);
                lowpass1.freq(lowpassLimit1);
              } else {
                highpassLimit1 = 3000*((f-midpoint)/midpoint);
                lowpass1.stop();
                highpass1.process(file1);
                highpass1.freq(highpassLimit1);
              }
             // D == filter for file2
            } else if (interaction == 'D') {
              file2.amp(gain2);
              if (f == midpoint) {
                highpass2.stop();
                lowpass2.stop();
              } else if (f < midpoint) {
                lowpassLimit2 = 4000 - (3800*(1-f/midpoint));
                highpass2.stop();
                lowpass2.process(file2);
                lowpass2.freq(lowpassLimit2);
              } else {
                highpassLimit2 = 3000*((f-midpoint)/midpoint);
                lowpass2.stop();
                highpass2.process(file2);
                highpass2.freq(highpassLimit2);
              }
              // E == new song combo (expects serial is already filtered and immediently changes song)
            } else if (interaction == 'E') {
              if (lastE == true) {
                //check for a cheers
                  print("WOWOOOOOOOO ITS E"+currentSong);
                  print("E"+f);
                  lastE = false;
                //code to handle case where songs do not change within 3 seconds of a cheer. 
                 int passedTime = millis() - savedTime;
                 if(passedTime > 5000 && f < 12.0 && f > 2.0){
                  print("hitin"+passedTime);
                  cheerSFX.stop();
                  cheerSFX = new SoundFile(this, "switch-" + int(random(1, 6)) + ".mp3");
                  cheerSFX.amp(0.3);
                  cheerSFX.play();
                  startSong();
                  savedTime = millis();
                }
              }
           }
        }
      } else if (f > max) {
        if (interaction == 'A') {
          gain1 = 0.5;
          gain2 = 0.5;
        } else if (interaction == 'B') {
          file1.rate(1);
          file2.rate(1);
        } else if (interaction == 'C') {
          file1.amp(0.01);
        } else if (interaction == 'D') {
          file2.amp(0.01);
        }
      }
      ////code to handle end of song
      //if(!file1.isPlaying() && !file2.isPlaying()){
      //  print("neither song is playing");
      //  int newSong = currentSong;
      
      //      while (currentSong == newSong) {
      //         print(" ITS inside the while loop now");
      //        currentSong = int(random(1, songCount));
      //         print(" current song "+ currentSong);
      //      }
            
      //      file1.stop();
      //        file2.stop();
      //        startSong();
      
      //}
    }
    }
    catch(RuntimeException e) {
      e.printStackTrace();
    }
}

// starts song combo with preset variables
void startSong() {
  setupComplete = false;
  file1.stop();
  file2.stop();
  if (!file1.isPlaying() && !file2.isPlaying()) {
    
    // randomly select song
    int newSong = currentSong;
    while (currentSong == newSong) {
      currentSong = int(random(1, songCount));
    }
    file1 = new SoundFile(this, "instr-"+currentSong+".mp3");
    file1.amp(0.01);
    file1.rate(playrate);
    file1.play();
    lowpass1 = new LowPass(this);
    highpass1 = new HighPass(this);
    //lowpass1.process(file1);
   // lowpass1.freq(lowpassLimit1);
   // highpass1.process(file1);
    //highpass1.freq(highpassLimit1);
    file2 = new SoundFile(this, "voc-"+currentSong+".mp3");
    file2.amp(0.01);
    file2.rate(playrate);
    file2.play();
    lowpass2 = new LowPass(this);
    highpass2 = new HighPass(this);
    //lowpass2.process(file2);
    //lowpass2.freq(lowpassLimit2);
    //highpass2.process(file2);
    //highpass2.freq(highpassLimit2);
    setupComplete = true;
  }
}
