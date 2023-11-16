#include <LiquidCrystal.h> // Biblioteca LCD
#include <DHT.h> //Biblioteca DHT
#include <SoftwareSerial.h> //Biblioteca bt

SoftwareSerial bluetooth(2,3); //objeto do bluetooth e entradas digitais

#define HT A0 //define a porta que vai medir o ar
#define TipoDHT DHT11 // define o tipo de sensor

// Inicializa a biblioteca DHT
DHT dht(HT, TipoDHT); //objeto do sensor

// Inicializa a biblioteca LCD
LiquidCrystal LCD(13,12,11,10,9,7); //objeto do LCD

//entrada do Sensor de lumi
int SensorLumiPino = A1;
//entrada do Sensor de umidade
int SensorUmidPino=A3;
// alerta de temperatura baixa 'led azul'
int AlertaTempBaixa=6;
//alerta de temperatura alta 'led vermelho'
int AlertaTempAlta=5;
//alerta de umidade baixa 'led vermelho'
int AlertaUmidBaixa=A4;

int seg = 0;

//Lumi vars
int luminosidade = 0;//Declara a variável valorldr como inteiro

//Temperature vars
float Ta=30; //cria uma var e define a temperatura alvo como 20 graus
float Tm; //cria uma var para guardar o valor de temperatura medido
float dT;

//Umidade vars
int estado = LOW; //estado da umidade do solo
float Ua=800; //cria uma var e define a umidade alvo como 60 %
float Um; //cria uma var para guardar o valor de umidade medido


void setup() 
{
  Serial.begin(9600);
  Serial.println("teste de DHT");
  bluetooth.begin(9600); //inicia a conexao bt
  dht.begin(); //inicia o sensor
  //------------------------------------------------------------------
  pinMode(AlertaTempBaixa, OUTPUT); // Define o pino de alerta de temperatura baixa como saida
  pinMode(AlertaTempAlta, OUTPUT); // Define o pino de alerta de temperatura alta como saida
  pinMode(AlertaUmidBaixa, OUTPUT); // Define o pino de alerta de temperatura baixa como saida
  pinMode(SensorUmidPino, INPUT);

  //-------------------------------------------------------------------
  LCD.begin(20,4); // Define a quantidade de colunas e linhas do LCD
  LCD.setCursor(0,0);
  LCD.print("bagulho iniciando.  ");
  delay(1000);
  LCD.setCursor(0,0);
  LCD.print("bagulho iniciando.. ");
  delay(1000);
  LCD.setCursor(0,0);
  LCD.print("bagulho iniciando...");
  delay(1000);
  LCD.setCursor(0,0);
  delay(1000);
  LCD.print("                     ");
}

