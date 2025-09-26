#include "Funciones.h"

int main() {
    int cantMensajes = 1;
    cout << "Ingrese el numero de mensajes a desencriptar:" << endl;
    cin >> cantMensajes;
    unsigned char*** arregloInfoEncriptado = crearArreglo(cantMensajes);
    int ***arregloLongitudes= crearArregloLen(cantMensajes);
    for(int i=0;i<cantMensajes;i++){
        char direccion[260];
        cout << "Ingrese la ruta del archivo "<<i+1<<" encriptado: ";
        cin >> direccion;
        leerArchivoEncriptado(arregloInfoEncriptado,i,0,direccion,arregloLongitudes);
        cout << "Ingrese la ruta del archivo "<<i+1<<" de la pista: ";
        cin >> direccion;
        leerArchivoEncriptado(arregloInfoEncriptado,i,1,direccion,arregloLongitudes);
    }
    //imprimirArreglos(arregloInfoEncriptado, cantMensajes);
    //imprimirArregloLen(arregloLongitudes,cantMensajes);
    desencriptarMensajes(arregloInfoEncriptado, cantMensajes, arregloLongitudes);
    liberarMemoria(arregloInfoEncriptado,arregloLongitudes, cantMensajes);
    system("pause");
    return 0;
}
