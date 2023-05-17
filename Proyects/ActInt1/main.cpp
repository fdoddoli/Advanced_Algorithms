#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Modifique en caso de que los casos de prueba se encuentren en subdirectorios
// Ejemplo: string TEST_PATH = "TC2038.ActInteg1.TestCases/Test1/"
const string TEST_PATH = "";

// Declaraciones de las 3 funciones principales y auxiliares
void findZFunction(string patternFile, string textFile);
void findLPSManacher(string transmission1);
void dynamicProgrammingLCS(string transmission1, string transmission2);
string extractFileString(string path);

int main(void) {
    // Parte 0: Obtención de las cadenas de texto desde archivos
    string mcode1 = extractFileString(TEST_PATH + "mcode1.txt");
    string mcode2 = extractFileString(TEST_PATH + "mcode2.txt");
    string mcode3 = extractFileString(TEST_PATH + "mcode3.txt");
    string transmission1 = extractFileString(TEST_PATH + "transmission1.txt");
    string transmission2 = extractFileString(TEST_PATH + "transmission2.txt");

    // Parte 1: Búsqueda de códigos maliciosos en los archivos de transmisión
    cout << "----- PARTE 1 -----" << endl;
    findZFunction(mcode1, transmission1);
    findZFunction(mcode2, transmission1);
    findZFunction(mcode3, transmission1);
    findZFunction(mcode1, transmission2);
    findZFunction(mcode2, transmission2);
    findZFunction(mcode3, transmission2);

    // Parte 2: Búsqueda del palíndromo más largo
    cout << "----- PARTE 2 -----" << endl;
    findLPSManacher(transmission1);
    findLPSManacher(transmission2);

    // Parte 3: Búsqueda de la subcadena común más larga
    cout << "----- PARTE 3 -----" << endl;
    dynamicProgrammingLCS(transmission1, transmission2);

    // Impresión de nota importante y fin del programa
    cout << endl << "NOTA: Todos los valores impresos son posiciones (iniciando en 1), no indices" << endl;
    return 0;
}

// Encuentra un patrón como subcadena de una cadena de transmission. Utiliza el algoritmo Z.
// Dado que se utiliza la optimización del último bloque generado, queda la complejidad en O(N)
void findZFunction(string pattern, string transmission) {
    // Creación de la cadena concatenada y el arreglo de conteo de prefijos
    string zString = pattern + "#" + transmission;
    int n = zString.size(), m = pattern.size();
    vector<int> zArray = vector<int>(n, 0);

    // Iteración por la cadena para general el arreglo de conteo de prefijos
    for (int i = 1, left = 0, right = 0; i <= n - m; i++) {
        // Adelanta iteraciones de j si tal espacio ya había sido revisado por el ciclo while siguiente
        if (right >= i) {
            zArray[i] = min(zArray[i - left], right - i + 1);
        }

        // La variable j permite que i guarde el caracter iterado mientras j se va con los siguientes
        int j = i + zArray[i];
        while (zString[j] == zString[j - i]) {
            j++;
        }
        zArray[i] = j - i;

        // Si se cumple que j avanzó más allá del último bloque hecho por el ciclo, se actualiza el bloque
        if (j - 1 > right) {
            left = i;
            right = j - 1;
        }

        // Dado que el patrón adjunto como prefijo se encontró, se imprime true
        if (zArray[i] == m) {
            // Se imprime la posición contando desde 1
            cout << "true" << " " << i - m << endl;
            return;
        }
    }

    // En el caso de no encontrar el patrón, se imprime false
    cout << "false" << endl;
    return;
}

