#include <SoftwareSerial.h>
#define TRIG 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 8 //ECHO 핀 설정 (초음파 받는 핀)


// 오른쪽 바퀴들
int Dir1Pin_R = 2;      // 제어신호 1핀
int Dir2Pin_R = 3;      // 제어신호 2핀
int SpeedPin_R = 10;    // PWM제어를 위한 핀 

// 왼쪽 바퀴들
int Dir6Pin_L = 6;
int Dir7Pin_L = 7;
int SpeedPin_L = 11;
void setup() {

  Serial.begin(9600); //PC모니터로 센서값을 확인하기위해서 시리얼 통신을 정의해줍니다. 
                       //시리얼 통신을 이용해 PC모니터로 데이터 값을 확인하는 부분



  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(Dir1Pin_R, OUTPUT);             // 제어 1번핀 출력모드 설정
  pinMode(Dir2Pin_R, OUTPUT);             // 제어 2번핀 출력모드 설정
  pinMode(SpeedPin_R, OUTPUT);            // PWM제어핀 출력모드 설정

  pinMode(Dir6Pin_L, OUTPUT);             // 제어 6번핀 출력모드 설정
  pinMode(Dir7Pin_L, OUTPUT);             // 제어 7번핀 출력모드 설정
  pinMode(SpeedPin_L, OUTPUT);            
}



void loop(){

  long duration, distance;

 
  
  digitalWrite(TRIG, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);


  duration = pulseIn (ECHO, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.





  //34000*초음파가 물체로 부터 반사되어 돌아오는시간 /1000000 / 2(왕복값이아니라 편도값이기때문에 나누기2를 해줍니다.)

 //초음파센서의 거리값이 위 계산값과 동일하게 Cm로 환산되는 계산공식 입니다. 수식이 간단해지도록 적용했습니다.

  distance = duration * 17 / 1000; 




  //PC모니터로 초음파 거리값을 확인 하는 코드 입니다.

  Serial.println(duration ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.

  Serial.print("\nDIstance : ");

  Serial.print(distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.

  if( distance <= 10 ){
    analogWrite(SpeedPin_R, 0);
    analogWrite(SpeedPin_L, 0);
  }else{
     // 앞으로 --------
  digitalWrite(Dir1Pin_R, LOW);         // 오른쪽 바퀴들 앞으로
  digitalWrite(Dir2Pin_R, HIGH);
  analogWrite(SpeedPin_R, 200);
 
  digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 앞으로
  digitalWrite(Dir7Pin_L, LOW);
  analogWrite(SpeedPin_L, 200);
  }
  
  Serial.println(" Cm");


  delay(100); //1초마다 측정값을 보여줍니다.


}