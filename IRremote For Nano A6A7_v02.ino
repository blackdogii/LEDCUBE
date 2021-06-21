//CarMp3紅外線自製解碼 測試程式v0.2
//Nano A6 A7 可使用

int IRpin = A7;      // IR 接收器輸入腳位
int IRstate = 0;        // IR 接收器輸入位元
int IRstateBefore = 0;  // IR 接收器之前輸入位元
unsigned long dT=0;
unsigned long timeNow=0;
unsigned long timeBefore = 1;   // 紀錄上一次 IRstate 變化的時間
int DIGIT = 0;
unsigned long DECODE1 = 0;
unsigned long DECODE2 = 0;
boolean isWaitting = false;    // 是否IR訊號接收中
const int durationMax = 3000;   // 一段時間沒變化就進入等待IR訊號狀態，單位us
const int durationMin = 100;     // PWM的最小持續時間，單位us

#define PB_0        22
#define PB_1        12
#define PB_2        24
#define PB_3        94
#define PB_4        8
#define PB_5        28
#define PB_6        90
#define PB_7        66
#define PB_8        82
#define PB_9        74
#define PB_100      25
#define PB_200      13
#define PB_MINUS    7
#define PB_PLUS     21
#define PB_EQ       9
#define PB_BACKWARD 68
#define PB_FORWARD  64
#define PB_PLAY     67
#define PB_CH_MINUS 69
#define PB_CH       70
#define PB_CH_PLUS  71

/*
 * 按鈕:   1,2,3,4,5,6,7,8,9,0,100+,200+,－,＋,EQ,<<,>>,>,CH-,Ch+,CH++,Error
 * 回傳值: 1,2,3,4,5,6,7,8,9,0,10  ,11  ,12,13,14,15,16,17,18,19 ,20  ,99
 */
int IR_rec_Check(int pin){
  
  IRstate =  analogRead(pin) > 100 ? 0 : 1;
  if(IRstate){
     delayMicroseconds(500);
     IRstate =  analogRead(IRpin) > 100 ? 0 : 1;
     if(IRstate){                                        //按鈕按下，開始鎖定偵測
          DIGIT = 0;
          DECODE1 = 0;
          DECODE2 = 0;
        do{
          IRstate =  analogRead(pin) > 100 ? 0 : 1;    // 讀取腳位元狀態，紅外線接收訊號為"反向"
          if( IRstate < IRstateBefore ){                 // 1->0
            //記錄此次時間
            timeBefore= micros();                 // 開始計時
            isWaitting = true;
          }
          else if(IRstate > IRstateBefore && isWaitting){              // 0->1
              timeNow = micros();
              dT = timeNow-timeBefore;            // 抓低電位(space)的時間
//              Serial.println(dT);
              if(dT >= durationMax &&DECODE1==0){           // 訊號開始為9ms HIGH 4.5ms LOW
//                isWaitting = true;                            // 偵測到 超過3ms LOW 表示有訊號
                DIGIT = 0;
//                Serial.print("DE:");
              }
              else if( dT < durationMax && dT > durationMin ){// 8+8'bit Custom Code , 8+8'bit Data Code
                int I = dT > 1000 ? 1 : 0;                    // LOW訊號560us為0，1690us為1
                if(I > 0 && DIGIT >=16 && DIGIT <24){         // 從第16字元開始解碼。8bit
                  bitSet( DECODE1, (DIGIT-16));               
                  }
                else if(I > 0 && DIGIT >=24 && DIGIT <32){         // 從第24字元開始解碼。8'bit
                  bitSet( DECODE2, (DIGIT-24));               
                  }
                  DIGIT++;        
                }
             else{
                isWaitting = false;                           //解碼完成
//                Serial.print("DECODE1:");
//                Serial.println(DECODE1,2);
//                Serial.print("DECODE2:");
//                Serial.println(DECODE2,2);
              if(DECODE1+DECODE2==255){
      switch (DECODE1){
        case PB_1:        return 1;    break;
        case PB_2:        return 2;    break;
        case PB_3:        return 3;    break;
        case PB_4:        return 4;    break;
        case PB_5:        return 5;    break;
        case PB_6:        return 6;    break;
        case PB_7:        return 7;    break;
        case PB_8:        return 8;    break;
        case PB_9:        return 9;    break;
        case PB_0:        return 10;    break;
        case PB_100:      return 11;   break;
        case PB_200:      return 12;   break;
        case PB_MINUS:    return 13;   break;
        case PB_PLUS:     return 14;   break;
        case PB_EQ:       return 15;   break;
        case PB_BACKWARD: return 16;   break;
        case PB_FORWARD:  return 17;   break;
        case PB_PLAY:     return 18;   break;
        case PB_CH_MINUS: return 19;   break;
        case PB_CH:       return 20;   break;
        case PB_CH_PLUS:  return 21;   break;
      }
            }
                }
            }
        //記錄此次狀態
        IRstateBefore = IRstate;
        }while(isWaitting);
     }
  }
  return 0; // 未偵測到紅外線
}

void setup(){
  Serial.begin(9600);
  pinMode(IRpin,INPUT); // 設定針腳I/O模式
}

void loop(){
int num=IR_rec_Check(IRpin);
  if(num > 0){
    Serial.println(num);
  }  
//  delay(5);
}
