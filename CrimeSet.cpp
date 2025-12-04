/*
 * Metodología de la Programación
 * Curso 2024/2025
 */

/** 
 * @file  CrimeSet.cpp
 * @note To be implemented by students
 * @author García Sollo, Miriam. 
 * Created on November 26, 2024, 11:04 AM
 */

#include <iostream>
#include <string>
#include <fstream>

#include "CrimeSet.h"
#include "DateTime.h"
#include "Crime.h"
#include "Coordinates.h"
using namespace std;

const string CrimeSet::MAGIC_STRING_T="MP-CRIME-T-1.0";
const float  CrimeSet::GROWING_RATIO = 1.5;


// Constructor con parametro size
CrimeSet::CrimeSet(int size) {
    _nCrimes = 0;
    _comment = "";
    
    if (size >= 0 && size < INITIAL_CAPACITY) _capacity = INITIAL_CAPACITY;    
    else if (size >= INITIAL_CAPACITY) _capacity = size;    
    else{
        _comment = "";
        _capacity = 0;
        _nCrimes = 0;
        _crimes = nullptr;
        
        throw std::out_of_range ("In CrimeSet::CrimeSet(int size): size cannot "
                "be negative.");
    }
    
    allocate(_capacity);
}

// Constructor de copia
CrimeSet::CrimeSet(const CrimeSet &orig) {
    copy(orig);
}

//Destructor
CrimeSet::~CrimeSet() {
    deallocate();
}

//Operador  de asignacion
CrimeSet &CrimeSet::operator=(const CrimeSet &orig) {
    if (this != &orig) {
        deallocate();
        copy(orig);
    }
    return *this;
}

// Metodo que devuelve el número de crímenes almacenados.
int CrimeSet::getSize() const {
    return _nCrimes;
}

// Metodo que devuelve la capacidad máxima del CrimeSet.
int CrimeSet::getCapacity() const {
    return _capacity;
}

// Metodo que devuelve el comentario asociado al CrimeSet.
std::string CrimeSet::getComment() const {
    return _comment;
}

// Metodo que establece un comentario en el CrimeSet.
void CrimeSet::setComment(const std::string &text) {
    _comment = text;
    
    //Añade un \n solo si no hay uno al final
    if (!_comment.empty() && _comment.back() != '\n') {
        _comment += '\n';
    }
}

// Metodo que devuelve una representación en string del CrimeSet.
std::string CrimeSet::toString() const {
    string crimes;
    
    // primera linea: numero de crimenes
    crimes += to_string(_nCrimes) + "\n";
    
    // Una linea por cada Crime 
    for (int i = 0; i < _nCrimes; i++){
       crimes += _crimes[i].toString() + "\n";
    }
    
    return crimes;
}

// Metodo que elimina todos los crímenes del CrimeSet.
void CrimeSet::clear() {
    deallocate();
    allocate(INITIAL_CAPACITY);
    _nCrimes = 0;
    _comment = "";
}

// Añade un crimen al conjunto, evitando duplicados
bool CrimeSet::append(const Crime &crime) {
    bool appended = false;

    if (findCrime(crime, 0, _nCrimes - 1) == -1) {
        if (_nCrimes == _capacity) {
            int new_capacity = (int)(ceil(_nCrimes * GROWING_RATIO));
            if (new_capacity <= _capacity)
                new_capacity = _nCrimes + 1;

            reallocate(new_capacity);
        }

        _crimes[_nCrimes] = crime;
        _nCrimes++;
        appended = true;
    }

    return appended;
}

// Devuelve el crimen en la posición especificada.
const Crime &CrimeSet::at(int pos) const{
    
    //Excepcion out of range
    if (pos < 0 || pos >= _nCrimes)
        throw out_of_range("In const Crime &CrimeSet::at(int pos) const: "
                "The provided position is not valid");
    
    return _crimes[pos];

}

// Busca un crimen en un rango determinado del CrimeSet.
int CrimeSet::findCrime(const Crime &crime, int initialPos, int finalPos) const {
    int result;    
    int i = initialPos;
    bool found = false;
    
    while (i <= finalPos && !found) {
        if (_crimes[i].getId() == crime.getId()) {
            result = i;
            found = true;
        }
        else i++;
    }    
    
    if (!found) result = -1;
    
    return result;
}

// Busca un crimen en todo el conjunto.
int CrimeSet::findCrime(const Crime &crime) const {
    return findCrime(crime, 0, this->getSize()- 1);
}

