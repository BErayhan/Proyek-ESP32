#include <LiquidCrystal_I2C.h> //memanggil library LCD
#include <ESP32Servo.h>  //memanggil library servo
Servo penutup;

const int trigPin = 5;
const int echoPin = 18;
const int relay = 26;
int servopin = 2;
int i;


//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  lcd.begin();
  lcd.backlight ();
  lcd.clear ();
  penutup.attach (servopin);
  penutup.write (0);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(relay, OUTPUT);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

     if (distanceCm < 10){
        lcd.clear ();
        lcd.setCursor (4, 0);
        lcd.print ("Jarak: ");
        lcd.setCursor (4, 1);
        lcd.print (distanceCm);
        lcd.setCursor (8, 1);
        lcd.print("cm");
        delay (1000);
        lcd.clear ();
        penutup.write (130);    //posisikan servo di 180 derajat
        delay (5000);           //lama penutup terbuka
        penutup.write (0);     // penutup kembali tertutup
        digitalWrite (relay, LOW); //menyalakan relay (lampu uvc)

        for (i = 60;i>=0;i--){
            lcd.setCursor (4, 0);
            lcd.print ("WAKTU:");
            lcd. setCursor (4, 1);
            lcd.print (i);
            lcd.setCursor (9, 1);
            lcd.print ("detik");
            delay (1000);
              if (i<=10)
                lcd.setCursor (5, 1);
                lcd.print (" ");
            }
               if (i==0) {
                  digitalWrite (relay, HIGH); //mematikan relay (lampu) 
                  penutup.write (130); //posisikan servo di 180 derajat
                  delay (5000);
                  penutup.write (0);
                  lcd.clear ();
           }    
      }
    else{
          penutup. write (0);  // servo di 0 derajat
          digitalWrite (relay, HIGH); //lampu tetap mati 
          lcd. setCursor (1, 0);
          lcd.print("Jarak > 10 cm");
          lcd. setCursor (1, 1);
          lcd.print ("Lampu UVC mati.");
          //delay(1000);
      
    }
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);


  delay(1000);
}
