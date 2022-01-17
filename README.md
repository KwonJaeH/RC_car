# RC_car
 Make auto


초음파 센서를 이용한 장애물 회피 기반, 
두가지 기능 구현 예정.

[1] 시작하고 일정 시간 지난 뒤, 다시 원래 시작 점으로 return.  
[2] 속도가 빠를 때, 갑자기 튀어나온 장애물에 대한 정지가 아닌 회피.

----------------------------------------

### [1] 시작하고 일정 시간 지난 뒤, 다시 원래 시작 점으로 return.
  
  > **가정**      
  >        1) 차는 좌,우 회전 시 '직각'으로만 회전한다.    
  >        2) 돌아오는 길에는 장애물이 없다.  
  >        3) "후진"은 하지않는다.    
  >        4) return 전 도착 지점이 x 또는 y가 0 인 경우는 고려하지 않는다.   
  >
  > * 2), 3), 4) 가정의 경우, 추가할 수 있으나 현재 구성을 단순히 작동하기 위함.   
  >
  > 시작점을 (0,0)으로 두고 1 ~ 4사분면에서 x, y 의 증감 상태를 기반으로 차의 진행 방향 파악.   
  > 차의 진행 방향 및 위치 파악 후, 시작점에 대한 회전 각도 계산.   
  > 회전 후, (0,0) 방향으로 직진 및 도착 시 정지.   
  >
  > **graph example**       
  > <img src="https://user-images.githubusercontent.com/81853056/148670117-3f5e3f85-546f-4467-bb3a-b61e19a60013.png" width="50%" height="50%" title="graph" alt="graph_ex"></img></br>
  > 빨간 곡선만큼 회전 후, 초록색 선을 따라 (0,0) 향해 직진.   
       
       

### [2] 속도가 빠를 때, 갑자기 튀어나온 장애물에 대한 정지가 아닌 회피.
  > **가정**    
  >        1) 갑자기 튀어나온 장애물은 폭이 작다. (ex. 현재 장애물 - 손)   
  >        
  > 속도가 최대이고, 장애물과의 거리가 가까울 때(20cm로 설정)    
  > 좌, 우 공간을 측정해 더 넓은 곳으로 이동.   
  >  
  > **example**   
  > <img src="https://user-images.githubusercontent.com/81853056/148670412-15df2f82-7420-419d-b1fc-146e264be4c5.png" width="30%" height="30%" title="move" alt="move_ex"></img></br>
  > 앞에 장애물이 갑자기 나왔을 때,   
  > 좌, 우 공간 확인 후, 더 넓은 공간으로 (example에서는 왼쪽) 좌회전 - 직진 - 우회전으로 장애물 회피 

