#include <SoftwareSerial.h>
#include <math.h>

/*
#define BT_RX A1
#define BT_TX A0
SoftwareSerial bluetooth(BT_RX,BT_TX);
*/

#define TRIG 8 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 9 //ECHO 핀 설정 (초음파 받는 핀)
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

// 복귀

double x = 0 , y = 0; // 시작 위치
int dir = 1;
int where = 0;
int x_state = 0 , y_state = 0;
boolean start_return = false;
double comeback_home;
// dir : 1 - 상 , 2 - 우 , 3 - 하 , 4 - 좌
// where : 1 - 1사분면 , 2 - 2사분면, 3 - 3사분면, 4 - 4사분면
// x_state , y_state : 1 - 증가 중, -1 - 감소 중

// < x, y 의 증감 상태 > + < 현재 어느 사분면인지 > 
// 를 알면 차의 진행 방향을 알 수 있음

// *** 가정 ***
// 차는 꼭 수직형태로만 움직인다고 가정
// 돌아오는 길에는 장애물이 없다고 가정
// "후진"은 따로 안하고, 방향 전환에만 사용한다 가정


void go_straight(int s){
  // 직진
    digitalWrite(Dir1Pin_R, LOW);         // 오른쪽 바퀴들 앞으로
    digitalWrite(Dir2Pin_R, HIGH);
    analogWrite(SpeedPin_R, s);
     
    digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 앞으로
    digitalWrite(Dir7Pin_L, LOW);
    analogWrite(SpeedPin_L, s);
    
    if(dir == 1){
      y++;
      y_state = 1;
      x_state = 0;
    }else if(dir == 2){
      x++;
      x_state = 1;
      y_state = 0;
    }else if(dir == 3){
      y--;
      y_state = -1;
      x_state = 0;
    }else if(dir == 4){
      x--;
      x_state = -1;
      y_state = 0;
    }
    
    if( x > 0 && y > 0){
      where = 1;
    }else if( x < 0 && y > 0){
      where = 2;
    }else if( x < 0 && y < 0){
      where = 3;
    }else if( x > 0 && y < 0){
      where = 4;
    }else 
      where = 0;
}

void turn_left(){
  //좌회전
    digitalWrite(Dir1Pin_R, LOW);         // 오른쪽 바퀴들 앞으로
    digitalWrite(Dir2Pin_R, HIGH);
    analogWrite(SpeedPin_R, 100);
    digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 뒤로         
    digitalWrite(Dir7Pin_L, HIGH);
    Serial.println("좌회전");  

                  // 이전 진행 방향
    if(dir == 1){ // 방향 상 
      dir = 4;
    }else if(dir == 2){ // 방향 우
      dir = 1;
    }else if(dir == 3){ // 방향 하
      dir = 2;
    }else if(dir == 4){ // 방향 좌
      dir = 3;
    }
}