// Carga la información de un CrimeSet desde un archivo.
void CrimeSet::load(const std::string &fileName) {
    clear(); // Elimina los datos actuales del objeto
    
    // Abre el archivo de entrada
    ifstream entrada(fileName);
    if (!entrada) {
        throw std::ios_base::failure("In CrimeSet::load(): cannot open file");
    }
    
    // Lee cadena mágica
    string magic_string;
    getline(entrada, magic_string);
    if (magic_string != MAGIC_STRING_T) {
        clear();
        throw std::invalid_argument("In CrimeSet::load(): invalid magic string");
    }

    readComments(entrada); // Lee los comentarios del archivo
    
    // Lee el número de crímenes
    int num_crimes;
    entrada >> num_crimes;
    entrada.ignore(); // Salta salto de línea 

    if (num_crimes < 0) { // Valida el número de crímenes 
        clear();
        throw std::out_of_range("In CrimeSet::load(): negative number of crimes");
    }

    // Redimensionar si no es suficiente capacidad
    // Asegura que la capacidad sea suficiente para alojar todos los crímenes
    reallocate(num_crimes);

    // Establece el tamaño lógico
    _nCrimes = num_crimes;

    
    // Lee cada crimen del archivo y lo pone en el array
    string crime_str;
    for (int i = 0; i < num_crimes; i++) {
        getline(entrada, crime_str);
        _crimes[i].set(crime_str);
    }

    if (!entrada) {
        clear();
        throw std::ios_base::failure("In CrimeSet::load(): error reading from file");
    }

    entrada.close(); // Cierra el archivo
}
   


// Guarda el CrimeSet en un archivo.
void CrimeSet::save(const std::string &fileName) const {
     // Abrir el archivo
    std::ofstream output_file(fileName);

    // Lanza una excepción si el archivo no se puede abrir
    if (!output_file) {
        throw std::ios_base::failure("In void CrimeSet::save(const std::string "
                        "fileName) const:\n the given file cannot be opened");
    } else {
        // Escribe la cadena mágica 
        output_file << MAGIC_STRING_T << endl;

        // Escribe los comentarios 
        saveComments(output_file);

        // Escribe el número de crímenes
        output_file << _nCrimes << "\n";

        // Escribe cada crimen en una línea
        for (int i = 0; i < _nCrimes; i++) {
            output_file << _crimes[i].toString() << "\n";
        }

        // Comprueba si ha habido error durante la escritura
        if (!output_file) {
            throw std::ios_base::failure("In void "
                    "CrimeSet::save(const std::string fileName) const:\n"
                    " there's an error while writing in the file.");
        }
    }

    // Cierra el archivo
    output_file.close();

}

// Metodo para unir otro CrimeSet al actual con crimenes no duplicados
void CrimeSet::join(const CrimeSet &crimeSet) {
    int num_crimes = crimeSet.getSize();
    
    for (int i = 0; i < num_crimes; i++) {
        if (findCrime(crimeSet.at(i)) == -1) {
            append(crimeSet.at(i));
        }
    }
}

// Normaliza los datos de los crímenes en el conjunto.
void CrimeSet::normalize() {
    for (int i = 0; i < _nCrimes; i++) {
        _crimes[i].normalize();
    }
}

// Calcula un histograma de crímenes por día de la semana u hora del día.
void CrimeSet::computeHistogram(int dataField, int histogram[]) const {
    
    // Excepcion out of range
    if (dataField != 0 && dataField != 1)
        throw out_of_range("In void CrimeSet::computeHistogram(int dataField, "
                "int histogram[]: dataField is neither 0 nor 1");
    
    //Asignar tamaño del histograma segun si es dia o horas
    int size;
    if (dataField == 0) size = 7;
    else size = 24;
        
    InitializeArrayInts(histogram, size);   //inicializa con ceros

    for (int i = 0; i < _nCrimes; i++) {
        DateTime date = _crimes[i].getDateTime();
        
        if (dataField == 0) {   //Aumentar frecuencia del dia correspondiente
            int day = date.day();
            histogram[day]++;
            
        } else {    //Aumentar frecuencia de la hora correspondiente
            int hour = date.hour();
            histogram[hour]++;
        }
    }
}

// Devuelve un nuevo CrimeSet con los crímenes que cumplen la condición.
CrimeSet CrimeSet::selectWhereEQ(const std::string &field, 
        const std::string &value) const {
    
    CrimeSet selected;
    
     // Copia el comentario y añade la restriccion
    selected.setComment(_comment + "Restricted to " + field + " = " + value);

    // Añade los crímenes que cumplen con la condición
    for (int i = 0; i < _nCrimes; i++) {
        
        if (_crimes[i].getField(field) == value) {            
            selected.append(_crimes[i]);
        }
    }
    
    return selected;    
}

// Devuelve un nuevo CrimeSet con los crímenes que tienen coordenadas válidas
CrimeSet CrimeSet::selectValidLocation() const {
    CrimeSet selected;
    
    // Copia el comentario y añade la restriccion
    selected.setComment(_comment + "Restricted to valid Coordinates");

    // Añade los crímenes que tienen coordenadas validas
    for (int i = 0; i < _nCrimes; i++) {
        if (_crimes[i].getLocation().isValid()) {
            selected.append(_crimes[i]);
        }
    }

    return selected;
}

