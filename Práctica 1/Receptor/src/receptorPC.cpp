#include <PCSerialUSB.h>
#include <iostream>
#include <unordered_map>

using namespace std;
int main(){
    int fd;
    char cadena[100];
    unordered_map<string,char> uMap;

    // Reservamos memoria para el unordered_map
    uMap.reserve(32);

    // Introducimos los valores de nuestro codigo para poder decodificar de forma eficiente con
    // un unordered map
    uMap["00000"] = 'A';
    uMap["00001"] = 'B';
    uMap["00010"] = 'C';
    uMap["00011"] = 'D';
    uMap["00100"] = 'E';
    uMap["00101"] = 'F';
    uMap["00110"] = 'G';
    uMap["00111"] = 'H';
    uMap["01000"] = 'I';
    uMap["01001"] = 'J';
    uMap["01010"] = 'K';
    uMap["01011"] = 'L';
    uMap["01100"] = 'M';
    uMap["01101"] = 'N';
    uMap["01110"] = '-';
    uMap["01111"] = 'O';
    uMap["10000"] = 'P';
    uMap["10001"] = 'Q';
    uMap["10010"] = 'R';
    uMap["10011"] = 'S';
    uMap["10100"] = 'T';
    uMap["10101"] = 'U';
    uMap["10110"] = 'V';
    uMap["10111"] = 'W';
    uMap["11000"] = 'X';
    uMap["11001"] = 'Y';
    uMap["11010"] = 'Z';
    uMap["11011"] = ' ';
    uMap["11100"] = '?';
    uMap["11101"] = ',';
    uMap["11110"] = '.';
    uMap["11111"] = '*';

    
    //abrimos el canal para comunicarnos con arduino y recibir el mensaje
    fd = InicializarUSB("/dev/ttyACM0");
    if(fd<0){
        cout << "ERROR"<<endl;
        return 0;
    }


    //bucle infinito que siempre esta esperando a recibir un mensaje por parte de Arduino
    while(1){
        cout << "Esperando el mensaje" << endl;
        string mensaje = "";
        if(receiveUSB(fd,cadena)){
            string codigo = cadena;
            //cout << cadena << endl;
            //decodificamos usando el map 
            for(int i =0 ; i < codigo.size()-2 ; i = i+5){
                string letra = codigo.substr(i,5);
                mensaje = mensaje + uMap[letra];
            }
            mensaje.pop_back();
            //imprimimos el mensaje por pantalla
            cout << ">>>> Arduino ha recibido: " << mensaje << endl;
        }
    }
    CerrarUSB(fd);
    return 0;
}
