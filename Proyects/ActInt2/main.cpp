// Fernando Doddoli, Andrés Guerra, Carlos del Rosal - Noviembre 11, 2021
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#define INT_MAX 999999
using namespace std;

// Estructura representando centrales, colonias y nuevas conexiones con coordenadas
struct Location {
  int x, y;
  string type;
};

// Estructura para almacenar vectores de datos compuestos en el algoritmo de Prim
struct PrimOption {
    int origin, destination, cost;

    PrimOption(int _o, int _d, int _c) {
        origin = _o;
        destination = _d;
        cost = _c;
    }
};

// Declaración por adelantado de los métodos principales del programa
int euclideanDist(Location &a, Location &b);
void printMatrix(vector<vector<int>> &matrix);
vector<vector<int>> primMST(vector<vector<int>> &graph);
void printShortestRoute(vector<vector<int>> &matrix, vector<vector<int>> &primsMST);
int printMaxFlux(vector<vector<int>> &matrix, int source, int sink);
vector<Location> readCoords(int qty, string type);
vector<vector<int>> graphFromCoords(vector<Location> &coords);
void printAllMaxFlux(vector<vector<int>> &fluxMatrix);
void printClosestLocations(vector<Location> &conns, vector<Location> &cents, vector<Location> &cols);
bool pathExistsBFS(vector<vector<int>> &residuals, vector<int> &parents, int source, int sink);
string nameANode(int n);

int main (void) {
    // Variables para la cantidad de coordenadas por leer
    int n, m, k;

    // Lectura 1 - Colonias con sus coordenadas
    cin >> n;
    vector<Location> colsCoords = readCoords(n, "col");

    // Lectura 2 - Matriz de flujos
    vector<vector<int>> colsFlux(n, vector<int>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> colsFlux[i][j];

    // Lectura 3 - Centrales con sus coordenadas
    cin >> m;
    vector<Location> centCoords = readCoords(m, "cen");

    // Lectura 4 - Nuevas conexiones con sus coordenadas
    cin >> k;
    vector<Location> connCoords = readCoords(k, "con");

    // Parte 1 - Impresión del grafo generado con distancias euclideanas
    cout << endl << "Matriz de adyacencias:" << endl;
    vector<vector<int>> colsGraph = graphFromCoords(colsCoords);
    printMatrix(colsGraph);

    // Parte 2 - Impresión del arbol de expansión con peso mínimo
    cout << endl << "Lista de arcos:" << endl;
    vector<vector<int>> primsMST = primMST(colsGraph);

    // Parte 3 - Impresión de la ruta mas corta para visitar todas las colonias
    cout << endl << "Ruta por seguir:" << endl;
    printShortestRoute(colsGraph, primsMST);

    // Parte 4 - Impresión de flujos máximos de cada colonia a otra colonia
    cout << endl << "Valores de flujo maximo:" << endl;
    printAllMaxFlux(colsFlux);

    // Parte 5 - Impresión de la colonia y la central más cercana para cada conexión
    cout << endl << "Lista de pares (central, colonia):" << endl;
    printClosestLocations(connCoords, centCoords, colsCoords);

    return 0;
}

// Usando el teórema de pitágoras, se obtiene la distancia real entre dos puntos en un plano. O(1)
int euclideanDist(Location &a, Location &b) {
    return (int)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Leyendo una cantidad qty de pares (x, y) se almacenan como coordenadas de una localización. O(N)
vector<Location> readCoords(int qty, string type) {
    vector<Location> coords (qty);
    for (int i = 0; i < qty; i++) {
        cin >> coords[i].x >> coords[i].y;
        coords[i].type = type;
    }
    return coords;
}

// Contando con coordenadas (x,y) se genera un grafo no dirigido con las distancia entre ellas. O(N^2)
vector<vector<int>> graphFromCoords(vector<Location> &coords) {
    int n = coords.size();
    vector<vector<int>> graph (n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = euclideanDist(coords[i], coords[j]);
            graph[j][i] = graph[i][j];
        }
    }
    return graph;
}

// Función que imprime la matriz de adyacencias con las distancias entre las colonias. O(N^2)
void printMatrix(vector<vector<int>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Algoritmo de Prim que encuentra el arbol de expansión mínima de un grafo. Como su similar, el algoritmo
// de Kruskal, realiza una selección Greedy de los arcos que agrega al MST en construcción. Su complejidad queda
// en O(Elog(N)) o bien, O(N^2log(N)) dado que depende de la cantidad de arcos en el grafo y en cada caso la
// inserción a la lista priorizada es en tiempo O(log(N))
vector<vector<int>> primMST(vector<vector<int>> &graph) {
    // Toma como punto de partida el primer nodo, se asegura de no regresar
    int nodeID = 0, MSTCost = 0, n = graph.size(), nextNode = -1;
    vector<pair<int, int>> MST;
    vector<bool> inMST (n, false);
    PrimOption node (-1, -1, -1);

    // Trabaja con una fila que se ordena según la conexión nueva que menos costaría
    auto orderOptions = [](PrimOption a, PrimOption b) { return a.cost > b.cost; };
    priority_queue<PrimOption, vector<PrimOption>, decltype(orderOptions)> possibleNextVertex(orderOptions);
    possibleNextVertex.push(PrimOption(-1, nodeID, 0));
    
    // Trabaja todos los arcos como posibles arcos del MST
    while (!possibleNextVertex.empty()) {
        // Toma el siguiente arco y lo descarta si ese vértice ya está en el MST
        node = possibleNextVertex.top();
        possibleNextVertex.pop();
        nodeID = node.destination;
        if (inMST[nodeID]) continue;

        // Registra que ese arco y el nodo que se llega son parte del MST
        MSTCost += node.cost;
        MST.push_back(make_pair(node.origin, node.destination));
        inMST[nodeID] = true;

        // Itera por los vecinos del nodo actual, agrengándolos a la lista priorizada
        for (int i = 0; i < n; i++) {
            if (!inMST[i]) {
                possibleNextVertex.push(PrimOption(nodeID, i, graph[nodeID][i]));
            }
        }
    }

    // No solo realiza la impresión del MST, sino que lo construye como matriz de adyacencias y
    // devuelve a main() para que sea utilizado por las siguientes partes del programa
    vector<vector<int>> primsMST(n, vector<int>(n, 0));
    for (int i = 1; i < MST.size(); i++) {
        primsMST[MST[i].first][MST[i].second] = graph[MST[i].first][MST[i].second];
        primsMST[MST[i].second][MST[i].first] = primsMST[MST[i].first][MST[i].second];
        cout << "(" << nameANode(MST[i].first + 1) << "," << nameANode(MST[i].second + 1) << ") ";
    }
    cout << "con una longitud de " << MSTCost << endl;
    return primsMST;
}

// Función que imprime las N^2 combinaciones de flujos máximos entre dos colonias tras llamar al
// algoritmo de Ford-Fulkerson. Por lo tanto, su complejidad queda en O(N^7)
void printAllMaxFlux(vector<vector<int>> &fluxMatrix) {
    int n = fluxMatrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                cout << "Colonia " << i + 1 << " -> " << "colonia " << j + 1 << " flujo = ";
                cout << printMaxFlux(fluxMatrix, i, j) << endl;
            }
        }
    }
}

