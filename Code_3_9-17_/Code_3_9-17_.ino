#include <SimpleDHT.h>
#include <LiquidCrystal.h>

#define DHTPIN              7
#define DHTTYPE             DHT11
#define LCD_resolution      6
SimpleDHT11 dht11(DHTPIN);
LiquidCrystal lcd (13,12,5,4,3,2) ;

#define Dust_LED_PIN    11
#define Dust_OUT_PIN    A0
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float dust_sensorValue = 0;
float voltage = 0;
float dust_density = 0;

#define RED      8
#define GREEN    9
#define BLUE     10

void setup() {
  Serial.begin(9600);
  lcd.begin (16,2);
  analogWrite (LCD_resolution , 100);
  lcd.clear();
  pinMode (Dust_LED_PIN , OUTPUT);
  pinMode (RED, OUTPUT);
  pinMode (GREEN, OUTPUT);
  pinMode (BLUE, OUTPUT);
}

void loop() {
  dht_check() ; //온습도 체크
  dust_check(); //미세먼지 체크
  RGB_LED_check(); //RFG LED체크
}

void dht_check() {
  delay(2000);
  byte t = 0;
  byte h = 0;
  dht11.read (&t,&h, NULL);

  lcd.setCursor(0,0);
  lcd.print ("T: ");
  lcd.print (t);
  lcd.print (" C");
  lcd.setCursor (9,0);
  lcd.print ("H:");
  lcd.print (h);
  lcd.print (" %");
}

void dust_check() {
  digitalWrite(Dust_LED_PIN, LOW);
  delayMicroseconds (samplingTime );
  dust_sensorValue = analogRead (Dust_OUT_PIN);
  delayMicroseconds(deltaTime);
  digitalWrite (Dust_LED_PIN ,HIGH);
  delayMicroseconds(sleepTime);
  voltage = dust_sensorValue * (5.0 / 1023.0);
  dust_density = 50*voltage;
  lcd.setCursor(0,1);
  lcd.print ("Dust: ");
  lcd.print (dust_density);
  lcd.print (" ug/m3");
}

void RGB_LED_check() {
  if(dust_density >= 151) {
    LED_Color (1,0,0);  //빨강
  }
  else if(dust_density >= 51) {
    LED_Color (1,1,0);  //노랑
  }
  else if(dust_density >= 16) {
    LED_Color (0,1,0);  //초록
  }
  else if(dust_density >= 0) {
    LED_Color (0,0,1);  //블루
  }
  else{
    LED_Color (0,0,0);  //no color
  }
}

void LED_Color (int r, int g, int b) {
  digitalWrite (RED, r);
  digitalWrite (GREEN, g);
  digitalWrite (BLUE, b);
}