// Encuentra el palíndromo más grande dentro de una cadena y devuelve sus posiciones de inicio y
// fin. Dado que se aplica el algoritmo de Manacher, que aprovecha las cualidades de los palíndromos
// y su simetría para no repetir iteraciones previamente realizadas, queda la complejidad en O(N)
void findLPSManacher(string transmission) {
    // Variables para guardar la longitud de palíndromo para cada caracter y el más largo al momento
    vector<int> palindromeLengths = vector<int>(transmission.size() * 2 + 1);
    int LPSStart, LPSEnd, LPSLength = 1;
    int currentPalLength = 1;

    // Agrega caracteres de relleno "#", formando oddString que siempre es impar
    vector<char> oddString(transmission.size() * 2 + 1);
    for (int i = 0; i < oddString.size(); i++) {
        i % 2 ? oddString[i] = '#' : oddString[i] = transmission[i / 2];
    }

    // Itera caracter por caracter, tomandolo como posible centro del palíndromo
    int n = oddString.size(), i = 0;
    while (i < n) {
        // Se trabaja el centro actual checando que sus lados sean iguales y no llegar a los límites del arreglo
        while (i - (currentPalLength / 2 + 1) >= 0 && i + (currentPalLength / 2 + 1) < n && 
            tolower(oddString[i - (currentPalLength / 2 + 1)]) == tolower(oddString[i + (currentPalLength / 2 + 1)])) {
            currentPalLength += 2;
        }

        // Almacena el arreglo de longitudes de palíndromos y guarda el mayor si es necesario
        palindromeLengths[i] = currentPalLength;
        if (currentPalLength > LPSLength) {
            LPSLength = currentPalLength;
            LPSStart = i - currentPalLength / 2;
            LPSEnd = i + currentPalLength / 2;
        }

        /*
        Dado que se busca recorrer el centro analizado para optimizar y no repetir iteraciones,
        pero se debe de conservar el conocimiento sobre el palíndromo mayor que engloba los caracteres
        tratados, se guardan los valores en variables antes de reutilizarlas.        
        */
        int previousCenter = i;
        int previousPalindrome = currentPalLength;
        currentPalLength = 1;
        i++;

        // Itera llegando hasta el límite del palíndromo que engloba los caracteres por optimizar
        while (i <= previousCenter + previousPalindrome / 2) {
            // Variables limite del espacio que podemos optimizar
            int mirrorCenter = previousCenter - (i - previousCenter);
            int expandedRadiusLimit = previousCenter + previousPalindrome / 2 - i;

            /* 
            Caso de Manacher 1
            Caso en el que el subpalíndromo espejo está contenido completamente en el palíndromo mayor,
            además de que su tamaño no empuja al subpalíndromo de la derecha a su límite de crecimiento.
            Es decir, el palíndromo de la derecha tiene espacio para ser más grande dentro del palíndromo
            mayor, pero el palíndromo de la izquierda nos informa que no es necesario tanto.
            */
            if (palindromeLengths[mirrorCenter] / 2 < expandedRadiusLimit) {
                palindromeLengths[i] = palindromeLengths[mirrorCenter];
            }
            /*
            Caso de Manacher 2
            Caso en donde el subpalíndromo a la izquiera sale del palíndromo mayor y no puede ser
            igualado por el de la derecha, porque de ser así simétrico, el palíndromo mayor sería
            realmente más grande y englobaría a ambos. Es entonces que el subpalíndromo de la derecha
            se limita al máximo que podía crecer según expandedRadiusLimit
            */
            else if (palindromeLengths[mirrorCenter] / 2 > expandedRadiusLimit) {
                palindromeLengths[i] = expandedRadiusLimit;
            }
            /*
            Caso de Manacher 3
            Caso en el que los palíndromos de ambos lados tienen las mismas condiciones, pero el de la
            derecha todavía tiene espacio para seguir creciendo, por lo que ese espacio libre se recorre
            con el ciclo while que compara caracteres. Para mantener la optimización de no repetir
            las iteraciones que se conocen, se da un adelanto a currentPalLength y ya no empieza en 1
            */
            else {
                currentPalLength = expandedRadiusLimit * 2 + 1;
                break;
            }
            i++;
        }
    }

    // Impresión de resultados de la parte 2, quitando las posiciones ocupadas por # y 
    // agregando uno para compensar la indexación desde 0.
    cout << "Palindromo: " << LPSStart / 2 + 2 << " - " << LPSEnd / 2 + 1 << endl;
}

/*
Se utiliza una tabla de programación dinámica para almacenar el sufijo común hasta dos caracteres
t1[i] y t2[j] de manera que se encuentre el substring común más largo. Dado que solo se utiliza un
renglón de tabla para los datos actuales y uno para los datos anteriores, se puede optimizar la
memoria de [N][M] a [2][M]. La complejidad computacional queda en O(N * M) derivada de los dos ciclos
esenciales de la función, todo lo demás son aritmética y accesos en memoria en tiempo constante.
*/
void dynamicProgrammingLCS(string transmission1, string transmission2) {
    // Se crea una tabla DP que guarde la longitud del sufijo común hasta un par de caracteres
    int m = transmission1.size();
    int n = transmission2.size();
    vector<vector<int>> commonSuffix = vector<vector<int>>(2, vector<int>(m + 1, 0));
    int maxSubsLength = 0, subsIndexInM = -1, subsIndexInN = -1;

    // Ciclos que recorren ambas cadenas, creando la parte de complejidad NxM
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            // Se verifica si los caracteres comparados son iguales
            if (transmission1[j - 1] == transmission2[i - 1]) {
                // Dado que son iguales, la longitud del sufijo común es aquella que tenían los
                // dos caracteres anteriores más uno
                commonSuffix[i % 2][j] = commonSuffix[(i - 1) % 2][j - 1] + 1;

                // Se guardan la longitud del substring más grande y su ubicación en ambas transmisiones
                if (maxSubsLength < commonSuffix[i % 2][j]) {
                    maxSubsLength = commonSuffix[i % 2][j];
                    subsIndexInN = i - maxSubsLength;
                    subsIndexInM = j - maxSubsLength;
                }
            }
            else {
                // Dado que no son iguales, la longitud del sufijo que tienen común es 0
                commonSuffix[i % 2][j] = 0;
            }
        }
    }

    // Impresión de resultados para la parte 3
    cout << "Substring: " << transmission1.substr(subsIndexInM, maxSubsLength) << endl;
    cout << "Substring en T1: " << subsIndexInM + 1 << " - " << subsIndexInM + maxSubsLength << endl;
    cout << "Substring en T2: " << subsIndexInN + 1 << " - " << subsIndexInN + maxSubsLength << endl;
}

// Función auxiliar para extraer una cadena de un archivo de texto. O(N)
// Se implementa dado que es código que se repetiría 5 veces en el programa
string extractFileString(string path)
{
    // Declaración de archivos y cadenas que guardan la información por analizar
    ifstream txtFile(path);
    string readString;

    // Verificación de una apertura correcta
    if (!txtFile.is_open())
    {
        cout << "Error en la apertura de archivos" << endl;
        return "";
    }

    // Extracción de las cadenas de texto desde los archivos
    getline(txtFile, readString);
    txtFile.close();
    return readString;
}
