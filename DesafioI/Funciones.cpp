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

void imprimirArreglos(char ***array, int cantMensajes) {
    int control=0;
    for (int i = 0; i < cantMensajes; i++) {
        cout << "=== Mensaje " << i + 1 << " ===" << endl;

        if (array[i][0] != nullptr) {
            cout << "Archivo encriptado: " << endl;
            cout << array[i][0] << endl;  // imprimirá como texto (si es binario puede salir raro)
        } else {
            cout << "Archivo encriptado vacío." << endl;
        }

        if (array[i][2] != nullptr) {
            cout << "Archivo pista: " << endl;
            cout << array[i][2] << endl;
        } else {
            cout << "Archivo pista vacío." << endl;
        }

        cout << "=========================" << endl;
        control=i;
    }
    cout<<endl<<"Control: "<<control<<endl;
}
