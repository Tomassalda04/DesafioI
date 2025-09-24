#include "Funciones.h"

void leerArchivoEncriptado(unsigned char ***array, int posicion, int mensajePista, char *direccion) {

    ifstream archivo;
    archivo.open(direccion);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo" << endl;
    }
    int capacidad=20,control=0;
    unsigned char *mensajeTemporal= new unsigned char[capacidad];
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

unsigned char ***crearArreglo(int numMensajes){
    unsigned char*** arregloInfo = new unsigned char**[numMensajes];
    for (int i = 0; i < numMensajes; i++) {
        arregloInfo[i] = new unsigned char*[3];
        arregloInfo[i][0] = nullptr; // mensaje encriptado
        arregloInfo[i][1] = nullptr; // copia
        arregloInfo[i][2] = nullptr; // pista
    }
    return arregloInfo;
}

void redimensionarArreglo(unsigned char *&arreglo, int &tamano){
    unsigned char* nuevoArray = new unsigned char[tamano + 20];
    for(int i=0;i<tamano;i++){
        nuevoArray[i]=arreglo[i];
    }
    delete[] arreglo;
    arreglo=nuevoArray;
    tamano+=20;
}

void revisarEspacios(unsigned char *&arreglo, int &tamanoReal){
    unsigned char* nuevoArray = new unsigned char[tamanoReal+1];
    for(int i=0;i<tamanoReal;i++){
        nuevoArray[i]=arreglo[i];
    }
    nuevoArray[tamanoReal]='\0';
    delete[] arreglo;
    arreglo=nuevoArray;
}

void desencriptarMensajes(unsigned char ***array, int cantMensajes){

    for(unsigned int m=0;m<cantMensajes;m++){
        if (array[m][1] == nullptr) continue;
        unsigned char *mensajeDesencriptar=array[m][1];
        int len = 0;
        while (mensajeDesencriptar[len] != '\0') len++;

        for(unsigned int XOR=0;XOR<255;XOR++){
            int controlXOR=XOR;


            for(unsigned int n=1;n<8;n++){


                for(unsigned int d=0;d<2;d++){

                }
            }
        }
    }
}