// Nombra un nodo con una (o más letras, ej. AA, AB) según un número dado. O(log(N))
string nameANode(int n) {
    string name = "";
    while (n > 0) {
        int remainder = n % 26;
        name = remainder ? (char)(remainder - 1 + 'A') + name : 'Z' + name;
        n = n / 26 - !remainder;
    }
    return name;
}

// Recorrido recursivo de un grafo, guardando una caminata "inorder". Dado que por cada nodo
// se checa si ya se visitó sus N vecinos, la complejidad queda en O(N^2)
vector<int> inOrderWalk(vector<vector<int>> &graph, int node) {
    // Se guardan variables estáticas para no sobrecargar de parámetros ni gastar memoria
    static vector<int> route;
    static int n = graph.size();
    static vector<bool> added(n, false);

    // Se agrega el nodo actual a la ruta, no se volverá a llamar dado que ya fue visitado
    route.push_back(node);
    added[node] = true;
    for (int i = 0; i < n; i++) {
        if (graph[node][i] > 0 && !added[i]) {
            // Llamada recursiva y se agrega de regreso el nodo de nuevo (caminata)
            inOrderWalk(graph, i);
            route.push_back(node);
        }
    }

    // Fin de la recursión, se devuelve la caminata construída
    return route;
}


// Imprime la ruta más corta para el problema del viajero, utiliza un acercamiento de viajar sobre
// el árbol de expansión mínima que brinda una buena aproximación a las mejores soluciones. Debido
// a que el recorrido de todos los nodos es en tiempo O(N^2), la función hereda su complejidad de O(N^2)
// que termina la construcción de los caminos y la selección de uno óptimo.
void printShortestRoute(vector<vector<int>> &matrix, vector<vector<int>> &primsMST) {
    // Genera las bases de una ruta con el árbol de expansión mínima, para aprovechar los arcos de poco peso
    vector<int> walk = inOrderWalk(primsMST, 0);

    // Guarda en variables el mejor camino de N propuestas
    vector<int> bestPath;
    int bestCost = INT_MAX;
    for (int i = 0; i < matrix.size(); i++) {
        // Variables de trabajo solo para esta iteración
        int currentCost = 0;
        vector<int> currentPath;
        vector<bool> visited(matrix.size(), false);
    
        // Genera el camino único, brincando los nodos repetidos en el walk
        for (int j = 0, pos = i; j < walk.size(); j++, pos = (pos + 1) % walk.size()) {
            if (!visited[walk[pos]]) {
                currentPath.push_back(walk[pos]);
                visited[walk[pos]] = true;
            }
        }

        // Cierra el ciclo del TSP regresando al punto de salida
        currentPath.push_back(walk[i]);

        // Calcula el precio de los arcos elegidos
        for (int j = 0; j < currentPath.size() - 1; j++) {
            currentCost += matrix[currentPath[j]][currentPath[j + 1]];
        }

        // Actualiza para solo trabajar con el mínimo
        if (currentCost < bestCost) {
            bestCost = currentCost;
            bestPath = currentPath;
        }
    }

    // Impresión de resultados
    cout << "Costo = " << bestCost << ". Path: ";
    cout << nameANode(bestPath[0] + 1);
    for (int i = 1; i < bestPath.size(); i++) {
        cout << " -> " << nameANode(bestPath[i] + 1);
    }
    cout << endl;
}

