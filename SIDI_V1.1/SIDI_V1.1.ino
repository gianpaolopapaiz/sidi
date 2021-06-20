//Programa: SIDI V1.0 (R1) 
//Autor: PROTTO
//Atenção: Esse código utiliza bibliotecas adicionais ou partes delas, que não foram desenvolvidas pela PROTTO.
//Objetivo: Código de funcionamento do sistema de monitoramento de plantas SIDI
//Desenvolvido para Wemos D1 R1 (ESP8266)
//Arduino IDE 1.8.13
//ESP 8266 2.5.2

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//ÁREA PRÉ SET UP
//Área responsável por adicionar bibliotecas, definir variáveis globais e definir as portas nas quais os módulos
//eletrônicos estão conectados.

//Wifi -> definições relacionadas ao funcionamento do Wifi -> ESP8266
#include <ESP8266mDNS.h>  
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ESP8266WiFi.h>
#include <elapsedMillis.h>
String ip;
int lenip = 0;
int menuwifi = 0;
int menuwifi2 = 0;
int conectado = 0;
String request = "";
// Cria uma instancia do servidor e especifica uma porta
WiFiServer server(80);
elapsedMillis timeout;


//EEPROM -> definições relacionadas ao salvamento de dados no sistema mesmo após o desligamento
#include <EEPROM.h>
int address = 0;
int esup;
int einf;


//Fita LED
#include <Adafruit_NeoPixel.h>
#define DATA_PIN D5  
#define NUMPIXELS 8
//número de LEDs na fita
Adafruit_NeoPixel leds(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);
int estadoled = 1;
int corled = 0;
int tipoefeito = 0;
int r = 0;
int g = 0;
int b = 0;
int ra = 15;
int ga = 15;
int ba = 15;

// Sensor de Umidade de Solo
#define sensorumidade A0
int valor_umidade;
int superior = 1100;
int inferior = 400;
int range = superior - inferior;

//Variáveis para os Menus 
int estadomenu = 0;
int anteriorestadomenu = 0;

//Variável para o menu de configuração
int menu = 0;

//Botão set
#define botaoset 13  
int anteriorset = 0;
int estadobotaoset = 0;
int anteriorestadobotaoset = 0;

//Botão mais
#define botaomais 12  
int anteriormais = 0;
int estadobotaomais = 0;
int anteriorestadobotaomais = 0;


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Função theaterChaseRainbow
//Função de teatro da Fita LED
void theaterChaseRainbow(uint8_t wait) {

Serial.print("TheaterChaseRainbow");
for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (uint16_t i=0; i < leds.numPixels(); i=i+3) {
            leds.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }
        leds.show();
        delay(wait);

       for (uint16_t i=0; i < leds.numPixels(); i=i+3) {
           leds.setPixelColor(i+q, 0);        //turn every third pixel off
           if (digitalRead(botaomais) == HIGH){
              delay(1000);
              estadomenu = estadomenu+1;
              return;
              }
           if (digitalRead(botaoset) == HIGH){
              delay(1000);
              anteriorestadobotaoset = digitalRead(botaoset);
              tipoefeito = tipoefeito+1;
              if(tipoefeito > 2) {
                tipoefeito = 0;
                }
              Serial.print("Tipo Efeito: ");
              Serial.println(tipoefeito);
              return;
              }
//-------------------------------------------------------------------------------
//Durante a função de efeito checa se houve algum comando via APP para executa-lo
//Checa se o cliente esta conectado
WiFiClient client = server.available();
if (client) {
 
   //Espera o cliente mandar alguma informação
   while (!client.available()) {
         delay(1);
         }
         
// Recebe a primeira linha da requisição do AlvaAPP
String request = client.readStringUntil('\r');
Serial.println(request); 

//Checa se precisa fazer algo a partir da requisição do cliente

//Reset do wifi via app
if (request.indexOf("rst") != -1) {
   for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(255, 255, 0));
   leds.show();
   delay(10);
   }
   //Reset do Wifi
   WiFiManager wifiManager;
   wifiManager.resetSettings();
   delay(2000);
   wifiManager.autoConnect("SIDI"); //Nome da rede a ser criada pelo módulo Wifi
   while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
         }
   estadomenu = 0;
   return;
   }
 
//Monitoramento Umidade
if (request.indexOf("umid") != -1) {
   estadomenu = 0;
   estadoled = 1;
   return;
   }
//Desligar LED
if (request.indexOf("desl") != -1) {
   estadomenu = 0;
   estadoled = 0;
   return;
   }
//Acender Cor Branca
if (request.indexOf("c0") != -1) {
   estadomenu = 1;
   corled = 0;
   return;
   }
//Acender Cor Azul Claro
if (request.indexOf("c1") != -1) {
   estadomenu = 1;
   corled = 1;
   return;
   }
//Acender Cor Azul 
if (request.indexOf("c2") != -1) {
   estadomenu = 1;
   corled = 2;
   return;
   }
//Acender Cor Verde Agua
if (request.indexOf("c3") != -1) {
   estadomenu = 1;
   corled = 3;
   return;
   }
//Acender Cor Verde
if (request.indexOf("c4") != -1) {
   estadomenu = 1;
   corled = 4;
   return;
   }
//Acender Cor Marrom
if (request.indexOf("c5") != -1) {
   estadomenu = 1;
   corled = 5;
   return;
   }
//Acender Cor Roxo
if (request.indexOf("c6") != -1) {
   estadomenu = 1;
   corled = 6;
   return;
   }
//Acender Cor Pink
if (request.indexOf("c7") != -1) {
   estadomenu = 1;
   corled = 7;
   return;
   }
