#include "MotorController.h"

MotorController::MotorController(int pin, int anguloAberto, int anguloFechado)
    : pin(pin), anguloAberto(anguloAberto), anguloFechado(anguloFechado) {}

void MotorController::iniciar() {
    motor.attach(pin);
    fechar();  // Fecha o motor na inicialização
    Serial.println("Motor iniciado e fechado.");
}

void MotorController::abrir() {
    motor.write(anguloAberto);
    delay(1000);  // Aguarda 1 segundo para garantir a abertura completa
    Serial.println("Motor aberto.");
}

void MotorController::fechar() {
    motor.write(anguloFechado);
    delay(1000);  // Aguarda 1 segundo para garantir o fechamento completo
    Serial.println("Motor fechado.");
}
