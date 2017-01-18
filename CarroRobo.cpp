/*******************************************************************
*       Biblioteca Inicial Para Programação do CarroRobo 2 Rodas   *
*                                                                  *
*******************************************************************/

#include "Arduino.h"
#include "CarroRobo.h"


// Inicializa Membros Estáticos
Roda CarroRobo::RodaE;
Roda CarroRobo::RodaD;
unsigned int CarroRobo::IntervaloDeMedida;

PID CarroRobo::PID_E(NULL, NULL, NULL, 0, 0, 0, 0);
PID CarroRobo::PID_D(NULL, NULL, NULL, 0, 0, 0, 0);


// Construtor Padrão sem Controle de Velocidade
CarroRobo::CarroRobo(char IN1, char IN2, char IN3, char IN4)
{
    // Define Pinos de Comando para a Roda DIREITA
    CarroRobo::IN1 = IN1;   
    CarroRobo::IN2 = IN2;   

    // Define Pinos de Comando para a Roda DIREITA
    CarroRobo::IN3 = IN3;   
    CarroRobo::IN4 = IN4; 

    // Configura Pinos Como Saida
    pinMode(CarroRobo::IN1, OUTPUT);
    pinMode(CarroRobo::IN2, OUTPUT);
    pinMode(CarroRobo::IN3, OUTPUT);
    pinMode(CarroRobo::IN4, OUTPUT);


    // Velocidade Padrão
    CarroRobo::Velocidade = 50.0; 
}

// Construtor Padrão com Controle de Velocidade
CarroRobo::CarroRobo(char IN1, char IN2, char IN3, char IN4, char RodaEsquerda, char RodaDireita)
{
    // Define Pinos de Comando para a Roda DIREITA
    CarroRobo::IN1 = IN1;   
    CarroRobo::IN2 = IN2;   

    // Define Pinos de Comando para a Roda DIREITA
    CarroRobo::IN3 = IN3;   
    CarroRobo::IN4 = IN4; 

    // Configura Pinos Como Saida
    pinMode(CarroRobo::IN1, OUTPUT);
    pinMode(CarroRobo::IN2, OUTPUT);
    pinMode(CarroRobo::IN3, OUTPUT);
    pinMode(CarroRobo::IN4, OUTPUT);


    // Velocidade Padrão
    CarroRobo::Velocidade = 25.0; 
    CarroRobo::IntervaloDeMedida = 50;
      
    // Ativa Funções para Medida de Velocidade
    // Subtrai-se 2 para adequar o uso da biblioteca (Apenas Pino 2 e 3 Podem Ser Usados)
    attachInterrupt(RodaEsquerda - 2, CarroRobo::RodaE_interrupt, RISING);
    attachInterrupt(RodaDireita - 2, CarroRobo::RodaD_interrupt, RISING);
      
      
    // Habilita Interrupção de Tempo Para Medir Velocidade - Lib. MsTimer2
    MsTimer2::set(IntervaloDeMedida, MedirPulsos);
    MsTimer2::start();


    // Inicializa e Configura PID
    PID_E.Begin(&RodaE.UltimaMedida, &RodaE.Potencia, &RodaE.SetPoint, KP, KI, KD, DIRECT);
    PID_D.Begin(&RodaD.UltimaMedida, &RodaD.Potencia, &RodaD.SetPoint, KP, KI, KD, DIRECT);

    PID_E.SetMode(AUTOMATIC);
    PID_D.SetMode(AUTOMATIC);
}



