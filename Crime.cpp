/*
 * Metodología de la Programación
 * Curso 2024/2025
 */

/** 
 * @file Crime.cpp
 * @author estudiante: García Sollo, Miriam.
 * 
 */

#include "Crime.h"

using namespace std;

/*
 * Initialization of the static string that contains the value assigned to any 
 * string field (string) which is found empty in the data of a Crime
 */
const std::string Crime::UNKNOWN_VALUE = "UNKNOWN";

/*
 * Initialization of the static string that contain the default value for the
 * DateTime field in a Crime 
 */
const string Crime::DATETIME_DEFAULT = "2017-01-20 02:00:00";

/* 
 * Initialization of the static string with the default values for every field in 
 * a Crime
 */
const string Crime::CRIME_DEFAULT("0," + UNKNOWN_VALUE + "," + UNKNOWN_VALUE + "," +
        UNKNOWN_VALUE + "," + UNKNOWN_VALUE + "," + UNKNOWN_VALUE + "," + UNKNOWN_VALUE +
        "," + ((std::string)"1") + "," + Crime::DATETIME_DEFAULT + "," +
        UNKNOWN_VALUE + ",181,181");

// Consstructor por defecto
Crime::Crime() : _counter(0), _id(UNKNOWN_VALUE), _code(UNKNOWN_VALUE), 
      _group(UNKNOWN_VALUE), _description(UNKNOWN_VALUE), 
      _district(UNKNOWN_VALUE), _areaReport(UNKNOWN_VALUE), _shooting(true), 
      _dateTime(DATETIME_DEFAULT), _street(UNKNOWN_VALUE), 
      _location(181.0, 181.0) {} 


// Constructor con una linea de archivo csv
Crime::Crime(const string &line) {
    set(line);  //Llamada a metodo set para no repetir código
}

// Métodos de consulta
int Crime::getCounter() const {
    return _counter;
}

std::string Crime::getId() const {
    return _id;
}

std::string Crime::getCode() const {
    return _code;
}

std::string Crime::getGroup() const {
    return _group;
}

std::string Crime::getDescription() const {
    return _description;
}

std::string Crime::getDistrict() const {
    return _district;
}

std::string Crime::getStreet() const {
    return _street;
}

std::string Crime::getAreaReport() const {
    return _areaReport;
}

bool Crime::isShooting() const {
    return _shooting;
}

DateTime Crime::getDateTime() const {
    return _dateTime;
}

Coordinates Crime::getLocation() const {
    return _location;
}

bool Crime::isIDUnknown() const {
    return (_id == UNKNOWN_VALUE);
}


// Método para convertir el objeto Crime en una cadena 
std::string Crime::toString() const {
    string shooting_str = "0";
    if (_shooting) {
        shooting_str = "1";
    } 
    
    return to_string(_counter) + "," + _id + "," + _code + "," + _group + 
            "," + _description + "," + _district + "," + _areaReport + "," + 
            shooting_str + "," + _dateTime.toString() + "," + _street + "," + 
            _location.toString();
}

// Métodos modificadores
void Crime::setCounter(int c) {
    _counter = c;
}


void Crime::setId(const std::string &id) {
    string trimmedId = id;
    Trim(trimmedId);
    
    //Excepción
    if (trimmedId.size() == 0) {
        throw std::invalid_argument(
                std::string("In void Crime::setId(const std::string &id) :")+
                "\nid is empty");
    } 
    this->_id = trimmedId;
}

void Crime::setCode(const std::string &code) {
    _code = code;
}


void Crime::setGroup(const std::string &group) {
    _group = group;
}


void Crime::setDescription(const std::string &description) {
    _description = description;
}


void Crime::setDistrict(const std::string &district) {
    _district = district;
}


void Crime::setAreaReport(const std::string &areaReport) {
    _areaReport = areaReport;
}


void Crime::setStreet(const std::string &street) {
    _street = street;
}


void Crime::setShooting(bool shooting) {
    _shooting = shooting;
}


void Crime::setDateTime(const string & time) {
    _dateTime.set(time);
}


void Crime::setLocation(const Coordinates & coordinates) {
    _location = coordinates;
}

void Crime::set(const std::string & line) {
    const int NFIELDS = 12;
    std::string data;
    size_t pos = 0, posn;

    // Verificar si la línea está vacía
    if (line.empty()) {
        throw std::invalid_argument(
            std::string("In void Crime::set(const std::string & line):") +
            "\nline is empty");
    }

    int nfield = 0;
    while (nfield < NFIELDS - 1) {  // Procesar campos hasta NFIELDS -1
        posn = line.find(',', pos); // Buscar la coma como delimitador

        // Si la posición no es una coma, tomar el resto de la línea
        if (posn == std::string::npos) {
            data = line.substr(pos);
            pos = line.size();  // Mover al final para salir del bucle
        } else {    // Extraer el resto del campo
            data = line.substr(pos, posn - pos);
            pos = posn + 1; // Omitir la coma y pasar a la siguiente posición
        }

        // Asignar el valor al campo correspondiente según su índice
        switch (nfield) {
            case 0:
                if (!data.empty()) 
                    setCounter(stoi(data));
                break;
            case 1:
                setId(data);
                break;
            case 2:
                setCode(data);
                break;
            case 3:
                setGroup(data);
                break;
            case 4:
                setDescription(data);
                break;
            case 5:
                setDistrict(data);
                break;
            case 6:
                setAreaReport(data);
                break;
            case 7:
                if (!data.empty())
                    setShooting(stoi(data) != 0);  // cualquier valor != 0 se considera true
                else
                    setShooting(false);            // vacío se interpreta como false
                break;
            case 8:
                setDateTime(data);
                break;
            case 9:
                setStreet(data);
                break;
            case 10:
                if (!data.empty()) 
                    _location.setLatitude(stof(data));
                else 
                    _location.setLatitude(181.0);
                break;
        }

        nfield++;
    }

    // Procesar el último campo (longitud)
    if (pos < line.size()) {
        std::string lastData = line.substr(pos);

        if (lastData.empty() || lastData == "UNKNOWN") {
            _location.setLongitude(181.0);  // Valor por defecto si el dato es UNKNOWN
        } else {
            _location.setLongitude(stof(lastData));  // Convertir a float
        }
    } else {
        _location.setLongitude(181.0);
    }
}



