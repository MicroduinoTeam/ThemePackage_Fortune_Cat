/*
  =================================================================================
                             | 什么是招财猫? |
  ---------------------------------------------------------------------------------

  按下碰撞开关，彩灯呼吸招财猫眨眼睛，招财猫摆手3次后叫一次，任意时候再按下碰撞开关，关闭。

  =================================================================================
                                   |开始|
  ---------------------------------------------------------------------------------
  利用碰撞开关控制招财猫！
  =================================================================================
                           |自己动手修改代码! |
  ---------------------------------------------------------------------------------
  BRIGHT_MAX: LED彩灯显示的最大亮度。

  修改完这些值后，记得再把代码上传到核心才能有反应。
  =================================================================================
                              | 头脑风暴|
  ---------------------------------------------------------------------------------
  1. 除了碰撞开关，还能用什么来控制招财猫？
     红外接收传感器? 光照传感器? 声音传感器?
  2. 招财猫的眼睛还能有啥别的效果？
     闪烁? 炫彩? 单色?
  3. 招财猫的手臂能摆的快一点吗？

    注意: 以上功能上的变动会涉及到程序上的改动。
  =================================================================================
                               | 端口连接|
  ---------------------------------------------------------------------------------
  碰撞开关: D6
  彩灯：    D4
  舵机:     D11
  蜂鸣器：  D8
  =================================================================================
                                  | 问题 |
  ---------------------------------------------------------------------------------
  欢迎访问 http://forum.microduino.cn/，在我们的论坛提交您的问题， 我们会尽快回复您。
*/

#include <Microduino_Key.h>
#include <Microduino_ColorLED.h> //引用彩灯库
#include <Servo.h>

#define PIN_BUZZER  8      //蜂鸣器引脚.
#define PIN_KEY     6      //碰撞开关引脚. 
#define PIN_SERVO   10     //舵机

#define PIN_LED     4      //彩灯引脚.
#define LED_NUM     2      //彩灯的数量 

#define BRIGHT_MAX  255     //LED彩灯显示的最大亮度。最大亮度是255，最小亮度是0


ColorLED strip = ColorLED(LED_NUM, PIN_LED); //将ColorLED类命名为strip，并定义彩灯数量和彩灯引脚号
Servo myservo;
DigitalKey keyCoin(PIN_KEY);

bool isUP = true;
bool buttonState;
bool State = 0;
int pos = 30;
char flag = 0;
int Bright;
float sinVal;

void setup() {
  Serial.begin(9600);
  pinMode( PIN_BUZZER, OUTPUT);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyCoin.begin(INPUT);
}

void loop() {
  if (keyCoin.readEvent() == SHORT_PRESS) //检查碰撞开关按下。
  {
    State = !State;
    if (State)
      myservo.attach(PIN_SERVO); //初始化舵机
  }

  if (State)
  {
    strip.setAllLED(COLOR_PURPLE);
    strip.show();
    //舵机角度控制
    if (pos == 30)
    {
      isUP = true;
      flag++;
    }
    else if (pos == 130)
      isUP = false;

    if (isUP)
      pos++;
    else
      pos--;
    myservo.write(pos);

    if (flag == 3)
    {
      flag = 1;
      for (int x = 70; x < 280; x++) {
        //转化角度到弧度
        sinVal = 100 * ((sin(x * (3.1412 / 180))) + 1);
        sinVal = map(sinVal, 100, 0, 1350, 1200);
        tone(PIN_BUZZER, sinVal);
        delay(2);
      }
      noTone(PIN_BUZZER);
    }
    delay(8);
  }
  else  //关闭招财猫
  {
    strip.setAllLED(COLOR_NONE);
    strip.show();
    myservo.write(30);
    myservo.detach(); //关舵机
  }
  delay(15);
}

