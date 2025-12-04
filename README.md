# Boston: Generador de Mapas de Calor de Crímenes

Este proyecto corresponde a la Práctica Final de la asignatura **Metodología de la Programación**. El objetivo es desarrollar una herramienta en C++ capaz de procesar datos de crímenes en la ciudad de Boston y generar un mapa de calor (Heat Map) visual en formato de imagen PPM.

## 📋 Descripción del Proyecto

El programa analiza un conjunto de crímenes ocurridos en un área geográfica rectangular (zona de interés) y calcula la frecuencia de delitos en subdivisiones (celdas) de dicha área.Las zonas con mayor frecuencia se representan con colores cálidos (rojo/amarillo) y las de menor frecuencia con colores fríos (azul/verde).

### Funcionalidades Principales
* **Fusión de Datos:** Combina múltiples archivos de crímenes (`.crm`) en un único conjunto de datos (`CrimeSet`).
* **Conteo Geográfico:** Divide el área de Boston en una cuadrícula (matriz) de $N \times M$ celdas usando la clase `CrimeCounter`.
* **Generación de Imagen:** Transforma la matriz de frecuencias en una imagen `.ppm` utilizando una paleta de colores personalizable (`ColorPalette`).

## 🏗️ Arquitectura

El proyecto se basa en las siguientes clases:

* **Coordinates:** Manejo de coordenadas geográficas (latitud/longitud).
* **Crime:** Representa un crimen individual (ID, fecha, ubicación, etc.).
* **CrimeSet:** Gestiona un conjunto de crímenes mediante memoria dinámica.
* ]**CrimeCounter:** (Nueva) Matriz dinámica para el conteo de frecuencias por zonas.
* **Color & ColorPalette:** (Proporcionadas) Gestión de colores RGB y paletas para el mapa de calor

## 🚀 Instalación y Compilación

Para compilar el proyecto, asegúrate de tener un compilador C++ (g++) y ejecuta:

```bash
g++ -I./include src/*.cpp -o boston4
