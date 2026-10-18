#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// esta es la estructura para almacenar cada dato de los pokemones 
struct Pokemon {
    int numeroPokedex;
    string nombre;
    string tipo1;
    string tipo2;
    int total;
    int hp;
    int ataque;
    int defensa;
    int ataqueEsp;
    int defensaEsp;
    int velocidad;
    int generacion;
    bool legendario;
};

// convierte todo texto a minusculas 
string aMinusculas(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// esta es una función para cargar los pokemones desde el archivo CSV usando memoria dinámica
Pokemon* cargarDesdeCSV(const string& nombreArchivo, int& cantidad) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        cantidad = 0;
        return nullptr;
    }
    string linea;
    // Lee y descartar la primera linea 
    getline(archivo, linea); 
    
    // cuenta cuantas lineas de registros validos tiene el archivo
    int contador = 0;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            contador++; // Incrementa por cada Pokémon encontrado
        }
    }

    // Validacion por si el archivo esta vacio
    if (contador == 0) {
        archivo.close();
        cantidad = 0;
        return nullptr;
    }

    //Reserva dinámica de memoria: Creamos un arreglo en memoria con el tamaño exacto del conteo
    Pokemon* pokedex = new Pokemon[contador];
    cantidad = contador; // Guardamos la cantidad total en la variable por referencia

    // para volver a leer desde el principio el archivo
    archivo.clear();             
    archivo.seekg(0, ios::beg);  
    getline(archivo, linea);     

    int idx = 0; 
    
    // leer el archivo linea por linea y parsear los datos
    while (getline(archivo, linea) && idx < cantidad) {
        if (linea.empty()) continue; // Salta líneas vacías por seguridad

        // stringstream para fragmentar la linea usando la coma como separador
        stringstream ss(linea);
        string campo;

        // Extrae y convierte cada columna a su tipo de dato correspondiente
        getline(ss, campo, ','); pokedex[idx].numeroPokedex = stoi(campo); // stoi: string to int
        getline(ss, pokedex[idx].nombre, ',');
        getline(ss, pokedex[idx].tipo1, ',');
        getline(ss, pokedex[idx].tipo2, ',');
        getline(ss, campo, ','); pokedex[idx].total = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].hp = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].ataque = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].defensa = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].ataqueEsp = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].defensaEsp = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].velocidad = stoi(campo);
        getline(ss, campo, ','); pokedex[idx].generacion = stoi(campo);
        
        getline(ss, campo, ','); 
        // Convertir el último campo (legendario) a booleano evaluando texto
        string campoB = aMinusculas(campo);
        pokedex[idx].legendario = (campoB == "true" || campoB == "1");

        idx++; // Avanza a la siguiente posición del arreglo dinámico
    }
    archivo.close();
    return pokedex;
}