#include <LiquidCrystal.h> // Biblioteca LCD
#include <DHT.h> //Biblioteca DHT
#include <SoftwareSerial.h> //Biblioteca bt

SoftwareSerial bluetooth(17,16); //objeto do bluetooth e entradas digitais (rx,tx)

#define HT A0 //define a porta que vai medir o ar
#define TipoDHT DHT11 // define o tipo de sensor

// Inicializa a biblioteca DHT
DHT dht(HT, TipoDHT); //objeto do sensor

// Inicializa a biblioteca LCD
LiquidCrystal LCD(42,44,46,48,50,52); //objeto do LCD

//entrada do Sensor de lumi
int SensorLumiPino = A1;
//entrada do Sensor de umidade
int SensorUmidPino=A3;
// alerta de temperatura baixa 'led azul'
int AlertaTempAlta=13;
//alerta de temperatura alta 'led vermelho'
int AlertaTempBaixa=12;
//alerta de umidade baixa 'led vermelho'
int AlertaUmidBaixa=11;
//alerta de luminosidade baixa
int AlertaLumiBaixa=10;
//alerta de umidade do ar baixa
int AlertaUmiArBaixa=7;
//alerta de umidade do ar alta
int AlertaUmiArAlta=9;

int seg = 3000;

//Lumi vars
float luminosidadeMedida = 0;//Declara a variável como inteiro
float luminosidadeAlvo=20; //cria uma var e define a luminosidade alvo

//Temperature vars
float TemperaturaAlvo; //cria uma var e define a temperatura alvo como 20 graus
float TemperaturaMedida; //cria uma var para guardar o valor de temperatura medido
float dT;
int potencia;

//Umidade solovars
float Ua; //cria uma var e define a umidade alvo como 60 %
float Um; //cria uma var para guardar o valor de umidade medido

//Umidade Ar vars
float umidadeArAlvo;
float umidadeArMedida;


