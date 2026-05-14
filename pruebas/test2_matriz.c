#include <stdio.h>
#include <stdlib.h>
#include <papi.h>

#define N 10000  // Tamaño de la matriz (10000x10000 doubles = ~800MB)

void handle_error(int retval) {
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}

int main() {
    int retval;
    int EventSet = PAPI_NULL;
    long long values[2]; 
    
    // Matriz en 1D para asegurar contigüidad física en memoria
    double *matriz = (double *)malloc(N * N * sizeof(double));
    if (matriz == NULL) { printf("Error de memoria\n"); return 1; }

    // Inicialización (Fuera de la medición)
    for (long i = 0; i < N * N; i++) matriz[i] = 1.0;

    // 1. Inicializar PAPI
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) exit(1);
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) handle_error(1);

    // 2. Añadir eventos: Fallos de Cache L1 y Ciclos Totales
    if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK) handle_error(1);
    if (PAPI_add_event(EventSet, PAPI_TOT_CYC) != PAPI_OK) handle_error(1);

    double suma = 0.0;

    // --- TEST 1: ACCESO POR FILAS (Eficiente) ---
    printf("Iniciando Test 1 (Row-Major)...\n");
    PAPI_start(EventSet);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            suma += matriz[i * N + j];
        }
    }
    PAPI_stop(EventSet, values);
    printf("Resultados Row-Major:\n");
    printf(" > Fallos L1: %lld\n", values[0]);
    printf(" > Ciclos:    %lld\n\n", values[1]);

    PAPI_reset(EventSet);
    suma = 0.0;

    // --- TEST 2: ACCESO POR COLUMNAS (Ineficiente) ---
    printf("Iniciando Test 2 (Column-Major)...\n");
    PAPI_start(EventSet);
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            suma += matriz[i * N + j];
        }
    }
    PAPI_stop(EventSet, values);
    printf("Resultados Column-Major:\n");
    printf(" > Fallos L1: %lld\n", values[0]);
    printf(" > Ciclos:    %lld\n", values[1]);

    // Limpieza
    free(matriz);
    PAPI_shutdown();
    return 0;
}