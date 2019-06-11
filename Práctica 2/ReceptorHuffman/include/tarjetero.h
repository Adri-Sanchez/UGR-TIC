#ifndef __TARJETERO__H
#define __TARJETERO__H

#include <ArduTIC.h>


/**
 * Estructura utilizada para representar un identificador único de tarjeta
 */
typedef struct {
	byte size; // Número de bytes del UID
	byte uidByte[10]; // UID de la tarjeta
	byte sak;	// El valor SAK (Select Acknowledge) devuelto por el chip PICC tras la selección de la tarjeta con éxito
} Uid;





/**
 * Inicia las comunicaciones con el tarjetero (Chip PCD, RC522) para lectura de tarjetas, incluido el bus SPI
 * @param pin_spi_slave Pin de Arduino usado para selección de entradas esclavo en el puerto SPI. Por defecto pin 10
 * @param pin_pcd_reset Pin de Arduino conectado al reset del PCD. Por defecto pin 9
 * @param maxErroresPor16bits Máximo número de errores a introducir en el mensaje por cada 16 bits. Por defecto: 2
 * @param pError Probabilidad de que en cada 16 bits haya al menos 1 error. Por defecto: 0.1
 */
void IniciarPCD(byte pin_spi_slave= 10, byte pin_pcd_reset= 9, uint8_t maxErroresPor16bits=2, float pError=0.7); 


// Comprueba si hay alguna nueva tarjeta presente (se consideran nuevas sólo las que estén en IDLE. Las que estén en HALT no)
/**
 * Comprueba si hay alguna nueva tarjeta presente (se consideran nuevas sólo las que estén en IDLE. Las que estén en HALT no)
 * @return true si hay una nueva tarjeta presente, false en caso contrario
 * @post Si hay alguna tarjeta presente, la activa. Si hay varias, sólo una se pone a activa (el resto a HALT).
 */
bool IsThereNewCardPICC();


/**
 * Lee el UID de una tarjeta en estado activo.
 * IMPORTANTE: Llamar a IsThereNewCardPICC primero, para despertar a la tarjeta que se va a leer
 * @retval uid El UID de la tarjeta leída, sólo modificado cuando la función devuelve true.
 * @return valor true si se pudo leer, false en caso contrario. Si se pudo leer, uid contiene el uid de la tarjeta.
 */
bool PICC_ReadCardSerial(Uid *uid);


/**
 * Cierra las comunicaciones con la tarjeta. Hace que el chip PICC de la tarjeta pase a HALT.
 * @return Devuelve true si OK, false si error
 */
bool PICC_HaltA();


/**
 * Función para escribir datos personales en la tarjeta.
 * @param uid UID válido de una tarjeta activa, previamente leído con la función PICC_ReadCardSerial
 * @param data Datos (máximo 64 bytes) a escribir en la tarjeta
 * param len Número de bytes en data (máximo 64 bytes) que se deben escribir en la tarjeta
 */
bool PICC_Write(Uid *uid, const uint8_t *data, uint8_t len);


/**
 * Función para leer datos personales de la tarjeta.
 * @param uid UID válido de una tarjeta activa, previamente leído con la función PICC_ReadCardSerial
 * @retval data Datos (máximo 64 bytes) leídos desde la tarjeta
 * @retval len Número de bytes útiles (máximo 64) leídos desde la tarjeta
 * @return true si la tarjeta se pudo leer (en tal caso, data[0..len-1] contendrá los datos leídos, de longitud len), false en caso contrario.
 */
bool PICC_Read(Uid *uid, uint8_t *data, uint8_t &len);

#endif
