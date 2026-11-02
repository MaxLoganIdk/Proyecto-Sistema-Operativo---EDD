#include <iostream>
#include <fstream>
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

// GESTOR DE PROCESOS (Lista Enlazada)

class GestorProcesos 
{
private:
    NodoProceso* cabeza;

public:
    GestorProcesos() { cabeza = NULL; }

    // Insertar proceso al final de la lista
    void insertar(int id, string nombre, int prioridad) 
	{
        // Verificar si ya existe un proceso con ese ID
        NodoProceso* verificar = cabeza;
        while (verificar != NULL) 
		{
            if (verificar->id == id) 
			{
                cout << "Ya existe un proceso con ID " << id <<"\n";
                return;
            }
            verificar = verificar->siguiente;
        }

        NodoProceso* nuevo = new NodoProceso();
        nuevo->id = id;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->siguiente = NULL;

        // Si la lista está vacía, el nuevo nodo es la cabeza
        if (cabeza == NULL) 
		{
            cabeza = nuevo;
        } 
		else 
		{
            // Recorrer hasta el último nodo y enlazar
            NodoProceso* actual = cabeza;
            while (actual->siguiente != NULL) 
			{
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cout << "Proceso '" << nombre << "' registrado con ID " << id << "\n";
    }

    // Eliminar proceso por ID
    void eliminar(int id) 
	{
        if (cabeza == NULL) 
		{
            cout << "No hay procesos registrados." << "\n";
            return;
        }
        // Eliminar la cabeza
        if (cabeza->id == id) 
		{
            NodoProceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            cout << "Proceso con ID " << id << " eliminado." << "\n";
            return;
        }
        // Buscar el nodo anterior al que se quiere eliminar
        NodoProceso* actual = cabeza;
        while (actual->siguiente != NULL) 
		{
            if (actual->siguiente->id == id) 
			{
                NodoProceso* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                cout << "Proceso con ID " << id << " eliminado." <<"\n";
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró proceso con ID " << id << "\n";
    }

    // Buscar proceso por ID
    void buscarPorID(int id) 
	{
        NodoProceso* actual = cabeza;
        while (actual != NULL) 
		{
            if (actual->id == id) 
			{
                cout << "-------------------------------\n";
                cout << "ID       : " << actual->id << "\n";
                cout << "Nombre   : " << actual->nombre << "\n";
                cout << "Prioridad: " << actual->prioridad << "\n";
                cout << "-------------------------------\n";
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró proceso con ID " << id <<"\n";
    }

    // Buscar proceso por nombre
    void buscarPorNombre(string nombre) 
	{
        NodoProceso* actual = cabeza;
        bool encontrado = false;
        while (actual != NULL) 
		{
            if (actual->nombre == nombre) 
			{
                cout << "-------------------------------\n";
                cout << "ID       : " << actual->id << "\n";
                cout << "Nombre   : " << actual->nombre << "\n";
                cout << "Prioridad: " << actual->prioridad <<"\n";
                cout << "-------------------------------\n";
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) {
            cout << "No se encontró proceso con nombre '" << nombre << "'" << "\n";
        }
    }

    // Modificar prioridad de un proceso
    void modificarPrioridad(int id, int nuevaPrioridad) 
	{
        NodoProceso* actual = cabeza;
        while (actual != NULL) 
		{
            if (actual->id == id) 
			{
                actual->prioridad = nuevaPrioridad;
                cout << "Prioridad del proceso " << id << " actualizada a " << nuevaPrioridad <<"\n";
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró proceso con ID " << id <<"\n";
    }

    // Mostrar todos los procesos
    void mostrar() 
	{
        if (cabeza == NULL) 
		{
            cout << "No hay procesos registrados\n";
            return;
        }
        cout << "-------------------------------\n";
        cout << "     LISTA DE PROCESOS         \n";
        cout << "-------------------------------\n";
        NodoProceso* actual = cabeza;
        while (actual != NULL) 
		{
            cout << "ID: " << actual->id << " | Nombre: " << actual->nombre << " | Prioridad: " << actual->prioridad <<"\n";
            actual = actual->siguiente;
        }
        cout << "-------------------------------\n";
    }

    // Guardar lista en archivo
    void guardar(ofstream& archivo) 
	{
        NodoProceso* actual = cabeza;
        while (actual != NULL) 
		{
            archivo << "P " << actual->id << " " << actual->nombre << " " << actual->prioridad << "\n";
            actual = actual->siguiente;
        }
    }

    // Cargar lista desde archivo
    void cargar(int id, string nombre, int prioridad) 
	{
        insertar(id, nombre, prioridad);
    }

    // Destructor: liberar toda la memoria al cerrar
    ~GestorProcesos() 
	{
        NodoProceso* actual = cabeza;
        while (actual != NULL) 
		{
            NodoProceso* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};


// PLANIFICADOR DE CPU (Cola con orden por prioridad)

class PlanificadorCPU 
{
private:
    NodoCola* frente;
    NodoCola* fin;

public:
    PlanificadorCPU() { frente = NULL; fin = NULL; }

    // Encolar proceso respetando el orden de prioridad
    void encolar(int id, string nombre, int prioridad) 
	{
        NodoCola* nuevo = new NodoCola();
        nuevo->id = id;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->siguiente = NULL;

        // Si la cola está vacía o el nuevo tiene mayor prioridad que el frente
        if (frente == NULL || prioridad > frente->prioridad) 
		{
            nuevo->siguiente = frente;
            frente = nuevo;
            if (fin == NULL) fin = nuevo;
        } 
		else 
		{
            // Buscar la posición correcta según prioridad
            NodoCola* actual = frente;
            while (actual->siguiente != NULL && actual->siguiente->prioridad >= prioridad) 
			{
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
            if (nuevo->siguiente == NULL) fin = nuevo;
        }
        cout << "Proceso '" << nombre << "' encolado con prioridad "<< prioridad << "\n";
    }

    // Desencolar y simular ejecución del proceso con mayor prioridad
    void desencolar() 
	{
        if (frente == NULL) 
		{
            cout << "La cola está vacía\n";
            return;
        }
        NodoCola* temp = frente;
        cout << "[EJECUTANDO] Proceso: " << temp->nombre << " | ID: " << temp->id << " | Prioridad: " << temp->prioridad <<"\n";
        frente = frente->siguiente;
        if (frente == NULL) fin = NULL;
        delete temp;
    }

    // Mostrar estado actual de la cola
    void mostrar() 
	{
        if (frente == NULL) 
		{
            cout << "La cola de está vacía\n";
            return;
        }
        cout << "------------------------------\n";
        cout << "     COLA DE EJECUCIÓN        \n";
        cout << "------------------------------\n";
        NodoCola* actual = frente;
        int posicion = 1;
        while (actual != NULL) 
		{
            cout << posicion << ". ID: " << actual->id << " | Nombre: " << actual->nombre << " | Prioridad: " << actual->prioridad <<"\n";
            actual = actual->siguiente;
            posicion++;
        }
        cout << "------------------------------\n";
    }

    // Guardar cola en archivo
    void guardar(ofstream& archivo) 
	{
        NodoCola* actual = frente;
        while (actual != NULL) 
		{
            archivo << "C " << actual->id << " "<< actual->nombre << " " << actual->prioridad << "\n";
            actual = actual->siguiente;
        }
    }

    // Destructor
    ~PlanificadorCPU() 
	{
        NodoCola* actual = frente;
        while (actual != NULL) 
		{
            NodoCola* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};


// GESTOR DE MEMORIA (Pila)

class GestorMemoria 
{
private:
    NodoPila* tope;

public:
    GestorMemoria() { tope = NULL; }

    // Push - asignar bloque de memoria a un proceso
    void push(int idBloque, int tamanio, int idProceso) 
	{
        NodoPila* nuevo = new NodoPila();
        nuevo->idBloque = idBloque;
        nuevo->tamanio = tamanio;
        nuevo->idProceso = idProceso;
        nuevo->siguiente = tope;
        tope = nuevo;
        cout << "[OK] Bloque " << idBloque << " (" << tamanio << " MB) asignado al proceso " << idProceso <<"\n";
    }

    // Pop -  liberar el bloque de memoria del tope
    void pop() 
	{
        if (tope == NULL) 
		{
            cout << "No hay bloques de memoria asignados\n";
            return;
        }
        NodoPila* temp = tope;
        cout << "Liberando bloque " << temp->idBloque << " (" << temp->tamanio << " MB) del proceso " << temp->idProceso <<"\n";
        tope = tope->siguiente;
        delete temp;
    }

    // Verificar estado actual de la memoria
    void verificar() 
	{
        if (tope == NULL) 
		{
            cout << "Memoria completamente libre\n";
            return;
        }
        cout << "-------------------------------\n";
        cout << "      ESTADO DE MEMORIA        \n";
        cout << "-------------------------------\n";
        NodoPila* actual = tope;
        while (actual != NULL) 
		{
            cout << "Bloque: " << actual->idBloque << " | Tamaño: " << actual->tamanio << " MB" << " | Proceso: " << actual->idProceso <<"\n";
            actual = actual->siguiente;
        }
        cout << "-------------------------------\n";
    }

    // Guardar pila en archivo
    void guardar(ofstream& archivo) 
	{
        NodoPila* actual = tope;
        while (actual != NULL) {
            archivo << "M " << actual->idBloque << " " << actual->tamanio << " " << actual->idProceso << "\n";
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


// PERSISTENCIA DE DATOS

// Guardar el estadoen un archivo
void guardarSistema(GestorProcesos& gp, PlanificadorCPU& cpu, GestorMemoria& mem) 
{
    ofstream archivo("sistema.txt");
    if (!archivo.is_open()) 
	{
        cout << "No se pudo abrir el archivo para guardar\n";
        return;
    }
    gp.guardar(archivo);
    cpu.guardar(archivo);
    mem.guardar(archivo);
    archivo.close();
    cout << "Estado del sistema guardado con el nombre 'sistema.txt'\n";
}

// Cargar el estado del sistema desde el archivo
void cargarSistema(GestorProcesos& gp, PlanificadorCPU& cpu, GestorMemoria& mem) 
{
    ifstream archivo("sistema.txt");
    if (!archivo.is_open()) 
	{
        cout << "No se encontró archivo guardado\n";
        return;
    }
    char tipo;
    while (archivo >> tipo) 
	{
        if (tipo == 'P') {
            int id, prioridad;
            string nombre;
            archivo >> id >> nombre >> prioridad;
            gp.cargar(id, nombre, prioridad);
        } else if (tipo == 'C') {
            int id, prioridad;
            string nombre;
            archivo >> id >> nombre >> prioridad;
            cpu.encolar(id, nombre, prioridad);
        } else if (tipo == 'M') {
            int idBloque, tamanio, idProceso;
            archivo >> idBloque >> tamanio >> idProceso;
            mem.push(idBloque, tamanio, idProceso);
        }
    }
    archivo.close();
    cout << "Estado del sistema cargado correctamente\n";
}

// MENÚ PRINCIPAL

int main() 
{
	setlocale(LC_ALL,"spanish");
    GestorProcesos gp;
    PlanificadorCPU cpu;
    GestorMemoria mem;

    cargarSistema(gp, cpu, mem);    // Cargar datos guardados al iniciar

    int op;

    do 
	{
    	
        cout << "\n----------------------------------------\n";
        cout << "   SISTEMA DE GESTION DE PROCESOS         \n";
        cout << "----------------------------------------- \n";
        cout << "1. Gestor de procesos\n";
        cout << "2. Planificador de CPU\n";
        cout << "3. Gestor de memoria\n";
        cout << "4. Guardar estado del sistema\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: "; cin >> op;

        //GESTOR DE PROCESOS
        if (op == 1) 
		{      	
            int subOp;
            cout << "\n   GESTOR DE PROCESOS \n";
            cout << "1. Registrar proceso\n";
            cout << "2. Eliminar proceso\n";
            cout << "3. Buscar por ID\n";
            cout << "4. Buscar por nombre\n";
            cout << "5. Modificar prioridad\n";
            cout << "6. Mostrar todos\n";
            cout << "Seleccione: "; cin >> subOp;

            if (subOp == 1) {
                int id, prioridad;
                string nombre;
                cout << "ID: "; cin >> id;
                cout << "Nombre: "; cin >> nombre;
                cout << "Prioridad: "; cin >> prioridad;
                gp.insertar(id, nombre, prioridad);

            } else if (subOp == 2) {
                int id;
                cout << "ID a eliminar: "; cin >> id;
                gp.eliminar(id);

            } else if (subOp == 3) {
                int id;
                cout << "ID a buscar: "; cin >> id;
                gp.buscarPorID(id);

            } else if (subOp == 4) {
                string nombre;
                cout << "Nombre a buscar: "; cin >> nombre;
                gp.buscarPorNombre(nombre);

            } else if (subOp == 5) {
                int id, nuevaPrioridad;
                cout << "ID del proceso: "; cin >> id;
                cout << "Nueva prioridad: "; cin >> nuevaPrioridad;
                gp.modificarPrioridad(id, nuevaPrioridad);

            } else if (subOp == 6) {
                gp.mostrar();
            }

        // PLANIFICADOR CPU
        } else if (op == 2) {
            int subOp;
            cout << "\n  PLANIFICADOR DE CPU \n";
            cout << "1. Encolar proceso\n";
            cout << "2. Ejecutar siguiente proceso\n";
            cout << "3. Ver cola actual\n";
            cout << "Seleccione una opción: "; cin >> subOp;

            if (subOp == 1) {
                int id, prioridad;
                string nombre;
                cout << "ID: "; cin >> id;
                cout << "Nombre: "; cin >> nombre;
                cout << "Prioridad: "; cin >> prioridad;
                cpu.encolar(id, nombre, prioridad);

            } else if (subOp == 2) {
                cpu.desencolar();

            } else if (subOp == 3) {
                cpu.mostrar();
            }

        //GESTOR DE MEMORIA
        } else if (op == 3) {
            int subOp;
            cout << "\n  GESTOR DE MEMORIA \n";
            cout << "1. Asignar bloque de memoria\n";
            cout << "2. Liberar ultimo bloque\n";
            cout << "3. Ver estado de memoria\n";
            cout << "Seleccione una opción: ";cin >> subOp;

            if (subOp == 1) {
                int idBloque, tamanio, idProceso;
                cout << "ID del bloque: "; cin >> idBloque;
                cout << "Tamanio (MB): "; cin >> tamanio;
                cout << "ID del proceso: "; cin >> idProceso;
                mem.push(idBloque, tamanio, idProceso);

            } else if (subOp == 2) {
                mem.pop();

            } else if (subOp == 3) {
                mem.verificar();
            }

        // GUARDAR 
        } else if (op == 4) {
            guardarSistema(gp, cpu, mem);
        }

    } while (op != 5);

    cout << "\nHasta luego\n";
    return 0;
}
