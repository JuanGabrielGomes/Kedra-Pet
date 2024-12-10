#ifndef SENSORCARGA_H
#define SENSORCARGA_H

#include <Arduino.h>
#include <HX711.h>

class SensorCarga {
private:
    HX711 balanca;
    int pinDOUT;
    int pinSCK;
    float fatorCalibracao;

public:
    SensorCarga(int pinDOUT, int pinSCK, float fatorCalibracao);
    void iniciar();
    float lerPeso();
    void calibrar(float fatorCalibracao);
};

#endif
