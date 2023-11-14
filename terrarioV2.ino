#include <SoftwareSerial.h>
#include <DHT.h> //importando as duas bibliotecas
int ldr = A1;//Atribui A0 a variável ldr
int luminosidade = 0;//Declara a variável valorldr como inteiro

SoftwareSerial bluetooth(2,3); //objeto do bluetooth

#define HT A0 //define a porta que vai medir a umidade
#define TipoDHT DHT11 // define o tipo de sensor

DHT dht(HT, TipoDHT); //objeto do sensor

void setup(){
  // sensor de temperatura e umidade
  Serial.begin(9600); //inicia o terminal
  Serial.println("teste de DHT");
  bluetooth.begin(9600); //inicia a conexao
  dht.begin(); //inicia o sensor
  
  // sensor de luminosidade
  pinMode(ldr, INPUT);//Define ldr (pino analógico A1) como saída
  Serial.begin(9600);//Inicialização da comunicação serial, com taxa de transferência em bits por segundo de 9600
}

void loop(){
  // umidade
  float umidade = dht.readHumidity(); //variavel que mede a umidade
  if(isnan(umidade)){ //checa se tem erros e recomeça o loop
    Serial.println("erro com DHT");
    return;
  }
  // temperatura
  float temperatura = dht.readTemperature(); //variavel que mede a temperatura
  if(isnan(umidade)){ //checa se tem erros e recomeça o loop
    Serial.println("erro com DHT");
    return;
  }
  // luminosidade
  luminosidade = analogRead(ldr);//Lê o valor do sensor ldr e armazena na variável valorldr
  Serial.print("Valor lido pelo LDR = ");//Imprime na serial a mensagem Valor lido pelo LDR
  Serial.println(luminosidade);//Imprime na serial os dados de valorldr

  Serial.print("Umidade: "); //printa a umidade
  Serial.print(umidade);
  Serial.println("%");
  Serial.print(" / Temperatura: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(temperatura, 0); //IMPRIME NA SERIAL O VALOR DE temperatura
  Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL

  Serial.println("Dados enviados");
  bluetooth.print(umidade);
  bluetooth.print(temperatura);
  bluetooth.print(luminosidade);

  delay(3000); //delay de 3 segundos
  
}
