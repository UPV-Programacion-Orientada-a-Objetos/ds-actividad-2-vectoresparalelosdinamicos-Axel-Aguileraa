#include <iostream>
#include <string>
#include <iomanip>
#include <new>

using namespace std;

int* ids = nullptr;
double* pesos = nullptr;
string* destinos = nullptr;
int* prioridades = nullptr;
char* estados = nullptr;

int capacidad = 0;
int frente = 0;
int final = 0;

void mostrarMenu();
void agregarPaquete();
bool duplicarCapacidad();
void despacharPaquete();
void inspeccionarFrente();
void reportePorDestino();
void liberarMemoria();

int main() {
    capacidad = 50;
    try {
        ids = new int[capacidad];
        pesos = new double[capacidad];
        destinos = new string[capacidad];
        prioridades = new int[capacidad];
        estados = new char[capacidad];
        cout << "--- Sistema de Despacho Logístico MegaEnvío (Modo Punteros) ---" << endl << endl;
        cout << "Inicializando sistema con capacidad para " << capacidad << " paquetes..." << endl;
    } catch (const bad_alloc& e) {
        cout << "Error fatal: No se pudo asignar la memoria inicial. " << e.what() << endl;
        return 1;
    }

    int opcion;
    do {
        cout << "\nCapacidad actual: " << capacidad << ". Paquetes en cola: " << (final - frente) << "." << endl;
        mostrarMenu();
        cin >> opcion;

        while (cin.fail()) {
            cout << "Entrada inválida. Por favor ingrese un número." << endl;
            cin.clear();
            cin.ignore(256,'\n');
            cin >> opcion;
        }
        cin.ignore(); 

        switch (opcion) {
            case 1:
                agregarPaquete();
                break;
            case 2:
                despacharPaquete();
                break;
            case 3:
                inspeccionarFrente();
                break;
            case 4:
                reportePorDestino();
                break;
            case 5:
                liberarMemoria();
                cout << "Sistema cerrado. ¡Memoria libre!" << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (opcion != 5);

    return 0;
}

void mostrarMenu() {
    cout << "\nSeleccione una operación:" << endl;
    cout << "1. Agregar Paquete (Encolar)" << endl;
    cout << "2. Despachar Paquete (Desencolar)" << endl;
    cout << "3. Inspeccionar Frente de Cola" << endl;
    cout << "4. Reporte por Destino" << endl;
    cout << "5. Salir (Liberar Memoria)" << endl;
    cout << "Opción seleccionada: ";
}

void agregarPaquete() {
    if (final == capacidad) {
        cout << "Capacidad máxima alcanzada. Intentando duplicar el espacio..." << endl;
        if (!duplicarCapacidad()) {
            cout << "No se pudo agregar el paquete por falta de memoria." << endl;
            return;
        }
    }

    cout << "Ingrese ID: ";
    cin >> ids[final];
    cout << "Ingrese Peso (kg): ";
    cin >> pesos[final];
    cout << "Ingrese Destino: ";
    cin.ignore();
    getline(cin, destinos[final]);
    cout << "Ingrese Prioridad (1=Alta, 2=Media, 3=Baja): ";
    cin >> prioridades[final];

    estados[final] = 'E';

    cout << "Paquete " << ids[final] << " encolado. Capacidad utilizada: " << (final + 1) << "/" << capacidad << "." << endl;
    final++;
}

bool duplicarCapacidad() {
    int nuevaCapacidad = capacidad * 2;
    try {
        int* nuevos_ids = new int[nuevaCapacidad];
        double* nuevos_pesos = new double[nuevaCapacidad];
        string* nuevos_destinos = new string[nuevaCapacidad];
        int* nuevas_prioridades = new int[nuevaCapacidad];
        char* nuevos_estados = new char[nuevaCapacidad];

        for (int i = 0; i < capacidad; ++i) {
            nuevos_ids[i] = ids[i];
            nuevos_pesos[i] = pesos[i];
            nuevos_destinos[i] = destinos[i];
            nuevas_prioridades[i] = prioridades[i];
            nuevos_estados[i] = estados[i];
        }

        liberarMemoria();

        ids = nuevos_ids;
        pesos = nuevos_pesos;
        destinos = nuevos_destinos;
        prioridades = nuevas_prioridades;
        estados = nuevos_estados;

        capacidad = nuevaCapacidad;
        cout << "¡Memoria duplicada! Nueva capacidad: " << capacidad << " paquetes." << endl;
        return true;
    } catch (const bad_alloc& e) {
        cout << "Error al duplicar la capacidad: " << e.what() << endl;
        return false;
    }
}

void despacharPaquete() {
    if (frente == final) {
        cout << "La cola de despacho está vacía. No hay paquetes para despachar." << endl;
        return;
    }

    cout << "Despachando paquete..." << endl;
    estados[frente] = 'D';
    cout << "Paquete " << ids[frente] << " despachado con éxito. Estado: '" << estados[frente] << "'." << endl;
    frente++;
}

void inspeccionarFrente() {
    if (frente == final) {
        cout << "La cola de despacho está vacía." << endl;
        return;
    }

    cout << "Frente de la cola:" << endl;
    string prioridadStr = (prioridades[frente] == 1) ? "Alta" : (prioridades[frente] == 2) ? "Media" : "Baja";
    cout << "  ID: " << ids[frente] << " | Peso: " << fixed << setprecision(1) << pesos[frente]
         << " kg | Destino: " << destinos[frente] << " | Prioridad: " << prioridadStr << endl;
}

void reportePorDestino() {
    if (frente == final) {
        cout << "La cola está vacía, no se pueden generar reportes." << endl;
        return;
    }
    
    string destinoBuscado;
    cout << "Ingrese el destino para el reporte: ";
    getline(cin, destinoBuscado);

    int contador = 0;
    double sumaPesos = 0.0;

    for (int i = frente; i < final; ++i) {
        if (destinos[i] == destinoBuscado && estados[i] == 'E') {
            contador++;
            sumaPesos += pesos[i];
        }
    }

    cout << "Reporte para destino '" << destinoBuscado << "':" << endl;
    cout << "  Paquetes En Cola: " << contador << endl;
    if (contador > 0) {
        double promedio = sumaPesos / contador;
        cout << "  Peso Promedio: " << fixed << setprecision(2) << promedio << " kg" << endl;
    } else {
        cout << "  Peso Promedio: 0.00 kg" << endl;
    }
}

void liberarMemoria() {
    cout << "Liberando " << capacidad << " bloques de memoria asignada..." << endl;
    delete[] ids;
    delete[] pesos;
    delete[] destinos;
    delete[] prioridades;
    delete[] estados;

    ids = nullptr;
    pesos = nullptr;
    destinos = nullptr;
    prioridades = nullptr;
    estados = nullptr;
}
