#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>
using namespace std;

// Declaración de la función

void leerArchivoEncriptado(unsigned char ***array, int posicion, int mensajePista, char *direccion);
unsigned char ***crearArreglo(int numMensajes);
void redimensionarArreglo(unsigned char *&arreglo, int &tamano);
void desencriptarMensajes(unsigned char ***array, int cantMensajes);
void imprimirArreglos(unsigned char ***array, int cantMensajes);
#endif
