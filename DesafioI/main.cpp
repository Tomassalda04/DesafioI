 #include "Funciones.h"

int main() {
    int cantMensajes = 1;
    cout << "Ingrese el numero de mensajes a desencriptar:" << endl;
    cin >> cantMensajes;
    char*** arregloInfoEncriptado = crearArreglo(cantMensajes);
    for(int i=0;i<cantMensajes;i++){
        char direccion[260];
        cout << "Ingrese la ruta del archivo encriptado: ";
        cin >> direccion;
        leerArchivoEncriptado(arregloInfoEncriptado,i,0,direccion);
        cout << "Ingrese la ruta del archivo de la pista: ";
        cin >> direccion;
        leerArchivoEncriptado(arregloInfoEncriptado,i,2,direccion);
    }
    for (int i = 0; i < cantMensajes; i++) {
        cout << "Mensaje " << i << " (encriptado): "
             << arregloInfoEncriptado[i][0] << endl;

        if (arregloInfoEncriptado[i][1] != nullptr) {
            cout << "Mensaje " << i << " (copia): "
                 << arregloInfoEncriptado[i][1] << endl;
        }

        if (arregloInfoEncriptado[i][2] != nullptr) {
            cout << "Mensaje " << i << " (pista): "
                 << arregloInfoEncriptado[i][2] << endl;
        }

        cout << "-----------------------" << endl;
    }

    system("pause");
    return 0;
}
