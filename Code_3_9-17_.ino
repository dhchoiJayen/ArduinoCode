#include <SimpleDHT.h>  //온습도센서 라이브러리
#include <LiquidCrystal.h>  //LCD 라이브러리

#define DHTPIN              7     //DHT핀 (온습도센서) -> 아두이노 연결핀 번호
#define DHTTYPE             DHT11 //DHT 타임 정하기 - DHT11
#define LCD_resolution      6   //LCD 화면 글자선명도 연결핀
SimpleDHT11 dht11(DHTPIN);    //DHT 설정(온습도 센서 설정 )
LiquidCrystal lcd (13,12,5,4,3,2) ; //LCD 핀 설정

//미세먼지 센서 변수 설정
#define Dust_LED_PIN    11    //미세먼지 센서 LED 핀
#define Dust_OUT_PIN    A0    //미세먼지 센서값 출력핀
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float dust_sensorValue = 0;
float voltage = 0;
float dust_density = 0;

//RGB LED pin
#define RED      8
#define GREEN    9
#define BLUE     10

void setup() {    //살정과 관련한 함수
  Serial.begin(9600);
  lcd.begin (16,2);   //LCD 사용 설정 
  analogWrite (LCD_resolution , 100);   //LCD 글자선명도 (0~255)
  lcd.clear();    //LCD 화면 지우기
  pinMode (Dust_LED_PIN , OUTPUT);    //미세먼지 LCD 출력 모드
  pinMode (RED, OUTPUT);
  pinMode (GREEN, OUTPUT);
  pinMode (BLUE, OUTPUT);
}

void loop() {   //메인 코드 
  dht_check() ; //온습도 체크  -> 출력
  dust_check(); //미세먼지 체크 -> 출력
  RGB_LED_check(); //RFG LED체크(미세먼지 농도에 따라 색깔 출력)
}

//온습도를 체크하는 코드
void dht_check() {  
  delay(2000);    //2초 딜레이를 줘야 온습도 센서가 올바로 작동됨
  byte t = 0;     //온도 변수
  byte h = 0;     //습도 변수
  dht11.read (&t,&h, NULL); //온습도 값 저장  (센서로 온습도 측정해서 t,h 에 넣으시오)

  lcd.setCursor(0,0); //LCD 왼쪽의 첫칸
  lcd.print ("T: ");
  lcd.print (t);    //온습도 출력
  lcd.print (" C");
  lcd.setCursor (9,0);    //LCD 첫줄 가운데쯤
  lcd.print ("H:");
  lcd.print (h);    //습도값 출력
  lcd.print (" %");
}

//미세먼지 측정, 출력 코드
void dust_check() {
  digitalWrite(Dust_LED_PIN, LOW);    //적외선 LED ON
  delayMicroseconds (samplingTime );    280us = 0.28ms(딜레이)
  dust_sensorValue = analogRead (Dust_OUT_PIN);   //미세먼지 출력값 읽기
  delayMicroseconds(deltaTime);   //for pulse width of 0.32ms = 0.28ms +0.04ms
  digitalWrite (Dust_LED_PIN ,HIGH);    //적외선 LED OFF
  delayMicroseconds(sleepTime);   //9.680ms (10ms = 9.680ms +0.32ms)
  voltage = dust_sensorValue * (5.0 / 1023.0);    //0~1023센서값을 0~5V로 변환하기
  dust_density = 50*voltage;    //ug/m3로 단위 변환
  lcd.setCursor(0,1);
  lcd.print ("Dust: ");
  lcd.print (dust_density);
  lcd.print (" ug/m3");   //PM 2.5
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

void LED_Color (int r, int g, int b) {  //by PM 2.5
  digitalWrite (RED, r);  //빨간색
  digitalWrite (GREEN, g);  //초록색
  digitalWrite (BLUE, b);   //파란색
}