void setup() 
{
  Serial.begin(9600);
  bluetooth.begin(9600); //inicia a conexao bt
  dht.begin(); //inicia o sensor
  //------------------------------------------------------------------
  pinMode(AlertaTempBaixa, OUTPUT); // Define o pino de alerta de temperatura baixa como saida
  pinMode(AlertaTempAlta, OUTPUT); // Define o pino de alerta de temperatura alta como saida
  pinMode(AlertaUmidBaixa, OUTPUT); // Define o pino de alerta de temperatura baixa como saida
  pinMode(AlertaLumiBaixa, OUTPUT); //Define o pino de alerta de luminosidade baixa como saida
  pinMode(AlertaUmiArBaixa, OUTPUT); //Define o pino de alerta de umidade do ar baixa como saida
  pinMode(AlertaUmiArAlta, OUTPUT); //Define o pino de alerta de umidade do ar alta como saida
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

int leituraSerial = Serial.read();
if (leituraSerial==49)//MODO QUENTE E SECO
{
TemperaturaAlvo=30;
umidadeArAlvo=50;
Ua=95;
}
else if (leituraSerial==50)//MODO FRIO E UMIDO
{
TemperaturaAlvo=20;
umidadeArAlvo=80;
Ua=10;
}

Serial.println(leituraSerial);


  

////////////////////////////////////////////////////////////////////////////
//TEMPERATURA---------------------------------------------------------------

TemperaturaMedida = dht.readTemperature(); //variavel que mede a temperatura
    
//definindo a diferenca entre a temperatura desejada e a medida para tomar decisoes
dT = TemperaturaMedida-TemperaturaAlvo;

if(dT>0)
{
  potencia = (dT*250/10);
  analogWrite(AlertaTempAlta, potencia);
  analogWrite(AlertaTempBaixa, 0);
}
else
{
  potencia = (-1*dT*250/10);
  analogWrite(AlertaTempBaixa, potencia);
  analogWrite(AlertaTempAlta, 0);
}

////////////////////////////////////////////////////////////////////////////
//MODO-TEMPERATURA-LCD------------------------------------------------------

//configurando o lcd para o modo temperatura

LCD.setCursor(0,2);
LCD.print("TEMP:    ");

//imprimindo o estado do sistema
if (dT>0) //temperatura e maior que o dejejado
  {
    LCD.setCursor(5,2);
    LCD.print("HOT   ");
  }

else //temperatura eh menor que o desejada
  {
    LCD.setCursor(5,2);
    LCD.print("FRIO  ");
  }

delay(seg);// Aguarda seg segundos

////////////////////////////////////////////////////////////////////////////////////
//UMIDADE_SOLO----------------------------------------------------------------------

//pegando o valor de out do sensor e transformando em umidade por cento
Um = analogRead(SensorUmidPino);
Um = ((Um*99)/1023);
if(Um>Ua){
digitalWrite(AlertaUmidBaixa, HIGH);}
else{(AlertaUmidBaixa, LOW);}

/////////////////////////////////////////////////////////////////////////////////////
//MODO-UMIDADE_SOLO-LCD--------------------------------------------------------------

LCD.setCursor(0,4);
LCD.print("SOlO:    ");

if (Um<Ua) //umidade e maior que o dejejado
  {
    LCD.setCursor(5,4);
    LCD.print("UMIDO");
    
  }
else //temperatura eh menor que o desejada
  {
    LCD.setCursor(5,4);
    LCD.print("SECO ");
  }

////////////////////////////////////////////////////////////////////////////
//LUMINOSIDADE--------------------------------------------------------------

luminosidadeMedida = analogRead(SensorLumiPino);
luminosidadeMedida=((luminosidadeMedida*99)/1023);

if(luminosidadeMedida>luminosidadeAlvo){analogWrite(AlertaLumiBaixa,150);}
else{analogWrite(AlertaLumiBaixa,0);}

////////////////////////////////////////////////////////////////////////////
//LUMINOSIDADE-LCD----------------------------------------------------------

LCD.setCursor(11,4);
LCD.print("LUM:     ");
LCD.setCursor(15,4);
if(luminosidadeMedida<luminosidadeAlvo){LCD.print("CLARO");}
else{LCD.print("DARK");}

////////////////////////////////////////////////////////////////////////////
//UMIDADE_AR----------------------------------------------------------------

umidadeArMedida = dht.readHumidity(); //variavel que mede a umidade
int dUA=umidadeArMedida-umidadeArAlvo;
if(dUA>0)
{
  potencia = (dUA*250/100);
  analogWrite(AlertaUmiArAlta,potencia);
  analogWrite(AlertaUmiArBaixa,0);
}
else
{
  potencia = (-1*dUA*250/100);
  analogWrite(AlertaUmiArAlta,0);
  analogWrite(AlertaUmiArBaixa,potencia);
}
////////////////////////////////////////////////////////////////////////////
//UMI-AR-LCD----------------------------------------------------------------

LCD.setCursor(11,2);
LCD.print("AR:      ");
LCD.setCursor(14,2);
if(umidadeArMedida>umidadeArAlvo){LCD.print("UMIDO");}
else{LCD.print("SECO ");}

////////////////////////////////////////////////////////////////////////////

int Temperatura_Medida_INT = TemperaturaMedida;
int Temperatura_Alvo_INT = TemperaturaAlvo;

int Umidade_Alvo_INT = umidadeArAlvo;
int Umidade_Medida_INT = umidadeArMedida;

int UmINT = Um;
int UaINT = Ua;

int luminosidadeAlvoINT = luminosidadeAlvo;
int luminosidadeMedidaINT = luminosidadeMedida;

//set mode

LCD.setCursor(0,0);
LCD.print("T:   A:   S:   L:");
LCD.setCursor(2,0);
LCD.print(Temperatura_Alvo_INT);
LCD.print("C");
LCD.setCursor(7,0);
LCD.print(Umidade_Alvo_INT);
LCD.print("%");
LCD.setCursor(12,0);
LCD.print(UaINT);
LCD.print("%");
LCD.setCursor(17,0);
LCD.print(luminosidadeAlvoINT);
LCD.print("%");

//

LCD.setCursor(0,1);
LCD.print("T:   A:   S:   L:");
LCD.setCursor(2,1);
LCD.print(Temperatura_Medida_INT);
LCD.print("C");
LCD.setCursor(7,1);
LCD.print(Umidade_Medida_INT);
LCD.print("%");
LCD.setCursor(12,1);
LCD.print(UmINT);
LCD.print("%");
LCD.setCursor(17,1);
LCD.print(luminosidadeMedidaINT);
LCD.print("%");

////////////////////////////////////////////////////////////////////////////
delay(seg);// Aguarda seg segundos

Serial.print("Temperatura:"); //IMPRIME O TEXTO NA SERIAL
Serial.println(TemperaturaMedida); //IMPRIME NA SERIAL O VALOR DE temperatura

//Serial.print("Umi Solo:");
//Serial.println(Um);

Serial.print("Umidade Ar:"); //printa a umidade
Serial.println(umidadeArMedida);

//Serial.println("%");

Serial.print("Luminosidade:");//Imprime na serial a mensagem Valor lido pelo LDR
Serial.println(luminosidadeMedida);//Imprime na serial os dados de luminosidade
  
  //Serial.println("Enviando dados");
  bluetooth.print(TemperaturaMedida);
  bluetooth.print("/");
  bluetooth.print(umidadeArMedida);
  bluetooth.print("/");
  bluetooth.print(luminosidadeMedida);
  //Serial.println("Dados enviados");

  delay(seg); //delay de 3 segundos

}
