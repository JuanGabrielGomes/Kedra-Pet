#include "Refeicao.h"

Refeicao::Refeicao(MotorController* motor, SensorCarga* sensor)
    : motor(motor), sensor(sensor) {}

void Refeicao::configurarBalanca() {
    Serial.println("Configurando balança...");
    sensor->iniciar();
    sensor->calibrar(1100.0); // Fator de calibração ajustável
    Serial.println("Balança configurada.");
}

float Refeicao::obterPesoAtual() {
    float pesoAtual = sensor->lerPeso();
    Serial.println("Peso atual lido: " + String(pesoAtual) + "g");
    return pesoAtual;
}

void Refeicao::distribuirAlimento(float quantidade) {
    Serial.println("Iniciando distribuição de alimento...");
    motor->abrir();
    while (sensor->lerPeso() < quantidade) {
        delay(500);  // Aguarda meio segundo para verificar o peso novamente
    }
    motor->fechar();
    Serial.println("Distribuição concluída!");
}
