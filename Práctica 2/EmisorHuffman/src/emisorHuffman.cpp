#include <ArduTIC.h>
#include <string.h>

int main (){
    const unsigned int U = 3;
    char msg[500];
    char * p;

    // Inicializamos el puerto serie
    Serial.begin(9600);
    
    // Inicializamos el pin 8, el cuál usaremos para manejar el laser.
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);

    while (1){;
        
        if (Serial.available()){
            Serial.readString(msg); // Leemos la cadena
            
            for (p = msg; *p != '\0'; p++){
            	if (*p == '1'){ // Si nos encontramos un símbolo '1' en la cadena
                    digitalWrite(8, HIGH);
                    delay(2*U);
                    digitalWrite(8, LOW);
                    delay(U);
                }

                else if (*p == '0'){ // Si nos encontramos un símbolo '0' en la cadena
                    digitalWrite(8, HIGH);
                    delay(U);
                    digitalWrite(8, LOW);
                    delay(U);
                }
            }
            
            Serial.println("Enviado.");
        }
        
    }

    return 0;
}

