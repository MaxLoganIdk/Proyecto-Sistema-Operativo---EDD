//SISTEMA OPERATIVO (Listas, Pilas y Colas)
#include <fstream>
#include <iostream>
using namespace std;

// NODOS Y ESTRUCTURAS DE DATOS

// Nodo para la lista enlazada de procesos
struct NodoProceso {
    int id;
    string nombre;
    int prioridad;
    NodoProceso* siguiente;
};

// Nodo para la cola de prioridad (Planificador CPU)
struct NodoCola {
    int id;
    string nombre;
    int prioridad;
    NodoCola* siguiente;
};

// Nodo para la pila de memoria
struct NodoPila {
    int idBloque;
    int tamanio;
    int idProceso;
    NodoPila* siguiente;
};
//garay Rosales Lionel
class GestorProcesos {
private:
    NodoProceso* cabeza;

public:
    GestorProcesos() { cabeza = NULL; }

    // Insertar proceso al final de la lista
    void insertar(int id, string nombre, int prioridad) {
        // Verificar si ya existe un proceso con ese ID
        NodoProceso* verificar = cabeza;
        while (verificar != NULL) {
            if (verificar->id == id) {
                cout << "[ERROR] Ya existe un proceso con ID " << id << endl;
                return;
            }
            verificar = verificar->siguiente;
        }

        NodoProceso* nuevo = new NodoProceso();
        nuevo->id = id;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->siguiente = NULL;

        // Si la lista est  vac a, el nuevo nodo es la cabeza
        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            // Recorrer hasta el  ltimo nodo y enlazar
            NodoProceso* actual = cabeza;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cout << "[OK] Proceso '" << nombre << "' registrado con ID " << id << endl;
    }

    // Eliminar proceso por ID
    void eliminar(int id) {
        if (cabeza == NULL) {
            cout << "[ERROR] No hay procesos registrados." << endl;
            return;

//Maldonado Meza Joseph 
void pushMemoria(int idBloque, int tamanio, int idProceso) {
    NodoPila* nuevo = new NodoPila{idBloque, tamanio, idProceso, topePila};
    topePila = nuevo;
    cout << "[Gestor Memoria] Bloque ID " << idBloque << " (" << tamanio 
         << " KB) reservado para Proceso ID " << idProceso << "\n";
}

void popMemoria() {
    if (!topePila) {
        cout << "[Gestor Memoria] La pila de memoria esta vacia.\n";
        return;
    }
    NodoPila* temp = topePila;
    cout << "[Gestor Memoria] Liberando bloque ID " << temp->idBloque 
         << " (" << temp->tamanio << " KB) del Proceso ID " << temp->idProceso << "\n";
    
    topePila = topePila->siguiente;
    delete temp;
}
void mostrarPilaMemoria() {
    cout << "    ESTADO DE LA MEMORIA (PILA)     \n";
    NodoPila* aux = topePila;
    if (!aux) {
        cout << "No hay bloques de memoria asignados.\n";
        return;
    }
    while (aux != nullptr) {
        cout << "| Bloque ID: " << aux->idBloque << " | Tamanio: " << aux->tamanio 
             << " KB | Proceso Asignado ID: " << aux->idProceso << " |\n";
        aux = aux->siguiente;
    }
}

