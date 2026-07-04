//#include <Servo.h>


const int trigPin = 4;
const int echoPin = 3;

const int greenLight = 13;
const int yellowLight = 12;
const int redLight = 11;

Servo servo;

float duration, distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(redLight, OUTPUT);
  Serial.begin(9600);
  servo.attach(9);
  servo.write(0);
}

void loop() {
  
  //This is the loop function that will run when there is power to the Arduino

  //These lines of code are sending out sound waves to detect how far an object is from the sensor
  digitalwrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;


  Serial.println(distance);


  if (distance > 80)
  {
    digitalWrite(greenLight,HIGH);
    digitalWrite(yellowLight,LOW);
    digitalWrite(redLight, LOW);
    Serial.println(1);
  }

  //This line translates to "If distance is less than 80 and distance is greater than 10 do this"
  else if (distance < 80 && distance >10)
  {
    digitalWrite(greenLight,LOW);
    digitalWrite(yellowLight,HIGH);
    digitalWrite(redLight, LOW);
    Serial.println(2);
  }

  else if(distance < 10)
  {
    servo.write(180);
    digitalWrite(greenLight,LOW);
    digitalWrite(yellowLight,LOW);
    digitalWrite(redLight, HIGH);
    Serial.println("3");
    delay(2000);
    servo.write(0);
  }

  //delay to give some time between executions
  //delay(500);
}