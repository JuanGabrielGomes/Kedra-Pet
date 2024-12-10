#ifndef REFEICAO_H
#define REFEICAO_H

#include <Arduino.h>
#include "MotorController.h"
#include "SensorCarga.h"

class Refeicao {
private:
    MotorController* motor;
    SensorCarga* sensor;

public:
    Refeicao(MotorController* motor, SensorCarga* sensor);
    void configurarBalanca();
    float obterPesoAtual();
    void distribuirAlimento(float quantidade);
};

#endif
