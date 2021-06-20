//CarMp3紅外線自製解碼程式
//Nano A6 A7 可使用

#define PB_CH_MINUS 706773130
#define PB_CH       705200290
#define PB_CH_PLUS  704676010
#define PB_BACKWARD 707297410
#define PB_FORWARD  715685890
#define PB_PLAY     713064490
#define PB_MINUS    2852126890
#define PB_PLUS     2720008330
#define PB_EQ       2829058570
#define PB_0        2718435490
#define PB_100      2694842890
#define PB_200      2820670090
#define PB_1        2821194370
#define PB_2        2695367170
#define PB_3        537430690
#define PB_4        2829582850
#define PB_5        2686978690
#define PB_6        545819170
#define PB_7        713588770
#define PB_8        579373090
#define PB_9        680034850

int IRpin = A7;      // IR 接收器輸入腳位
int IRstate = LOW;        // IR 接收器輸入位元
int IRstateBefore = LOW;  // IR 接收器之前輸入位元
unsigned long timeBefore = 0;   // 紀錄上一次 IRstate 變化的時間
int DIGIT = 0;
unsigned long DECODE = 0;

//boolean isWait = true;    // 是否在等待IR訊號模式
const int durationMax = 20000;   // 一段時間沒變化就進入等待IR訊號狀態，單位us
const int durationMin = 40;     // PWM的最小持續時間，單位us

void IR_rec_Check(){
  IRstate =  analogRead(IRpin) > 100 ? 0 : 1;    // 讀取腳位元狀態，紅外線接收訊號為反向
  if( IRstate != IRstateBefore ){                // 這次跟上次腳位元狀態不同
    long int timeNow = micros();                // 取得目前時間
    long int dT = timeNow-timeBefore;            // 上一次腳位元狀態變化經過的時間
    if(dT >= durationMax ){           // 位元狀態不變的時間
      DIGIT = 0;    
      switch (DECODE){
        case PB_CH_MINUS: Serial.print( "CH-" );  break;
        case PB_CH:       Serial.print( "CH" );   break;
        case PB_CH_PLUS:  Serial.print( "CH+" );  break;
        case PB_BACKWARD: Serial.print( "<<" );   break;
        case PB_FORWARD:  Serial.print( ">>" );   break;
        case PB_PLAY:     Serial.print( ">" );    break;
        case PB_MINUS:    Serial.print( "－" );   break;
        case PB_PLUS:     Serial.print( "＋" );   break;
        case PB_EQ:       Serial.print( "EQ" );   break;
        case PB_0:        Serial.print( "0" );    break;
        case PB_100:      Serial.print( "100+" ); break;
        case PB_200:      Serial.print( "200+" ); break;
        case PB_1:        Serial.print( "1" );    break;
        case PB_2:        Serial.print( "2" );    break;
        case PB_3:        Serial.print( "3" );    break;
        case PB_4:        Serial.print( "4" );    break;
        case PB_5:        Serial.print( "5" );    break;
        case PB_6:        Serial.print( "6" );    break;
        case PB_7:        Serial.print( "7" );    break;
        case PB_8:        Serial.print( "8" );    break;
        case PB_9:        Serial.print( "9" );    break;
        default:          Serial.print( "FF" );
      }
      Serial.print("。DECODE:");
      Serial.println(DECODE);
      DECODE = 0;
    }
    else if( dT < durationMax && dT > durationMin ){ // 位元狀態不變的時間<等待時間 and 位元狀態不變的時間>取樣時間
      int I = dT > 1000 ? 1 : 0;
      if(I > 0 && DIGIT >=32 && DIGIT <64){                   // 從第32字元開始解碼
        bitSet( DECODE, (DIGIT-32));               
        }
        DIGIT++;
    }
//記錄此次時間
  timeBefore = timeNow;
  }
//記錄此次狀態
IRstateBefore = IRstate;
}

void setup(){
  Serial.begin(9600);
  pinMode(IRpin,INPUT); // 設定針腳I/O模式
}

void loop(){
  IR_rec_Check();
}
