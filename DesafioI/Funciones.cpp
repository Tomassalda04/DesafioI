#include "Funciones.h"
#include <iostream>
#include <fstream>
using namespace std;

/* Esta funcion lee el .txt y guarda cada caracter en un arreglo dinámico
   el cual contiene n mensajes y cada uno tiene dos posiciones:
   [0] = mensaje encriptado, [1] = pista */
void leerArchivoEncriptado(unsigned char ***array, int posicion, int mensajePista, char *direccion, int ***arraylen) {
    ifstream archivo(direccion, ios::binary);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo" << endl;
        return;
    }

    int capacidad = 20, control = 0;
    unsigned char *mensajeTemporal = new unsigned char[capacidad];

    unsigned char c;
    while (archivo.read((char*)&c, 1)) {
        if (control == capacidad) {
            redimensionarArreglo(mensajeTemporal, capacidad);
        }
        mensajeTemporal[control] = c;
        control++;
    }
    archivo.close();

    array[posicion][mensajePista] = mensajeTemporal;
    arraylen[posicion][mensajePista] = new int(control);
}

/* Reserva memoria para el arreglo que contiene los mensajes y pistas */
unsigned char ***crearArreglo(int numMensajes) {
    unsigned char ***arregloInfo = new unsigned char**[numMensajes];
    for (int i = 0; i < numMensajes; i++) {
        arregloInfo[i] = new unsigned char*[2];
        arregloInfo[i][0] = nullptr; // mensaje encriptado
        arregloInfo[i][1] = nullptr; // pista
    }
    return arregloInfo;
}

/* Reserva memoria para guardar la longitud de cada mensaje y pista */
int ***crearArregloLen(int numMensajes) {
    int ***arregloLen = new int**[numMensajes];
    for (int i = 0; i < numMensajes; i++) {
        arregloLen[i] = new int*[2];
        arregloLen[i][0] = nullptr; // len mensaje
        arregloLen[i][1] = nullptr; // len pista
    }
    return arregloLen;
}

/* Redimensiona el arreglo cuando se llena */
void redimensionarArreglo(unsigned char *&arreglo, int &tamano) {
    unsigned char *nuevoArray = new unsigned char[tamano + 20];
    for (int i = 0; i < tamano; i++) {
        nuevoArray[i] = arreglo[i];
    }
    delete[] arreglo;
    arreglo = nuevoArray;
    tamano += 20;
}

/* Imprime los mensajes y pistas */
void imprimirArreglos(unsigned char ***array, int cantMensajes) {
    for (int i = 0; i < cantMensajes; i++) {
        cout << "=== Mensaje " << i + 1 << " ===" << endl;

        if (array[i][0] != nullptr) {
            cout << "Archivo encriptado: " << endl;
            cout << array[i][0] << endl;
        } else {
            cout << "Archivo encriptado vacío." << endl;
        }

        if (array[i][1] != nullptr) {
            cout << "Archivo pista: " << endl;
            cout << array[i][1] << endl;
        } else {
            cout << "Archivo pista vacío." << endl;
        }
        cout << "=========================" << endl;
    }
}

/* Imprime las longitudes de mensajes y pistas */
void imprimirArregloLen(int ***array, int cantMensajes) {
    for (int i = 0; i < cantMensajes; i++) {
        if (array[i][0] != nullptr)
            cout << "Len Mensaje Encriptado " << i + 1 << ": " << *array[i][0] << endl;
        if (array[i][1] != nullptr)
            cout << "Len Pista " << i + 1 << ": " << *array[i][1] << endl;
    }
}

/* Desencripta los mensajes aplicando XOR + rotación + descompresión */
void desencriptarMensajes(unsigned char ***array, int cantMensajes, int ***arrayLen) {
    for (int m = 0; m < cantMensajes; m++) {
        if (array[m][0] == nullptr) continue;

        int longitudMensaje = *arrayLen[m][0];
        int longitudPista   = *arrayLen[m][1];

        for (int K = 0; K < 256; K++) {
            unsigned char *copiaXOR = new unsigned char[longitudMensaje];
            for (int i = 0; i < longitudMensaje; i++) {
                copiaXOR[i] = array[m][0][i] ^ (unsigned char)K;
            }

            for (int n = 1; n < 8; n++) {
                unsigned char *copiaRotacion = new unsigned char[longitudMensaje];
                for (int i = 0; i < longitudMensaje; i++) {
                    unsigned char b = copiaXOR[i];
                    unsigned char rot = (unsigned char)((b >> n) | (b << (8 - n)));
                    copiaRotacion[i] = rot;
                }
                int desLen = 0;
                unsigned char *des = descompresionRLE(copiaRotacion, longitudMensaje, desLen);
                delete[] des;
                delete[] copiaRotacion;
            }
            delete[] copiaXOR;
        }
    }
}

/* Descompresión RLE */
unsigned char* descompresionRLE(unsigned char *array, int lenMensaje, int &LenDescomprimido) {
    int i = 0;
    int capacidad = 256;
    unsigned char *mensDescomprimido = new unsigned char[capacidad];
    LenDescomprimido = 0;

    while (i < lenMensaje) {
        int cont = 0;

        if (i + 1 < lenMensaje && array[i] == 0x00) {
            cont = array[i + 1];
            i += 2;
        }
        else if (array[i] >= '0' && array[i] <= '9') {
            while (i < lenMensaje && array[i] >= '0' && array[i] <= '9') {
                cont = cont * 10 + (array[i] - '0');
                i++;
            }
        }
        else {
            cont = 1;
        }

        if (i >= lenMensaje) break;

        unsigned char sym = array[i++];

        if (LenDescomprimido + cont >= capacidad) {
            int newCap = capacidad;
            while (LenDescomprimido + cont >= newCap) newCap *= 2;
            unsigned char *mensDescTemp = new unsigned char[newCap];
            for (int k = 0; k < LenDescomprimido; k++) mensDescTemp[k] = mensDescomprimido[k];
            delete[] mensDescomprimido;
            mensDescomprimido = mensDescTemp;
            capacidad = newCap;
        }
        for (int k = 0; k < cont; k++) {
            mensDescomprimido[LenDescomprimido++] = sym;
        }
    }

    unsigned char *result = new unsigned char[LenDescomprimido + 1];
    for (int k = 0; k < LenDescomprimido; k++) result[k] = mensDescomprimido[k];
    result[LenDescomprimido] = '\0';
    delete[] mensDescomprimido;

    return result;
}

/* Libera toda la memoria reservada */
void liberarMemoria(unsigned char ***array, int ***arrayLen, int cantMensajes) {
    for (int i = 0; i < cantMensajes; i++) {
        for (int j = 0; j < 2; j++) {
            if (array[i][j] != nullptr) {
                delete[] array[i][j];
                array[i][j] = nullptr;
            }
            if (arrayLen[i][j] != nullptr) {
                delete arrayLen[i][j];
                arrayLen[i][j] = nullptr;
            }
        }
        delete[] array[i];
        delete[] arrayLen[i];
    }
    delete[] array;
    delete[] arrayLen;
}

