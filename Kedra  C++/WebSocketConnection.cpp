#include "WebSocketConnection.h"

WebSocketConnection::WebSocketConnection(const char* host, int port, Refeicao* refeicao)
    : refeicao(refeicao) {
    webSocket.begin(host, port, "/");
    webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        this->handleWebSocketEvent(type, payload, length);
    });
}

void WebSocketConnection::conectar() {
    webSocket.setReconnectInterval(5000);
    webSocket.sendTXT("esp32-client");
    Serial.println("Conexão WebSocket estabelecida.");
}

void WebSocketConnection::handleWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        String acao = doc["acao"];

        // Evento: Distribuir Alimento
        if (acao == "distribuirAlimento") {
            float quantidade = doc["dados"]["quantidade"];
            refeicao->distribuirAlimento(quantidade);
            webSocket.sendTXT("{\"evento\":\"distribuirAlimento\",\"status\":\"sucesso\"}");

        // Evento: Calibrar Balança
        } else if (acao == "calibrarBalanca") {
            refeicao->configurarBalanca();
            webSocket.sendTXT("{\"evento\":\"calibrarBalanca\",\"status\":\"sucesso\"}");

        // Evento: Ler Peso Atual
        } else if (acao == "lerPesoAtual") {
            float pesoAtual = refeicao->obterPesoAtual();
            String resposta = "{\"evento\":\"lerPesoAtual\",\"peso\":" + String(pesoAtual) + "}";
            webSocket.sendTXT(resposta);

        // Evento: Status do Motor
        } else if (acao == "statusMotor") {
            bool motorAberto = refeicao->motor->estaAberto();
            String resposta = "{\"evento\":\"statusMotor\",\"aberto\":" + String(motorAberto) + "}";
            webSocket.sendTXT(resposta);

        } else {
            Serial.println("Ação não reconhecida: " + acao);
            webSocket.sendTXT("{\"evento\":\"erro\",\"mensagem\":\"Ação não reconhecida\"}");
        }
    }
}

void WebSocketConnection::loop() {
    webSocket.loop();
}
