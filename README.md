## 1. INSTALAR DEPENDENCIAS

### Herramientas de compilacion y librerias de desarrollo del kernel

```bash
sudo apt update
sudo apt install build-essential gcc gfortran libpfm4-dev git
```

## 2. DESCARGAR CODIGO FUENTE

```bash
git clone https://github.com/icl-utk-edu/papi.git
cd papi/src
```

## 3. MODIFICAR PERMISOS

### Modifica permisos para acceso a contadore de rendimiento

- Esto vuelven a su valor original (4) al reiniciar el sistema.

```bash
sudo sysctl -w kernel.perf_event_paranoid=1
```

## 4. CONFIGURAR RUTA DE DESTINO

```bash
# Crea primero la carpeta donde vivirá PAPI
mkdir -p $HOME/tesis_papi

# Configura apuntando a esa carpeta
./configure --prefix=$HOME/tesis_papi
```

---

Dado que PAPI esta para versiones anteriores de GNU/Linux este tira error en el flasg Werror. Para esto en la carptea papi/src/libpfm4/config.mk comente los flags referentes a Werror.

---

Correr el ./config nuevamente.

---

## 5. COMPILAR E INSTALAR

```bash
make -j$(nproc)
make install
```

## 6. PRUEBAS

```bash
make test
make fulltest
```

---

## 7. CREAR PRIMER ARCHIVO DE MEDICION

### 7.1 RUTAS PARA EL ENTORNO PAPI tesis_papi/env.sh

```bash
export PAPI_DIR=$HOME/tesis_papi
export PATH=$PAPI_DIR/bin:$PATH
export INCLUDE=$PAPI_DIR/include:$INCLUDE
export C_INCLUDE_PATH=$PAPI_DIR/include:$C_INCLUDE_PATH
export LIBRARY_PATH=$PAPI_DIR/lib:$LIBRARY_PATH
export LD_LIBRARY_PATH=$PAPI_DIR/lib:$LD_LIBRARY_PATH

echo "Entorno de PAPI cargado."
```

### 7.2 CREACION DE ARCHIVO EN tesis_papi/pruebas

### 7.3 CORRER PROGRAMA

1. Ejecutar source env.sh
2. Ejecutar cc programa.c -lpapi -o programa
3. Ejecutar con ./programa
4. SALIDA

---

## PROGRAMA 1.

Se corre un progrma que contiene:

```c
long suma = 0;
for (volatile int i = 0; i < 50000000; i++) {
    suma += i;
}
```

UTILIZANDO *PAPI_TOT_INS* permite medir las instrucciones totales.

*SALIDA:*

```bash
Procesando...
Instrucciones ejecutadas: 450001600
```

---

## PROGRAMA 2

El programa contiene un matriz NxN y se mide los fallos en cache si se lee por columna y por fila.

Utilizando *PAPI_L1_DCM* para fallos en cache y *PAPI_TOT_CYC* para ciclos totales.

*SALIDA*

```bash
niciando Test 1 (Row-Major)...
Resultados Row-Major:
 > Fallos L1: 12527255
 > Ciclos:    967294568

Iniciando Test 2 (Column-Major)...
Resultados Column-Major:
 > Fallos L1: 200518698
 > Ciclos:    2616316746

```

---

## PROGRAMA 3

Para medir el consumo de energia en la GPU con una GTX 1060 debo instalar cuda toolkit.

```bash
sudo apt update
sudo apt install nvidia-cuda-toolkit
```

en papi/src ejecutamos: 
```bash
./configure --prefix=$HOME/tesis_papi --with-components="nvml"
```

Se realizaron mediciones de la GPU en cuanto a energia total consumida, potencia y temperatura. Asi como el calculo final de potencia media [Energia(J)/Tiempo(s)]. 

Se considero el uso normal de la GPU con un sleep(10). 

```c
"nvml:::NVIDIA_GeForce_GTX_1060:device_0:total_energy_consumption",
"nvml:::NVIDIA_GeForce_GTX_1060:device_0:power",
"nvml:::NVIDIA_GeForce_GTX_1060:device_0:temperature"
```

*SALIDA Y MEDICIONES: *
```bash
=== Iniciando medición de GPU ===
Midiendo durante 10 segundos...

=== Resultados de la Medición ===
1. Energía Total Consumida : 63463525 mJ (63463.525 Joules)
2. Potencia Instantánea    : 2705 mW (2.705 Watts)
3. Temperatura Final       : 32 °C
---------------------------------
-> Potencia Media Calculada: 15865.881 Watts
```