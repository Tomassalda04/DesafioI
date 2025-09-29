#include "Funciones.h"
#include <iostream>
#include <fstream>
using namespace std;

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

unsigned char ***crearArreglo(int numMensajes) {
    unsigned char ***arregloInfo = new unsigned char**[numMensajes];
    for (int i = 0; i < numMensajes; i++) {
        arregloInfo[i] = new unsigned char*[2];
        arregloInfo[i][0] = nullptr; // mensaje encriptado
        arregloInfo[i][1] = nullptr; // pista
    }
    return arregloInfo;
}

int ***crearArregloLen(int numMensajes) {
    int ***arregloLen = new int**[numMensajes];
    for (int i = 0; i < numMensajes; i++) {
        arregloLen[i] = new int*[2];
        arregloLen[i][0] = nullptr; // len mensaje
        arregloLen[i][1] = nullptr; // len pista
    }
    return arregloLen;
}

void redimensionarArreglo(unsigned char *&arreglo, int &tamano) {
    unsigned char *nuevoArray = new unsigned char[tamano + 20];
    for (int i = 0; i < tamano; i++) {
        nuevoArray[i] = arreglo[i];
    }
    delete[] arreglo;
    arreglo = nuevoArray;
    tamano += 20;
}

void desencriptarMensajes(unsigned char ***array, int cantMensajes, int ***arrayLen) {
    unsigned char **Desencriptados = new unsigned char*[cantMensajes];
    int *longitudesDesencriptados = new int[cantMensajes];
    int *clavesKDesencriptados = new int[cantMensajes];
    int *rotacionesNDesencriptados = new int[cantMensajes];
    int *metodosDesencriptados = new int[cantMensajes];

    for (short int m = 0; m < cantMensajes; m++) {
        Desencriptados[m] = nullptr;
        longitudesDesencriptados[m] = 0;
        clavesKDesencriptados[m] = -1;
        rotacionesNDesencriptados[m] = -1;
        metodosDesencriptados[m] = -1;
        bool encontroDesencriptado = false;
        if (array[m][0] == nullptr) continue;
        int longitudMensaje = *arrayLen[m][0];
        int longitudPista   = *arrayLen[m][1];
        for (unsigned char K = 0; K < 256 && !encontroDesencriptado; K++) {
            unsigned char *copiaXOR = new unsigned char[longitudMensaje];
            for (short int i = 0; i < longitudMensaje; i++) {
                copiaXOR[i] = array[m][0][i]^K;
            }
            for (unsigned char n = 1; n < 8 && !encontroDesencriptado; n++) {
                unsigned char *copiaRotacion = new unsigned char[longitudMensaje];
                for (int i = 0; i < longitudMensaje; i++) {
                    unsigned char b = copiaXOR[i];
                    unsigned char rotacion =((b >> n) | (b << (8 - n)));
                    copiaRotacion[i] = rotacion;
                }
                int longitudDesencriptado = 0;
                unsigned char *desRLE = descompresionRLE(copiaRotacion, longitudMensaje, longitudDesencriptado);
                if (CompararMensaje(array[m][1], desRLE, longitudPista, longitudDesencriptado)) {
                    Desencriptados[m] = new unsigned char[longitudDesencriptado + 1];
                    for (int k = 0; k < longitudDesencriptado; k++) {
                        Desencriptados[m][k] = desRLE[k];
                    }
                    Desencriptados[m][longitudDesencriptado] = '\0';
                    longitudesDesencriptados[m] = longitudDesencriptado;
                    clavesKDesencriptados[m] = K;
                    rotacionesNDesencriptados[m] = n;
                    metodosDesencriptados[m] = 0; // RLE
                    encontroDesencriptado = true;
                }
                delete[] desRLE;
                if (!encontroDesencriptado) {
                    int longitudDesencriptado2 = 0;
                    unsigned char *desLZ = descompresionLZ78(copiaRotacion, longitudMensaje, longitudDesencriptado2);
                    if (CompararMensaje(array[m][1], desLZ, longitudPista, longitudDesencriptado2)) {
                        Desencriptados[m] = new unsigned char[longitudDesencriptado2 + 1];
                        for (int k = 0; k < longitudDesencriptado2; k++) {
                            Desencriptados[m][k] = desLZ[k];
                        }
                        Desencriptados[m][longitudDesencriptado2] = '\0';
                        longitudesDesencriptados[m] = longitudDesencriptado2;
                        clavesKDesencriptados[m] = K;
                        rotacionesNDesencriptados[m] = n;
                        metodosDesencriptados[m] = 1; // LZ78
                        encontroDesencriptado = true;
                    }
                    delete[] desLZ;
                }
                delete[] copiaRotacion;
            }
            delete[] copiaXOR;
        }
    }
    cout << "\n----- RESULTADOS FINALES -----\n";
    for (int m = 0; m < cantMensajes; m++) {
        cout << "Mensaje " << m+1 << ":\n";
        if (Desencriptados[m] != nullptr) {
            cout << " Metodo: ";
            if (metodosDesencriptados[m] == 0) cout << "RLE";
            else if (metodosDesencriptados[m] == 1) cout << "LZ78";
            cout << " | K=" << clavesKDesencriptados[m]
                 << " | n=" << rotacionesNDesencriptados[m] << "\n";
            cout << " Texto: " << Desencriptados[m] << "\n";
        } else {
            cout << " No se encontro clave/rotacion valida\n";
        }
        cout << "------------------------------\n";
    }
    for (int m = 0; m < cantMensajes; m++) {
        if (Desencriptados[m] != nullptr) delete[] Desencriptados[m];
    }
    delete[] Desencriptados;
    delete[] longitudesDesencriptados;
    delete[] clavesKDesencriptados;
    delete[] rotacionesNDesencriptados;
    delete[] metodosDesencriptados;
}

