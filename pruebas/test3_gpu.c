#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para la función sleep()
#include <papi.h>

// Función auxiliar para manejar errores de PAPI claramente
void handle_error(int retval, const char *lugar) {
    fprintf(stderr, "Error PAPI en %s: %s\n", lugar, PAPI_strerror(retval));
    exit(1);
}

int main() {
    int retval;
    int EventSet = PAPI_NULL;
    
    // Vamos a medir 3 eventos
    const int num_events = 3;
    long long values[num_events];

    // Los nombres de los sensores de tu placa
    char *event_names[] = {
        "nvml:::NVIDIA_GeForce_GTX_1060:device_0:total_energy_consumption",
        "nvml:::NVIDIA_GeForce_GTX_1060:device_0:power",
        "nvml:::NVIDIA_GeForce_GTX_1060:device_0:temperature"
    };

    // 1. Inicializar la librería PAPI
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr, "Error de versión PAPI.\n");
        exit(1);
    }

    // 2. Crear el conjunto de eventos
    retval = PAPI_create_eventset(&EventSet);
    if (retval != PAPI_OK) handle_error(retval, "PAPI_create_eventset");

    // 3. Añadir los sensores al EventSet
    for (int i = 0; i < num_events; i++) {
        retval = PAPI_add_named_event(EventSet, event_names[i]);
        if (retval != PAPI_OK) {
            fprintf(stderr, "No se pudo añadir el evento: %s\n", event_names[i]);
            handle_error(retval, "PAPI_add_named_event");
        }
    }

    printf("=== Iniciando medición de GPU ===\n");

    // 4. Comenzar a registrar
    retval = PAPI_start(EventSet);
    if (retval != PAPI_OK) handle_error(retval, "PAPI_start");

    // --- CARGA DE TRABAJO ---
    // Aquí iría el algoritmo que estresa la GPU.
    // Para esta prueba, simplemente pausamos el programa 10 segundos
    // para darle tiempo al sensor de acumular energía.
    printf("Midiendo durante 10 segundos...\n");
    sleep(10);
    // ------------------------

    // 5. Detener la medición y leer los resultados
    retval = PAPI_stop(EventSet, values);
    if (retval != PAPI_OK) handle_error(retval, "PAPI_stop");

    // 6. Procesar y mostrar los resultados para la tesis
    printf("\n=== Resultados de la Medición ===\n");
    
    // total_energy_consumption se devuelve en miliJoules (mJ)
    double energia_joules = (double)values[0] / 1000.0;
    printf("1. Energía Total Consumida : %lld mJ (%.3f Joules)\n", values[0], energia_joules);
    
    // power se devuelve en miliWatts (mW)
    double potencia_watts = (double)values[1] / 1000.0;
    printf("2. Potencia Instantánea    : %lld mW (%.3f Watts)\n", values[1], potencia_watts);
    
    // temperature se devuelve en grados Celsius
    printf("3. Temperatura Final       : %lld °C\n", values[2]);

    // Cálculo extra para la tesis: Potencia Media
    // Potencia (W) = Energía (J) / Tiempo (s)
    printf("---------------------------------\n");
    printf("-> Potencia Media Calculada: %.3f Watts\n", energia_joules / 10.0);
    printf("=================================\n");

    // 7. Limpieza
    PAPI_cleanup_eventset(EventSet);
    PAPI_destroy_eventset(&EventSet);
    PAPI_shutdown();

    return 0;
}