// FUNCIONES EXTERNAS

// Función para borrar espacios y tabuladores del principio y final del string
void Trim(std::string &myString) {
    size_t first = myString.find_first_not_of(" \t");
    size_t last = myString.find_last_not_of(" \t");

    if (first == std::string::npos) {
        myString.clear();
    } else {
        myString = myString.substr(first, last - first + 1);
    }
}


// Función para pasar a mayuscula todas las letras de un string
void Capitalize(string & myString) {
    for (size_t i = 0; i < myString.length(); i++) {
        myString.at(i) = toupper(myString.at(i));
        
    }
}

// Función para normalizar (pasar a mayuscula y borrar espacios) los campos de un crimen

void Normalize(Crime &crime) {
    std::string normalized;
    
    // Normalizar ID (solo es necesario pasar a mayúscula)
    normalized = crime.getId();
    Trim(normalized);
    Capitalize(normalized);
    crime.setId(normalized);
    
    // Normalizar grupo
    normalized = crime.getGroup();
    Trim(normalized);
    Capitalize(normalized);
    crime.setGroup(normalized);
    
    // Normalizar descripción
    normalized = crime.getDescription();
    Trim(normalized);
    Capitalize(normalized);
    crime.setDescription(normalized);
    
    // Normalizar distrito
    normalized = crime.getDistrict();
    Trim(normalized);
    Capitalize(normalized);
    crime.setDistrict(normalized);
    
    // Normalizar calle
    normalized = crime.getStreet();
    Trim(normalized);
    Capitalize(normalized);
    crime.setStreet(normalized);
}


//Método que obtiene el valor de un campo específico del crimen en formato string.
std::string Crime::getField(const std::string &field) const{
    string field_str;
    
    if (field == "Counter") field_str = to_string(_counter);
    else if (field == "ID") field_str = _id;
    else if (field == "Code") field_str = _code;
    else if (field == "Group") field_str = _group;
    else if (field == "Description") field_str = _description;
    else if (field == "District") field_str = _district;
    else if (field == "Area") field_str = _areaReport;
    else if (field == "Street") field_str = _street;
    else if (field == "Shooting"){
        if (_shooting) field_str = "1";
        else field_str = "0";
    }
    else if (field == "DateTime") field_str = _dateTime.toString();
    else if (field == "Location") field_str = _location.toString();
    else throw std::invalid_argument
            (std::string("In std::string Crime::getField(const ") +
            "std::string &field) const: Campo no valido: " + field);
    
    return field_str;
}

//Método que normaliza los campos string del crimen, eliminando espacios innecesarios.
void Crime::normalize() {
    Capitalize(_id);

    Trim(_code);
    Capitalize(_code);

    Trim(_group);
    Capitalize(_group);

    Trim(_description);
    Capitalize(_description);

    Trim(_district);
    Capitalize(_district);

    Trim(_areaReport);
    Capitalize(_areaReport);

    Trim(_street);
    Capitalize(_street);
}


#include <iostream>
using namespace std;

// Sobrecarga del perador de salida <<
std::ostream &operator<<(std::ostream &os, const Crime &crime) {
    string shooting_str;
    
    if (crime.isShooting())  shooting_str = "1";
    else shooting_str = "0";

    os << crime.getCounter() << ","
       << crime.getId() << ","
       << crime.getCode() << ","
       << crime.getGroup() << ","
       << crime.getDescription() << ","
       << crime.getDistrict() << ","
       << crime.getAreaReport() << ","
       << shooting_str << ","
       << crime.getDateTime().toString() << ","
       << crime.getStreet() << ","
       << crime.getLocation().toString();

    return os;
}

// Sobrecarga del perador de entrada >>
std::istream &operator>>(std::istream &is, Crime &crime) {
    string entrada;
    getline(is, entrada);
    if (!entrada.empty()) {
        crime.set(entrada);
    }
    return is;
}

// Operador <
bool operator<(const Crime &crime1, const Crime &crime2) {
    bool menor = false;

    if (crime1.getDateTime() < crime2.getDateTime()) {
        menor = true;
    } else if (!(crime2.getDateTime() < crime1.getDateTime()) && 
            crime1.getId() < crime2.getId()) {
        menor = true;
    }

    return menor;
}

// Operador >
bool operator>(const Crime &crime1, const Crime &crime2) {
    return crime2 < crime1;
}

// Operador ==
bool operator==(const Crime &crime1, const Crime &crime2) {
    bool igual = false;
    if (!(crime1 < crime2) && !(crime1 > crime2)) {
        igual = true;
    }
    return igual;
}

// Operador !=
bool operator!=(const Crime &crime1, const Crime &crime2) {
    return !(crime1 == crime2);
}

// Operador <=
bool operator<=(const Crime &crime1, const Crime &crime2) {
    bool menor_igual = false;
    if ((crime1 < crime2) || (crime1 == crime2)) {
        menor_igual = true;
    }
    return menor_igual;
}

// Operador >=
bool operator>=(const Crime &crime1, const Crime &crime2) {
    bool mayor_igual = false;
    if ((crime1 > crime2) || (crime1 == crime2)) {
        mayor_igual = true;
    }
    return mayor_igual;
}


