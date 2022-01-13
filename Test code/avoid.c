#include <SoftwareSerial.h>
//#include <Servo.h>
#define TRIG 8 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 9 //ECHO 핀 설정 (초음파 받는 핀)

// 오른쪽,왼쪽 거리 감지
#define R_TRIG 6 
#define R_ECHO 7
#define L_TRIG 12
#define L_ECHO 13


// 오른쪽 바퀴들
int Dir1Pin_R = 2;      // 제어신호 1핀
int Dir2Pin_R = 3;      // 제어신호 2핀
int SpeedPin_R = 10;    // PWM제어를 위한 핀 

// 왼쪽 바퀴들
int Dir6Pin_L = 4;
int Dir7Pin_L = 5;
int SpeedPin_L = 11;

int speed = 100; // 속도 제어


void go_straight(int s){
  // 직진
    digitalWrite(Dir1Pin_R, LOW);         // 오른쪽 바퀴들 앞으로
    digitalWrite(Dir2Pin_R, HIGH);
    analogWrite(SpeedPin_R, s);
     
    digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 앞으로
    digitalWrite(Dir7Pin_L, LOW);
    analogWrite(SpeedPin_L, s);  
}

void turn_left(){
  //좌회전
    digitalWrite(Dir1Pin_R, LOW);         // 오른쪽 바퀴들 앞으로
    digitalWrite(Dir2Pin_R, HIGH);
    analogWrite(SpeedPin_R, 100);
    digitalWrite(Dir6Pin_L, HIGH);         
    digitalWrite(Dir7Pin_L, HIGH);

    Serial.println("좌회전");  
}

void turn_right(){
  //우회전
     digitalWrite(Dir1Pin_R, HIGH);         
     digitalWrite(Dir2Pin_R, HIGH);
     digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 앞으로
     digitalWrite(Dir7Pin_L, LOW);
     analogWrite(SpeedPin_L, 100);


     Serial.println("우회전"); 
}

void back_straight(){
  // 후진
    digitalWrite(Dir1Pin_R, HIGH);         // 오른쪽 바퀴들 뒤로
    digitalWrite(Dir2Pin_R, LOW);
    analogWrite(SpeedPin_R, 100);
     
    digitalWrite(Dir6Pin_L, LOW);         // 왼쪽 바퀴들 뒤로
    digitalWrite(Dir7Pin_L, HIGH);
    analogWrite(SpeedPin_L, 100);  
}

long detect_distance(int trig, int echo){

    long duration, distance;

    pinMode(trig,OUTPUT);
    pinMode(echo,INPUT);

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn (echo, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.
    distance = duration * 17 / 1000; 
    return distance;
}
   
void setup() {
  
    
  Serial.begin(9600); //PC모니터로 센서값을 확인하기위해서 시리얼 통신을 정의해줍니다. 
                       //시리얼 통신을 이용해 PC모니터로 데이터 값을 확인하는 부분


 // pinMode(TRIG, OUTPUT);
 // pinMode(ECHO, INPUT);

  pinMode(Dir1Pin_R, OUTPUT);             // 제어 1번핀 출력모드 설정
  pinMode(Dir2Pin_R, OUTPUT);             // 제어 2번핀 출력모드 설정
  pinMode(SpeedPin_R, OUTPUT);            // PWM제어핀 출력모드 설정

  pinMode(Dir6Pin_L, OUTPUT);             // 제어 6번핀 출력모드 설정
  pinMode(Dir7Pin_L, OUTPUT);             // 제어 7번핀 출력모드 설정
  pinMode(SpeedPin_L, OUTPUT);            
}

void loop(){

  long distance;

  /*digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //34000*초음파가 물체로 부터 반사되어 돌아오는시간 /1000000 / 2(왕복값이아니라 편도값이기때문에 나누기2를 해줍니다.)
  //초음파센서의 거리값이 위 계산값과 동일하게 Cm로 환산되는 계산공식 입니다. 수식이 간단해지도록 적용했습니다.
  front_duration = pulseIn (ECHO, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.
  front_distance = front_duration * 17 / 1000; 
  Serial.println(front_duration ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.
  Serial.print("\nDIstance : ");
  Serial.print(front_distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm");
  */

  distance = detect_distance(TRIG,ECHO);
  Serial.print("\nDIstance : ");
  Serial.print(distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm");
  
  if( distance <= 30){ // 앞에 물체가 있다면 속도를 서서히 줄이기
    while(speed>0){
      speed -= 20;
      analogWrite(SpeedPin_R, speed);
      analogWrite(SpeedPin_L, speed);
      delay(50);
    }

    back_straight();
    delay(1000);
    long L_dist = detect_distance(L_TRIG,L_ECHO);
    long R_dist = detect_distance(R_TRIG,R_ECHO);


    Serial.print("\n왼 DIstance : ");
    Serial.print(L_dist); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
    Serial.println(" Cm");
    Serial.print("\n오 DIstance : ");
    Serial.print(R_dist); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
    Serial.println(" Cm");

    
    if(L_dist > R_dist && L_dist > 30){
      
      // 좌회전
      turn_left();
      delay(1000);
          
    }else if(L_dist<R_dist && R_dist > 30){
      
      // 우회전
      turn_right();
      delay(1000); 
    }else if(L_dist > 50){
      // 좌회전
      turn_left();
      delay(1000);
    }else{
      back_straight();
      delay(1000);
    }
    
   
  }else{
     // 앞으로 --------
    
     while(speed < 100){
      speed += 20;
      go_straight(speed);
      delay(100);
    }
    go_straight(speed); 
  }
  

  
  delay(100); //1초마다 측정값을 보여줍니다.
}




