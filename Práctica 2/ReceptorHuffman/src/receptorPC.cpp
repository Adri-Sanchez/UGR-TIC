#include <PCSerialUSB.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

/***
 * Funcion para decodifiar un codigo de 0 y 1, dado un arbol
 * Para la decodificacion el proceso a seguir es situarnos en el nodo raiz
 * he ir descendiendo a hijo izquierda si es '0' o a hijo derecha sies '1'
 * Al alcanzar un nodo hoja, hemos encontrado el simbolo y volvemos a decodificar
 * desde la raiz hasta que la cadena recibida haya sido decodificada por completo*/
string decodifica(vector<vector<int>> &arbol, string &cadena, int inicio){
    int pos;
    string mensaje;
    //debemos decodificar toda la cadena de entrada
    for(auto it = cadena.begin(); it != cadena.end() ; it){
        pos = inicio;
        //iteramos soblre la cadena hasta alcanzar un nodo hoja
        while( (arbol[1][pos] != -1) && (arbol[2][pos]!= -1) ){
            //cout << pos << " " << *it << " " << endl;
            if((char)*it == '0'){
                //pasamos al hijo a la izquierda
                //cout << " es un cero" << endl;
                pos=arbol[1][pos];
                it++;
            }
            else{
                //cout << " es un uno" << endl;
                //pasamos al hijo a la derecha
                pos=arbol[2][pos];
                it++;
            }

        }
        //cout << "SALE" << endl;
        //añadimos el nuevo caracter decodificado al mensaje
        mensaje.push_back( (char)arbol[0][pos] );
    }
    return mensaje;
}
/**
 * Funcion para cargar un archivo y calcular las frecuencias de cada simblo*/
std::unordered_map<char, unsigned int> loadFrec (const std::string &file){
    std::unordered_map<char, unsigned int> uMap;
    std::ifstream fin;
    std::string str;

    //abrimos el fichero
    fin.open(file, std::ios::in);

    if (!fin){
        std::cerr << "Error al leer el fichero: " << file << std::endl;
    }
    else{
        std::cout << "Leyendo archivo " << file << "..." << std::endl;
  
        // Mientras lleguemos al final del archivo
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


//estructura para almacenar los nodos en el set ordenado
struct nodoParcial{
    char simbolo;
    unsigned int frecuencia;
    int pos;
};

//funtor que nos permite ordenar por frecuencias los nodos
struct classcomp {
  bool operator() (const nodoParcial& lhs, const nodoParcial& rhs) const
  {return lhs.frecuencia >= rhs.frecuencia;}
};


int creaHuffman(set<nodoParcial,classcomp>simbolos,vector<vector<int>> &arbol,int cont){
    int tamanio_m = cont;
    while(simbolos.size() > 1){
        //sacamos los dos menos probables de los simbolos
        auto it = simbolos.end();
        it--;
        nodoParcial hijo1 = *(it);
        simbolos.erase( it);
        //cout << hijo1.simbolo  << "  " ;
        //sacamos el segundo hijo
        it--;
        nodoParcial hijo2 = *(it);
        simbolos.erase(it);
        //cout << hijo2.simbolo  << "  " ;
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


int main(){
    int fd;
    char cadena[100];


    //paso 1 y 2 lectura de frecuencias del quijote
    unordered_map<char,unsigned int> frecuencias = loadFrec("quijote.txt");
    frecuencias[':'] = 0;
    
    if(true){
        float suma=0;
        for(auto it =frecuencias.begin(); it != frecuencias.end() ; it++){
            suma += it->second;
        }
        for(auto it =frecuencias.begin(); it != frecuencias.end() ; it++){
            cout << it->first << " " << it->second/suma << endl;
        }
    }

    //creamos el arbol y el set ordenado de simbolos
    vector<vector<int>> arbol(4,vector<int>(64,-1));
    set<nodoParcial,classcomp> simbolos;

    //paso3 generar una lista con los simbolos
    int cont = 0;
    for(auto it = frecuencias.begin() ; it != frecuencias.end() ; it++){
        nodoParcial nuevo = {it->first,it->second,cont};
        simbolos.insert(nuevo);
        cont++;
    }

    //paso4 asignar cada simbolo terminal de la lista a un nodo terminal
    for(auto it = simbolos.begin(); it != simbolos.end() ; it++){
        arbol[0][it->pos] = it->simbolo;
    }
    

    ////////////////////////////////////////////
    //paso 5 creacion del codigo huffman
    /////////////////////////////////////////
    int raiz = creaHuffman(simbolos,arbol,cont);

    //para imprimir la matriz del arbol
    

    //abrimos el puerto USB
    fd = InicializarUSB("/dev/ttyACM0");
    if(fd<0){
        cout << "ERROR"<<endl;
        return 0;
    }

    while(1){
        cout << "Esperando el mensaje" << endl;
        string mensaje = "";
        if(receiveUSB(fd,cadena)){
            string codigo = cadena;
            codigo.pop_back();
            codigo.pop_back();
            //cout << codigo << "EERER" << endl;
            string mensaje = decodifica(arbol,codigo,raiz);
            cout << ">>>> Arduino ha recibido: " << mensaje << endl;
        }
    }
    CerrarUSB(fd);
    return 0;
}