//Acender Cor Vermelho
if (request.indexOf("c8") != -1) {
   estadomenu = 1;
   corled = 8;
   return;
   }
//Acender Cor Laranja
if (request.indexOf("c9") != -1) {
   estadomenu = 1;
   corled = 9;
   return;
   }
//Acender Cor Amarelo
if (request.indexOf("cx") != -1) {
   estadomenu = 1;
   corled = 10;
   return;
   }
//Efeito Teatro
if (request.indexOf("e0") != -1) {
   estadomenu = 2;
   tipoefeito = 0;
   return;
   }
//Efeito Rainbow
if (request.indexOf("e1") != -1) {
   estadomenu = 2;
   tipoefeito = 1;
   return;
   }
//Efeito Pulse
if (request.indexOf("e2") != -1) {
   estadomenu = 2;
   tipoefeito = 2;
   return;
   }
//Configuração da cor r (ex: rsl214)
if (request.indexOf("rsl") != -1) {
   String txtr = request.substring(request.indexOf("rsl") + 3, request.indexOf("rsl") + 6);
   ra = txtr.toInt();
   Serial.print("ra: ");
   Serial.print(ra);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor g (ex: gsl214)
if (request.indexOf("gsl") != -1) {
   String txtg = request.substring(request.indexOf("gsl") + 3, request.indexOf("gsl") + 6);
   ga = txtg.toInt();
   Serial.print("ga: ");
   Serial.print(ga);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor b (ex: bsl214)
if (request.indexOf("bsl") != -1) {
   String txtb = request.substring(request.indexOf("bsl") + 3, request.indexOf("bsl") + 6);
   ba = txtb.toInt();
   Serial.print("ba: ");
   Serial.print(ba);
   estadomenu = 3;
   delay(100);
   return;
   }

Serial.flush();
}
//Fim da checagem do WiFi
//-------------------------------------------------------------------------------
        }
      }
    }
}

