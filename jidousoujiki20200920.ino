//センサー2個とモーター2個の制御を行う
//2020.08.15 beftech
//ARDUINO UNO
//モータードライバー:BA6956AN
//光変換型フォトIC:S6809
//
//
//-----------------------------------------------------------------------------------------------------------
//スケッチ内で使用する定数・変数の宣言
const int PILOT_LED=2;           //2PINを定数PILOT_LEDとし整数型と定義する
const int SEN_R_IN=7;            //7PINを定数SEN_R_INとし整数型と定義する
const int SEN_L_IN=8;            //8PINを定数SEN_L_INとし整数型と定義する
const int GUARD_R_IN=3;          //3PINを定数GUARD_R_INとし整数型と定義する
const int GUARD_L_IN=4;          //4PINを定数GUARD_L_INとし整数型と定義する

int val_R=0;                     //変数val_Rを整数型と定義し0を代入する
int val_L=0;                     //変数val_Lを整数型と定義し0を代入する
int guard_val_R=0;               //変数guard_val_Rを整数型と定義し0を代入する
int guard_val_L=0;               //変数guard_val_Lを整数型と定義し0を代入する

const int MT_R_FO=5;             //5PINをモーターの定数MT_R_FOとし整数型と定義する※forward=前進
const int MT_R_RE=6;             //6PINをモーターの定数MT_R_REとし整数型と定義する※reverse=後退
const int MT_L_FO=9;             //9PINをモーターの定数MT_L_FOとし整数型と定義する
const int MT_L_RE=10;            //10PINをモーターの定数MT_L_REとし整数型と定義する


//動作時間の設定
int stoptime=500;                //停止時間
int backtime=300;                //バックする時間
int turntimeR=300;               //右旋回する時間
int turntimeL=500;               //左旋回する時間


//速度の設定
float RLbalance=1.2;                  //左右の実際の車輪速度の誤差まっすぐ走らんのを補正　1で等速となり比率でR側の速度が変わる
                                      //※小数なので浮動小数点型とする

int gospeed=200;                       //車輪回転速度 0～255で指定
int turnspeed=200;                     //旋回速度 0～255で指定
int backspeed=200;                     //バックする速度 0～255で指定

int gospeedR=gospeed*RLbalance;        //前進 R車輪回転速度×補正値※代入する数値は小数だけどint型指定で小数点以下は自動切り捨てされる
int gospeedL=gospeed;                  //前進 L車輪回転速度×補正値
int turnspeedR=turnspeed*RLbalance;    //旋回 R車輪回転速度×補正値
int turnspeedL=turnspeed;              //旋回 L車輪回転速度×補正値
int backspeedR=backspeed*RLbalance;    //バック R車輪回転速度×補正値
int backspeedL=backspeed;              //バック L車輪回転速度×補正値



//-----------------------------------------------------------------------------------------------------------
void setup() {                  //セットアップを行う電源投入後一度だけ実行
  
  pinMode(SEN_R_IN,INPUT);      //SEN_R_IN（のPIN）はINPUTとする
  pinMode(SEN_L_IN,INPUT);      //SEN_L_IN（のPIN）はINPUTとする
  pinMode(GUARD_R_IN,INPUT);    //GUARD_R_IN（のPIN）はINPUTとする
  pinMode(GUARD_L_IN,INPUT);    //GUARD_L_IN（のPIN）はINPUTとする
  
  pinMode(MT_R_FO,OUTPUT);      //MT_R_FO（のPIN）はOUTPUTとする（モータードライバーへ信号を出す）
  pinMode(MT_R_RE,OUTPUT);      //MT_R_RE（のPIN）はOUTPUTとする（モータードライバーへ信号を出す）
  pinMode(MT_L_FO,OUTPUT);      //MT_L_FO（のPIN）はOUTPUTとする（モータードライバーへ信号を出す）
  pinMode(MT_L_RE,OUTPUT);      //MT_L_RE（のPIN）はOUTPUTとする（モータードライバーへ信号を出す）
  
  pinMode(PILOT_LED,OUTPUT);         //PILOTLED（のPIN）はOUTPUTとする（パイロットLED点灯用）
  Serial.begin(9600);                //シリアル通信のデータ転送レートを9600bpsとする（シリアルモニターを使用するという宣言）

  Start();                           //初期動作を行う
  Go();                              //前進開始
  delay(1000);                       //電源投入後1000ﾐり秒は前進する
}

//-----------------------------------------------------------------------------------------------------------
//動作を定義　モータードライバーのデータシートを参照

