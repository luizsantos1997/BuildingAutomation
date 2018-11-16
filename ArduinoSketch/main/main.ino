/*
  BuildingAutomation
  Aplicação com o proposito de coordenar ativação e desativação
  automática de eletro-eletronicos em predios comerciais
*/

#include "IRremote.h" //biblioteca do controle remoto

//setar os sensores
const int sensorPIR = 3;
const int sensorTemperatura = 4;

//setar os atuadores
const int atuadorRele = 6;
const int atuadorControleInfravermelho = 7;
IRsend irSend;

//constantes utilizadas
const unsigned long comandoDeLigarArCondicionado = 0x39C600FF; //exemplo: 0x39C600FF
const unsigned long comandoDeDesligarArCondicionado;
const String comandoStringParaLigar = "l";
const String comandoStringParaDesligar = "d";


//setar variaveis de controle
bool estadoAr = false; // Controle de estado do ar condicionado
String serialReturn;
int statusSensorPIR = 0;
int statusSensorTemperatura = 0;

void setup() {
  Serial.begin(9600); //comunicação Serial

  //sensores
  pinMode( sensorPIR, INPUT );
  //o sensorTemperatura é analogico
  
  //atuadores
  pinMode( atuadorRele, OUTPUT );
  pinMode( atuadorControleInfravermelho, OUTPUT );
  receberTodosOsDadosExternos();
}

void loop() { 
    // Premissa maxima para evento. Os eventos so poderão acontecer no horário 
    if(serialReturn.equals(comandoStringParaLigar)){
        ligarEletronicos();
          if(isPresencaOn()){
                if(estadoAr == false){
                    ligarEletronicos();
                }else if(estadoAr == true){
                    if(statusSensorTemperatura > 23){
                        delay(300000); // esperar 5 min #### Delay para o processamento da CPU verificar outra maneira de esperar.
                    } // Testar se está gelando
                    if(statusSensorTemperatura > 14 && statusSensorTemperatura < 24){
                        delay(10000); // Sensor vai ler temperatura de 10 em 10 Segundos.
                    }

                }
          }
    }else if(serialReturn.equals(comandoStringParaDesligar)){
          if(estadoAr == true){      
              desligarEletronicos();
          }else if(estadoAr == false){
              if(statusSensorTemperatura > 23){
                delay(300000);                
              }
              if(statusSensorTemperatura > 14 && statusSensorTemperatura <24){
                delay(300000);
              }
          }
    }
  }

void ligarEletronicos(){
  digitalWrite( atuadorRele, HIGH );
  irSend.sendNEC( comandoDeLigarArCondicionado, 32);
  estadoAr = true;
}

void desligarEletronicos(){
  digitalWrite( atuadorRele, LOW );
  irSend.sendNEC(comandoDeDesligarArCondicionado, 32);
  estadoAr = false;
}

String receberDadosSerial(){
  if(Serial.available() > 0){
    serialReturn = Serial.readString();
    return serialReturn;
  }
    return "";
}
void receberDadosSensores(){
  statusSensorPIR = digitalRead( sensorPIR );
  statusSensorTemperatura = analogRead( sensorTemperatura );
}

void receberTodosOsDadosExternos(){
    receberDadosSensores();
    receberDadosSerial();
}

bool isPresencaOn(){
    if(statusSensorPIR == 1){
        return true;
    }
    return false;
}

