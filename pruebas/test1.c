#include <stdio.h>
#include <stdlib.h>
#include <papi.h>

int main() {
    long long values[1];
    int EventSet = PAPI_NULL;

    // Inicializar PAPI
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        printf("Error inicializando PAPI\n");
        exit(1);
    }

    // Crear el conjunto de eventos
    PAPI_create_eventset(&EventSet);
    
    // Añadir el evento que queremos medir (Instrucciones Totales)
    PAPI_add_event(EventSet, PAPI_TOT_INS);

    // Iniciar medición
    PAPI_start(EventSet);

    // ---- TU "BENCHMARK" DE PRUEBA ----
    printf("Procesando...\n");
    long suma = 0;
    for (volatile int i = 0; i < 50000000; i++) {
        suma += i;
    }
    // ----------------------------------

    // Detener y leer el resultado
    PAPI_stop(EventSet, values);

    // Mostrar en pantalla
    printf("Instrucciones ejecutadas: %lld\n", values[0]);

    // Limpieza
    PAPI_cleanup_eventset(EventSet);
    PAPI_destroy_eventset(&EventSet);
    PAPI_shutdown();

    return 0;
}

