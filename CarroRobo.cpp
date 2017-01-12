/*******************************************************************
*       Biblioteca Inicial Para Programação do CarroRobo 2 Rodas   *
*                                                                  *      
*                                                                  *
*       Nathanael Nardoto Batista - Ifes Campus Cariacica          *
*       Data: 11-01-2017                                           *  
*       Versao: 0.1                                                *
*                                                                  *
*******************************************************************/

#include "Arduino.h"
#include "CarroRobo.h"


CarroRobo::CarroRobo(char IN1, char IN2, char IN3, char IN4)
{
  // Define Pinos de Comando
  CarroRobo::IN1 = IN1;   
  CarroRobo::IN2 = IN2;   
  CarroRobo::IN3 = IN3;   
  CarroRobo::IN4 = IN4; 

  // Configura Pinhos Como Saida
  pinMode(CarroRobo::IN1, OUTPUT);
  pinMode(CarroRobo::IN2, OUTPUT);
  pinMode(CarroRobo::IN3, OUTPUT);
  pinMode(CarroRobo::IN4, OUTPUT);

  CarroRobo::Velocidade = 40.0;
}

void CarroRobo::Andar(bool sentido, float distancia)
{
    float v = map(CarroRobo::Velocidade, 0, 100, 0, 255);
    
    // anda para frente
    if(sentido)
    {
        analogWrite(CarroRobo::IN1, v);
        analogWrite(CarroRobo::IN2, 0);
         
        analogWrite(CarroRobo::IN3, v);
        analogWrite(CarroRobo::IN4, 0);   
    }

    // anda para tras
    else
    {
        analogWrite(CarroRobo::IN1, 0);
        analogWrite(CarroRobo::IN2, v);
         
        analogWrite(CarroRobo::IN3, 0);
        analogWrite(CarroRobo::IN4, v);   
    }

    delay(int(distancia));

    digitalWrite(CarroRobo::IN1, HIGH);
    digitalWrite(CarroRobo::IN2, HIGH);
     
    digitalWrite(CarroRobo::IN3, HIGH);
    digitalWrite(CarroRobo::IN4, HIGH);
    
}

void CarroRobo::Virar(bool direcao, float angulo)
{
    float v = map(CarroRobo::Velocidade, 0, 100, 0, 255);
    if(direcao)
    {
        analogWrite(CarroRobo::IN1, v);
        analogWrite(CarroRobo::IN2, 0);
         
        digitalWrite(CarroRobo::IN3, HIGH);
        digitalWrite(CarroRobo::IN4, HIGH);
    }
    else
    {
        digitalWrite(CarroRobo::IN1, HIGH);
        digitalWrite(CarroRobo::IN2, HIGH);
         
        analogWrite(CarroRobo::IN3, 0);
        analogWrite(CarroRobo::IN4, v);   
    }

    delay(int(angulo));

    digitalWrite(CarroRobo::IN1, HIGH);
    digitalWrite(CarroRobo::IN2, HIGH);
     
    digitalWrite(CarroRobo::IN3, HIGH);
    digitalWrite(CarroRobo::IN4, HIGH);

}