// Input de valor de 0 a 255 para obter valor da cor
// As cores alteram entre R,G e B e retornam para o RThe colours are a transition r - g - b - back to r
uint32_t Wheel(byte WheelPos) {
WheelPos = 255 - WheelPos;
if(WheelPos < 85) {
  return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
if(WheelPos < 170) {
  WheelPos -= 85;
  return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
WheelPos -= 170;
return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Função rainbowCycle
//Função rainbow da Fita LED
void rainbowCycle(uint8_t wait) {

uint16_t i, j;
Serial.print("RainbowCycle");
for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< leds.numPixels(); i++) {
       leds.setPixelColor(i, Wheel(((i * 256 / leds.numPixels()) + j) & 255));
       if (digitalRead(botaoset) == HIGH){
          delay(1000);
          tipoefeito = tipoefeito+1;
          if(tipoefeito > 2) {
            tipoefeito = 0;
            }
          Serial.print("Tipo Efeito: ");
          Serial.println(tipoefeito);
          return;      
          }
       if (digitalRead(botaomais) == HIGH){
          delay(1000);
          estadomenu = estadomenu+1;
          return;
          }
//-------------------------------------------------------------------------------
//Durante a função de efeito checa se houve algum comando via APP para executa-lo
//Checa se o cliente esta conectado
WiFiClient client = server.available();
if (client) {
   //Espera o cliente mandar alguma informação
   while (!client.available()) {
   delay(1);
   }

// Recebe a primeira linha da requisição do AlvaAPP
String request = client.readStringUntil('\r');
Serial.println(request); 

//Checa se precisa fazer algo a partir da requisição do cliente

//Reset do wifi via app
if (request.indexOf("rst") != -1) {
   for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(255, 255, 0));
   leds.show();
   delay(10);
   }
   //Reset do Wifi
   WiFiManager wifiManager;
   wifiManager.resetSettings();
   delay(2000);
   wifiManager.autoConnect("SIDI"); //Nome da rede a ser criada pelo módulo Wifi
   while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
         }
   estadomenu = 0;
   return;
   }
 
//Monitoramento Umidade
if (request.indexOf("umid") != -1) {
   estadomenu = 0;
   estadoled = 1;
   return;
   }
//Desligar LED
if (request.indexOf("desl") != -1) {
   estadomenu = 0;
   estadoled = 0;
   return;
   }
//Acender Cor Branca
if (request.indexOf("c0") != -1) {
   estadomenu = 1;
   corled = 0;
   return;
   }
//Acender Cor Azul Claro
if (request.indexOf("c1") != -1) {
   estadomenu = 1;
   corled = 1;
   return;
   }
//Acender Cor Azul 
if (request.indexOf("c2") != -1) {
   estadomenu = 1;
   corled = 2;
   return;
   }
//Acender Cor Verde Agua
if (request.indexOf("c3") != -1) {
   estadomenu = 1;
   corled = 3;
   return;
   }
//Acender Cor Verde
if (request.indexOf("c4") != -1) {
   estadomenu = 1;
   corled = 4;
   return;
   }
//Acender Cor Marrom
if (request.indexOf("c5") != -1) {
   estadomenu = 1;
   corled = 5;
   return;
   }
//Acender Cor Roxo
if (request.indexOf("c6") != -1) {
   estadomenu = 1;
   corled = 6;
   return;
   }
//Acender Cor Pink
if (request.indexOf("c7") != -1) {
   estadomenu = 1;
   corled = 7;
   return;
   }
//Acender Cor Vermelho
if (request.indexOf("c8") != -1) {
   estadomenu = 1;
   corled = 8;
   return;
   }
//Acender Cor Laranja
if (request.indexOf("c9") != -1) {
   estadomenu = 1;
   corled = 9;
   return;
   }
//Acender Cor Amarelo
if (request.indexOf("cx") != -1) {
   estadomenu = 1;
   corled = 10;
   return;
   }
//Efeito Teatro
if (request.indexOf("e0") != -1) {
   estadomenu = 2;
   tipoefeito = 0;
   return;
   }
//Efeito Rainbow
if (request.indexOf("e1") != -1) {
   estadomenu = 2;
   tipoefeito = 1;
   return;
   }
//Efeito Pulse
if (request.indexOf("e2") != -1) {
   estadomenu = 2;
   tipoefeito = 2;
   return;
   }

//Configuração da cor r (ex: rsl214)
if (request.indexOf("rsl") != -1) {
   String txtr = request.substring(request.indexOf("rsl") + 3, request.indexOf("rsl") + 6);
   ra = txtr.toInt();
   Serial.print("ra: ");
   Serial.print(ra);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor g (ex: gsl214)
if (request.indexOf("gsl") != -1) {
   String txtg = request.substring(request.indexOf("gsl") + 3, request.indexOf("gsl") + 6);
   ga = txtg.toInt();
   Serial.print("ga: ");
   Serial.print(ga);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor b (ex: bsl214)
if (request.indexOf("bsl") != -1) {
   String txtb = request.substring(request.indexOf("bsl") + 3, request.indexOf("bsl") + 6);
   ba = txtb.toInt();
   Serial.print("ba: ");
   Serial.print(ba);
   estadomenu = 3;
   delay(100);
   return;
   }

Serial.flush();
}
//Fim da checagem do WiFi
//------------------------------------------------------------------------------- 
    leds.show();
    delay(wait);
  }
 }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Função RGBLoop
//Função Pulse da Fita LED

void RGBLoop(){

for(int j = 0; j < 6; j++ ) {
   // Fade IN
   for(int k = 0; k < 256; k++) {
      switch(j) {
        case 0: setAll(k,0,0); break;
        case 1: setAll(k,k,0); break;
        case 2: setAll(0,k,k); break; 
        case 3: setAll(0,0,k); break;
        case 4: setAll(0,k,0); break;
        case 5: setAll(k,k,k); break;
        }
      if (digitalRead(botaoset) == HIGH){
         delay(1000);
         tipoefeito = tipoefeito+1;
         if(tipoefeito > 2) {
           tipoefeito = 0;
           }
         Serial.print("Tipo Efeito: ");
         Serial.println(tipoefeito);
         return;   
         }
      if (digitalRead(botaomais) == HIGH){
         delay(1000);
         estadomenu = estadomenu+1;
         return;
         }
//-------------------------------------------------------------------------------
//Durante a função de efeito checa se houve algum comando via APP para executa-lo
//Checa se o cliente esta conectado
WiFiClient client = server.available();
if (client) {
 
   //Espera o cliente mandar alguma informação
   while (!client.available()) {
         delay(1);
         }
   // Recebe a primeira linha da requisição do AlvaAPP
   String request = client.readStringUntil('\r');
   Serial.println(request); 

//Checa se precisa fazer algo a partir da requisição do cliente

//Reset do wifi via app
if (request.indexOf("rst") != -1) {
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(255, 255, 0));
      leds.show();
      delay(10);
      }
   //Reset do Wifi
   WiFiManager wifiManager;
   wifiManager.resetSettings();
   delay(2000);
   wifiManager.autoConnect("SIDI"); //Nome da rede a ser criada pelo módulo Wifi
   while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
         }
   estadomenu = 0;
   return;
   }
 
//Monitoramento Umidade
if (request.indexOf("umid") != -1) {
   estadomenu = 0;
   estadoled = 1;
   return;
   }
//Desligar LED
if (request.indexOf("desl") != -1) {
   estadomenu = 0;
   estadoled = 0;
   return;
   }
//Acender Cor Branca
if (request.indexOf("c0") != -1) {
   estadomenu = 1;
   corled = 0;
   return;
   }
//Acender Cor Azul Claro
if (request.indexOf("c1") != -1) {
   estadomenu = 1;
   corled = 1;
   return;
   }
//Acender Cor Azul 
if (request.indexOf("c2") != -1) {
   estadomenu = 1;
   corled = 2;
   return;
   }
//Acender Cor Verde Agua
if (request.indexOf("c3") != -1) {
   estadomenu = 1;
   corled = 3;
   return;
   }
//Acender Cor Verde
if (request.indexOf("c4") != -1) {
   estadomenu = 1;
   corled = 4;
   return;
   }
//Acender Cor Marrom
if (request.indexOf("c5") != -1) {
   estadomenu = 1;
   corled = 5;
   return;
   }
//Acender Cor Roxo
if (request.indexOf("c6") != -1) {
   estadomenu = 1;
   corled = 6;
   return;
   }
//Acender Cor Pink
if (request.indexOf("c7") != -1) {
   estadomenu = 1;
   corled = 7;
   return;
   }
//Acender Cor Vermelho
if (request.indexOf("c8") != -1) {
   estadomenu = 1;
   corled = 8;
   return;
   }
//Acender Cor Laranja
if (request.indexOf("c9") != -1) {
   estadomenu = 1;
   corled = 9;
   return;
   }
//Acender Cor Amarelo
if (request.indexOf("cx") != -1) {
   estadomenu = 1;
   corled = 10;
   return;
   }
//Efeito Teatro
if (request.indexOf("e0") != -1) {
   estadomenu = 2;
   tipoefeito = 0;
   return;
   }
//Efeito Rainbow
if (request.indexOf("e1") != -1) {
   estadomenu = 2;
   tipoefeito = 1;
   return;
   }
//Efeito Pulse
if (request.indexOf("e2") != -1) {
   estadomenu = 2;
   tipoefeito = 2;
   return;
   }

 //Configuração da cor r (ex: rsl214)
 if (request.indexOf("rsl") != -1) {
    String txtr = request.substring(request.indexOf("rsl") + 3, request.indexOf("rsl") + 6);
    ra = txtr.toInt();
    Serial.print("ra: ");
    Serial.print(ra);
    estadomenu = 3;
    delay(100);
    return;
    }
//Configuração da cor g (ex: gsl214)
if (request.indexOf("gsl") != -1) {
   String txtg = request.substring(request.indexOf("gsl") + 3, request.indexOf("gsl") + 6);
   ga = txtg.toInt();
   Serial.print("ga: ");
   Serial.print(ga);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor b (ex: bsl214)
if (request.indexOf("bsl") != -1) {
   String txtb = request.substring(request.indexOf("bsl") + 3, request.indexOf("bsl") + 6);
   ba = txtb.toInt();
   Serial.print("ba: ");
   Serial.print(ba);
   estadomenu = 3;
   delay(100);
   return;
   }
Serial.flush();
}
//Fim da checagem do WiFi
//-------------------------------------------------------------------------------
       leds.show();
       delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) {
      switch(j) {
        case 0: setAll(k,0,0); break;
        case 1: setAll(k,k,0); break;
        case 2: setAll(0,k,k); break; 
        case 3: setAll(0,0,k); break;
        case 4: setAll(0,k,0); break;
        case 5: setAll(k,k,k); break;
      }
      if (digitalRead(botaoset) == HIGH){
         delay(1000);
         tipoefeito = tipoefeito+1;
         if(tipoefeito > 2) {
           tipoefeito = 0;
           }
         Serial.print("Tipo Efeito: ");
         Serial.println(tipoefeito);
         return;   
         }
      if (digitalRead(botaomais) == HIGH){
         delay(1000);
         estadomenu = estadomenu+1;
         return;
         }
//-------------------------------------------------------------------------------
//Durante a função de efeito checa se houve algum comando via APP para executa-lo
//Checa se o cliente esta conectado
WiFiClient client = server.available();
if (client) {
   //Espera o cliente mandar alguma informação
   while (!client.available()) {
         delay(1);
         }
   // Recebe a primeira linha da requisição do AlvaAPP
   String request = client.readStringUntil('\r');
   Serial.println(request); 
   //Checa se precisa fazer algo a partir da requisição do cliente
   //Reset do wifi via app
   if (request.indexOf("rst") != -1) {
      for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(255, 255, 0));
      leds.show();
      delay(10);
      }
   //Reset do Wifi
   WiFiManager wifiManager;
   wifiManager.resetSettings();
   delay(2000);
   wifiManager.autoConnect("SIDI"); //Nome da rede a ser criada pelo módulo Wifi
   while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
         }
   estadomenu = 0;
   return;
   }
 