unsigned char* descompresionRLE(unsigned char *array, int lenMensaje, int &LenDescomprimido) {
    int posicion = 0;
    int capacidadMensaje = 256;
    unsigned char *mensajeDesencriptado = new unsigned char[capacidadMensaje];
    LenDescomprimido = 0;
    while (posicion < lenMensaje) {
        int repeticiones = 0;
        if (posicion + 1 < lenMensaje && array[posicion] == 0x00) {
            repeticiones = array[posicion + 1];
            posicion += 2;
        }
        else if (array[posicion] >= '0' && array[posicion] <= '9') {
            while (posicion < lenMensaje && array[posicion] >= '0' && array[posicion] <= '9') {
                repeticiones = repeticiones * 10 + (array[posicion] - '0');
                posicion++;
            }
        }
        else {
            repeticiones = 1;
        }
        if (posicion >= lenMensaje) break;
        unsigned char caracterActual = array[posicion++];
        if (LenDescomprimido + repeticiones >= capacidadMensaje) {
            int nuevaCapacidadMensaje = capacidadMensaje;
            while (LenDescomprimido + repeticiones >= nuevaCapacidadMensaje){
                nuevaCapacidadMensaje *= 2;
            }
            unsigned char *mensajeTemporal = new unsigned char[nuevaCapacidadMensaje];
            for (int k = 0; k < LenDescomprimido; k++){
                mensajeTemporal[k] = mensajeDesencriptado[k];
            }
            delete[] mensajeDesencriptado;
            mensajeDesencriptado = mensajeTemporal;
            capacidadMensaje = nuevaCapacidadMensaje;
        }
        for (int k = 0; k < repeticiones; k++) {
            mensajeDesencriptado[LenDescomprimido++] = caracterActual;
        }
    }
    unsigned char *mensajeFinal = new unsigned char[LenDescomprimido + 1];
    for (int k = 0; k < LenDescomprimido; k++){
        mensajeFinal[k] = mensajeDesencriptado[k];
    }
    mensajeFinal[LenDescomprimido] = '\0';
    delete[] mensajeDesencriptado;
    return mensajeFinal;
}

