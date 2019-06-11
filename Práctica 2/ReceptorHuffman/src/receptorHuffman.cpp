#include <ArduTIC.h>
#include <string.h>
const int U = 3;
const int T = U/3;

/***
 * Función que comprueba si se estan recibiendo datos y en caso
 * de ser así, cuando se recibe un HIGH, lee el dato y lo devuelve
 * por referencia. La función devuelve un bool en función de si se ha
 * leeído un dato o no.**/
bool RecibeBit(char & dato){
    int contador = 0 ;
    int lector;

    //se lee cero y se comprueba si se pasa el límite para determinar que
    //ya no se trata de un dato.
    while(digitalRead(8) == LOW && contador <= (U/T)){
        contador++;
        delay(T);
    }
    //no se estan recibiendo datos
    if(contador > (U/T)){
        return false;
    }
    //Se ha recibido un 1, tenemos que recibir el dato
    contador=1;
    delay(T);
    while(digitalRead(8) == HIGH){
        contador++;
        delay(T);
    }
    //si es mayor, quiere decir que tenemos un '1'
    if(contador >  U/T){
        dato = '1';
    }
    else{
        dato = '0';
    }
    //indicamos que hemos recibido un dato
    return true;
}

int main(){
    char buffer[500];
    int puntero=0;
    Serial.begin(9600);
    pinMode(8,INPUT);
    pinMode(12,OUTPUT);
    digitalWrite(12,LOW);
    int lector=0;
    int contador1 =0;
    char dato;

    while(1){

        //llamamos la funcion que comprueba si se estan recibiendo datos
        if(RecibeBit(dato)){
            digitalWrite(12,HIGH);
            buffer[puntero] = dato;
            puntero++;
        }
        else{
            //si ya no se reciben datos y tenemos datos en el buffer
            //debemos enviarlo
            if(puntero > 0){
                digitalWrite(12,HIGH);
                buffer[puntero+1] = '\0';
                Serial.println(buffer);
                puntero = 0;
            }
           
        }
    }
    return 0;
}