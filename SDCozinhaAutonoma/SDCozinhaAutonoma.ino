#include "dht.h"

bool sistema = false;
String dados = "";
char conteudo;
int tempoAtual, tempoAnte = 0;
int pot, gas, temp, hum;
const int pinoDHT11 = A2;
const int pinoMQ2 = A4;
const int pinoPOT = A0;

dht DHT; //VARIÁVEL DO TIPO DHT

//LÊ AS INFORMAÇÕES DOS SENSORES
void leituraDados( int timeT)
{
  String dados2 = "";

  DHT.read11(pinoDHT11);
  pot = analogRead(pinoPOT);
  gas = analogRead(pinoMQ2);
  hum = DHT.humidity;
  temp = DHT.temperature;
  dados2.concat(gas);
  dados2.concat(" ");
  dados2.concat(hum);
  dados2.concat(" ");
  dados2.concat(temp);
  dados2.concat(" ");
  dados2.concat(timeT);
  Serial.println(dados2);


}

bool estado = false;

String dados2 = "";
char conteudo2;

void manual() {
  int  botao = digitalRead(4);
  if (botao == HIGH) {
    delay(1000);
    if (estado == true) {
      sistema = false;
      estado = false;
      digitalWrite(2, LOW);  //EXAUSTOR                                 LED-VERMELHO
      digitalWrite(10, LOW); //FORNECIMENTO DE ENERGIGA ELETRICA        LED-BRANCO
      digitalWrite(11, LOW); //SISTEMA DE GÁS                           LED-AMARELO
      digitalWrite(12, LOW);
      analogWrite(9, 0);
    }
    else {
      sistema = true;
      estado = true;
      digitalWrite(2, HIGH);  //EXAUSTOR                                 LED-VERMELHO
      digitalWrite(10, HIGH); //FORNECIMENTO DE ENERGIGA ELETRICA        LED-BRANCO
      digitalWrite(11, HIGH); //SISTEMA DE GÁS                           LED-AMARELO
      digitalWrite(12, LOW);
      analogWrite(9, 140);
    }

  }
}
void verifica()
{

  while (Serial.available())
  {
    conteudo = Serial.read();
    if (conteudo != '\n') {
      // Concatena valores
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
  pinMode(13, OUTPUT);

  digitalWrite(2, LOW);  //EXAUSTOR                                 LED-VERMELHO
  digitalWrite(10, LOW); //FORNECIMENTO DE ENERGIGA ELETRICA        LED-BRANCO
  digitalWrite(11, LOW); //SISTEMA DE GÁS                           LED-AMARELO
  digitalWrite(12, LOW);  //SISTEMA CONTRA INCÊNDIO                 LED-VERDE


}
bool teste = false;
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
      teste = true;
    }
    manual();
    verifica();
    if(teste == true){
      leituraDados(millis() - Tgasto);  
    }
    teste = false;
  }

}
