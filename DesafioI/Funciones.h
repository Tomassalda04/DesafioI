#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>
using namespace std;

void leerArchivoEncriptado(unsigned char ***array, int posicion, int mensajePista, char *direccion, int ***arraylen);
unsigned char ***crearArreglo(int numMensajes);
int ***crearArregloLen(int numMensajes);
void redimensionarArreglo(unsigned char *&arreglo, int &tamano);
void desencriptarMensajes(unsigned char ***array, int cantMensajes,int ***arrayLen);
unsigned char* descompresionRLE(unsigned char *array, int longitud,  int &LenDescomprimido);
unsigned char* descompresionLZ78(unsigned char *array, int lenMensaje,  int &LenDescomprimido);
bool CompararMensaje(unsigned char *arrayPista, unsigned char *arrayDescom, int lenPista,int lenDescom);
void liberarMemoria(unsigned char ***array, int ***arrayLen, int cantMensajes);
#endif
