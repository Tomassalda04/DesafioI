#include "Funciones.h"

void leerArchivoEncriptado(char ***array, int posicion, int mensajePista, char *direccion) {
    ifstream archivo(direccion, ios::binary);  // <-- abrir en BINARIO
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo" << endl;
        return;
    }

    int capacidad = 20, control = 0;
    char *mensajeTemporal = new char[capacidad];
    char c;

    while (archivo.get(c)) {
        if (control == capacidad) {
            redimensionarArreglo(mensajeTemporal, capacidad);
        }
        mensajeTemporal[control] = c;
        control++;

        // Imprimir en HEXA para debug (no en texto)
        printf("%02X ", (unsigned char)c);
    }
    cout << endl;

    archivo.close();
    cout << "Bytes leídos: " << control << endl;

    array[posicion][mensajePista] = mensajeTemporal;
}

char ***crearArreglo(int numMensajes){
    char*** arregloInfo = new char**[numMensajes];
    for (int i = 0; i < numMensajes; i++) {
        arregloInfo[i] = new char*[3];
        arregloInfo[i][0] = nullptr; // mensaje encriptado
        arregloInfo[i][1] = nullptr; // copia
        arregloInfo[i][2] = nullptr; // pista
    }
    return arregloInfo;
}

void redimensionarArreglo(char *&arreglo, int &tamano){
    char* nuevoArray = new char[tamano + 20];
    for(int i=0;i<tamano;i++){
        nuevoArray[i]=arreglo[i];
    }
    delete[] arreglo;
    arreglo=nuevoArray;
    tamano+=20;
}

void revisarEspacios(char *&arreglo, int &tamanoReal){
    char* nuevoArray = new char[tamanoReal+1];
    for(int i=0;i<tamanoReal;i++){
        nuevoArray[i]=arreglo[i];
    }
    nuevoArray[tamanoReal]='\0';
    delete[] arreglo;
    arreglo=nuevoArray;
}










