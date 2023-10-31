#include <LiquidCrystal.h> // Biblioteca LCD

// Inicializa a biblioteca LCD
LiquidCrystal LCD(7,8,5,4,3,2);

// Define o pino analogico A0 como entrada do Sensor de Temperatura
int SensorTempPino=A0; //potenciometro
// Define o pino analogico A0 como entrada do Sensor de umidade
int SensorUmidPino=A1;
// Define o pino 6 para o alerta de temperatura baixa 'led azul'
int AlertaTempBaixa=6;
// Define o pino 9 para o alerta de temperatura alta 'led vermelho'
int AlertaTempAlta=9;
// Define o pino A0 para o alerta de umidade alta
int AlertaUmidBaixa=A5;

int seg = 500;
int estado = LOW; //estado da umidade do solo

//Temperature vars
float Ta=30; //cria uma var e define a temperatura alvo como 20 graus
float Tm; //cria uma var para guardar o valor de temperatura medido
float dT;

//Umidade vars
float Ua=800; //cria uma var e define a umidade alvo como 60 %
float Um; //cria uma var para guardar o valor de umidade medido


void setup() 
{
  Serial.begin(9600);
  pinMode(AlertaTempBaixa, OUTPUT); // Define o pino de alerta de temperatura baixa como saida
  pinMode(AlertaTempAlta, OUTPUT); // Define o pino de alerta de temperatura alta como saida
  pinMode(AlertaUmidBaixa, OUTPUT); // Define o pino de alerta de temperatura baixa como saida
  pinMode(SensorUmidPino, INPUT);
  pinMode(SensorTempPino, INPUT);
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
//TEMPERATURA---------------------------------------------------------------

//transformando a tensao em valores de temperatura em graus Celsius

Tm = analogRead(SensorTempPino); //TESTETESTETESTEEEEE
Tm = Tm/10;
//definindo a diferenca entre a temperatura desejada e a medida para tomar decisoes
dT = Tm-Ta;

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
//UMIDADE---------------------------------------------------------------------------

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
//MODO-UMIDADE-LCD-------------------------------------------------------------------

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

Serial.println(Tm);
Serial.println(estado);

}
