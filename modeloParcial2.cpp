#include <Servo.h>

#define LEDV 13
#define LEDR 12
#define BOTONDOWN 11
#define MOTOR 6
#define POT A0
#define SENSORLUZ A1
#define SENSORDIST 8
#define SERVO 3

int botondown;
int ultimoEstadoBoton;
int i;
int potenciometro;
int intensidadRetornada;
int luz;
unsigned long pulso;
float distancia;
Servo miServo;
int flag;

//FUNCIONES
int botonAntirebote();
void funcBoton(int(*pFunc)());
void regularMotor();
void funcMotor(int ledV, void(*pFuncion)(),float dist);
float initSensorDist();
void servo(float dist);

  
void setup()
{
  pinMode(LEDV, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(BOTONDOWN, INPUT);
  miServo.attach(SERVO,250,2750);
  
  botondown=0;
  ultimoEstadoBoton=0;
  i=0;
  flag=0;
 
  Serial.begin(9600);
}

void loop()
{
  funcBoton(botonAntirebote);
  distancia = initSensorDist();
  funcMotor(LEDV,regularMotor,distancia);  
  servo(distancia);
}




int botonAntirebote()
{ 
  botondown = digitalRead(BOTONDOWN);
  
  if(botondown != ultimoEstadoBoton && botondown == HIGH)
  {
    i++;
  }
  ultimoEstadoBoton = botondown;
  
  return i;
}

void funcBoton(int(*pFunc)())
{	
  i = pFunc();
  
  switch(i)
  {
    case 0:
      digitalWrite(LEDV, HIGH);
      digitalWrite(LEDR, LOW);
      break;
    case 1:
      digitalWrite(LEDV, LOW);
      digitalWrite(LEDR, HIGH);
      break;
    case 2:
      i=0;
      break;
  }
}

void regularMotor()
{
  potenciometro = analogRead(POT);
  intensidadRetornada = map(potenciometro,0,1024,0,255);
  analogWrite(MOTOR,intensidadRetornada); 
}

void funcMotor(int ledV, void(*pFuncionPot)(),float dist)
{
  luz = analogRead(SENSORLUZ);
  ledV = digitalRead(ledV);
    
  if(ledV == HIGH && luz <200 && dist > 100)
  {
    if(flag == 0)
    {
      pFuncionPot();
      delay(5000);
      analogWrite(MOTOR,0);
      delay(100);
      flag = 1;
    }
  }
  else
  {
    analogWrite(MOTOR,0);
    flag = 0;
  }
}

float initSensorDist()
{
  pinMode(SENSORDIST,OUTPUT);
  digitalWrite(SENSORDIST,HIGH);
  delay(10);
  digitalWrite(SENSORDIST,LOW);
  pinMode(SENSORDIST,INPUT);
  pulso = pulseIn(SENSORDIST,HIGH);
    
  distancia = (float)(((pulso/1000) * 34.32)/2);
  
  return distancia;
}

void servo(float dist)
{
  miServo.write(0);
  if(dist <150)
  {
    miServo.write(45);
  }
}