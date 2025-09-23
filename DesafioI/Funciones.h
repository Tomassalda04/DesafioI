#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>
using namespace std;

// Declaración de la función
void leerArchivoEncriptado(char ***array, int posicion, int mensajePista, char *direccion);
char ***crearArreglo(int numMensajes);
void redimensionarArreglo(char *&arreglo, int &tamano);
void clavesRotaciones(char ***array);
#endif