//Monitoramento Umidade
if (request.indexOf("umid") != -1) {
   estadomenu = 0;
   estadoled = 1;
   return;
   }
//Desligar LED
if (request.indexOf("desl") != -1) {
   estadomenu = 0;
   estadoled = 0;
   return;
   }
//Acender Cor Branca
if (request.indexOf("c0") != -1) {
   estadomenu = 1;
   corled = 0;
   return;
   }
//Acender Cor Azul Claro
if (request.indexOf("c1") != -1) {
   estadomenu = 1;
   corled = 1;
   return;
   }
//Acender Cor Azul 
if (request.indexOf("c2") != -1) {
   estadomenu = 1;
   corled = 2;
   return;
   }
//Acender Cor Verde Agua
if (request.indexOf("c3") != -1) {
   estadomenu = 1;
   corled = 3;
   return;
   }
//Acender Cor Verde
if (request.indexOf("c4") != -1) {
   estadomenu = 1;
   corled = 4;
   return;
   }
//Acender Cor Marrom
if (request.indexOf("c5") != -1) {
   estadomenu = 1;
   corled = 5;
   return;
   }
//Acender Cor Roxo
if (request.indexOf("c6") != -1) {
   estadomenu = 1;
   corled = 6;
   return;
   }
//Acender Cor Pink
if (request.indexOf("c7") != -1) {
   estadomenu = 1;
   corled = 7;
   return;
   }
//Acender Cor Vermelho
if (request.indexOf("c8") != -1) {
   estadomenu = 1;
   corled = 8;
   return;
   }
//Acender Cor Laranja
if (request.indexOf("c9") != -1) {
   estadomenu = 1;
   corled = 9;
   return;
   }
//Acender Cor Amarelo
if (request.indexOf("cx") != -1) {
   estadomenu = 1;
   corled = 10;
   return;
   }
//Efeito Teatro
if (request.indexOf("e0") != -1) {
   estadomenu = 2;
   tipoefeito = 0;
   return;
   }
//Efeito Rainbow
if (request.indexOf("e1") != -1) {
   estadomenu = 2;
   tipoefeito = 1;
   return;
   }
//Efeito Pulse
if (request.indexOf("e2") != -1) {
   estadomenu = 2;
   tipoefeito = 2;
   return;
   }

