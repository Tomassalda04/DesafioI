#include "Funciones.h"

void leerArchivoEncriptado(char ***array, int posicion, int mensajePista, char *direccion) {

    ifstream archivo;
    archivo.open(direccion);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo" << endl;
    }
    int capacidad=20,control=0;
    char *mensajeTemporal= new char[capacidad];
    char c;
    while (archivo.get(c)) {
        if(c==' ') continue;
        if(control==capacidad){
            redimensionarArreglo(mensajeTemporal,capacidad);
        }
        mensajeTemporal[control]=c;
        control+=1;
    }
    archivo.close();
    revisarEspacios(mensajeTemporal,control);
    array[posicion][mensajePista]=mensajeTemporal;
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