// Ordena los crímenes por fecha y, en caso de empate, por ID.
void CrimeSet::sort() {    
    // Algoritmo de ordenación burbuja 
    for (int i = 0; i < _nCrimes - 1; i++) {        
        for (int j = i + 1; j < _nCrimes; j++) {            
            if (_crimes[j].getDateTime() < _crimes[i].getDateTime() ||
                (_crimes[j].getDateTime() == _crimes[i].getDateTime() &&
                 _crimes[j].getId() < _crimes[i].getId())) {
                
                // Intercambia los crímenes
                swap(_crimes[i], _crimes[j]);
            }
        }
    }
}

//Métodoo at modificador
Crime& CrimeSet::at(int pos) {
        // Excepción si la posición no es válida
    if (pos < 0 || pos >= _nCrimes)
        throw out_of_range("In Crime &CrimeSet::at(int pos): "
                           "The provided position is not valid");
    
    return _crimes[pos];
}

// Lee comentarios de archivo y los guarda el _comment del CrimeSet.
void CrimeSet::readComments(std::istream &inputStream) {
    _comment = "";  //Inicializar como cadena vacia
    string line;    //Para guardar cada linea
    char next_char;  //Para ver el siguiente caracter del fluj    
    bool seguir = true;    
    
    while (seguir) {
        next_char = inputStream.peek(); // Miramos el primer carácter de la línea 
        
        // Si es una línea de comentario, la leemos entera
        if (next_char == '#') {
            getline(inputStream, line);            
             // Eliminamos el '#' y añadimos el resto más un '\n'
            _comment += line.substr(1) + '\n';            
        } else {
            seguir = false; 
        }
    }
}

// Guarda los comentarios en un flujo de salida
void CrimeSet::saveComments(std::ostream &outputStream) const {
    outputStream << FormatAsComment(_comment, '#');
}

// Funciones externas
string FormatAsComment(const string &comment, char commentCharacter){
    size_t start = 0, end;
    string formatedComment;
    if (comment.size() > 0) { // If comment has at least one character
        while ((end = comment.find('\n', start)) != std::string::npos) {
            formatedComment += commentCharacter + comment.substr(start, end - start) + "\n";
            start = end + 1;
        }
        if(start<comment.size()){ // This happens when last line of comment does not have \n
            formatedComment += commentCharacter + comment.substr(start, comment.size() - start) + "\n";
        }
    }
    return formatedComment;
}

void InitializeArrayInts(int array[], int size){    
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

void PrintHistogramArrayCrimes(int dataField, const int histogram[]) {
    if (dataField == 0) {
        for (int i = 0; i < 7; i++)
            cout << DateTime::dayName(i) << " " << histogram[i] << endl;
    }
    else if (dataField == 1) {
        for (int i = 0; i < 24; i++)
            cout << i << " " << histogram[i] << endl;
    }
    else cout << "" << endl;
}

void CrimeSet::allocate(int capacity) {
    _crimes = nullptr;
    _comment = "";
    _nCrimes = 0;
    
    if (capacity > 0) {
        _capacity = capacity;
        _crimes = new Crime[_capacity];
    }
}

void CrimeSet::deallocate() {
    if (_crimes != nullptr) {
        delete[] _crimes;
        _crimes = nullptr;
    }
    _comment = "";
    _nCrimes = 0;
    _capacity = 0;
}

void CrimeSet::reallocate(int capacity) {
    if (capacity < _nCrimes) capacity = _nCrimes;
    
    Crime *aux = new Crime [capacity];
        
    for (int i = 0; i < capacity && i < _nCrimes; i++)
        aux[i] = _crimes[i];
    
    delete[] _crimes;
    _crimes = aux;
    _capacity = capacity;  
}

void CrimeSet::copy(const CrimeSet &other) {
    allocate(other._capacity);
    
    for (int i = 0; i < other._nCrimes; i++)
        _crimes[i] = other._crimes[i];
    
    _nCrimes = other._nCrimes;
    _comment = other._comment;
}

// Operador de salida <<
std::ostream &operator<<(std::ostream &os, const CrimeSet &crimeSet) {
    os << crimeSet.getSize() << "\n";
    
    for (int i = 0; i < crimeSet.getSize(); i++) {
        os << crimeSet.at(i).toString() << "\n";
    }
    
    return os;
}

// Operador de entrada >>
std::istream &operator>>(std::istream &is, CrimeSet &crimeSet) {
    crimeSet.clear();

    int n;
    is >> n;
    is.ignore();  // Ignorar el salto de línea después del número

    if (n < 0) {
        crimeSet.clear();
        throw std::out_of_range("In CrimeSet operator>>: negative number of crimes");
    }

    for (int i = 0; i < n; i++) {
        string entrada;
        getline(is, entrada);
        Crime crime;
        crime.set(entrada);
        crimeSet.append(crime);
    }

    return is;
}

// Operadores []
Crime CrimeSet::operator[](int index) const{
    return at(index);  
}

Crime CrimeSet::operator[](int index) {
    return at(index);  
}

// Operador +=
CrimeSet CrimeSet::operator+=(CrimeSet other) {
    int n = other.getSize();
    for (int i = 0; i < n; i++) {
        append(other.at(i));
    }
    return *this;
}