//Configuração da cor r (ex: rsl214)
if (request.indexOf("rsl") != -1) {
   String txtr = request.substring(request.indexOf("rsl") + 3, request.indexOf("rsl") + 6);
   ra = txtr.toInt();
   Serial.print("ra: ");
   Serial.print(ra);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor g (ex: gsl214)
if (request.indexOf("gsl") != -1) {
   String txtg = request.substring(request.indexOf("gsl") + 3, request.indexOf("gsl") + 6);
   ga = txtg.toInt();
   Serial.print("ga: ");
   Serial.print(ga);
   estadomenu = 3;
   delay(100);
   return;
   }
//Configuração da cor b (ex: bsl214)
if (request.indexOf("bsl") != -1) {
   String txtb = request.substring(request.indexOf("bsl") + 3, request.indexOf("bsl") + 6);
   ba = txtb.toInt();
   Serial.print("ba: ");
   Serial.print(ba);
   estadomenu = 3;
   delay(100);
   return;
   }
Serial.flush();
}
//Fim da checagem do WiFi
//------------------------------------------------------------------------------- 

      leds.show();
      delay(3);
    }
  }
}

void setAll(byte red, byte green, byte blue) {
for(int i = 0; i < NUMPIXELS; i++ ) {
   leds.setPixelColor(i, red, green, blue);
   }
leds.show();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//ÁREA DE SET UP
//Área responsável pelo preparo (set up) dos componentes do projeto.

void setup() {

delay(2000);

Serial.begin(115200);

EEPROM.begin(512);

//Ao ligar o Wemos D1 R1 recupera os valores do limite superior e inferior do sensor de umidade de solo
superior = EEPROM.read(0)*5;
Serial.println(superior);
inferior = EEPROM.read(1)*5;
Serial.println(inferior);
if (superior == inferior) {
   inferior = superior - 100;
   }
range = superior - inferior;
Serial.println(range);

// Sensores de Umidade de Solo
pinMode(sensorumidade, INPUT);

//Botão Set
pinMode(botaoset, INPUT); //define o botao "set" como um input

//Botao Mais
pinMode(botaomais, INPUT); //define o botao "mais" como um input

//Fita LED
leds.begin(); 
leds.clear();

//Show de abertura: Fita LED acendendo cada LED na cor verde
for(int i=0; i<NUMPIXELS; i++){
        leds.setPixelColor(i, leds.Color(0, 0, 0));
        leds.show();
        delay(10);
        }
        delay(500);
for(int i=0; i<NUMPIXELS; i++){
        leds.setPixelColor(i, leds.Color(0, 255, 0));
        leds.show();
        delay(200);
        }

for(int i=0; i<NUMPIXELS; i++){
        leds.setPixelColor(i, leds.Color(0, 0, 0));
        leds.show();
        delay(200);
        }
delay(500);

//Wifi
WiFiManager wifiManager;

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//ÁREA DO LOOP
//Área responsável pelo processamento continuo do sistema.

void loop() {

//Testa se o WIfi está conectado
if (WiFi.status() == WL_CONNECTED && conectado == 0) {
 
   Serial.println("");
   Serial.println("WiFi Conectado");
   // Incia o Servidor
   server.begin();
   Serial.println("Servidor Iniciado");                    
   // Endereço IP
   timeout = 0;
   while (!Serial.available() && timeout < 3000) {
         ip = WiFi.localIP().toString();
         Serial.println(ip);
         delay(500);
         }
   conectado = 1;
  }  
if (WiFi.status() != WL_CONNECTED && conectado == 1) {
   conectado = 0;
   ip = "";
   }

//Printa o valor das variáveis no monitor serial 
Serial.println(analogRead(sensorumidade));
Serial.print("Estado Botao Set: ");
Serial.println(estadobotaoset);
Serial.print("Estado Botao Mais: ");
Serial.println(estadobotaomais);
Serial.print("Menu: ");
Serial.println(estadomenu);
Serial.print("Estadoled: ");
Serial.println(estadoled);
Serial.print("Cor led: ");
Serial.println(corled);
Serial.print("Tipo Efeito: ");
Serial.println(tipoefeito);
Serial.print("Superior: ");
Serial.println(superior);

//Valor de umidade do solo em %: 0% seco a 100% umido
valor_umidade = 100 - ((analogRead(sensorumidade)-inferior)*100/range); 
  
//Botão Mais -> troca entre os estados de menu de funcionamento
estadobotaomais = digitalRead(botaomais);
if (estadobotaomais == HIGH && anteriorestadobotaomais == LOW){
   estadomenu = estadomenu + 1;
   }
anteriorestadobotaomais = estadobotaomais;
anteriorestadomenu = estadomenu;
if (estadomenu > 3) {
   estadomenu = 0;
   }
 
//Botão Set -> troca entre as opçoes secundárias de cada menu de funcionamento
estadobotaoset = digitalRead(botaoset);

//Botão Set no menu 0 -> acende e apaga a Fita LED
if (estadobotaoset == HIGH && anteriorestadobotaoset == LOW && estadomenu == 0){
   if (estadoled == 0){ 
      estadoled = 1;
      } else if (estadoled == 1) {
                estadoled = 0;
                }
      }

//Botão Set no menu 1 -> troca entre as 11 cores disponíveis da Fita LED
if (estadobotaoset == HIGH && anteriorestadobotaoset == LOW && estadomenu == 1){  
   corled = corled + 1;
   if (corled > 10) {
      corled = 0; 
      }
   }
//Botão Set no menu 2 -> troca entre os 3 tipos de efeitos da Fita LED
if (estadobotaoset == HIGH && anteriorestadobotaoset == LOW && estadomenu == 2){  
   tipoefeito = tipoefeito + 1;
   if (tipoefeito > 2) {
      tipoefeito = 0; 
      }
   }
anteriorestadobotaoset = estadobotaoset;

//------------------------------------------------------------------------
// Menu de funcionamento 1 -> Cores com base na umidade de solo
if (estadomenu == 0) {
   if (anteriorestadomenu != 0) {
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(0, 0, 0));
         leds.show();
         delay(10);
         }
      estadoled = 1;
      }
   if (estadoled == 1){
      Serial.print("Umidade:"); 
      Serial.print(valor_umidade);
      Serial.println("%");
    //verifica se valor de leitura está abaixo de 400
   if (valor_umidade < 25){
      // Acende cor vermelha
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(255, 0, 0));
         leds.show();
         delay(10);
         }
      } else if(valor_umidade >= 25 && valor_umidade < 50){
               // Acende cor amarela
               for(int i=0; i<NUMPIXELS; i++){
                  leds.setPixelColor(i, leds.Color(255, 255, 0));
                  leds.show();
                  delay(10);
                  }
               } else if(valor_umidade >= 50 && valor_umidade < 75){
                        // Acende cor verde
                        for(int i=0; i<NUMPIXELS; i++){
                           leds.setPixelColor(i, leds.Color(0, 255, 0));
                           leds.show();
                           delay(10);
                           }
                        } else{
                              // Acende cor azul
                              for(int i=0; i<NUMPIXELS; i++){
                                 leds.setPixelColor(i, leds.Color(0, 0, 255));
                                 leds.show();
                                 delay(10);
                                 }
                              }
         //Fite LED desligada
       } else if (estadoled == 0) {
                 for(int i=0; i<NUMPIXELS; i++){
                    leds.setPixelColor(i, leds.Color(0, 0, 0));
                    leds.show();
                    delay(10);
                    }
                 }
  }
