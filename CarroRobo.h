/*******************************************************************
*       Biblioteca Inicial Para Programação do CarroRobo 2 Rodas   *
*                                                                  *      
*                                                                  *
*                                                                  *
*       Data: 11-01-2017                                           *  
*       Versao: 0.1                                                *
*                                                                  *
*******************************************************************/



#ifndef CarroRobo_h
#define CarroRobo_h

#include "Arduino.h"

// Necessário Para Usar o Timer2 (Desabilita a Lib Tone)
#include <MsTimer2.h>


// Macros para Facilitar Utilização da Lib
#define FRENTE 0
#define RE 1

#define ESQUERDA 0
#define DIREITA 1

#define HABILITADO 1
#define DESABILITADO 0


// Macros dos Modos de Velocidade
#define CMS 0
#define PPI 1


// Cria Estrutura Básica de Uma Roda
struct Roda
{
      double Pulsos = 0;
      double UltimaMedida = 0;
      long long MedidaAcumulada = 0;
      float Potencia = 0;
};


class CarroRobo
{
      public:
      
            // Construtores da Classe
            CarroRobo(char IN1, char IN2, char IN3, char IN4); 
            CarroRobo(char IN1, char IN2, char IN3, char IN4, char RodaEsquerda, char RodaDireita); 
            
            // Movimentação em Linha Reta
            void Andar(bool sentido, float distancia);
            void Andar(bool sentido, float distancia, float velocidade);

            // Rotação em Torno do Centro do Eixo
            void Virar(bool direcao, float angulo);
            void Virar(bool direcao, float angulo, float velocidade);
                
            // Alteções de SetPoint
            void MudarVelocidade(float velocidade);
            
            // Retorno da Velocidade em cm/s
            static float MedirVelocidade(char modo, char roda);
            static long long PulsosAcumulados(char roda);

                       
          
      private:
            // Variaveis para Acionamento das Rodas
            char IN1, IN2, IN3, IN4;  
            
            // SetPoint para Velocidade
            float Velocidade;
            
            // Intervalo de Medição de Velocidade em Milissegundos (ms)
            static unsigned int IntervaloDeMedida;
            
            // Rodas do CarroRobo
            static Roda RodaE, RodaD;
            
            // Funções Internas de Controle e Medida
            static void MedirPulsos();
            
            // Incrementa a cada Interrupção do Encoder
            static void RodaD_interrupt();
            static void RodaE_interrupt();
            
            
};
#endif
