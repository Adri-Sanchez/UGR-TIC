#include <ArduTIC.h>
#include <string.h>
const int U = 3;
const int T = U/3;


//Funcion Para detectar si se recibe un 1 o 0 por el fotoreceptor 
bool RecibeBit(){
    int contador = 1 ;
    int lector;
    //bucle hasta recibir un L
    do{
        delay(T);
        lector = digitalRead(8);
        if(lector == HIGH){
            contador++;
        }
    }while(lector == HIGH);

    //Si el numero de H es mayor que U>T sabemos que hemos recibido un 1
    //En caso contrario hemos recibido un 0
    return (contador > U/T); 
    

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
    //bucle infinito que siempre espera recibir informacion por el fotoreceptor
    while(1){

        //muestreamos el receptor
        lector = digitalRead(8);
        
        //si se recibe un H debemos comenzar a recibir informacion
        if(lector == HIGH){
            //Serial.println("Empiezo ha recibir");
            //comprobamos si se recibe un 1 o 0
            bool bit = RecibeBit();
            
            //Usaremos el contador de 1 para comprobar cuando se recibe 5 1 consecutivos en un mismo simbolo
            //esto nos indica que es el simbol ode final de cadena *

            //añadimos los 1 y 0 que vamos recibiendo
            if(bit){
                contador1++;
                buffer[puntero]= '1';
                puntero++;      
            }
            else{
                buffer[puntero]= '0';
                puntero++;
                contador1 =0;
            }
            
            
            //si se reconoce que es un simbolo con todo 1, es decir * hay que enviar todo el buffer al PC
            if( contador1 ==5 ){
                digitalWrite(12,HIGH);
                buffer[puntero+1] = '\0';
                Serial.println(buffer);
                puntero = 0;
                contador1 = 0;
            }

            //Comprobamos si estamos en el inicio de un simbolo para reiniciar el contador de 1 para evitar que se cuenten durante
            //varios simbolos
            //     00111 11000 --> si no reiniciamos, esto se tomaria como simbolo de parada y seria un error
            if ( (puntero) % 5 == 0)
                contador1 = 0;
        }
        //introducimos el delay de espera
        delay(T);
    }
    return 0;
}