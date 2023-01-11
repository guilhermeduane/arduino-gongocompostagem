
/* SISTEMA DE CONTROLE AUTOMATIZADO DA PRODUÇÃO DE GONGOLO
 *  
 * O sistema foi desenvolvido para controle automatizado do ambiente de produção de gongolo para fins de produção de adubo otimizada controlando umidade e temperatura.
 * Consiste em um controle de umidade e temperatura que ao detectar a umidade abaixo do valor estipulado aciona através de um relé uma valvula solenoide de agua até o material e
 * caso a temperatura do ambiente esteja abaixo do estipulado um conjunto de lampadas é acionado até a temperatura ideal seja alcançada.
 * Projeto foi proposto e financiado integralmente por Gabriel e prototipado e desenvolvido por Guilherme Duane com base no projeto de criação de adubo otimizado da embrapa utilizando gongolo. 
 * Utiliza as bibliotecas LiquidCrystal, SimpleDHT e timer e possuem licença MIT.
 *  
 */


#include <LiquidCrystal.h> 
#include <SimpleDHT.h> 
#include "timer.h"
   
    //pino sensor DHT11
    int pinDHT11 = 11; 
    //Condição de acionamento da valvula de água em porcentagem (Menor que o valor informado a valvula é acionada)
    int humidityConditionWaterSolenoidValve = 60;
    //Condição de acionamento das lampadas em graus(Menor que o valor informado as lampadas são ligadas)
    int tempConditionLamp = 27;
    //Numero do pino que o relé da valvula solenoide de agua está ligado na placa arduino  
    int pinRelaySolenoidValve = 12; 
    //Numero do pino que o relé do conjunto de lampadas está ligado na placa arduino
    int pinRelayLamp = 2;
    //Controla o timer do sensor atraves da biblioteca timer.h que facilita a implementação de timers com os registradores da placa
    Timer timer;
    //variavel do sensor dht11 da biblioteca SimpleDHT.h
    SimpleDHT11 dht11;
    // pinos do lcd keypad df robot 16x2
    LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 

 // Pega a temperatura e umidade do sensor e faz o acionamento dos relés de acordo com a condição
 void getHumidityTemperature()
    {
      
    byte temperature = 0;
    byte humidity = 0; 
    
    int err = SimpleDHTErrSuccess; 
    
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
     { 
      lcd.print("Leitura do sensor falhou! err="); 
      lcd.println(err);
         return;
      
         } 
         
         //ESTE RELÉ TRABALHA COM A LÓGICA DE ACIONAMENTO EM 0 (LOW), FICAR ATENTO CASO TROCAR O RELÉ, SE NÃO FUNCIONAR TROCAR O LOW POR HIGH E HIGH POR LOW

        //Condição de acionamento da lampada
        if((int)temperature < tempConditionLamp){
          digitalWrite(pinRelayLamp, LOW);   
        }else{
          digitalWrite(pinRelayLamp, HIGH);   
        }

        //condição de acionamento da valvula solenoide de agua.
        if((int)humidity < humidityConditionWaterSolenoidValve){
          digitalWrite(pinRelaySolenoidValve, LOW);   
        }else{
          digitalWrite(pinRelaySolenoidValve, HIGH);   
      }

      //imprime resultados no lcd keypad 16x2
      lcd.setCursor(0,0); 
      lcd.print("Temperatura: "); 
      lcd.print((int)temperature); 
      Serial.println((int)temperature);
      lcd.print((char)223);
      lcd.setCursor(0,1); 
      lcd.print("Umidade    : "); 
      lcd.print((int)humidity); 
      Serial.println((int)humidity);
      lcd.print("%");
      
    }

//configuração dos periféricos e timer;
 void setup() 
    { 
      Serial.begin(9600);
      //Seta o cursor do display LCD
      lcd.begin(16, 2);
      lcd.setCursor(0,0);
      //Intro do programa
      lcd.print("GONGOCOMPOSTAGEM");
      lcd.setCursor(3,1); 
      lcd.print("by Gabriel");
      delay(3000);
 
      lcd.clear(); 
      lcd.setCursor(0,1); 
      lcd.clear(); 
      
      //Seta os pinos do rele das lampadas e do rele da valvula como saida(OUTPUT)
      pinMode(pinRelayLamp, OUTPUT);
      pinMode(pinRelaySolenoidValve, OUTPUT);
      
      //configura o timer nos registradores e a função de callback
      timer.setInterval(1500);
      timer.setCallback(getHumidityTemperature);
      timer.start();
    
    }

//função principal
void loop()
   { 
    timer.update();
   }

   