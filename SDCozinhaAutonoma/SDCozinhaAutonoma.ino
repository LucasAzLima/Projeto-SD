#include "dht.h" //biblioteca necessária para o sensor de temperatura(DHT11) funcionar

bool sistema = false; //variavel que controla o sistem em ON/OFF
String dados = ""; //leitura dos dados recebidos
char conteudo; //leitura dos dador recebidos
int tempoAtual, tempoAnte = 0; //temporizador
int pot, gas, temp, umi; //Sensores - Potenciometro - gas - temperatura - umidade
const int pinoDHT11 = A2; //pino do sensor de temperatura e umidade
const int pinoMQ2 = A4; //pino do sensor de gas/fumaça
const int pinoPOT = A0; //pino do Potenciometro

dht DHT; //VARIÁVEL DO TIPO DHT

//LÊ AS INFORMAÇÕES DOS SENSORES
void leituraDados( int timeT)
{
  String dados2 = "";
  //leitura dod dados
  DHT.read11(pinoDHT11);
  pot = analogRead(pinoPOT);
  gas = analogRead(pinoMQ2);
  umi = DHT.humidity;
  temp = DHT.temperature;
  //concatena em uma string para ser enviado
  dados2.concat(gas);
  dados2.concat(" ");
  dados2.concat(umi);
  dados2.concat(" ");
  dados2.concat(temp);
  dados2.concat(" ");
  dados2.concat(timeT);

  //envia a string
  Serial.println(dados2);


}


bool estado = false;// variavel do botao que controla o sistema

//função de acionamento/desligamemto manual do sistema
void manual() {

  int  botao = digitalRead(4);//botao

  if (botao == HIGH) {
    delay(1000);
    if (estado == true) {
      sistema = false;
      estado = false;
      digitalWrite(2, LOW); 
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);                    
      digitalWrite(12, LOW);
      analogWrite(9, 0);
    }
    else {
      sistema = true;
      estado = true;
      digitalWrite(2, HIGH);                               
      digitalWrite(10, HIGH);     
      digitalWrite(11, HIGH);                  
      digitalWrite(12, LOW);
      analogWrite(9, 140);
    }

  }
}

//verifica o comamdo enviado do servidor 
void verifica()
{

  while (Serial.available())
  {
    conteudo = Serial.read();
    if (conteudo != '\n') {
      dados.concat(conteudo);
    }

    if (dados == "on")
    {
      sistema = true;
      estado = true;
      digitalWrite(2, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      analogWrite(9, 140);

    }
    if (dados == "off")
    {
      sistema = false;
      estado = false;
      digitalWrite(2, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      analogWrite(9, 0);
    }
    if (sistema == true) {
      //exaustor e sistema 
      if (dados == "S1")
      {
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(12, LOW);
        digitalWrite(2, HIGH);
        analogWrite(9, 140);
        estado = true;
      }
      if (dados == "S2")
      {
        digitalWrite(2, LOW);
        analogWrite(9, 0);
        estado = true;
      }
      //muita fumaça/gas no ambiente
      if (dados == "S3")
      {
        digitalWrite(11, LOW);
        digitalWrite(10, LOW);
        digitalWrite(12, LOW);
        digitalWrite(2, HIGH);
        analogWrite(9, 160);
        estado = false;
        sistema = false;
      }
      if (dados == "S4")
      {
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        estado = true;
      }
      //possivel incendio
      if (dados == "S5")
      {
        digitalWrite(2, HIGH);
        analogWrite(9, 160);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        estado = false;
        sistema = false;
      }
    }
  }
  delay(500);
  dados = "";


}
void setup()
{
  Serial.begin(9600);
  pinMode(pinoDHT11, INPUT);
  pinMode(pinoMQ2, INPUT);
  pinMode(pinoPOT, INPUT);
  pinMode(2, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);


  digitalWrite(2, LOW);  //EXAUSTOR                                 LED-VERMELHO
  digitalWrite(10, LOW); //FORNECIMENTO DE ENERGIGA ELETRICA        LED-BRANCO
  digitalWrite(11, LOW); //SISTEMA DE GÁS                           LED-AMARELO
  digitalWrite(12, LOW);  //SISTEMA CONTRA INCÊNDIO                 LED-VERDE


}
bool EnvDados = false;
int Tgasto = 0;
void loop()
{

  manual();
  verifica();
  while (sistema == true)
  {
    tempoAtual = millis() / 1000;
    Tgasto = millis();
    if (tempoAtual % 5 == 0) {
      delay(1000);
      EnvDados = true;
    }
    manual();
    verifica();
    if(EnvDados == true){
      leituraDados(millis() - Tgasto);  
    }
    EnvDados = false;
  }

}
