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

// Guarda la Pokedex completa de nuevo en el archivo CSV
void guardarEnCSV(const string& nombreArchivo, Pokemon* pokedex, int cantidad) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para guardar datos." << endl;
        return;
    }

    // cabecera
    archivo << "Nº Pokedex,Nombre,Tipo 1,Tipo 2,Total,HP,Ataque,Defensa,Ataque Esp.,Defensa Esp.,Velocidad,Generación,Legendario\n";

    for (int i = 0; i < cantidad; i++) {
        archivo << pokedex[i].numeroPokedex << ","
                << pokedex[i].nombre << ","
                << pokedex[i].tipo1 << ","
                << pokedex[i].tipo2 << ","
                << pokedex[i].total << ","
                << pokedex[i].hp << ","
                << pokedex[i].ataque << ","
                << pokedex[i].defensa << ","
                << pokedex[i].ataqueEsp << ","
                << pokedex[i].defensaEsp << ","
                << pokedex[i].velocidad << ","
                << pokedex[i].generacion << ","
                << (pokedex[i].legendario ? "True" : "False") << "\n";
    }

    archivo.close();
    cout << "-> Datos guardados exitosamente en " << nombreArchivo << endl;
}

// Muestra la información individual de un Pokémon
void mostrarPokemon(const Pokemon& p) {
    cout << "----------------------------------------------------" << endl;
    cout << "Nº Pokedex : #" << p.numeroPokedex << endl;
    cout << "Nombre     : " << p.nombre << (p.legendario ? " ★ [LEGENDARIO]" : "") << endl;
    cout << "Tipo(s)    : " << p.tipo1 << (p.tipo2.empty() ? "" : " / " + p.tipo2) << endl;
    cout << "Est. Total : " << p.total << endl;
    cout << "   - HP          : " << p.hp << endl;
    cout << "   - Ataque      : " << p.ataque << endl;
    cout << "   - Defensa     : " << p.defensa << endl;
    cout << "   - Ataque Esp. : " << p.ataqueEsp << endl;
    cout << "   - Defensa Esp.: " << p.defensaEsp << endl;
    cout << "   - Velocidad   : " << p.velocidad << endl;
    cout << "Generación : " << p.generacion << endl;
    cout << "----------------------------------------------------" << endl;
}