void loop() 
{
////////////////////////////////////////////////////////////////////////////
//LUMINOSIDADE--------------------------------------------------------------

luminosidade = analogRead(SensorLumiPino);
//Serial.println("OK1");

////////////////////////////////////////////////////////////////////////////
//UMIDADE_AR----------------------------------------------------------------

  float umidade = dht.readHumidity(); //variavel que mede a umidade

////////////////////////////////////////////////////////////////////////////
//TEMPERATURA---------------------------------------------------------------
//Serial.println("OK2");
//transformando a tensao em valores de temperatura em graus Celsius


Tm = dht.readTemperature(); //variavel que mede a temperatura
    
//definindo a diferenca entre a temperatura desejada e a medida para tomar decisoes
dT = Tm-Ta;
//Serial.println("OK3");
  if (dT>0) //temperatura e maior que o dejejado
  {
    if (dT>5) //muito maior
    {
    analogWrite(AlertaTempBaixa, 0);
    analogWrite(AlertaTempAlta, 255);
    }
    else if(dT>3)//pouco maior
    {
    analogWrite(AlertaTempBaixa, 0);
    analogWrite(AlertaTempAlta, 100);
    }
    else if(dT>1)//muito pouco maior
    {
    analogWrite(AlertaTempBaixa, 0);
    analogWrite(AlertaTempAlta, 20);
    }
    else
    {
    analogWrite(AlertaTempBaixa, 0);
    analogWrite(AlertaTempAlta, 0);
    }
  }
  
  else //temperatura eh menor que o desejada
  {
    if (dT<-5) //muito menor
    {
    analogWrite(AlertaTempBaixa, 255);
    analogWrite(AlertaTempAlta, 0);
    }
    else if (dT<-3) //medio menor
    {
    analogWrite(AlertaTempBaixa, 100);
    analogWrite(AlertaTempAlta, 0);
    }
    else if(dT<-1)//muito pouco menor
    {
    analogWrite(AlertaTempBaixa, 20);
    analogWrite(AlertaTempAlta, 0);
    }
    else
    {
    analogWrite(AlertaTempBaixa, 0);
    analogWrite(AlertaTempAlta, 0);
    }
  }

////////////////////////////////////////////////////////////////////////////
//MODO-TEMPERATURA-LCD------------------------------------------------------

int Temperatura_Medida_INT = Tm;
int Temperatura_Alvo_INT = Ta;
int Delta_Temp = dT;

//configurando o lcd para o modo temperatura

LCD.setCursor(0,0); // Muda o cursor para a primeira coluna e primeira linha do LCD 
LCD.print("TM:   C  TA:   C");
LCD.setCursor(0,1);
LCD.print("ESTADO:    ");

//imprimindo no display a temperatura desejada
LCD.setCursor(12,0);
LCD.print(Temperatura_Alvo_INT);

//imprrimindo no display a temperatura medida
LCD.setCursor(3,0);
LCD.print(Temperatura_Medida_INT);

//imprimindo o estado do sistema (std:"estado")
if (Delta_Temp>0) //temperatura e maior que o dejejado
  {
    LCD.setCursor(7,1);
    LCD.print("HOT ");
  }

else //temperatura eh menor que o desejada
  {
    LCD.setCursor(7,1);
    LCD.print("COLD");
  }

delay(seg);// Aguarda seg segundos

////////////////////////////////////////////////////////////////////////////////////
//UMIDADE_SOLO----------------------------------------------------------------------

//pegando o valor de out do sensor e transformando em umidade por cento
Um = analogRead(SensorUmidPino);

  if (Um<Ua) //umidade e maior que o dejejado (o sensor mede 1023 quando esta seco e 0 se esta totalmente umido)
  {
    estado = LOW;
  }
  
  else //umidade eh menor que o desejada
  {
    estado = HIGH;
  }
  digitalWrite(AlertaUmidBaixa, estado);

/////////////////////////////////////////////////////////////////////////////////////
//MODO-UMIDADE_SOLO-LCD--------------------------------------------------------------

LCD.setCursor(0,4);
LCD.print("ESTADO:    ");

if (Um<Ua) //umidade e maior que o dejejado
  {
    LCD.setCursor(7,4);
    LCD.print("WET ");
    
  }
else //temperatura eh menor que o desejada
  {
    LCD.setCursor(7,4);
    LCD.print("DRY ");
  }

delay(seg);// Aguarda seg segundos

  Serial.print("Umi Solo: ");
  Serial.println(estado);
  Serial.print("Umidade:"); //printa a umidade
  Serial.print(umidade);
  Serial.println("%");
  Serial.print("Temperatura:"); //IMPRIME O TEXTO NA SERIAL
  Serial.println(Tm); //IMPRIME NA SERIAL O VALOR DE temperatura
  Serial.print("Luminosidade:");//Imprime na serial a mensagem Valor lido pelo LDR
  Serial.println(luminosidade);//Imprime na serial os dados de luminosidade
  
  Serial.println("Dados enviados");
  bluetooth.print(umidade);
  bluetooth.print(Tm);
  bluetooth.print(luminosidade);

  delay(3000); //delay de 3 segundos

}
