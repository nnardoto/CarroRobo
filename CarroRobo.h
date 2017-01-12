/*******************************************************************
*       Biblioteca Inicial Para Programação do CarroRobo 2 Rodas   *
*                                                                  *      
*                                                                  *
*       Nathanael Nardoto Batista - Ifes Campus Cariacica          *
*       Data: 11-01-2017                                           *  
*       Versao: 0.1                                                *
*                                                                  *
*******************************************************************/



#ifndef CarroRobo_h
#define CarroRobo_h

#include "Arduino.h"


#define FRENTE 0
#define RE 1

#define ESQUERDA 0
#define DIREITA 1

class CarroRobo
{

  public:
    void Andar(bool sentido, float distancia);
    void Andar(bool sentido, float distancia, float velocidade);

    void Virar(bool direcao, float angulo);
    void Virar(bool direcao, float angulo, float velocidade);
    
    void MudarVelocidade(float velocidade);
    
    
    CarroRobo(char IN1, char IN2, char IN3, char IN4); 
    
  private:
    char IN1, IN2, IN3, IN4;  
    float Velocidade; 
};

#endif
