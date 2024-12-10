#include "SensorCarga.h"

SensorCarga::SensorCarga(int pinDOUT, int pinSCK, float fatorCalibracao)
    : pinDOUT(pinDOUT), pinSCK(pinSCK), fatorCalibracao(fatorCalibracao) {}

void SensorCarga::iniciar() {
    balanca.begin(pinDOUT, pinSCK);
    balanca.set_scale(fatorCalibracao);
    balanca.tare();  // Reseta o peso inicial para 0
    Serial.println("Sensor de carga iniciado.");
}

float SensorCarga::lerPeso() {
    if (balanca.is_ready()) {
        float peso = balanca.get_units(10); // Retorna a média de 10 leituras
        return peso > 0 ? peso : 0.0;       // Garante que o peso não seja negativo
    } else {
        Serial.println("Sensor de carga não está pronto!");
        return -1.0;  // Indica erro
    }
}

void SensorCarga::calibrar(float fatorCalibracao) {
    this->fatorCalibracao = fatorCalibracao;
    balanca.set_scale(fatorCalibracao);
    Serial.println("Balança calibrada.");
}