// Precisa Documentar e Revisar Essa Parte do Código Ainda
void CarroRobo::Andar(bool sentido, float distancia)
{

    // Calcula Quantidade de Pulsos Necessários para andar uma distância "distancia"
    long QtdPulsosNecessarios = distancia/(DEG_TO_RAD*RAIO_RODA*DTHETA);


    // Reseta Deslocamentos Acumulados
    CarroRobo::ResetPulsosAcumulados(ESQUERDA);
    CarroRobo::ResetPulsosAcumulados(DIREITA);

    RodaE.SetPoint = Velocidade;
    RodaD.SetPoint = Velocidade;


    // anda para frente
    if(sentido)
    {
        while(QtdPulsosNecessarios > PulsosAcumulados(DIREITA) + PulsosAcumulados(ESQUERDA))
        {
            analogWrite(CarroRobo::IN1, RodaE.Potencia);
            analogWrite(CarroRobo::IN2, 0);

            analogWrite(CarroRobo::IN3, RodaD.Potencia);
            analogWrite(CarroRobo::IN4, 0);   

            // Prototipo Básico de Controle de Velocidade
            PID_E.Compute();
            PID_D.Compute();
        } 
    }

    // anda para tras
    else
    {
        while(QtdPulsosNecessarios > PulsosAcumulados(DIREITA) + PulsosAcumulados(ESQUERDA))
        {
            analogWrite(CarroRobo::IN1, 0);
            analogWrite(CarroRobo::IN2, RodaE.Potencia);

            analogWrite(CarroRobo::IN3, 0);
            analogWrite(CarroRobo::IN4, RodaD.Potencia);  
               
            // Prototipo Básico de Controle de Velocidade
            PID_E.Compute();
            PID_D.Compute();
        }
    }




    // Freia o CarroRobo
    digitalWrite(CarroRobo::IN1, HIGH);
    digitalWrite(CarroRobo::IN2, HIGH);

    digitalWrite(CarroRobo::IN3, HIGH);
    digitalWrite(CarroRobo::IN4, HIGH);

}


// Precisa Documentar e Revisar Essa Parte do Código Ainda
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


// Incrementa Contagem de Pulsos
void CarroRobo::MedirPulsos()
{
    // Desabilita interrupções para Realização das Contas
    noInterrupts();

        // Salva Ultima Medida
        CarroRobo::RodaE.UltimaMedida = CarroRobo::RodaE.Pulsos;
        CarroRobo::RodaD.UltimaMedida = CarroRobo::RodaD.Pulsos;
        
        CarroRobo::RodaE.MedidaAcumulada += CarroRobo::RodaE.UltimaMedida;
        CarroRobo::RodaD.MedidaAcumulada += CarroRobo::RodaD.UltimaMedida;  

        // Reinicia Contagem de Pulsos
        CarroRobo::RodaE.Pulsos = 0;
        CarroRobo::RodaD.Pulsos = 0;
        
    interrupts();
}

// Atualiza Contagem de Pulsos da Roda Esquerda
void CarroRobo::RodaE_interrupt()
{
    CarroRobo::RodaE.Pulsos += 1;
}


// Atualiza Contagem e Pulsos da Roda Direira
void CarroRobo::RodaD_interrupt()
{
    CarroRobo::RodaD.Pulsos += 1;
}



// Retorna a Velocidade Atual de acordo com o Modo Escolhido
float CarroRobo::MedirVelocidade(char modo, char roda)
{
    if(modo == CMS)
    {
        if(roda == ESQUERDA)
        {
            return RodaE.UltimaMedida/CarroRobo::IntervaloDeMedida;
        }
        else if(roda == DIREITA)
        {
            return RodaD.UltimaMedida/CarroRobo::IntervaloDeMedida;
        }
    }
      
      
    else if(modo == PPI)
    {
        if(roda == ESQUERDA)
        {
            return RodaE.UltimaMedida;
        }
        else if(roda == DIREITA)
        {
            return RodaD.UltimaMedida;
        }
    }
    return 0;
}


// Retorna Pulsos Computados Desde o Ultimo Reset
long long CarroRobo::PulsosAcumulados(char roda)
{
    if(roda == ESQUERDA)
    {
        return CarroRobo::RodaE.MedidaAcumulada;
    }
    else if(roda == DIREITA)
    {
        return CarroRobo::RodaD.MedidaAcumulada;
    }

   return 0;
}


// Reseta Valores Acumulados
void CarroRobo::ResetPulsosAcumulados(char roda)
{
    if(roda == ESQUERDA)
    {
        CarroRobo::RodaE.MedidaAcumulada = 0;
    }
    else if(roda == DIREITA)
    {
        CarroRobo::RodaD.MedidaAcumulada = 0;
    }
}
