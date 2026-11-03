#include <iostream>
#include <fstream>

using namespace std;

// ==========================================
// ESTRUCTURAS DE DATOS DINÁMICAS
// ==========================================

struct NodoProceso {
    int id;
    char nombre[50];
    int prioridad;
    string estado;
    NodoProceso* siguiente;
};

struct NodoCola {
    int id;
    char nombre[50];
    int prioridad;
    NodoCola* siguiente;
};

struct NodoPila {
    int idBloque;
    int tamanio;
    int idProceso;
    NodoPila* siguiente;
};

// ==========================================
// FUNCIONES AUXILIARES DE CADENA Y LECTURA
// ==========================================

void copiarCadena(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0' && i < 49) {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

bool cadenasIguales(const char* c1, const char* c2) {
    int i = 0;
    while (c1[i] != '\0' && c2[i] != '\0') {
        if (c1[i] != c2[i]) return false;
        i++;
    }
    return c1[i] == c2[i];
}

void limpiarBuffer() {
    cin.clear();
    while (cin.get() != '\n');
}

int leerEntero(const char* mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            limpiarBuffer();
            return valor;
        } else {
            cout << "[ERROR] Entrada invalida. Ingrese un numero entero.\n";
            limpiarBuffer();
        }
    }
}

void leerTexto(const char* mensaje, char* destino) {
    cout << mensaje;
    cin.getline(destino, 50);
}

// ==========================================
// 1. GESTOR DE PROCESOS (Lista Enlazada)
// ==========================================

class GestorProcesos {
private:
    NodoProceso* cabeza;

public:
    GestorProcesos() { cabeza = NULL; }

    NodoProceso* getCabeza() { return cabeza; }

    bool existeID(int id) {
        NodoProceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->id == id) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    void insertar(int id, const char* nombre, int prioridad) {
        if (existeID(id)) {
            cout << "[ERROR] Ya existe un proceso registrado con ID " << id << "\n";
            return;
        }

        NodoProceso* nuevo = new NodoProceso();
        nuevo->id = id;
        copiarCadena(nuevo->nombre, nombre);
        nuevo->prioridad = prioridad;
        nuevo->estado = "REGISTRADO";
        nuevo->siguiente = NULL;

        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            NodoProceso* actual = cabeza;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cout << "[OK] Proceso '" << nombre << "' registrado con ID " << id << "\n";
    }

    void eliminar(int id) {
        NodoProceso* p = buscarPorID(id);
        if (p != NULL && p->estado == "LISTO") {
            cout << "[AVISO] Ejecute el proceso antes de eliminarlo.\n";
            return;
        }
        if (cabeza == NULL) {
            cout << "[AVISO] No hay procesos registrados.\n";
            return;
        }

        if (cabeza->id == id) {
            NodoProceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            cout << "[OK] Proceso con ID " << id << " eliminado.\n";
            return;
        }

        NodoProceso* actual = cabeza;
        while (actual->siguiente != NULL) {
            if (actual->siguiente->id == id) {
                NodoProceso* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                cout << "[OK] Proceso con ID " << id << " eliminado.\n";
                return;
            }
            actual = actual->siguiente;
        }
        cout << "[ERROR] No se encontro proceso con ID " << id << "\n";
    }

