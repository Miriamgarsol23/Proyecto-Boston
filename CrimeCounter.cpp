/*
 * Metodología de la Programación
 * Curso 2024/2025
 */

/** 
 * @file:  CrimeCounter.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * Created on September 24, 2024, 3:19 PM
 */

#include <iostream>
#include <fstream>
#include <cmath>

#include "CrimeCounter.h"
using namespace std;


/**
 * DEFAULT_COORD_BOTTOMLEFT is set to the bottom left corner of the Boston area
 */
const Coordinates CrimeCounter::DEFAULT_COORD_BOTTOMLEFT(42.207760,-71.178673);  

/**
 * DEFAULT_COORD_TOPRIGHT is set to the top right corner of the Boston area
 */
const Coordinates CrimeCounter::DEFAULT_COORD_TOPRIGHT(42.395042,-70.953728);  


//Constructor de la clase
CrimeCounter::CrimeCounter(int nRows, int nCols, Coordinates bottomLeftCoord, Coordinates topRightCoord) {
    if (nRows > 0) _nRows = nRows;
    else _nRows = 0;
    
    if (nCols > 0) _nCols = nCols;
    else _nCols = 0;
    
    if (!bottomLeftCoord.isValid() || !topRightCoord.isValid()) {
        _nRows = 0;
        _nCols = 0;
        _bottomLeftCoord = Coordinates();
        _topRightCoord = Coordinates();
        _frequency = nullptr;
        
        throw invalid_argument("In CrimeCounter::CrimeCounter(int nRows," 
                "int nCols, Coordinates bottomLeftCoord, Coordinates topRightCoord) {"
                "Coordinates not valid");
    } else{
        _bottomLeftCoord = bottomLeftCoord;
            _topRightCoord = topRightCoord;
            _frequency = nullptr;

            if (_nRows > 0 && _nCols > 0) {
                _frequency = new int*[_nRows];
                for (int i = 0; i < _nRows; i++) {
                    _frequency[i] = new int[_nCols];
                    for (int j = 0; j < _nCols; j++)
                        _frequency[i][j] = 0;
                }
            }
    }
}

// Constructor de copia
CrimeCounter::CrimeCounter(const CrimeCounter &orig) {
    _nRows = orig._nRows;
    _nCols = orig._nCols;
    _bottomLeftCoord = orig._bottomLeftCoord;
    _topRightCoord = orig._topRightCoord;

    if (_nRows > 0 && _nCols > 0) {
        _frequency = new int*[_nRows];
        for (int i = 0; i < _nRows; i++) {
            _frequency[i] = new int[_nCols];
            for (int j = 0; j < _nCols; j++)
                _frequency[i][j] = orig._frequency[i][j];
        }
    } else {
        _frequency = nullptr;
    }
}

// Destructor
CrimeCounter::~CrimeCounter() {
    if (_frequency != nullptr) {
        for (int i = 0; i < _nRows; i++)
            delete[] _frequency[i];
        delete[] _frequency;
    }
}

// Operador asignación
CrimeCounter& CrimeCounter::operator=(const CrimeCounter &orig) {
    if (this != &orig) {
        this->~CrimeCounter();
        _nRows = orig._nRows;
        _nCols = orig._nCols;
        _bottomLeftCoord = orig._bottomLeftCoord;
        _topRightCoord = orig._topRightCoord;

        if (_nRows > 0 && _nCols > 0) {
            _frequency = new int*[_nRows];
            for (int i = 0; i < _nRows; i++) {
                _frequency[i] = new int[_nCols];
                for (int j = 0; j < _nCols; j++)
                    _frequency[i][j] = orig._frequency[i][j];
            }
        } else {
            _frequency = nullptr;
        }
    }
    return *this;
}


// Métodos de consulta
int CrimeCounter::getNumRows() const {
    return _nRows;
}

int CrimeCounter::getNumCols() const {
    return _nCols;
}

Coordinates CrimeCounter::getLeftLocation() const {
    return _bottomLeftCoord;
}

Coordinates CrimeCounter::getRightLocation() const {
    return _topRightCoord;
}

string CrimeCounter::toString() const {
    string cadena = to_string(_nRows) + " " + to_string(_nCols) + "\n";
    cadena += _bottomLeftCoord.toString() + "\n";
    cadena += _topRightCoord.toString() + "\n";
    for (int i = 0; i < _nRows; i++) {
        for (int j = 0; j < _nCols; j++) {
            cadena += to_string(_frequency[i][j]);
            if (j < _nCols - 1) cadena += " ";
        }
        cadena += "\n";
    }
    return cadena;
}

