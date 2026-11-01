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

//Terrel - GESTOR DE MEMORIA (Pila)

class GestorMemoria 
{
private:
    NodoPila* tope;

public:
    GestorMemoria() 
   { 
       tope = NULL; 
   }

    // Push: asignar bloque de memoria a un proceso

    void push(int idBloque, int tamanio, int idProceso)
   {
        NodoPila* nuevo = new NodoPila();
        nuevo->idBloque = idBloque;
        nuevo->tamanio = tamanio;
        nuevo->idProceso = idProceso;
        nuevo->siguiente = tope;
        tope = nuevo;
        cout << "[OK] Bloque " << idBloque << " (" << tamanio
             << " MB) asignado al proceso " << idProceso <<"\n";
    }

    // Pop: liberar el bloque de memoria del tope

    void pop() 
    {
        if (tope == NULL)
        {
            cout << "[ERROR] No hay bloques de memoria asignados." << "\n";
            return;
        }
        NodoPila* temp = tope;
        cout << "[OK] Liberando bloque " << temp->idBloque
             << " (" << temp->tamanio << " MB) del proceso " << temp->idProceso <<"\n";
        tope = tope->siguiente;
        delete temp;
    }

    // Verificar estado actual de la memoria

    void verificar() 
    {
        if (tope == NULL) 
        {
            cout << "Memoria libre." <<"\n";
            return;
        }
        cout << "===============================\n";
        cout << "      ESTADO DE MEMORIA        \n";
        cout << "===============================\n";
        NodoPila* actual = tope;
        while (actual != NULL) 
        {
            cout << "Bloque: " << actual->idBloque
                 << " | Tamaño: " << actual->tamanio << " MB" << " | Proceso: " << actual->idProceso << "\n";
            actual = actual->siguiente;
        }
        cout << "===============================" << "\n";
    }

    // Guardar pila en archivo

    void guardar(ofstream& archivo) 
    {
        NodoPila* actual = tope;
        while (actual != NULL) 
        {
            archivo << "M " << actual->idBloque << " "
                    << actual->tamanio << " " << actual->idProceso << "\n";
            actual = actual->siguiente;
        }
    }

    // Destructor

    ~GestorMemoria() 
    {
        NodoPila* actual = tope;
        while (actual != NULL) 
        {
            NodoPila* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

// ============================================================
// PERSISTENCIA DE DATOS
// ============================================================

// Guardar el estado completo del sistema en un archivo

void guardarSistema(GestorProcesos& gp, PlanificadorCPU& cpu, GestorMemoria& mem) 
{
    ofstream archivo("sistema.txt");
    if (!archivo.is_open()) 
    {
        cout << "No se pudo abrir el archivo para guardar" << "\n";
        return;
    }
    gp.guardar(archivo);
    cpu.guardar(archivo);
    mem.guardar(archivo);
    archivo.close();
    cout << "Estado del sistema guardado" <<"\n";
}

// Cargar el estado del sistema desde el archivo

void cargarSistema(GestorProcesos& gp, PlanificadorCPU& cpu, GestorMemoria& mem) {
    ifstream archivo("sistema.txt");
    if (!archivo.is_open()) 
    {
        cout << "No se encontró archivo guardado. Iniciando sistema vacío" << "\n";
        return;
    }
    char tipo;
    while (archivo >> tipo) 
    {
        if (tipo == 'P') 
        {
            int id, prioridad;
            string nombre;
            archivo >> id >> nombre >> prioridad;
            gp.cargar(id, nombre, prioridad);
        } 
        else if (tipo == 'C') 
        {
            int id, prioridad;
            string nombre;
            archivo >> id >> nombre >> prioridad;
            cpu.encolar(id, nombre, prioridad);
        } 
        else if (tipo == 'M') 
        {
            int idBloque, tamanio, idProceso;
            archivo >> idBloque >> tamanio >> idProceso;
            mem.push(idBloque, tamanio, idProceso);
        }
    }
    archivo.close();
    cout << "Estado del sistema cargado correctamente." << "\n";
}