// Menu de Funcionamento 2 -> Cores Fixas
if (estadomenu == 1) {
   if (anteriorestadomenu != 1) {
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(0, 0, 0));
         leds.show();
         delay(10);
         }
      }
    
    if (corled == 0) {
    // Acende fita cor Branca
       r = 255;
       g = 255;
       b = 255;
       for(int i=0; i<NUMPIXELS; i++){
          leds.setPixelColor(i, leds.Color(r, g, b));
          leds.show();
          delay(10);
          }
       }
    else if(corled == 1){
    // Acende fita cor Azul Claro
       r = 0;
       g = 191;
       b = 255;
       for(int i=0; i<NUMPIXELS; i++){
          leds.setPixelColor(i, leds.Color(r, g, b));
          leds.show();
          delay(10);
          }
       } 
    else if(corled == 2){
    // Acende fita cor azul
      r = 0;
      g = 0;
      b = 255;
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(r, g, b));
         leds.show();
         delay(10);
         }
      }
     else if(corled == 3){
     // Acende fita cor verde agua
      r = 0;
      g = 250;
      b = 154;
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(r, g, b));
         leds.show();
         delay(10);
         }
      } 
    else if(corled == 4){
    // Acende fita cor verde
      r = 0;
      g = 255;
      b = 0;
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(r, g, b));
         leds.show();
         delay(10);
         }
      } 
    else if(corled == 5){
    // Acende fita cor marrom
      r = 210;
      g = 105;
      b = 30;
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(r, g, b));
         leds.show();
         delay(10);
         }
      }
    else if(corled == 6){
    // Acende fita cor roxo
      r = 128;
      g = 0;
      b = 128;
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(r, g, b));
         leds.show();
         delay(10);
         }
      }
    else if(corled == 7){
    // Acende fita cor pink
       r = 255;
       g = 0;
       b = 255;
       for(int i=0; i<NUMPIXELS; i++){
          leds.setPixelColor(i, leds.Color(r, g, b));
          leds.show();
          delay(10);
          }
      } 
    else if(corled == 8){
    // Acende fita cor vermelho
       r = 255;
       g = 0;
       b = 0;
       for(int i=0; i<NUMPIXELS; i++){
          leds.setPixelColor(i, leds.Color(r, g, b));
          leds.show();
          delay(10);
          }
       }
    else if(corled == 9){
    // Acende fita cor laranja
       r = 255;
       g = 165;       
       b = 0;
       for(int i=0; i<NUMPIXELS; i++){
          leds.setPixelColor(i, leds.Color(r, g, b));
          leds.show();
          delay(10);
          }
      }
    else if(corled == 10){
    // Acende fita cor amarela
       r = 255;
       g = 255;       
       b = 0;
       for(int i=0; i<NUMPIXELS; i++){
          leds.setPixelColor(i, leds.Color(r, g, b));
          leds.show();
          delay(10);
          }
       }
 
   }
// Menu de Funcionamento 3 -> Efeitos Cores Aleatorias
if (estadomenu == 2) {
   if (anteriorestadomenu != 2) {
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(0, 0, 0));
         leds.show();
         delay(10);
         }
      }
   delay(500);
   if (tipoefeito == 0){
      theaterChaseRainbow(50);//função do efeito Teatro da Fita LED
      }
   if (tipoefeito == 1) {
      rainbowCycle(20);//função do efeito rainbow da Fita LED
      }
   if (tipoefeito == 2) {
      RGBLoop();//função do efeito pulse da Fita LED
      }
   }
 
// Menu de Funcionamento 4 -> Cor Personalizada App  
if (estadomenu == 3) {
   if (anteriorestadomenu != 3) {
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(0, 0, 0));
         leds.show();
         delay(10);
         }
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(ra, ga, ba));
      leds.show();
      delay(10);
      }
  }