//初期動作
void Start(){
Go();
delay(500);
Back();
delay(500);
Right();
delay(500);
Left();
delay(1000);
Right();
delay(500);
Stop();
delay(500);


}

//前進
void Go(){                  
  analogWrite(MT_R_FO,gospeedR);    
  analogWrite(MT_R_RE,0);   
  analogWrite(MT_L_FO,gospeedL);    
  analogWrite(MT_L_RE,0);   
}
//ストップ　※ブレーキがかかる
void Stop(){
  analogWrite(MT_R_FO,255);    
  analogWrite(MT_R_RE,255);   
  analogWrite(MT_L_FO,255);    
  analogWrite(MT_L_RE,255);   
}
//バック
void Back(){
  analogWrite(MT_R_FO,0);    
  analogWrite(MT_R_RE,backspeedR);   
  analogWrite(MT_L_FO,0);    
  analogWrite(MT_L_RE,backspeedL);   
}
//右ターン
void Right(){
  analogWrite(MT_R_FO,0);    
  analogWrite(MT_R_RE,turnspeedR);   
  analogWrite(MT_L_FO,turnspeedL);    
  analogWrite(MT_L_RE,0);    
}
//左ターン
void Left(){
  analogWrite(MT_R_FO,turnspeedR);    
  analogWrite(MT_R_RE,0);   
  analogWrite(MT_L_FO,0);    
  analogWrite(MT_L_RE,turnspeedL);    
}




//-----------------------------------------------------------------------------------------------------------
void loop() {     //繰り返し実行するスケッチ本体

  digitalWrite(PILOT_LED,HIGH);             //デジタル出力でPILOT_LEDをHIGHにする※回路で使用していないので点きっぱなし
  val_R=digitalRead(SEN_R_IN);              //デジタルで読み取ったSEN_R_INの値をval_Rに代入する（右センサーの検知）
  val_L=digitalRead(SEN_L_IN);              //デジタルで読み取ったSEN_L_INの値をval_Lに代入する（左センサーの検知）
  guard_val_R=digitalRead(GUARD_R_IN);      //デジタルで読み取ったGUARD_R_INの値をguard_val_Rに代入する（右ガードの検知）
  guard_val_L=digitalRead(GUARD_L_IN);      //デジタルで読み取ったGUARD_L_INの値をguard_val_Lに代入する（左ガードの検知）
  
  Serial.print(val_L);                      //検証のためにval_Lの値をシリアルモニターに表示する
  Serial.print(guard_val_L);                //検証のためにguard_val_Lの値をシリアルモニターに表示する
  Serial.print(" : ");                      //:をシリアルモニターに表示する
  Serial.print(val_R);                      //検証のためにval_Rの値をシリアルモニターに表示する
  Serial.println(guard_val_R);                //検証のためにguard_val_Rの値をシリアルモニターに表示する

  Serial.print(gospeedR);                   //検証のためにgospeedRの値をシリアルモニターに表示する
  Serial.print(" : ");                      //:をシリアルモニターに表示する
  Serial.println(gospeedL);                 //検証のためにgospeedLの値をシリアルモニターに表示する


if(val_R==HIGH || guard_val_R==HIGH){       //val_Rの値とguard_val_Rの値のどちらかがHIGH（シリアルモニターではHIGHの場合は1でLOWの場合は0で表示される）
                                            //ならば下記を実行し、そうでなければ実行しない
                                            
  digitalWrite(PILOT_LED,LOW);              //デジタル出力でPILOT_LEDをLOWにする※回路で使用していないので点きっぱなし
  Stop();
  delay(stoptime);                          //停止時間
  Back();
  delay(backtime);                          //バックする時間  
  Left();
  delay(turntimeL);                         //旋回する時間
  Stop();
  delay(stoptime);                          //停止時間

}


if (val_L==HIGH || guard_val_L==HIGH){      //val_Lの値とguard_val_Lの値のどちらかがHIGH（シリアルモニターではHIGHの場合は1でLOWの場合は0で表示される）
                                            //ならば下記を実行し、そうでなければ実行しない
                                               
  digitalWrite(PILOT_LED,LOW);              //デジタル出力でPILOT_LEDをLOWにする※回路で使用していないので点きっぱなし
  Stop();
  delay(stoptime);                          //停止時間
  Back();
  delay(backtime);                          //バックする時間  
  Right();
  delay(turntimeR);                         //旋回する時間
  Stop();
  delay(stoptime);                          //停止時間

}
Go();
}
