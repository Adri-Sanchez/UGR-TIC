#include <PCSerialUSB.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <stack>
#include <algorithm>
#include <set>

// Estructura de Nodo Parcial
struct nodoParcial{
    char simbolo;
    unsigned int frecuencia;
    int pos;
};

// Functor para el set
struct classcomp {

	bool operator() (const nodoParcial& lhs, const nodoParcial& rhs) const
	{
  		return lhs.frecuencia >= rhs.frecuencia;
  	}
};

/*
	Función para leer un archivo y devolver un 'unordered_map' conteniendo
	los caracteres encontrados junto al número de apariciones
*/
std::unordered_map<char, unsigned int> loadFrec (const std::string &file){
	std::unordered_map<char, unsigned int> uMap;
	std::ifstream fin;
	std::string str;

	fin.open(file, std::ios::in);

	if (!fin){
		std::cerr << "Error al leer el fichero: " << file << std::endl;
	}
	else{
		std::cout << "Leyendo archivo " << file << "..." << std::endl;
		
		// Mientras no lleguemos al final del archivo
		while (!fin.eof()){
			
			getline(fin, str, '\n');
			
			for (size_t i = 0; i < str.size(); i++){
				char c = toupper(str[i]);

				if (uMap.find(c) != uMap.end()){
					uMap[c] += 1;
				}
				else{
					uMap[c] = 1;
				}
			}
		}

		std::cout << "Archivo leído." << std::endl;				
	}
	

	return uMap;
}


// Función para codificar una cadena a partir de una matriz de codificación Huffman.

char * codificaCadena (const std::vector<std::vector <int>> &M, const int &Raiz, const std::string &cad){
	
	std::stack<char> pila;
	std::string msg = "";
	int actual, padre;

	for (int i = 0; i < cad.size(); i++){
		
		int j = 0;

		while (M[0][j] != cad[i]){ // Buscamos la posición del elemento
			j++;
		}
		
		padre = j;

		do {
			actual = padre;
			padre = M[3][actual];
			char hijo = (M[1][padre] == actual) ? '0':'1'; // Determinamos si 'actual' es hijo izquierda o derecha.
			pila.push(hijo);

		}while(padre != Raiz);

		while (!pila.empty()){
			msg += pila.top();
			pila.pop();
		}
	}

	return strdup(msg.c_str());
}

void toUpperPointer (char * temp){
	char * p = temp;

	while (*p){
		*p = toupper((unsigned char) *p);
		p++;
	}

}

// Función para crear la matriz de codificación Huffman.
int creaHuffman(std::set<nodoParcial,classcomp> simbolos, std::vector<std::vector<int>> &arbol, const int &cont){
    int tamanio_m = cont;
    
    while(simbolos.size() > 1){
        
        // Extraemos los dos menos probables de los simbolos
        auto it = simbolos.end();
        it--;
        nodoParcial hijo1 = *(it);
        simbolos.erase(it);
        
        //sacamos el segundo hijo
        it--;
        nodoParcial hijo2 = *(it);
        simbolos.erase(it);
        

        //introducimos un nuevo nodo en el arbol
        arbol[0][tamanio_m] = '*';
        arbol[1][tamanio_m] = hijo1.pos;
        arbol[2][tamanio_m] = hijo2.pos;

        //modificar el padre de los hijos que hemos sacado
        arbol[3][hijo1.pos] = tamanio_m;
        arbol[3][hijo2.pos] = tamanio_m;

        //añadimos a simbolos el nuevo nodo intermedio
        nodoParcial nuevo = {'~',hijo1.frecuencia+hijo2.frecuencia,tamanio_m};
        simbolos.insert(nuevo);

        tamanio_m++;
    }
    
    return tamanio_m-1;
}

// -------------------------------------------------------

int main(){
	int fd;
	std::vector<std::vector <int> > arbol(4, std::vector<int>(64, -1));
	std::unordered_map<char, unsigned int> uMap;
	std::set<nodoParcial, classcomp> simbolos;
	char cadena[100];
	char * cadena_bits;

	// Inicializamos el puerto USB
	fd = InicializarUSB("/dev/ttyACM0");

	if (fd < 0)
		return 0;

	

	// -- Paso 1 & 2: Lectura y cálculo de frecuencias de quijote.txt --
	// Cargamos las frecuencias 
	uMap = loadFrec("quijote.txt");
	uMap[':'] = 0;

    // -- Paso 3: Generar una lista con los simbolos --
    int cont = 0;
    
    for (auto it = uMap.begin() ; it != uMap.end() ; it++){
        nodoParcial nuevo = {it->first,it->second,cont};
        simbolos.insert(nuevo);
        cont++;
    }

    // -- Paso 4: Asignar cada simbolo terminal de la lista a un nodo terminal --
    for(auto it = simbolos.begin(); it != simbolos.end() ; it++){
        arbol[0][it->pos] = it->simbolo;
    }
    
    // -- Paso 5: Creacion del codigo Huffman --
    int raiz = creaHuffman(simbolos, arbol, cont);

	// Introducimos la cadena a enviar
	std::cout << "Introduzca la cadena a enviar: ";
	std::cin.getline(cadena, sizeof(cadena));


	while (strcmp(cadena, "fin") != 0){

		// Añadimos el carácter de finalización
		//strcat(cadena, "*");

		// Convertimos toda la cadena a mayúsculas
		toUpperPointer(cadena);

		std::cout << "Introducido: \"" << cadena << "\"" << std::endl;

		// Codificamos la cadena recibida antes de enviarla.
		cadena_bits = codificaCadena (arbol, raiz, cadena);

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