#include <NewPing.h>
#include <DHTesp.h>
#include <EspMQTTClient.h>

#define TRIGGER_PIN 12 // Pino de disparo do sensor
#define ECHO_PIN 14    // Pino de eco do sensor
#define MAX_DISTANCE 200 // Distância máxima de leitura do sensor (em centímetros)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//Definicoes e constantes
char SSIDName[] = "Wokwi-GUEST"; //nome da rede WiFi
char SSIDPass[] = ""; //senha da rede WiFI

const int DHT_PIN = 15; //terminal do sensor de temperatura e umidade

char BrokerURL[] = "broker.hivemq.com"; //URL do broker MQTT
char BrokerUserName[] = ""; //nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; //senha para autenticar no broker MQTT
char MQTTClientName[] = ""; //nome do cliente MQTT
int BrokerPort = 1883; //porta do broker MQTT

float cm;
String TopicoPrefixo = "OBJ-MACK-321"; //prefixo do topico
String Topico_01 = TopicoPrefixo+"/altura"; //nome do topico 01


//Variaveis globais e objetos
DHTesp dhtSensor; //instancia o objeto dhtSensor a partir da classa DHTesp

EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); //inicializa o cliente MQTT

//Este prototipo de funcao deve estar sempre presente
void onConnectionEstablished() {
}

void enviarDados() {
   if (clienteMQTT.isWifiConnected() == 1) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected() == 1) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }
  cm = 0.0344/2 * readUltrasonicDistance(12, 14);
  //unsigned int distance = sonar.ping_cm(); // Realiza a leitura da distância em centímetros
  
  clienteMQTT.publish(Topico_01, String(cm)); 


}

long readUltrasonicDistance(int triggerPin, int echoPin)
{
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    return pulseIn(echoPin, HIGH);
}

void setup() {
  Serial.begin(115200);
   clienteMQTT.enableDebuggingMessages();
}

void loop() {

  clienteMQTT.loop(); //funcao necessaria para manter a conexao com o broker MQTT ativa e coletar as mensagens recebidas
 
  delay(5000); // Aguarda 500ms entre as leituras
  
  
 

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

    enviarDados(); //funcao para publicar os dados no broker MQTT

}