// Busqueda por número o por fragmento del nombre
void buscarPokemon(Pokemon* pokedex, int cantidad) {
    int opcion;
    cout << "\n1. Buscar por Nº de Pokedex\n2. Buscar por Nombre\nSeleccione opcion: ";
    cin >> opcion;

    if (opcion == 1) {
        int id;
        cout << "Ingrese el Nº Pokedex: ";
        cin >> id;
        bool encontrado = false;
        for (int i = 0; i < cantidad; i++) {
            if (pokedex[i].numeroPokedex == id) {
                mostrarPokemon(pokedex[i]);
                encontrado = true;
                break;
            }
        }
        if (!encontrado) cout << "No se encontró ningún Pokémon con el número " << id << endl;
    } else if (opcion == 2) {
        string subNombre;
        cout << "Ingrese el nombre a buscar: ";
        cin >> subNombre;
        subNombre = aMinusculas(subNombre);
        
        bool encontrado = false;
        for (int i = 0; i < cantidad; i++) {
            if (aMinusculas(pokedex[i].nombre).find(subNombre) != string::npos) {
                mostrarPokemon(pokedex[i]);
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron coincidencias para '" << subNombre << "'" << endl;
    }
}

// Filtra la lista por Tipo (Planta, Fuego, Agua, etc.)
void filtrarPorTipo(Pokemon* pokedex, int cantidad) {
    string tipoBusqueda;
    cout << "Ingrese el tipo a filtrar (ej. Fire, Water, Grass, Poison): ";
    cin >> tipoBusqueda;
    tipoBusqueda = aMinusculas(tipoBusqueda);

    int encontrados = 0;
    cout << "\n--- POKÉMON ENCONTRADOS DEL TIPO: " << tipoBusqueda << " ---" << endl;
    for (int i = 0; i < cantidad; i++) {
        if (aMinusculas(pokedex[i].tipo1) == tipoBusqueda || aMinusculas(pokedex[i].tipo2) == tipoBusqueda) {
            cout << "#" << left << setw(4) << pokedex[i].numeroPokedex 
                   << setw(15) << pokedex[i].nombre 
                 << " [" << pokedex[i].tipo1 << (pokedex[i].tipo2.empty() ? "" : "/" + pokedex[i].tipo2) << "]" << endl;
            encontrados++;
        }
    }
    cout << "Total encontrados: " << encontrados << endl;
}
// Agrega un nuevo Pokémon reasignando dinámicamente la memoria
void agregarPokemon(Pokemon*& pokedex, int& cantidad, const string& nombreArchivo) {
    // Crear un nuevo arreglo con espacio para un elemento adicional
    Pokemon* nuevoArreglo = new Pokemon[cantidad + 1];

    // Copiar elementos previos
    for (int i = 0; i < cantidad; i++) {
        nuevoArreglo[i] = pokedex[i];
    }

    Pokemon nuevo;
    nuevo.numeroPokedex = cantidad + 1;
    cout << "\n--- REGISTRAR NUEVO POKÉMON (ID #" << nuevo.numeroPokedex << ") ---" << endl;
    
    cout << "Nombre: "; cin >> nuevo.nombre;
    cout << "Tipo 1: "; cin >> nuevo.tipo1;
    cout << "Tipo 2 (dejar '-' si no tiene): "; cin >> nuevo.tipo2;
    if (nuevo.tipo2 == "-") nuevo.tipo2 = "";

    cout << "HP: "; cin >> nuevo.hp;
    cout << "Ataque: "; cin >> nuevo.ataque;
    cout << "Defensa: "; cin >> nuevo.defensa;
    cout << "Ataque Especial: "; cin >> nuevo.ataqueEsp;
    cout << "Defensa Especial: "; cin >> nuevo.defensaEsp;
    cout << "Velocidad: "; cin >> nuevo.velocidad;
    cout << "Generacion: "; cin >> nuevo.generacion;
    
    char leg;
    cout << "¿Es Legendario? (s/n): "; cin >> leg;
    nuevo.legendario = (leg == 's' || leg == 'S');

    // Calcula el stat Total sumando las estadísticas
    nuevo.total = nuevo.hp + nuevo.ataque + nuevo.defensa + nuevo.ataqueEsp + nuevo.defensaEsp + nuevo.velocidad;

    // Guarda el nuevo registro en la última posición
    nuevoArreglo[cantidad] = nuevo;

    // Libera la memoria del antiguo arreglo y redirige el puntero
    delete[] pokedex;
    pokedex = nuevoArreglo;
    cantidad++;

    guardarEnCSV(nombreArchivo, pokedex, cantidad);
    cout << "¡Pokémon agregado y archivo guardado exitosamente!" << endl;
}
int main() {
    string archivo = "pokemon_150_registros.csv";
    int cantidad = 0;

    // Cargar los datos mediante punteros y memoria dinámica
    Pokemon* pokedex = cargarDesdeCSV(archivo, cantidad);

    if (pokedex == nullptr) {
        cout << "No se pudo cargar la base de datos de la Pokédex." << endl;
        return 1;
    }

    cout << "====================================================" << endl;
    cout << "                    SISTEMA POKÉDEX                 " << endl;
    cout << "====================================================" << endl;
    cout << "Registros cargados exitosamente: " << cantidad << endl;

    int opcion;
    do {
        cout << "\n------------------ MENÚ PRINCIPAL ------------------" << endl;
        cout << "1. Mostrar todos los Pokémon" << endl;
        cout << "2. Buscar Pokémon (Nº o Nombre)" << endl;
        cout << "3. Filtrar por Tipo" << endl;
        cout << "4. Registrar nuevo Pokémon" << endl;
        cout << "5. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                for (int i = 0; i < cantidad; i++) {
                    cout << "#" << left << setw(4) << pokedex[i].numeroPokedex 
                         << setw(15) << pokedex[i].nombre 
                         << " Total: " << setw(5) << pokedex[i].total 
                         << " Tipo 1: " << setw(10) << pokedex[i].tipo1 
                         << " Tipo 2: " << pokedex[i].tipo2 << endl;
                }
                break;
            case 2:
                buscarPokemon(pokedex, cantidad);
                break;
            case 3:
                filtrarPorTipo(pokedex, cantidad);
                break;
            case 4:
                agregarPokemon(pokedex, cantidad, archivo);
                break;
            case 5:
                cout << "Cerrando sistema Pokédex..." << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
        }
    } while (opcion != 5);

    // Liberar la memoria dinámica asignada antes de finalizar el programa
    delete[] pokedex;
    pokedex = nullptr;

    return 0;
}