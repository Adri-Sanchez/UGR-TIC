#ifndef __RF2272__H
#define __RF2272__H

#include <ArduTIC.h>

#define PIN_D0 8    // Asigna el pin 8 al pin D0 del receptor RF
#define PIN_D1 9    // Asigna el pin 9 al pin D1 del receptor RF
#define PIN_D2 10   // Asigna el pin 10 al pin D2 del receptor RF
#define PIN_D3 11   // Asigna el pin 11 al pin D3 del receptor RF
#define PIN_VT 12   // Asigna el pin 12 al pin VT del receptor RF


/**
 * Inicia las comunicaciones con el receptor de radio por protocolo 2272
 * @param _pinD0 Pin de Arduino UNO donde se conecta la patilla D0 del receptor. Por defecto pin PIN_D0
 * @param _pinD1 Pin de Arduino UNO donde se conecta la patilla D1 del receptor. Por defecto pin PIN_D1
 * @param _pinD2 Pin de Arduino UNO donde se conecta la patilla D2 del receptor. Por defecto pin PIN_D2
 * @param _pinD3 Pin de Arduino UNO donde se conecta la patilla D3 del receptor. Por defecto pin PIN_D3
 * @param pError Probabilidad de que en el mensaje haya error en 1 bit. Por defecto, 0.1
 */
void IniciarRF2272(int _pinD0= PIN_D0, int _pinD1= PIN_D1, int _pinD2= PIN_D2, int _pinD3= PIN_D3, int _pinVT= PIN_VT, float pError=0.1);



/**
 * Realiza una lectura de los bits de los códigos de los botones pulsados en el emisor RF2262 y recibidos en el RF2272.
 * @return El código leído, que se corresponde con un código de Hamming (7,4) con las siguiente distribución:
 *  - Bit 0: Paridad de Hamming 1
 *  - Bit 1: Paridad de Hamming 2
 *  - Bit 2: Bit de datos. Botón A pulsado (1) o no pulsado (0)
 *  - Bit 3: Paridad de Hamming 3
 *  - Bit 4: Bit de datos. Botón B pulsado (1) o no pulsado (0)
 *  - Bit 5: Bit de datos. Botón C pulsado (1) o no pulsado (0)
 *  - Bit 6: Bit de datos. Botón D pulsado (1) o no pulsado (0)
 */
uint8_t leerRF2272();



#endif
