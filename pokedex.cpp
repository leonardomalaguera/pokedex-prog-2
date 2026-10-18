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