void turn_right(){
  //우회전
     digitalWrite(Dir1Pin_R, HIGH);        // 오른쪽 바퀴들 뒤로         
     digitalWrite(Dir2Pin_R, HIGH);
     digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 앞으로
     digitalWrite(Dir7Pin_L, LOW);
     analogWrite(SpeedPin_L, 100);

     Serial.println("우회전"); 

     if(dir == 1){
        dir = 2;
     }else if(dir == 2){
        dir = 3;
     }else if(dir == 3){
        dir = 4;
     }else if(dir == 4){
        dir = 1;
     }
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

void obstacle(long distance, long L_dist,long R_dist){
   Serial.print(distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
   Serial.println(" Cm");
    
   if( distance < 30){ // 앞에 물체가 있다면 속도를 서서히 줄이기
        while(speed>0){
          speed -= 20;
          analogWrite(SpeedPin_R, speed);
          analogWrite(SpeedPin_L, speed);
          delay(50);
        }
  
        back_straight();
        delay(500);
        
        Serial.print("\n왼 DIstance : ");
        Serial.print(L_dist); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
        Serial.println(" Cm");
        Serial.print("\n오 DIstance : ");
        Serial.print(R_dist); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
        Serial.println(" Cm");
    
    
        // 더 넓은 공간으로 회전
        // 그럴 경우 드물겠지만 좌,우 공간이 같을 때, default 좌회전
        // 양 옆 공간 없을 때 후진
        
        if(L_dist > R_dist && L_dist > 30){ 
          turn_left();
          delay(800);
        }else if(L_dist<R_dist && R_dist > 30){
          turn_right();
          delay(800); 
        }else if(L_dist > 30){
          turn_left();
          delay(800);
        }else{
          back_straight();
          delay(1000);
        }
       
    }
}

void left_rotation(int val){

  //2400 대략 한바퀴
     digitalWrite(Dir1Pin_R, HIGH);        // 오른쪽 바퀴들 뒤로         
     digitalWrite(Dir2Pin_R, LOW);
     analogWrite(SpeedPin_R, 100);
     digitalWrite(Dir6Pin_L, HIGH);         // 왼쪽 바퀴들 앞으로
     digitalWrite(Dir7Pin_L, LOW);
     analogWrite(SpeedPin_L, 100);
     delay(val); 
     Serial.print("왼쪽으로 회전\n");
}

void right_rotation(int val){
    digitalWrite(Dir1Pin_R, LOW);        // 오른쪽 바퀴들 앞으로         
     digitalWrite(Dir2Pin_R, HIGH);
     analogWrite(SpeedPin_R, 100);
     digitalWrite(Dir6Pin_L, LOW);         // 왼쪽 바퀴들 뒤로
     digitalWrite(Dir7Pin_L, HIGH);
     analogWrite(SpeedPin_L, 100);
     delay(val); 
     Serial.print("오른쪽으로 회전\n");
}

void rotate_to_return(double x, double y){
    double angle = atan2(y,x) * (180/3.1415);
      
    Serial.print("현재 좌표\n");Serial.print(x);
    Serial.print(" ");Serial.print(y);Serial.print(" : 각도 - ");Serial.print(angle);
    Serial.print("\n");

    double car_angle;
    
  // delay(2400) 한바퀴라 가정
  // val = delay 할 시간
    double val;
    
    if( where == 1){           // 1사분면에서 각 x, y 가 증가( 출발점에서 바깥으로)할 때, 감소(출발점으로 안으로)할 때 돌릴 방향 결정
      if(x_state == 1){
          car_angle = 180 - angle;
          val = car_angle*(2400/365);
          right_rotation(val);
      }else if(y_state == 1){
          car_angle = 90 + angle;
          val = car_angle*(2400/365);
          left_rotation(val);
      }else if(x_state == -1){
          car_angle = angle;
          val = car_angle*(2400/365);
          left_rotation(val);
      }else if(y_state == -1){
          car_angle = 90 - angle;
          val = car_angle*(2400/365);
          right_rotation(val);
      } 
    }else if(where == 2){                           // 2사분면
      if(x_state == 1){
          car_angle = 180 - angle;
          val = car_angle*(2400/365);
          right_rotation(val);
      }else if(y_state == 1){
          car_angle = 270 - angle;
          val = car_angle*(2400/365);
          right_rotation(val);
      }else if(x_state == -1){
          car_angle = angle;
          val = car_angle*(2400/365);
          left_rotation(val);
      }else if(y_state == -1){
          car_angle = angle - 90;
          val = car_angle*(2400/365);
          left_rotation(val);
      } 
    }else if(where == 3){                        // 3사분면   , 3,4 분면은 각도가 -로 나옴. 1사분면에서의 각도
      if(x_state == 1){
          car_angle = 180 + angle;
          val = car_angle*(2400/365);
          left_rotation(val);
      }else if(y_state == 1){
          car_angle = 90 - (180 + angle);
          val = car_angle*(2400/365);
          right_rotation(val);
      }else if(x_state == -1){
          car_angle = angle * -1;
          val = car_angle*(2400/365);
          right_rotation(val);
      }else if(y_state == -1){
          car_angle = 270 + angle ;
          val = car_angle*(2400/365);
          left_rotation(val);
      } 
    }else if(where == 4){                      // 4사분면
      if(x_state == 1){
          car_angle = 180 + angle;
          val = car_angle*(2400/365);
          left_rotation(val);
      }else if(y_state == 1){
          car_angle = 90 + angle;
          val = car_angle*(2400/365);
          left_rotation(val);
      }else if(x_state == -1){
          car_angle = angle * -1;
          val = car_angle*(2400/365);
          right_rotation(val);
      }else if(y_state == -1){
          car_angle = 90 + ( angle * -1) ;
          val = car_angle*(2400/365);
          right_rotation(val);
      } 
    }
    
      
}
   
void setup() {
  
    
  Serial.begin(9600); //PC모니터로 센서값을 확인하기위해서 시리얼 통신을 정의해줍니다. 
                       //시리얼 통신을 이용해 PC모니터로 데이터 값을 확인하는 부분
 // bluetooth.begin(9600);
  
  pinMode(Dir1Pin_R, OUTPUT);             // 제어 1번핀 출력모드 설정
  pinMode(Dir2Pin_R, OUTPUT);             // 제어 2번핀 출력모드 설정
  pinMode(SpeedPin_R, OUTPUT);            // PWM제어핀 출력모드 설정

  pinMode(Dir6Pin_L, OUTPUT);             // 제어 6번핀 출력모드 설정
  pinMode(Dir7Pin_L, OUTPUT);             // 제어 7번핀 출력모드 설정
  pinMode(SpeedPin_L, OUTPUT);            
}

void loop(){

 /*
  if (bluetooth.available()){
    Serial.print("전송");
    Serial.write(bluetooth.read());
  }
   if(Serial.available()){
    Serial.print("전송2");
    bluetooth.write(Serial.read());
  }
  
  */

  
  long distance = detect_distance(TRIG,ECHO);;
  long L_dist = detect_distance(L_TRIG,L_ECHO);
  long R_dist = detect_distance(R_TRIG,R_ECHO);
  
  Serial.print("\nDIstance : ");
  Serial.print(distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm\n");

  // 10 초 됐을 때 원래 자리로 복귀
  // 고려해야할게 많으나 현재는 단순히 원래 자리 위치로만 복귀 가정

  
  //obstacle(distance,L_dist,R_dist); // 장애물 회피
 
  if(millis()>15000){
    
    if(start_return == false){
      rotate_to_return(x,y); 
  
      comeback_home = sqrt(pow(x-0,2) + pow(y-0,2));
  
      Serial.print("0,0 까지 직선 거리 : ");
      Serial.print(comeback_home);
      Serial.print("\n");
      start_return = true;
    }
    
    go_straight(100);
    comeback_home--;
    Serial.print(comeback_home);
    
    if(comeback_home < 0 ){
      Serial.print("종료\n");
      exit(0);
    }
  }else if(millis() > 7000){  // 그냥 배터리 부착 시간 + 해서 7초 뒤 출발 가정

    
    obstacle(distance,L_dist,R_dist); // 장애물 회피
    // 앞으로 --------    
    while(speed < 100){
      speed += 20;
      go_straight(speed);
      delay(100);
    }
    
    go_straight(speed); 

    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print("\n");
  }
  



 
  
 
  //Serial.print(millis());
  //Serial.print("\n");
  

  
  
  delay(100); //0.1초마다 측정값을 보여줍니다.
}