#include <ArduTIC.h>
#include <string.h>

int main (){
    const unsigned int U = 3; // Definimos U a 3 ms
    char msg[500];
    char * p;

    // Establecemos el puerto serie a 9600 baud
    Serial.begin(9600);
    
    // Inicializamos el pin a utilizar y escribimos LOW.
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);

    while (1){;
        
        if (Serial.available()){
            Serial.readString(msg); // Leemos la cadena
            
            for (p = msg; *p != '\0'; p++){
            	if (*p == '1'){
            		// Si el símbolo es un '1', escribimos HHL
                    digitalWrite(8, HIGH);
                    delay(2*U);
                    digitalWrite(8, LOW);
                    delay(U);
                }

                else if (*p == '0'){
                	// Si el símbolo es un '0', escribimos HL
                    digitalWrite(8, HIGH);
                    delay(U);
                    digitalWrite(8, LOW);
                    delay(U);
                }
            }
            
            Serial.println("Enviado."); // Confirmamos el envío por laser
        }
        
    }

    return 0;
}

