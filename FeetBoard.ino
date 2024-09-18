//Código de teclado para los pies

#include "Joystick.h"           //Biblioteca que permite configurar
                                //la tarjeta como mando de juegos.

//Arreglo matricial de botones

int F = 2;                      //Número de filas del arreglo.
int C = 4;                      //Número de columnas del arreglo.

int filas[]={8,9};              //Pines correspondientes a las filas.
int columnas[]={6,7,16,10};     //Pines correspondientes a las columnas.

int ls_tecla[2][4]={{0,0,0,0},  //Arreglo que almacena el estado    
                    {0,0,0,0}}; //anterior de las teclas.
                    
int cs_tecla[2][4]={{0,0,0,0},  //Arreglo que almacena el estado
                    {0,0,0,0}}; //actual de las teclas.

//Botón de bloqueo del teclado
bool lock;                      //Estado del botón de bloqueo.

//Variables de conteo
int i,j,k;
                      
//Delay entre cada lectura de los estados de las teclas
int del=10;                     

//Configuración del mando de juegos
Joystick_ Joystick(0x04,        //Creación del mando de juegos.
  JOYSTICK_TYPE_JOYSTICK,       
  8, 0,                         //N. teclas, N. botones de sombrero.
  false, false, false,          //Ejes "X", "Y" y "Z".
  false, false, false,          //Rotación en "X", "Y" y "Z".
  false, false,                 //Timón, Palanca de aceleración.
  false, false, false);         //Acelerador, Freno, Volante.

void setup() {

  //Inicialización de todos los pines de las filas y columnas
  //como entradas.
  for (i=0;i<F;i++){
    pinMode(filas[i],INPUT);
  }
  for (i=0;i<C;i++){
    pinMode(columnas[i],INPUT);
  }

  //Inicialización del pin del botón de bloqueo como entrada.
  pinMode(5,INPUT);

  //Inicialización del mando de juegos.
  Joystick.begin();
}

//Función de la lectura del estado de cada una de las teclas.
void leerMatriz(){
  //Ciclo para leer fila por fila.
  for (i=0;i<F;i++){
    pinMode(filas[i],OUTPUT);
    digitalWrite(filas[i],LOW);
    //Ciclo para leer columna por columna.
    for (j=0;j<C;j++){
      pinMode(columnas[j],INPUT_PULLUP);
      //Almacenamiento del estado leído.
      cs_tecla[i][j]=!digitalRead(columnas[j]);
      pinMode(columnas[j],INPUT);
    }
    pinMode(filas[i],INPUT);
  }
}

void loop() {

  //Lectura del estado del botón de bloqueo.
  lock=digitalRead(5);

  //Si el botón está en reposo, se permite la
  //lectura del estado de las teclas.
  if(lock==0){
    leerMatriz();
    
    k=-1;
    //Una vez leídos los estados de las 8 teclas, se asignan dichos
    //estados a los botones del mando de juegos para comunicar las 
    //instrucciones de pulsación hacia la computadora.
    for (i=0;i<F;i++){
      for (j=0;j<C;j++){
        k++;
        //Solo si existe un cambio de estado es que se realiza
        //esta actualización en la tecla correspondiente.
        if(cs_tecla[i][j]!=ls_tecla[i][j]){
          Joystick.setButton(k,cs_tecla[i][j]);
          //Finalmente se actualiza el estado previo de la tecla por
          //el estado actual, para seguir realizando la comparativa
          //en la siguiente lectura.
          ls_tecla[i][j]=cs_tecla[i][j];       
        }
      }
    }
    delay(del);
  }
  
}
