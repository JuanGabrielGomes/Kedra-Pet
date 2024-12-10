#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Servo.h>

class MotorController {
private:
    Servo motor;
    int pin;
    int anguloAberto;
    int anguloFechado;

public:
    MotorController(int pin, int anguloAberto = 90, int anguloFechado = 0);
    void iniciar();
    void abrir();
    void fechar();
};

#endif
