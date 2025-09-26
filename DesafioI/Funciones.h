#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>
using namespace std;


void leerArchivoEncriptado(unsigned char ***array, int posicion, int mensajePista, char *direccion, int ***arraylen);

unsigned char ***crearArreglo(int numMensajes);

int ***crearArregloLen(int numMensajes);

void redimensionarArreglo(unsigned char *&arreglo, int &tamano);

void imprimirArreglos(unsigned char ***array, int cantMensajes);

void desencriptarMensajes(unsigned char ***array, int cantMensajes,int ***arrayLen);

unsigned char* descompresionRLE(unsigned char *array, int longitud, int &outLen);

void liberarMemoria(unsigned char ***array, int ***arrayLen, int cantMensajes);

void imprimirArregloLen(int ***array, int cantMensajes);

#endif
