#include <WiFi.h>
#include "WebSocketConnection.h"
#include "Refeicao.h"
#include "MotorController.h"
#include "SensorCarga.h"

// Configurações de rede e servidor
const char* ssid = "SEU_SSID";          // Substitua pelo nome da sua rede Wi-Fi
const char* password = "SUA_SENHA";    // Substitua pela senha da sua rede Wi-Fi
const char* serverHost = "192.168.1.100"; // IP do servidor WebSocket
const int serverPort = 8080;              // Porta do servidor WebSocket

// Objetos globais
MotorController* motor;
SensorCarga* sensor;
Refeicao* refeicao;
WebSocketConnection* webSocketConnection;

void setup() {
    // Inicia a comunicação serial
    Serial.begin(115200);
    Serial.println("Inicializando...");

    // Conecta ao Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Conectando ao Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConectado ao Wi-Fi!");

    // Inicialização dos componentes
    motor = new MotorController(5); // Substitua pelo pino do servo motor
    sensor = new SensorCarga(2, 4, 1100.0); // Pinos DOUT, SCK e fator de calibração
    refeicao = new Refeicao(motor, sensor);

    // Inicializa o motor e configura a balança
    motor->iniciar();
    refeicao->configurarBalanca();

    // Inicializa o WebSocket
    webSocketConnection = new WebSocketConnection(serverHost, serverPort, refeicao);
    webSocketConnection->conectar();

    Serial.println("Sistema inicializado.");
}

void loop() {
    // Mantém a conexão WebSocket ativa
    webSocketConnection->loop();

    // Pequena pausa para evitar sobrecarga
    delay(10);
}
