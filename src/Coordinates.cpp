/*
 * Metodología de la Programación
 * Curso 2024/2025
 */

/** 
 * @file Coordinates.cpp  
 * @author : García Sollo, Miriam.
 */

#include "Coordinates.h"
#include <string>

const float Coordinates::INVALID_COORDINATE=181.0;

using namespace std;

//Constructor
Coordinates::Coordinates(float latitude, float longitude) {
    _latitude = latitude;
    _longitude = longitude;
}

// Métodos de consulta
float Coordinates::getLatitude() const {
    return _latitude;
}

float Coordinates::getLongitude() const {
    return _longitude;
}

bool Coordinates::isValid() const {
    return (_latitude >= -90 && _latitude <= 90) && 
            (_longitude >= -180 && _longitude <= 180);
}

// Métodos modificadores
void Coordinates::set(float latitude, float longitude){
    _latitude = latitude;
    _longitude = longitude;
}

void Coordinates::setLatitude(float latitude) {
    _latitude = latitude;
}

void Coordinates::setLongitude(float longitude) {
    _longitude = longitude;
}

// Método para obtener una representación en string
std::string Coordinates::toString() const {
    return to_string(_latitude) + "," + to_string(_longitude);
}

// Método para calcular la dimensión de un rectángulo entre dos coordenadas
Coordinates Coordinates::length(const Coordinates &second) const{
    float lat = abs(second.getLatitude() - _latitude);
    float longit = abs(second.getLongitude() - _longitude);
    return Coordinates(lat, longit);
}                        

// Método para verificar si una coordenada está dentro de un rectángulo definido
bool Coordinates::isInsideArea(const Coordinates &bottomLeft, 
        const Coordinates &topRight) const {
    
    return (_latitude >= bottomLeft.getLatitude() && 
            _latitude <= topRight.getLatitude() &&
            _longitude >= bottomLeft.getLongitude() && 
            _longitude <= topRight.getLongitude());
}  

// Sobrecarga del operador <<
std::ostream &operator<<(std::ostream &os, Coordinates obj) {
    os << std::to_string(obj.getLatitude()) << "," << std::to_string(obj.getLongitude());
    return os;
}

// Sobrecarga del operador >>
std::istream &operator>>(std::istream &is, Coordinates obj) {
    float latitude, longitude;
    char separador;

    is >> latitude;
    is >> separador;
    is >> longitude;

    if (separador == ',') {
        obj.set(latitude, longitude);
    }

    return is;
}