int CrimeCounter::getMaxFrequency() const {
    int max_freq = 0;
    for (int i = 0; i < _nRows; i++)
        for (int j = 0; j < _nCols; j++)
            if (_frequency[i][j] > max_freq)
                max_freq = _frequency[i][j];
    return max_freq;
}

int CrimeCounter::getTotalLocated() const {
    int total = 0;
    for (int i = 0; i < _nRows; i++)
        for (int j = 0; j < _nCols; j++)
            total += _frequency[i][j];
    return total;
}

// clear
void CrimeCounter::clear() {
    for (int i = 0; i < _nRows; i++) {
        
        for (int j = 0; j < _nCols; j++) {
            _frequency[i][j] = 0;
        }
    }
}

// Aumentar frecuencia
void CrimeCounter::increaseFrequency(const Crime &crime, int frequency) {
    Coordinates loc = crime.getLocation();
    if (loc.isValid() && loc.isInsideArea(_bottomLeftCoord, _topRightCoord)) {
        int row, col;
        getRowColumn(loc, row, col);
        if (frequency == 0)
            frequency = 1;
        _frequency[row][col] += frequency;
    }
}

// calcular frequencias
void CrimeCounter::calculateFrequencies(const CrimeSet &crimes) {
    clear();
    int size = crimes.getSize();
    for (int i = 0; i < size; i++)
        increaseFrequency(crimes.at(i));
}

void CrimeCounter::saveAsPPMTextImage(const string &fileName,
                                      const ColorPalette &palette,
                                      const string &comment) const {
    int max = getMaxFrequency();// Compute maxFrequency as the maximum frequency in the frequency matrix
    
    // Save heading and comments into the ppm file
    ofstream salida;
    salida.open(fileName);
    
    if(salida){
        salida << "P3" << endl;
        salida << comment;
        salida << _nCols << " " << _nRows << endl;
        salida << "255" << endl;
        
        // For every cell in the frequency matrix
            // colorIndex = GetIndexPalette(palette, 
                  //log(this->_frequency[row][column]+1), log(maxFrequency+1));
            // Save into the ppm file the RGB components of the color at 
                  //the position colorIndex of the palette
        for (int i = 0; i < _nRows; i++){
            for (int j = 0; j < _nCols; j++){
                int index = GetIndexPalette(palette, log((*this)(i,j)+1), log(max+1));
                
                Color color = palette.getColor(index);
                salida << color.toString() << " ";
            }
        }
        
        if(!salida){
            throw std::ios_base::failure(string("In void CrimeCounter::saveAsPPMTextImage"
                    "(const string &fileName, const ColorPalette &palette, const string &comment) const:"
                    "\nError occured while writing to the file"));
        }
        
        salida.close();
    } else {
        throw std::ios_base::failure(string("In void CrimeCounter::saveAsPPMTextImage"
                "(const string &fileName, const ColorPalette &palette, const string &comment) const:"
                "\nCannot open the given file"));
    }
}
  
void CrimeCounter::getRowColumn(const Coordinates &coordinates, int& row, 
        int& column) const {
    if (!coordinates.isValid() || 
            !coordinates.isInsideArea(_bottomLeftCoord, _topRightCoord)) {
        throw std::invalid_argument(
            string("void CrimeCounter::getRowColumn((const Coordinates &loc, int& row, int& column) const: ") +
            coordinates.toString() + " is not a valid location or is outside the target area");
    }
    Coordinates targetArea = this->_bottomLeftCoord.length(this->_topRightCoord);
    float cellHeight = targetArea.getLatitude()/_nRows;
    float cellWidth = targetArea.getLongitude()/_nCols;
    Coordinates delta = _bottomLeftCoord.length(coordinates);
    
    row = _nRows - (delta.getLatitude() / cellHeight);
    column = (delta.getLongitude() / cellWidth);
    if (row == _nRows) row = _nRows-1;
    if (column == _nCols) column = _nCols-1;
}


// operador () consulta y modificación
int& CrimeCounter::operator()(int row, int col) {
    return _frequency[row][col];
}

int CrimeCounter::operator()(int row, int col) const {
    return _frequency[row][col];
}

// External functions
int GetIndexPalette(const ColorPalette &palette, float frequency, float maxFrequency) {
    return frequency == maxFrequency ? palette.getNumColors() - 1 : 
        frequency * palette.getNumColors() / maxFrequency;
}