unsigned char* descompresionLZ78(unsigned char *array, int lenMensaje, int &LenDescomprimido) {
    int capacidadDiccionario = 256;
    unsigned char **diccionario = new unsigned char*[capacidadDiccionario];
    int tamanoDiccionario = 1;
    diccionario[0] = nullptr;
    int capacidadResultado = 512;
    unsigned char *resultado = new unsigned char[capacidadResultado];
    LenDescomprimido = 0;
    int posicion = 0;
    while (posicion + 2 < lenMensaje) {
        int indice = (array[posicion] << 8) | array[posicion + 1];
        unsigned char caracter = array[posicion + 2];
        posicion += 3;
        unsigned char *prefijo = nullptr;
        int longitudPrefijo = 0;
        if (indice > 0 && indice < tamanoDiccionario) {
            unsigned char *entrada = diccionario[indice];
            while (entrada[longitudPrefijo] != '\0') {
                longitudPrefijo++;
            }
            prefijo = entrada;
        }
        int nuevaLongitud = longitudPrefijo + 1;
        unsigned char *nuevaCadena = new unsigned char[nuevaLongitud + 1];
        for (int k = 0; k < longitudPrefijo; k++) {
            nuevaCadena[k] = prefijo[k];
        }
        nuevaCadena[longitudPrefijo] = caracter;
        nuevaCadena[nuevaLongitud] = '\0';
        if (tamanoDiccionario >= capacidadDiccionario) {
            int nuevaCapacidad = capacidadDiccionario * 2;
            unsigned char **diccionarioTemporal = new unsigned char*[nuevaCapacidad];
            for (int k = 0; k < capacidadDiccionario; k++) {
                diccionarioTemporal[k] = diccionario[k];
            }
            delete[] diccionario;
            diccionario = diccionarioTemporal;
            capacidadDiccionario = nuevaCapacidad;
        }
        diccionario[tamanoDiccionario++] = nuevaCadena;
        if (LenDescomprimido + nuevaLongitud >= capacidadResultado) {
            int nuevaCapacidad = capacidadResultado * 2;
            while (LenDescomprimido + nuevaLongitud >= nuevaCapacidad) {
                nuevaCapacidad *= 2;
            }
            unsigned char *resultadoTemporal = new unsigned char[nuevaCapacidad];
            for (int k = 0; k < LenDescomprimido; k++) {
                resultadoTemporal[k] = resultado[k];
            }
            delete[] resultado;
            resultado = resultadoTemporal;
            capacidadResultado = nuevaCapacidad;
        }
        for (int k = 0; k < nuevaLongitud; k++) {
            resultado[LenDescomprimido++] = nuevaCadena[k];
        }
    }
    unsigned char *mensajeFinal = new unsigned char[LenDescomprimido + 1];
    for (int k = 0; k < LenDescomprimido; k++) {
        mensajeFinal[k] = resultado[k];
    }
    mensajeFinal[LenDescomprimido] = '\0';
    delete[] resultado;
    for (int k = 1; k < tamanoDiccionario; k++) {
        delete[] diccionario[k];
    }
    delete[] diccionario;
    return mensajeFinal;
}


bool CompararMensaje(unsigned char *arrayPista, unsigned char *arrayDescom, int lenPista, int lenDescom) {
    if (lenPista <= 0 || lenDescom <= 0){
        return false;
    }
    if (lenPista > lenDescom){
        return false;
    }
    for (int i = 0; i + lenPista <= lenDescom; i++) {
        bool coincide = true;
        for (int j = 0; j < lenPista; j++) {
            if (arrayDescom[i + j] != arrayPista[j]) {
                coincide = false;
                break;
            }
        }
        if (coincide){
            return true;
        }
    }
    return false;
}

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