//Chama a função que ativa o modo de configuração do projeto
if (digitalRead(botaoset) == HIGH & digitalRead(botaomais) == HIGH) {
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);
   menu = 0;
   menuconfig(); //função de configuração de parâmetros do projeto
   }
//-----------------------------------------------------------------------
//Codigo Wifi para receber dados do APP -> Cliente = App

//Checa se o cliente esta conectado
WiFiClient client = server.available();
if (!client) {
 return;
 } 
 
//Espera o cliente mandar alguma informação
while (!client.available()) {
 delay(1);
 }

// Recebe a primeira linha da requisição do APP
String request = client.readStringUntil('\r');
Serial.println(request); 

//Checa se precisa fazer algo a partir da requisição do cliente

//Reset do wifi via APP
if (request.indexOf("rst") != -1) {
  
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(255, 255, 0));
      leds.show();
      delay(10);
      }
      
   //Reset do Wifi
   WiFiManager wifiManager;
   wifiManager.resetSettings();
   delay(2000);
   wifiManager.autoConnect("SIDI"); //Nome da rede a ser criada pelo módulo Wifi
   while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
         }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000); 
   estadomenu = 0;
   } 

//Monitoramento Umidade
if (request.indexOf("umid") != -1) {
   estadomenu = 0;
   estadoled = 1;
   }

//Desligar LED
if (request.indexOf("desl") != -1) {
   estadomenu = 0;
   estadoled = 0;
   }

//Acender Cor Branca
if (request.indexOf("c0") != -1) {
   estadomenu = 1;
   corled = 0;
   }
   
//Acender Cor Azul Claro
if (request.indexOf("c1") != -1) {
   estadomenu = 1;
   corled = 1;
   }
   
//Acender Cor Azul 
if (request.indexOf("c2") != -1) {
   estadomenu = 1;
   corled = 2;
   }
   
//Acender Cor Verde Agua
if (request.indexOf("c3") != -1) {
   estadomenu = 1;
   corled = 3;
   }
   
//Acender Cor Verde
if (request.indexOf("c4") != -1) {
   estadomenu = 1;
   corled = 4;
   }

//Acender Cor Marrom
if (request.indexOf("c5") != -1) {
   estadomenu = 1;
   corled = 5;
   }

//Acender Cor Roxo
if (request.indexOf("c6") != -1) {
   estadomenu = 1;
   corled = 6;
   }

//Acender Cor Pink
if (request.indexOf("c7") != -1) {
   estadomenu = 1;
   corled = 7;
   }

//Acender Cor Vermelho
if (request.indexOf("c8") != -1) {
   estadomenu = 1;
   corled = 8;
   }

//Acender Cor Laranja
if (request.indexOf("c9") != -1) {
   estadomenu = 1;
   corled = 9;
   }

//Acender Cor Amarelo
if (request.indexOf("cx") != -1) {
   estadomenu = 1;
   corled = 10;
   }

//Efeito Teatro
if (request.indexOf("e0") != -1) {
   estadomenu = 2;
   tipoefeito = 0;
   }

//Efeito Rainbow
if (request.indexOf("e1") != -1) {
   estadomenu = 2;
   tipoefeito = 1;
   }

//Efeito Pulse
if (request.indexOf("e2") != -1) {
   estadomenu = 2;
   tipoefeito = 2;
   }

//Configuração da cor r (ex: rsl214)
if (request.indexOf("rsl") != -1) {
   String txtr = request.substring(request.indexOf("rsl") + 3, request.indexOf("rsl") + 6);
   ra = txtr.toInt();
   Serial.print("ra: ");
   Serial.print(ra);
   estadomenu = 3;
   delay(100);
   }
 
//Configuração da cor g (ex: gsl214)
if (request.indexOf("gsl") != -1) {
   String txtg = request.substring(request.indexOf("gsl") + 3, request.indexOf("gsl") + 6);
   ga = txtg.toInt();
   Serial.print("ga: ");
   Serial.print(ga);
   estadomenu = 3;
   delay(100);
   }
   
//Configuração da cor b (ex: bsl214)
if (request.indexOf("bsl") != -1) {
   String txtb = request.substring(request.indexOf("bsl") + 3, request.indexOf("bsl") + 6);
   ba = txtb.toInt();
   Serial.print("ba: ");
   Serial.print(ba);
   estadomenu = 3;
   delay(100);
   }

Serial.flush();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Função menuconfig
//Modo de configuração do Arduino, após acionar o botão "Set"e o botão "Mais", ao mesmo tempo.

void menuconfig() {

//Escolha  inicial das configurações: Wifi ou Sensor de Umidade
while (digitalRead(botaoset) == LOW) {
      if (digitalRead(botaomais) == LOW) {
         menu = menu;
         }
      else {
           menu = menu + 1;
           if (menu > 1) {
              menu = 0;
              }
           }
      // Azul piscante -> configuração WiFI     
      if (menu == 0) {
         Serial.println("Configuracao Wifi");
         for(int i=0; i<NUMPIXELS; i++){
            leds.setPixelColor(i, leds.Color(0, 0, 0));
            leds.show();
            delay(10);
            }
         delay(100);
         for(int i=0; i<NUMPIXELS; i++){
            leds.setPixelColor(i, leds.Color(0, 0, 255));
            leds.show();
            delay(10);
            }
         delay(100);
         } 
      //Vermelho Piscante -> configuração sensor de umidade de solo   
      else if (menu == 1) {
         Serial.println("Configuracao Sensor");
         for(int i=0; i<NUMPIXELS; i++){
            leds.setPixelColor(i, leds.Color(0, 0, 0));
            leds.show();
            delay(10);
            }
         delay(100);
         for(int i=0; i<NUMPIXELS; i++){
            leds.setPixelColor(i, leds.Color(255, 0, 0));
            leds.show();
            delay(10);
            }
         delay(100);     
         } 
      }

//Cancela o modo de configuração e retorna ao menu inicial
if (digitalRead(botaoset) == HIGH & digitalRead(botaomais) == HIGH) {
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);
   estadomenu = 0;
   return;
   }
  
