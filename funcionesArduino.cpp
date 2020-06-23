//ARDUINO//

//Algunas funciones y explicaciones//


/**Esta funcion hace titilar a un led que recibe como parametro
 * determina tambien por parametro que tiempo estara encendido o apagado
 */
void titilar (int led, int tiempo)
{
  digitalWrite (led, HIGH);
  delay (tiempo);
  digitalWrite (led, LOW);
  delay (tiempo);
}

/**Esta funcion lee el pin del boton, 
 * asegura un cambio de estado con el antirebote
 * en caso de ser presionado contara la cantidad de veces que se presiona el boton
 * determina que acciones realizar segun la cantidad de veces que fue presionado 
 */
void botonDown(int ledR, int ledV)
{ 
botondown = digitalRead(BOTONDOWN);
  
  if(botondown != ultimoEstadoBoton)// && botondown == HIGH)
  {
    if(botondown == HIGH)
    {
      i++;
      //Serial.println("prendido");
    }
    else
    {
      //Serial.println("apagado");
    }
    ultimoEstadoBoton = botondown;
  }
  //ultimoEstadoBoton = botondown;
 
  switch(i)
  {
    case 1:
      digitalWrite(ledR, HIGH);  
      digitalWrite(ledV, LOW);
      break;
    case 2:
      digitalWrite(ledR, LOW);
      digitalWrite(ledV, HIGH);
      i=0;
      break;
  }  
}
////SERVO////

/**
 * Se incluye la biblioteca: #include <Servo.h>
 * se define en que pin se conectara: #define SERVOPIN _
 * se declara globalmente un variable de tipo  Servo
 ** Servo miServo; // (Servo es el tipo, miServo es el nombre de la variable)
 * se inicializa en la funcion setup(): miServo.attach(SERVOPIN, 250, 2750);//estos valores pueden variar
 * finalmente el la funcion loop(): miServo.write(grados); //por parametro ira la cant de grados que quiero que gire
 */
#include <Servo.h>
#define SERVOPIN 7
Servo miServo;
 
 void setup()
 {
     miServo.attach(SERVOPIN);
 }

 void loop()
 {
     miServo.write(90); //90 son los grados
 }

///POTENCIOMETRO///
/**
 * Son Analogos. No hace falta inicializarlos en setup.
 * analogWrite(0 a 255) - Se pasa el PIN y el estado HIGH o LOW
 * analogRead(0 a 1024) - Se pasa el PIN
 * Declaro una variable para poder trabajar con la lectura.
 * Es conveniente mapear para regular // funcion map(PIN,fromLow, fromHIGH, toLow, toHigh)
 * fromLow sera el valor minimo de donde viene y toLow el minimo de donde va
 * fromHigh sera el valor maximo de donde viene y toHigh el maximo de donde va
 * seria: map(PIN,0,1024,0,255) // sera primero leido con analogRead, luego interpretado por analogWrite.
 */
//Ejemplo con un motor:

#define MOTOR 5
#define POT A0

int pot;

//recordar que no hace falta inicializar en setup

void loop()
{
    pot = analogRead(POT); //lee de 0 a 1024
    pot = map(pot,0,1023,0,255); //mapea
    analogWrite(MOTOR,pot); //establece el estado de HIGH o LOW a traves de la variable pot

}

///CONTROL REMOTO///
/**
 * Se incluye la biblioteca: #include <IRremote.h>
 * se define en que pin ira el receptor: #define IR _
 * se declara globalmente un variable de tipo  IRrecv
 ** IRrecv receptor(IR); // (IRrecv es el tipo, receptor es el nombre de la variable y el pin va en parentesis(IR))
 ** decode_results resultado; //y aca declaro una variable de tipo decode_results
 * se inicializa el receptor en la funcion setup(): receptor.enableIRIn();
 * finalmente el la funcion loop() imprimo el valor expresado en hexadecimal con la siguientes sentencias:
 * 
  if (receptor.decode(&resultado)) 
  {
    Serial.println(resultado.value, HEX);
    receptor.resume(); 
  }
  * Para poder trabajar con los valores de cada boton, establezco defines con los valores impresos
  * Puedo hacer un switch con la variable resultado.value (switch(resultado.value){})
  */

 #include <IRremote.h>
 #define IR 3
 IRrecv receptor(IR);
 decode_results resultado;

 //el define de los botones se hace como: BOTON1 0x(aca va lo que imprime)
 
 void setup()
 {
     receptor.enableIRIn();
     Serial.begin(9600);
 }

 void loop()
 {
    if (receptor.decode(&resultado)) 
    {
    Serial.println(resultado.value, HEX);
    receptor.resume(); 
    }

    switch(resultado.value)
    {
        case BOTON1:
        //tal cosa;
        break;
        .
        .
        .
    }
 }

