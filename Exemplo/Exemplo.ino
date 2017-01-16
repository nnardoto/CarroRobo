// Programa Básico Para Carro-Robo
#include <CarroRobo.h>


CarroRobo Carrinho(5, 6, 9, 10, 2, 3);



void setup() 
{
  
  analogWrite(5, 80);
  analogWrite(6, 0);
         
  analogWrite(9, 80);
  analogWrite(10, 0); 
  
  
  Serial.begin(9600);  
}


void loop() 
{  
 Serial.print("Roda E: "); 
 Serial.println(Carrinho.MedirVelocidade(PPI, ESQUERDA));
 Serial.print("Roda D: "); 
 Serial.println(Carrinho.MedirVelocidade(PPI, DIREITA));

}
