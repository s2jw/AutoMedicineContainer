#include <SPI.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <DS1302.h>
#include <TimeLib.h>
#include <Stepper.h>

#define piezo A0
#define vib A1
#define ECHO 6
#define TRIGGER 5
#define RED 8
String lcdString = ""; 
IRrecv irrecv(7); //적외선 센서가 연결된 디지털 핀 번호 매핑
decode_results results; //수신된 적외선 신호를 저장할 변수
LiquidCrystal lcd(10); //RS 핀, E핀, 데이터 핀 4개
DS1302 rtc(2,3,4);
const int stepsPerRevolution = 300;
Stepper myStepper(stepsPerRevolution, A2, A3, A4, A5);
Time t;
int date;
int theDate[4];
int i = 0, j = 0, k = 0;
long duration, distance;

//알람이 울릴시간인지 체크하는 함수
void checkTheAlarmTime (int alarmHour,int alarmMinute) 
{
  t = rtc.getTime();
  Serial.println(t.hour);
  Serial.println(t.min);
  Serial.println(alarmHour);
  Serial.println(alarmMinute);

  
  if(alarmHour == t.hour && alarmMinute == t.min && j == 0) {
    Serial.println("success");
    j = 1;
    analogWrite(piezo, 128); // 맞으면 부저가 울림!
    digitalWrite(vib, HIGH); //진동
    delay(500);
    digitalWrite(vib, LOW);
    myStepper.step(stepsPerRevolution); //반 바퀴 회전
    delay(20000);
    analogWrite(piezo, 0);
    delay(30000);
   }

    if (j == 1) {
      if (distance > 5) {
        k = 1;
      }
      if (k == 0) {
      analogWrite(piezo, 128);
      delay(20000);
      k = 1;
      analogWrite(piezo, 0);
    }
    }

}
  
void setup()
{
  irrecv.enableIRIn(); //적외선 센서 활성화, 리모콘
  lcd.begin(16,2); //LCD 크기 지정, 2줄 16칸
  lcd.clear(); //화면 초기화
  Serial.begin(9600);
  rtc.halt(false);
  rtc.writeProtect(false);
 
  rtc.setDOW(TUESDAY);    
  rtc.setTime(13, 20, 00);     
  rtc.setDate(14, 12, 2021);   

  pinMode(piezo, OUTPUT); //부저
  pinMode(vib, OUTPUT); //진동 모
  pinMode(RED, OUTPUT);
  myStepper.setSpeed(30);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
}

void loop()
{
  Serial.println(k);
  irrecv.enableIRIn();
  lcd.clear();
  lcd.setCursor(4, 0);
  Serial.println("시간츨력");
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 1);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));

  lcd.setCursor(6, 1);
  lcd.print(rtc.getDateStr());

  delay (1000);
  
 
 while(i<4){
  if (irrecv.decode(&results)) { //적외선 신호를 해석
    //if(results.value==0XFF906F){
       //irrecv.resume(); //다음 신호를 받을 수 있도록 초기화
   
    Serial.println(results.value, HEX); //적외선 신호 값을 시리얼에 출력
    
    /* 반드시 작동할 컴퓨터에서 리모콘 값 확인 후 맞춰서 수정할 것! */
    /* 리모콘이 눌리는 느낌이 잘 안 나서 잘 된 건지 아닌지 알 수가 없는데 시리얼 모니터로만 확인할 수 있기 때문에
    숫자가 잘 눌렸다면 LED에 불이 들어 오는 식으로 확인하면 좋을 것 같아 추가함! */
    if (results.value == 0XFF6897) { //숫자 0이 눌리면
      Serial.println("0");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 0;
    }
    else if (results.value == 0XFF30CF) { //숫자 1
      Serial.println("1");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 1; //입력한 숫자를 배열에 받음
    }
    else if (results.value == 0XFF18E7) { //숫자 2
      Serial.println("2");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 2;
    }
    else if (results.value == 0XFF7A85) { //숫자 3
      Serial.println("3");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 3;
    }
    else if (results.value == 0XFF10EF) { //숫자 4
      Serial.println("4");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 4;
    }
    else if (results.value == 0XFF38C7) { //숫자 5
      Serial.println("5");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 5;
    }
    else if (results.value == 0XFF5AA5) { //숫자 6
      Serial.println("6");
     digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 6;
    }
    else if (results.value == 0XFF42BD) { //숫자 7
      Serial.println("7");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 7;
    }
    else if (results.value == 0XFF4AB5) { //숫자 8
      Serial.println("8");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 8;
    }
    else if (results.value == 0XFF52AD) { //숫자 9
      Serial.println("9");
      digitalWrite(RED, HIGH);
      delay(500);
      digitalWrite(RED, LOW);
      theDate[i++] = 9;
    }
    
    delay(30);
    irrecv.resume(); //다음 신호를 받을 수 있도록 초기화
 }
}
//date = atoi(theDate);
    //lcd.print(day); //LCD에 알람 입력 시간 출력, 이 될까?
    //알람 설정 시간을 LCD에 출력을 어떻게 해야할까?

  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 34000 / 1000000 / 2;
  date = theDate[0] * 1000 + theDate[1] * 100 + theDate[2]*10 + theDate[3];
  checkTheAlarmTime(date / 100, date % 100); //알람 시계
  Serial.print("distance");
  Serial.println(distance);

  
  
  //알람시계가 울릴 때 진동, 부저, LED 등을 구현해야 하고 (함수에 일단 적어둠)
  //알람이 울림과 동시에 모터가 돌아가면서 약 봉지가 출력되는 코드 구현
  //이후 초음파 센서를 통해 동작 감지, 가져갔는지 아닌지 감지하는 코드 구현
}