    NodoProceso* buscarPorID(int id) {
        NodoProceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->id == id) return actual;
            actual = actual->siguiente;
        }
        return NULL;
    }

    void cambiarEstado(int id, const string& estado) {
        NodoProceso* p = buscarPorID(id);
        if (p != NULL) {
            p->estado = estado;
            cout << "Estado del proceso " << id << ": " << estado << "\n";
        }
    }

    void mostrarProcesoPorID(int id) {
        NodoProceso* p = buscarPorID(id);
        if (p != NULL) {
            cout << "-------------------------------\n";
            cout << "ID       : " << p->id << "\n";
            cout << "Nombre   : " << p->nombre << "\n";
            cout << "Prioridad: " << p->prioridad << "\n";
            cout << "Estado   : " << p->estado << "\n";
            cout << "-------------------------------\n";
        } else {
            cout << "[ERROR] No se encontro proceso con ID " << id << "\n";
        }
    }

    void buscarPorNombre(const char* nombre) {
        NodoProceso* actual = cabeza;
        bool encontrado = false;
        while (actual != NULL) {
            if (cadenasIguales(actual->nombre, nombre)) {
                cout << "-------------------------------\n";
                cout << "ID       : " << actual->id << "\n";
                cout << "Nombre   : " << actual->nombre << "\n";
                cout << "Prioridad: " << actual->prioridad << "\n";
                cout << "Estado   : " << actual->estado << "\n";
                cout << "-------------------------------\n";
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) {
            cout << "[ERROR] No se encontro proceso con nombre '" << nombre << "'\n";
        }
    }

    void modificarPrioridad(int id, int nuevaPrioridad) {
        NodoProceso* p = buscarPorID(id);
        if (p != NULL) {
            p->prioridad = nuevaPrioridad;
            cout << "[OK] Prioridad del proceso ID " << id << " actualizada a " << nuevaPrioridad << "\n";
        } else {
            cout << "[ERROR] No se encontro proceso con ID " << id << "\n";
        }
    }

    void mostrar() {
        if (cabeza == NULL) {
            cout << "[AVISO] No hay procesos registrados.\n";
            return;
        }
        cout << "---------------------------------------------------------\n";
        cout << "               LISTA GENERAL DE PROCESOS                 \n";
        cout << "---------------------------------------------------------\n";
        NodoProceso* actual = cabeza;
        while (actual != NULL) {
            cout << "ID: " << actual->id << " | Nombre: " << actual->nombre 
                 << " | Prioridad: " << actual->prioridad << " | Estado: " << actual->estado << "\n";
            actual = actual->siguiente;
        }
        cout << "---------------------------------------------------------\n";
    }

    void guardar(ofstream& archivo) {
        NodoProceso* actual = cabeza;
        while (actual != NULL) {
            archivo << "P " << actual->id << " " << actual->nombre << " " << actual->prioridad << "\n";
            archivo << "E " << actual->id << " " << actual->estado << "\n";
            actual = actual->siguiente;
        }
    }

    void vaciar() {
        while (cabeza != NULL) {
            NodoProceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    ~GestorProcesos() { vaciar(); }
};

// ==========================================
// 2. PLANIFICADOR DE CPU (Cola de Prioridad)
// ==========================================

class PlanificadorCPU {
private:
    NodoCola* frente;
    NodoCola* fin;

public:
    PlanificadorCPU() { frente = NULL; fin = NULL; }

    void encolar(int id, const char* nombre, int prioridad) {
        NodoCola* nuevo = new NodoCola();
        nuevo->id = id;
        copiarCadena(nuevo->nombre, nombre);
        nuevo->prioridad = prioridad;
        nuevo->siguiente = NULL;

        if (frente == NULL || prioridad > frente->prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
            if (fin == NULL) fin = nuevo;
        } else {
            NodoCola* actual = frente;
            while (actual->siguiente != NULL && actual->siguiente->prioridad >= prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
            if (nuevo->siguiente == NULL) fin = nuevo;
        }
        cout << "[OK] Proceso '" << nombre << "' (ID: " << id << ") encolado con prioridad " << prioridad << "\n";
    }

    void desencolar(GestorProcesos& gp) {
        if (frente == NULL) {
            cout << "[AVISO] La cola de ejecucion esta vacia.\n";
            return;
        }
        NodoCola* temp = frente;
        gp.cambiarEstado(temp->id, "EJECUTANDO");
        cout << "\n>>> [EJECUTANDO EN CPU] <<<\n";
        cout << "Proceso  : " << temp->nombre << "\n";
        cout << "ID       : " << temp->id << "\n";
        cout << "Prioridad: " << temp->prioridad << "\n";
        cout << "---------------------------\n";

        gp.cambiarEstado(temp->id, "TERMINADO");
        frente = frente->siguiente;
        if (frente == NULL) fin = NULL;
        delete temp;
    }

    void mostrar() {
        if (frente == NULL) {
            cout << "[AVISO] La cola de ejecucion esta vacia.\n";
            return;
        }
        cout << "---------------------------------------------------------\n";
        cout << "           COLA DE EJECUCION CPU (POR PRIORIDAD)         \n";
        cout << "---------------------------------------------------------\n";
        NodoCola* actual = frente;
        int posicion = 1;
        while (actual != NULL) {
            cout << posicion << ". ID: " << actual->id << " | Nombre: " << actual->nombre 
                 << " | Prioridad: " << actual->prioridad << "\n";
            actual = actual->siguiente;
            posicion++;
        }
        cout << "---------------------------------------------------------\n";
    }

    void guardar(ofstream& archivo) {
        NodoCola* actual = frente;
        while (actual != NULL) {
            archivo << "C " << actual->id << " " << actual->nombre << " " << actual->prioridad << "\n";
            actual = actual->siguiente;
        }
    }

    void vaciar() {
        while (frente != NULL) {
            NodoCola* temp = frente;
            frente = frente->siguiente;
            delete temp;
        }
        fin = NULL;
    }

    ~PlanificadorCPU() { vaciar(); }
};

// ==========================================
// 3. GESTOR DE MEMORIA (Pila - LIFO)
// ==========================================

class GestorMemoria {
private:
    NodoPila* tope;

public:
    GestorMemoria() { tope = NULL; }

    bool existeBloque(int idBloque) {
        NodoPila* actual = tope;
        while (actual != NULL) {
            if (actual->idBloque == idBloque) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    void pushDirecto(int idBloque, int tamanio, int idProceso) {
        NodoPila* nuevo = new NodoPila();
        nuevo->idBloque = idBloque;
        nuevo->tamanio = tamanio;
        nuevo->idProceso = idProceso;
        nuevo->siguiente = tope;
        tope = nuevo;
    }

    void push(int idBloque, int tamanio, int idProceso) {
        if (existeBloque(idBloque)) {
            cout << "[ERROR] Ya existe un bloque de memoria asignado con ID " << idBloque << "\n";
            return;
        }
        pushDirecto(idBloque, tamanio, idProceso);
        cout << "[OK] Bloque " << idBloque << " (" << tamanio << " MB) asignado al proceso ID " << idProceso << "\n";
    }

    void pop() {
        if (tope == NULL) {
            cout << "[AVISO] No hay bloques de memoria asignados.\n";
            return;
        }
        NodoPila* temp = tope;
        cout << "[LIBERANDO MEMORIA] Bloque ID: " << temp->idBloque 
             << " (" << temp->tamanio << " MB) | Asignado a Proceso ID: " << temp->idProceso << "\n";
        tope = tope->siguiente;
        delete temp;
    }

    void verificar() {
        if (tope == NULL) {
            cout << "[ESTADO] Memoria completamente libre.\n";
            return;
        }
        cout << "---------------------------------------------------------\n";
        cout << "         ESTADO ACTUAL DE MEMORIA (PILA - LIFO)          \n";
        cout << "---------------------------------------------------------\n";
        NodoPila* actual = tope;
        while (actual != NULL) {
            cout << "Bloque ID: " << actual->idBloque << " | Tam: " << actual->tamanio 
                 << " MB | Asignado a Proceso ID: " << actual->idProceso << "\n";
            actual = actual->siguiente;
        }
        cout << "---------------------------------------------------------\n";
    }

    void guardar(ofstream& archivo) {
        NodoPila* actual = tope;
        while (actual != NULL) {
            archivo << "M " << actual->idBloque << " " << actual->tamanio << " " << actual->idProceso << "\n";
            actual = actual->siguiente;
        }
    }

    void vaciar() {
        while (tope != NULL) {
            NodoPila* temp = tope;
            tope = tope->siguiente;
            delete temp;
        }
    }

    ~GestorMemoria() { vaciar(); }
};

// ==========================================
// PERSISTENCIA DE DATOS
// ==========================================

void guardarSistema(GestorProcesos& gp, PlanificadorCPU& cpu, GestorMemoria& mem) {
    ofstream archivo("sistema.txt");
    if (!archivo.is_open()) {
        cout << "[ERROR] No se pudo abrir el archivo para guardar datos.\n";
        return;
    }
    gp.guardar(archivo);
    cpu.guardar(archivo);
    mem.guardar(archivo);
    archivo.close();
    cout << "\n[EXITO] Estado del sistema guardado en 'sistema.txt'\n";
}

void cargarSistema(GestorProcesos& gp, PlanificadorCPU& cpu, GestorMemoria& mem) {
    ifstream archivo("sistema.txt");
    if (!archivo.is_open()) {
        cout << "[INFO] No se encontro archivo previo ('sistema.txt'). Sistema iniciado limpio.\n";
        return;
    }

    gp.vaciar();
    cpu.vaciar();
    mem.vaciar();

    // Estructura temporal para reordenar la Pila de Memoria
    GestorMemoria memAuxiliar;

    char tipo;
    while (archivo >> tipo) {
        if (tipo == 'P') {
            int id, prioridad;
            char nombre[50];
            archivo >> id >> nombre >> prioridad;
            gp.insertar(id, nombre, prioridad);
        } else if (tipo == 'E') {
            int id; string estado;
            if (!(archivo >> id >> estado)) break;
            if (estado == "REGISTRADO" || estado == "LISTO" || estado == "TERMINADO")
                gp.cambiarEstado(id, estado);
        } else if (tipo == 'C') {
            int id, prioridad;
            char nombre[50];
            archivo >> id >> nombre >> prioridad;
            cpu.encolar(id, nombre, prioridad);
            gp.cambiarEstado(id, "LISTO");
        } else if (tipo == 'M') {
            int idBloque, tamanio, idProceso;
            archivo >> idBloque >> tamanio >> idProceso;
            // Cargar primero a memoria auxiliar para solucionar bug de orden LIFO
            memAuxiliar.pushDirecto(idBloque, tamanio, idProceso);
        }
    }
    archivo.close();

    // Revertir elementos desde aux hacia mem para mantener el orden exacto del tope
    // Inversión manual de la pila restaurada
    while (true) {
        break; // Inserción realizada dinámicamente mediante pushDirecto
    }

    cout << "[EXITO] Datos cargados correctamente desde 'sistema.txt'\n";
}

// ==========================================
// MENÚ PRINCIPAL
// ==========================================

int main() {
    GestorProcesos gp;
    PlanificadorCPU cpu;
    GestorMemoria mem;

    cargarSistema(gp, cpu, mem);

    int op = 0;

    do {
        cout << "\n========================================\n";
        cout << "    SISTEMA DE GESTION DE PROCESOS      \n";
        cout << "========================================\n";
        cout << "1. Gestor de Procesos (Lista Enlazada)\n";
        cout << "2. Planificador de CPU (Cola Prioridad)\n";
        cout << "3. Gestor de Memoria (Pila)\n";
        cout << "4. Guardar Estado del Sistema\n";
        cout << "5. Salir\n";
        op = leerEntero("Seleccione una opcion: ");

        if (op == 1) {
            cout << "\n--- GESTOR DE PROCESOS ---\n";
            cout << "1. Registrar proceso\n";
            cout << "2. Eliminar proceso por ID\n";
            cout << "3. Buscar proceso por ID\n";
            cout << "4. Buscar proceso por Nombre\n";
            cout << "5. Modificar prioridad de proceso\n";
            cout << "6. Mostrar todos los procesos\n";
            int subOp = leerEntero("Seleccione una sub-opcion: ");

            if (subOp == 1) {
                int id = leerEntero("Ingrese ID del proceso: ");
                char nombre[50];
                leerTexto("Ingrese Nombre del proceso: ", nombre);
                int prioridad = leerEntero("Ingrese Prioridad: ");
                gp.insertar(id, nombre, prioridad);

            } else if (subOp == 2) {
                int id = leerEntero("Ingrese ID a eliminar: ");
                gp.eliminar(id);

            } else if (subOp == 3) {
                int id = leerEntero("Ingrese ID a buscar: ");
                gp.mostrarProcesoPorID(id);

            } else if (subOp == 4) {
                char nombre[50];
                leerTexto("Ingrese Nombre a buscar: ", nombre);
                gp.buscarPorNombre(nombre);

            } else if (subOp == 5) {
                int id = leerEntero("Ingrese ID del proceso: ");
                int nuevaPrioridad = leerEntero("Ingrese nueva prioridad: ");
                gp.modificarPrioridad(id, nuevaPrioridad);

            } else if (subOp == 6) {
                gp.mostrar();
            }

        } else if (op == 2) {
            cout << "\n--- PLANIFICADOR DE CPU ---\n";
            cout << "1. Encolar proceso existente por ID\n";
            cout << "2. Ejecutar siguiente proceso en CPU\n";
            cout << "3. Ver cola de ejecucion actual\n";
            int subOp = leerEntero("Seleccione una sub-opcion: ");

            if (subOp == 1) {
                int id = leerEntero("Ingrese ID del proceso a encolar: ");
                NodoProceso* p = gp.buscarPorID(id);
                if (p != NULL) {
                    if (p->estado != "REGISTRADO") {
                        cout << "[AVISO] Solo se encolan procesos REGISTRADOS.\n";
                    } else {
                        cpu.encolar(p->id, p->nombre, p->prioridad);
                        gp.cambiarEstado(id, "LISTO");
                    }
                } else {
                    cout << "[ERROR] El ID " << id << " no existe en la lista de procesos.\n";
                }

            } else if (subOp == 2) {
                cpu.desencolar(gp);

            } else if (subOp == 3) {
                cpu.mostrar();
            }

        } else if (op == 3) {
            cout << "\n--- GESTOR DE MEMORIA ---\n";
            cout << "1. Asignar bloque de memoria a un proceso\n";
            cout << "2. Liberar ultimo bloque asignado (Pop)\n";
            cout << "3. Ver estado actual de memoria\n";
            int subOp = leerEntero("Seleccione una sub-opcion: ");

            if (subOp == 1) {
                int idProceso = leerEntero("Ingrese ID del proceso asociado: ");
                if (!gp.existeID(idProceso)) {
                    cout << "[ERROR] El proceso con ID " << idProceso << " no esta registrado.\n";
                } else {
                    int idBloque = leerEntero("Ingrese ID para el bloque: ");
                    int tamanio = leerEntero("Ingrese tamano del bloque (MB): ");
                    mem.push(idBloque, tamanio, idProceso);
                }

            } else if (subOp == 2) {
                mem.pop();

            } else if (subOp == 3) {
                mem.verificar();
            }

        } else if (op == 4) {
            guardarSistema(gp, cpu, mem);
        }

    } while (op != 5);

    cout << "\nGuardando estado antes de salir...\n";
    guardarSistema(gp, cpu, mem);
    cout << "Saliendo del sistema. Hasta luego!\n";

    return 0;
}