////SENSORES///
/**
 * Los sensores de luz, temperatura o gas son analogos. Los sensores de distancia, o de movimiento(PIR) son digitales.
 * para inicializar un sensor analogo simplemente es determinar con una sentencia lo siguiente:
 * asignarle a una variable, la lectura del pin
 * Ejemplo:
 * float lecturaTempe;
 * lecturaTempe = analogRead(PINANALOGO);
 * //FIN EJEMPLO
 * Luego trabajo con esa variable 
 * En el caso de sensores digitales, hay q inicializar en setup(el de distancia usado en este caso no)
 * Se inicializa como INPUT en la funcion setup()// pinMode(PINDIGITAL, INPUT) 
 * luego en la funcion loop se utiliza digitalRead para leer al sensor
 * /

///Funciones para los distintos sensores///

/**Esta funcion lee el pin del sensor, 
 * convierte la lectura en celsius. 
 * Retorna la temperatura en celsius
 */
float SensorTemp()
{
  lecturaSensorTemp = analogRead(TMP);  
  // Voltaje entre 0 y 5V
  voltaje = lecturaSensorTemp * 5.0 / 1024.0;
  // Temperatura en ° Celsius
  temperatura = (voltaje - 0.5) * 100; 
  //Serial.println(temperatura);
  
  return temperatura;
}

/**Esta funcion inicia como OUTPUT e INPUT al sensor, 
 * mide un pulso entre cada poder determinar la distancia 
 * Retorna la distancia obtenida en cm.
 */
float SensorDist()
{
  //trigger
  pinMode(SENSORDIST,OUTPUT);
  digitalWrite(SENSORDIST,HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSORDIST, LOW);
  
  //medir pulso
  pinMode(SENSORDIST, INPUT);
  pulso = pulseIn(SENSORDIST,HIGH);
  distancia = (float)(((pulso/1000) * 34.32)/2);
    
  return distancia;
}

/**Esta funcion lee el pin del sensor, 
 * convierte la lectura en lux. 
 * Retorna la luminancia medida en lux
 */
int SensorLuz()
{
  int R = 10;//medido en kohms
  lecturaSensorLuz = analogRead(SENSORLUZ);
  lux = ((long)lecturaSensorLuz*1000*10)/((long)15*R*(1024-lecturaSensorLuz));
  Serial.println(lux);

  return lux;
}
// OTRA OPCION mas sencilla y sin medirlo en lux:
int SensorLuz()
{
  lecturaLuz = analogRead(SENSORLUZ);
 
  return lecturaLuz;
}

/** Esta funcion lee el pin del sensor, 
 * mapea la lectura del gas
 * Retorna la medida de gas en cuestion de cercania o lejania (mas cerca mas alto el valor)
 */
int SensorGas()
{
  gas = analogRead(PINGAS);
  gas = map(gas,0,1023,0,255);
  
  return gas;
}

/** Esta funcion lee el pin del sensor, 
 * determina si hay movimiento,
 * de haberlo, enciende un led que recibe por parametro
 */

void SensorAccionPorMov(int led)
{
  hayMovimiento = digitalRead(PIR);
    
  if(hayMovimiento == HIGH)
  {
    digitalWrite(led, HIGH);
    delay(500);
  }
  else
  {
    digitalWrite(led,LOW);
    delay(500);
  }
}

int SensorInclinacion()
{
  lecturaTilt = digitalRead(TILTPIN);
 
  return lecturaTilt; //devuelve un estado de HIGH o LOW
}



//////////////////ALGUNAS FUNCIONES USANDO PUNTEROS A FUNCION /////////////////////////////////

//El llamado a la funcion se hace asi: hacerTitilar(LED1,LED2,LED3,titilar);

void titilar(int led, int tiempo)
{
  digitalWrite(led, HIGH);
  delay(tiempo);
  digitalWrite(led, LOW);
  delay(tiempo);
}

void hacerTitilar(int led1, int led2, int led3,void (*pFuncion)(int,int))
{
  pFuncion(led1, 500);
  pFuncion(led2, 500);
  pFuncion(led3, 500);
  
}

/////boton antirebote////

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
  //Serial.println(i);
  
  switch(i)
  {
    case 1:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      break;
    case 2:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      i=0;
    break;
  }
}

