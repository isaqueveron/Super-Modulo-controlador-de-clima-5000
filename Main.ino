#include <LiquidCrystal.h> // Biblioteca LCD
#include <DHT.h> //Biblioteca DHT
#include <SoftwareSerial.h> //Biblioteca bt

SoftwareSerial Bluetooth(10,11); //ROXO,PRETO objeto do bluetooth e entradas digitais (rx,tx)
char code;

#define HT A0 //define a porta que vai medir o ar
#define TipoDHT DHT11 // define o tipo de sensor

// Inicializa a biblioteca DHT
DHT dht(HT, TipoDHT); //objeto do sensor

// Inicializa a biblioteca LCD
LiquidCrystal LCD(52,50,48,46,44,42); //objeto do LCD

//entrada do Sensor de lumi
int SensorLumiPino = A1;
//entrada do Sensor de umidade
int SensorUmidPino=A3;


//lampada
int Pos_Bomba=5;
int Neg_Bomba=6;

//pino pwm +peltier
int Pos_Peltier=4;
//pino pwm -peltier
int Neg_Peltier=3;

int seg = 0;

//Lumi vars
float luminosidadeMedida;//Declara a variável como inteiro
float luminosidadeAlvo=50; //cria uma var e define a luminosidade alvo

//Temperature vars
float TemperaturaAlvo=25; //cria uma var e define a temperatura alvo como 20 graus
float TemperaturaMedida; //cria uma var para guardar o valor de temperatura medido
float dT;
int potencia;

//Umidade solovars
float Ua=20; //cria uma var e define a umidade alvo como 20 %
float Um; //cria uma var para guardar o valor de umidade medido

//Umidade Ar vars
float umidadeArAlvo=65;
float umidadeArMedida;


void setup() 
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Bluetooth.begin(9600); //inicia a conexao bt
  Bluetooth.println("Hello, world?");
  Serial.println(Bluetooth.read());
  dht.begin(); //inicia o sensor
  //------------------------------------------------------------------
  
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
code = Bluetooth.read();
Serial.println(code, DEC);
Serial.println();
if (code==97)//verao
{TemperaturaAlvo=33;}
if (code==98)//outono
{TemperaturaAlvo=26;}
if (code==99)//inverno
{TemperaturaAlvo=23;}
if (code==100)//primavera
{TemperaturaAlvo=29;}


////////////////////////////////////////////////////////////////////////////
//TEMPERATURA---------------------------------------------------------------

TemperaturaMedida = dht.readTemperature(); //variavel que mede a temperatura
    
//definindo a diferenca entre a temperatura desejada e a medida para tomar decisoes
dT = TemperaturaMedida-TemperaturaAlvo;
//Serial.print(">>>>");
//Serial.println(dT);
if((dT<1)&&(dT>0.3)) //se pouco quente
{
  potencia = (dT*255);
  ligar_peltier(Pos_Peltier,Neg_Peltier,0,potencia);
  //Serial.println("pouco quente");
  //Serial.println(potencia);
}
else if((dT>-1)&&(dT<-0.3)) //se pouco frio
{
  potencia = (-1*dT*255);
  ligar_peltier(Pos_Peltier,Neg_Peltier,potencia,0);
  //Serial.println("pouco frio");
  //Serial.println(potencia);
}
else if(1<dT)//se  quente
{
  potencia = 255;
  ligar_peltier(Pos_Peltier,Neg_Peltier,0,255);
  //Serial.println("quente");
  //Serial.println(potencia);
}
else if(dT<-1)//se frio
{
  potencia = 255;
  ligar_peltier(Pos_Peltier,Neg_Peltier,255,0); 
  //Serial.println("frio") ;
  //Serial.println(potencia);
}
else if ((dT>-0.4)&&(dT<0.4))
{
  ligar_peltier(Pos_Peltier,Neg_Peltier,0,0); 
  //Serial.println("Temperatura OK") ;
}


////////////////////////////////////////////////////////////////////////////
//MODO-TEMPERATURA-LCD------------------------------------------------------

//configurando o lcd para o modo temperatura

LCD.setCursor(0,2);
LCD.print("TEMP:    ");

//imprimindo o estado do sistema
if (dT>0.3) //temperatura e maior que o dejejado
  {
    LCD.setCursor(5,2);
    LCD.print("HOT   ");
  }
else if ((dT>-0.3)&&(dT<0.3)) //temperatura e maior que o dejejado
  {
    LCD.setCursor(5,2);
    LCD.print("OK    ");
  }

else //temperatura eh menor que o desejada
  {
    LCD.setCursor(5,2);
    LCD.print("FRIO  ");
  }

delay(seg);// Aguarda seg segundos

////////////////////////////////////////////////////////////////////////////////////
//UMIDADE_SOLO----------------------------------------------------------------------OK

//pegando o valor de out do sensor e transformando em umidade por cento
Um = analogRead(SensorUmidPino);
Um = ((Um*99)/1023);
Um = (100-Um);

digitalWrite(Neg_Bomba,LOW);//GND
if(Um<Ua)
{
  digitalWrite(Pos_Bomba, HIGH);
}
else{digitalWrite(Pos_Bomba, LOW);}


/////////////////////////////////////////////////////////////////////////////////////
//MODO-UMIDADE_SOLO-LCD--------------------------------------------------------------

LCD.setCursor(0,4);
LCD.print("SOlO:    ");

if (Um>Ua) //umidade e maior que o dejejado
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
if(luminosidadeAlvo>50){
LCD.print("OFF");}
else{LCD.print("ON ");}

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
if(luminosidadeMedida>luminosidadeAlvo){
LCD.print("OFF");}
else{LCD.print("ON ");}


////////////////////////////////////////////////////////////////////////////

//Serial.print("Temperatura:"); //IMPRIME O TEXTO NA SERIAL
//Serial.println(TemperaturaMedida); //IMPRIME NA SERIAL O VALOR DE temperatura

//Serial.print("Umi Solo:");
//Serial.println(Um);

//Serial.print("Umidade Ar:"); //printa a umidade
//Serial.println(umidadeArMedida);

//Serial.print("Luminosidade:");//Imprime na serial a mensagem Valor lido pelo LDR
//Serial.println(luminosidadeMedida);//Imprime na serial os dados de luminosidade
  
//Serial.println("Enviando dados");
Bluetooth.print(TemperaturaMedida);
Bluetooth.print("/");
Bluetooth.print(umidadeArMedida);
Bluetooth.print("/");
Bluetooth.print(luminosidadeMedida);
//Serial.println("Dados enviados");
   
delay(seg); //delay de seg/1000 segundos

}

void ligar_peltier(int OUT1, int OUT2, int POTENCIA1, int POTENCIA2)
{
analogWrite(OUT1,POTENCIA1);
analogWrite(OUT2,POTENCIA2); 
}