//Fita LED pisca duas vezes verde indicando a opção escolhida   
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
        leds.setPixelColor(i, leds.Color(0, 255, 0));
        leds.show();
        delay(10);
        }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
        leds.setPixelColor(i, leds.Color(0, 0, 0));
        leds.show();
        delay(10);
        }
delay(100);     

if (menu == 0) {
   configwifi(); //chama função de configuração do horário e data
   } 
else if (menu == 1) {
   configsensor(); //chama função de configuração do horário do despertador
   }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Função mconfigwifi
//Função responsável pelo reset do WiFi
void configwifi(){
menu = 0;
while (digitalRead(botaoset) == LOW) {
      if (digitalRead(botaomais) == LOW) {
         menu = menu;
         }
      else {
         menu = menu + 1;
         if (menu > 1) {
            menu = 0;
            }
         }
      //Fita LED verde indica "sim" para resetar o WiFi
      if (menu == 0) {
         Serial.println("Resetar Wifi");
         for(int i=0; i<NUMPIXELS; i++){
            leds.setPixelColor(i, leds.Color(0, 255, 0));
            leds.show();
            delay(10);
            }
         delay(100);
         } 
      // Fita LED vermelha indica "não" para resetar o WiFi
      else if (menu == 1) {
         Serial.println("Não resetar Wifi");
         for(int i=0; i<NUMPIXELS; i++){
            leds.setPixelColor(i, leds.Color(255, 0, 0));
            leds.show();
            delay(10);
            }
        delay(100);
        }
      }
      
//Cancela o modo de configuração e retorna ao menu inicial
if (digitalRead(botaoset) == HIGH & digitalRead(botaomais) == HIGH) {
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);
   return;
   }
  

//Fita LED pisca duas vezes verde indicando a opção escolhida   
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
        }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
delay(100);     

if (menu == 1) {
   estadomenu = 0;
   //Fita LED pisca duas vezes verde indicando a opção escolhida   
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 255, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 255, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);     
   return;
   }
if (menu == 0) {
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(255, 255, 0));
      leds.show();
      delay(10);
      }
    //Reset do Wifi
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    delay(3000); 
    wifiManager.autoConnect("SIDI"); //Nome da rede a ser criada pelo módulo Wifi
    while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
          }
    //Fita LED pisca duas vezes verde indicando a opção escolhida   
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 255, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 255, 0));
      leds.show();
      delay(10);
      }
   delay(100);
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(100);  
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);   
   estadomenu = 0;
   estadoled = 1;
   }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Função mconfigsensor
//Calibrar o sensor de umidade com base no solo seco e molhado.

void configsensor(){

//Fita LED marron indicando calibração a seco do sensor
while (digitalRead(botaoset) == LOW) {
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(210, 105, 30));
         leds.show();
         delay(10);
      }
      Serial.print("Sensor Umidade: ");
      Serial.println(analogRead(sensorumidade));
      superior = analogRead(sensorumidade);
      }

//Cancela o modo de configuração e retorna ao menu inicial
if (digitalRead(botaoset) == HIGH & digitalRead(botaomais) == HIGH) {
  for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);
   estadomenu = 0;
   return;
   }
 
Serial.println("Superior: ");
Serial.println(superior);

//Fita LED pisca duas vezes verde indicando a opção escolhida 
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }

//Salva na EEPROM o valor seco do sensor
esup = superior / 5;
EEPROM.write(0, esup);
EEPROM.commit();
delay(500);  
Serial.print("EEPROM Superior Write ");
Serial.println(EEPROM.read(0)); 
delay(2000);
//-----------------------------------------------------------------------
//Fita LED azul claro indicando calibração umida do sensor
while (digitalRead(botaoset) == LOW) {
      for(int i=0; i<NUMPIXELS; i++){
         leds.setPixelColor(i, leds.Color(0, 191, 255));
         leds.show();
         delay(10);
         }
      Serial.print("Sensor Umidade: ");
      Serial.println(analogRead(sensorumidade));
      inferior = analogRead(sensorumidade);
      }

//Cancela o modo de configuração e retorna ao menu inicial
if (digitalRead(botaoset) == HIGH & digitalRead(botaomais) == HIGH) {
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 255));
      leds.show();
      delay(200);
      }
   for(int i=0; i<NUMPIXELS; i++){
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
      delay(10);
      }
   delay(1000);
    estadomenu = 0;
    return;
    }
 
Serial.println("Inferior: ");
Serial.println(inferior);

//Fita LED pisca duas vezes verde indicando a opção escolhida
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 255, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   }
delay(100);
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 255));
   leds.show();
   delay(200);
   }
for(int i=0; i<NUMPIXELS; i++){
   leds.setPixelColor(i, leds.Color(0, 0, 0));
   leds.show();
   delay(10);
   
   }
delay(1000);

//Salva na EEPROM o valor umido do sensor
einf = inferior / 5;
EEPROM.write(1, einf);
EEPROM.commit();
delay(500);  
Serial.print("EEPROM Inferior Write: ");
Serial.println(EEPROM.read(1)); 
delay(2000);
menu = 0;
estadoled = 1;
estadomenu = 0;
range = superior - inferior;
delay(1000); 
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