// Devuelve si una búsqueda de amplitud primero logra alcanzar el nodo sumidero desde el nodo
// fuente. Debido a una implementación subóptima de los arcos, la complejidad queda en O(N^2)
bool pathExistsBFS(vector<vector<int>> &residuals, vector<int> &parents, int source, int sink) {
    // Variables de trabajo de un Breadth First Search, llevando un registro de nodos visitados
    int currentNode;
    int n = residuals.size();
    vector<bool> visited(n, false);
    queue<int> frontier;
    frontier.push(source);
    visited[source] = true;
    parents[source] = -1;

    // Ciclo con complejidad O(N^2) donde N son los nodos existentes
    while (!frontier.empty()) {
        currentNode = frontier.front();
        frontier.pop();
        for (int v = 0; v < n; v++) {
            if (!visited[v] && residuals[currentNode][v] > 0) {
                frontier.push(v);
                parents[v] = currentNode;
                visited[v] = true;
            }
        }
    }
    return visited[sink];
}

// Algoritmo Ford-Fulkerson. Cuenta originalmente con una complejidad dependiente en la cantidad 
// de caminos que componen al flujo máximo F O(EF). No obstante, debido al uso de un grafo en
// matriz y un BFS limitado en optimización, la complejidad queda en O(EN^3) que resulta ser O(N^5)
int printMaxFlux(vector<vector<int>> &matrix, int source, int sink) {
    // Variables de trabajo. El flujo máximo inicia en 0 y va sumando el flujo por camino
    int maxFlow = 0;
    int pathFlow, nextNode, currentNode;
    int n = matrix.size();
    vector<vector<int>> residuals(matrix);
    vector<int> parents(n, 0);

    // Sigue aumentando el flujo mientras haya un camino que lo permita según BFS
    while (pathExistsBFS(residuals, parents, source, sink)) {
        pathFlow = INT_MAX;
        // Al tomar primero los flujos mínimos, se asegura de no depender de ellos en un futuro
        for (currentNode = sink; currentNode != source; currentNode = nextNode) {
            nextNode = parents[currentNode];
            pathFlow = min(pathFlow, residuals[nextNode][currentNode]);
        }
        // Actualiza los valores residuales tras escoger un flujo
        for (currentNode = sink; currentNode != source; currentNode = nextNode) {
            nextNode = parents[currentNode];
            residuals[nextNode][currentNode] -= pathFlow;
            residuals[currentNode][nextNode] += pathFlow;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

// Imprime para cada conexión su central y colonia asignada (más cercanas). En el caso de
// realizar esta operación de manera frecuente, podría se de útilidad construir un diagrama de
// Voronoi con los datos. No obstante, en este caso estos ciclos anidados O(N^2) son suficientes
void printClosestLocations(vector<Location> &conns, vector<Location> &cents, vector<Location> &cols) {
    // Itera por todas las nuevas conexiones checando, también por iteraciones, cual central y colonia
    // existente resulta más cercana. Se imprimen los resultados con las distancias
    int closestCen, closestCol, minCenDistance, minColDistance, dist;
    for (int i = 0; i < conns.size(); i++) {
        // Iteración para las centrales
        minCenDistance = INT_MAX;
        for (int j = 0; j < cents.size(); j++) {
            dist = euclideanDist(conns[i], cents[j]);
            if (dist < minCenDistance) {
                minCenDistance = dist;
                closestCen = j;
            }
        }

        // Iteración para las colonias
        minColDistance = INT_MAX;
        for (int j = 0; j < cols.size(); j++) {
            dist = euclideanDist(conns[i], cols[j]);
            if (dist < minColDistance) {
                minColDistance = dist;
                closestCol = j;
            }
        }

        // Impresión de los resultados anteriores
        cout << "Conexion " << i + 1 << " (central " << closestCen + 1 << " d = " << minCenDistance;
        cout << ", colonia " << closestCol + 1 << " d = " << minColDistance << ")" << endl;
    }
}