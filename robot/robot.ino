const int lineLeftPin = 4;     // PINS FOR IR LINE FOLLOWER 4 & 7 
const int lineRightPin = 5;

const int trigPin = A0;        // PINS FOR ULTRASONIC A0 &A1
const int echoPin = A1;
const int leftFollower = A2;   // PINS FOR IR HUMAN FOLLOWER A2 & A3
const int rightFollower = A3;

int Motor1A = 5;
int Motor1B = 6;
int Motor2A = 7;
int Motor2B = 8;

 
float readDistance(){      // READ DISTANCE FROM ULTRASONIC
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  float time = pulseIn (echoPin, HIGH);
  return time / 29 / 2;
}

// MOTOR MOVEMENT 
void forward(){
  analogWrite(9,150);
  analogWrite(10,150);
  digitalWrite(Motor1A,HIGH);
  digitalWrite(Motor1B,LOW);
  digitalWrite(Motor2A,HIGH);
  digitalWrite(Motor2B,LOW);
}

void backward(){
  analogWrite(9,150);
  analogWrite(10,150);
  digitalWrite(Motor1A,LOW);
  digitalWrite(Motor1B,HIGH);
  digitalWrite(Motor2A,LOW);
  digitalWrite(Motor2B,HIGH);
}

void backwardRight(){
  analogWrite(9,150);
  analogWrite(10,150);
  digitalWrite(Motor1A,LOW);
  digitalWrite(Motor1B,LOW);
  digitalWrite(Motor2A,LOW);
  digitalWrite(Motor2B,HIGH);
}

void backwardLeft(){
  analogWrite(9,150);
  analogWrite(10,150);
  digitalWrite(Motor1A,LOW);
  digitalWrite(Motor1B,HIGH);
  digitalWrite(Motor2A,LOW);
  digitalWrite(Motor2B,LOW);
}


void turnLeft(){
  analogWrite(9,110);
  analogWrite(10,110);
  digitalWrite(Motor1A,LOW);
  digitalWrite(Motor1B,HIGH);
  digitalWrite(Motor2A,HIGH);
  digitalWrite(Motor2B,LOW);
}

void turnRight(){
  analogWrite(9,110);
  analogWrite(10,110);
  digitalWrite(Motor1A,HIGH);
  digitalWrite(Motor1B,LOW);
  digitalWrite(Motor2A,LOW);
  digitalWrite(Motor2B,HIGH);
}

void stop(){
  digitalWrite(Motor1A,LOW);
  digitalWrite(Motor1B,LOW);
  digitalWrite(Motor2A,LOW);
  digitalWrite(Motor2B,LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(lineLeftPin, INPUT);
  pinMode(lineRightPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(leftFollower, INPUT);
  pinMode(rightFollower, INPUT);
  pinMode(Motor1A,OUTPUT);
  pinMode(Motor1B,OUTPUT);
  pinMode(Motor2A,OUTPUT);
  pinMode(Motor2B,OUTPUT);
}

void loop() {
    //analogWrite(10,110);
    //analogWrite(11,110);
  //lineFollower();
  Serial.println(readDistance());
  //obstacleAvoid();
  //humanFollower();
}


void lineFollower(){
  int leftSide = digitalRead(lineLeftPin);
  int rightSide = digitalRead(lineRightPin);
  if(leftSide && rightSide)
    forward();
  else if(leftSide && !rightSide){
    turnRight();
    delay(200);
  }
  else if(!leftSide && rightSide){
    turnLeft();
    delay(200);
  }
  else
    stop();
}

void obstacleAvoid(){
  float distance = readDistance();
  delay(10);
  if (distance > 19) 
    forward();
  else{
    stop();
    delay(500);
    backward();
    delay(500);
    stop();
    delay(100);
    turnRight();
    delay(350);
  }
}

void humanFollower(){
  int Right_Value = digitalRead(rightFollower);
  int Left_Value  = digitalRead(leftFollower);
  float distance = readDistance();
  if(Right_Value && (distance>=10 && distance<=30)&& Left_Value)
    forward();
  else if(!Right_Value && Left_Value)
    turnRight();
  else if(Right_Value && !Left_Value)
    turnLeft();
  else if(Right_Value && Left_Value)
    stop();
  else if(distance > 5 && distance < 10)
    stop();
  else if(distance < 5)
    backward();
}
