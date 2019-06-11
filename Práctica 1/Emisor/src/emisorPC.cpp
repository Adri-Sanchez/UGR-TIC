#include <PCSerialUSB.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unordered_map>

/*
	Función que modifica una tabla hash pasada como parámetro con los valores que hemos estimado oportunos para codificar el lenguaje.
*/
void codificacionUMAP(std::unordered_map<char, std::string> & uMap){

	// Reservamos memoria para el unordered_map
	uMap.reserve(32);

	// Introducimos los valores
	uMap['A'] = "00000";
	uMap['B'] = "00001";
	uMap['C'] = "00010";
	uMap['D'] = "00011";
	uMap['E'] = "00100";
	uMap['F'] = "00101";
	uMap['G'] = "00110";
	uMap['H'] = "00111";
	uMap['I'] = "01000";
	uMap['J'] = "01001";
	uMap['K'] = "01010";
	uMap['L'] = "01011";
	uMap['M'] = "01100";
	uMap['N'] = "01101";
	uMap['-'] = "01110";
	uMap['O'] = "01111";
	uMap['P'] = "10000";
	uMap['Q'] = "10001";
	uMap['R'] = "10010";
	uMap['S'] = "10011";
	uMap['T'] = "10100";
	uMap['U'] = "10101";
	uMap['V'] = "10110";
	uMap['W'] = "10111";
	uMap['X'] = "11000";
	uMap['Y'] = "11001";
	uMap['Z'] = "11010";
	uMap[' '] = "11011";
	uMap['?'] = "11100";
	uMap[','] = "11101";
	uMap['.'] = "11110";
	uMap['*'] = "11111";
}

/*
	Función que devuelve un string de C, conteniendo la cadena proporcionada como parámetro ya codificada.
*/
char * codificaCadena(const char *cadena, std::unordered_map<char, std::string> uMap){
	std::string output = "";

	for (unsigned int i = 0; i < strlen(cadena); i++){
		char aux = cadena[i];

		output += uMap[aux];
	}

	return strdup(output.c_str());
}

/*
	Función que modifica mediante punteros una cadena de C para convertirla en mayúscula.
*/
void toUpperPointer (char * temp){
	char * p = temp;

	while (*p){
		*p = toupper((unsigned char) *p);
		p++;
	}

}

int main(){
	int fd;
	std::unordered_map<char, std::string> uMap;
	char cadena[100];
	char * cadena_bits;

	// Inicializamos el puerto USB
	fd = InicializarUSB("/dev/ttyACM0"); 

	if (fd < 0) 
		return 0;

	// Cargamos la codificación realizada en el unordered_map
	codificacionUMAP(uMap);

	// Introducimos la cadena a enviar
	std::cout << "Introduzca la cadena a enviar: ";
	std::cin.getline(cadena, sizeof(cadena));


	while (strcmp(cadena, "fin") != 0){

		// Añadimos el carácter de finalización
		// El usuario no necesitaría introducir dicho carácter cada vez que envía un mensaje
		strcat(cadena, "*");

		// Convertimos toda la cadena a mayúsculas
		toUpperPointer(cadena);

		std::cout << "Introducido: \"" << cadena << "\"" << std::endl;

		// Codificamos la cadena recibida antes de enviarla.
		cadena_bits = codificaCadena(cadena, uMap);

		std::cout << "Codificación -> " << cadena_bits << "\n" << std::endl;

		if (sendUSB(fd, cadena_bits))
			std::cout << "Enviando..." << std::endl;

		if (receiveUSB(fd, cadena))
			std::cout << "Arduino >> " << cadena << std::endl;

		std::cin.getline(cadena, sizeof(cadena));
	}

	CerrarUSB(fd);

	return 0;
}