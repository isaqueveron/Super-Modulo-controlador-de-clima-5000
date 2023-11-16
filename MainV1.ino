#include <SoftwareSerial.h>
#include <DHT.h> //importando as duas bibliotecas

SoftwareSerial bluetooth(2,3); //objeto do bluetooth

#define HT A0 //define a porta que vai medir a umidade
#define TipoDHT DHT11 // define o tipo de sensor

DHT dht(HT, TipoDHT); //objeto do sensor

void setup(){
  Serial.begin(9600); //inicia o terminal
  Serial.println("teste de DHT");
  bluetooth.begin(9600); //inicia a conexao
  dht.begin(); //inicia o sensor
}

void loop(){
  float umidade = dht.readHumidity(); //variavel que mede a umidade
  if(isnan(umidade)){ //checa se tem erros e recomeça o loop
    Serial.println("erro com DHT");
    return;
  }
  float temperatura = dht.readTemperature(); //variavel que mede a temperatura
  if(isnan(umidade)){ //checa se tem erros e recomeça o loop
    Serial.println("erro com DHT");
    return;
  }
  Serial.print("Umidade: "); //printa a umidade
  Serial.print(umidade);
  Serial.println("%");
  Serial.print(" / Temperatura: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(temperatura, 0); //IMPRIME NA SERIAL O VALOR DE temperatura
  Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL

  Serial.println("Dados enviados");
  bluetooth.print(umidade);
  bluetooth.print(temperatura);

  delay(3000); //delay de 3 segundos
  
}
