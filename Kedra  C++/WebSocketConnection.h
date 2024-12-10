#ifndef WEBSOCKETCONNECTION_H
#define WEBSOCKETCONNECTION_H

#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "Refeicao.h"
#include "MotorController.h"
#include "SensorCarga.h"

class WebSocketConnection {
private:
    WebSocketsClient webSocket;
    Refeicao* refeicao;

    // Método privado para lidar com eventos WebSocket
    void handleWebSocketEvent(WStype_t type, uint8_t* payload, size_t length);

public:
    // Construtor
    WebSocketConnection(const char* host, int port, Refeicao* refeicao);

    // Métodos públicos
    void conectar();
    void loop();
};

#endif
