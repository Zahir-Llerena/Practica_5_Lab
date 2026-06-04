# Simulación de Sistemas Físicos: Colisiones Múltiples en Tiempo Discreto
## Práctica 5 - Informática II (Facultad de Ingeniería)
### Universidad de Antioquia (2026)

Este componente del repositorio contiene el desarrollo de la **Actividad 2.1** de la Práctica 5[cite: 1, 68]. El objetivo principal es el modelado cinemático y la simulación numérica en tiempo discreto de múltiples partículas interactuando dentro de un espacio cerrado bidimensional con obstáculos fijos[cite: 10, 11, 70]. El proyecto está diseñado en dos capas: un motor de cálculo físico desarrollado en C++ base y un script analítico de visualización en Python 3.12[cite: 12, 15, 61].

---

## 1. Características del Sistema Físico y Requerimientos

La simulación modela de manera computacional el comportamiento de $n$ partículas circulares, cada una dotada de una posición inicial, velocidad vectorial y masa cuantitativa, moviéndose en una caja rectangular[cite: 74, 75]. El entorno contiene **cuatro obstáculos cuadrados estáticos**[cite: 76].

El software gestiona de forma estricta tres leyes de interacción fundamentales:

1. **Colisión Partícula - Pared (Perfectamente Elástica):** Al impactar los límites de la caja, la componente de la velocidad perpendicular a la superficie se invierte ($e = 1.0$), conservando la energía cinética total del cuerpo[cite: 78, 79].
2. **Colisión Partícula - Obstáculo (Inelástica):** Al impactar contra las caras de los cuadrados estáticos, se determina el lado del impacto y se aplica un coeficiente de restitución ($e = 0.75$) exclusivamente a la componente perpendicular de la velocidad ($\vec{v}_{\perp}$), provocando una pérdida de energía simulada[cite: 80, 81, 82].
3. **Colisión Partícula - Partícula (Completamente Inelástica / Fusión):** Cuando dos cuerpos entran en contacto geométrico, se fusionan instantáneamente para dar origen a una nueva entidad macroscópica con masa $M = m_1 + m_2$[cite: 85, 86]. El nuevo estado cinemático se rige bajo la ley de **conservación del momento lineal global**[cite: 87]:
   $$\vec{v'} = \frac{m_1\vec{v_1} + m_2\vec{v_2}}{m_1 + m_2}$$
   La partícula absorbida se desactiva lógicamente para garantizar la consistencia de la masa invariante del sistema.

---

## 2. Arquitectura del Software (C++ Base)

La implementación en C++ se diseñó bajo el paradigma de **Programación Orientada a Objetos (POO)** con una sintaxis limpia y pedagógica, ideal para entornos de compilación estrictos como **Xcode** en macOS[cite: 10, 18].

### Componentes Principales:
* **`Vector2D` (Clase Matemática):** Encapsula los atributos coordenados privados `x` e `y`. Proporciona la interfaz algebraica para operaciones del espacio euclídeo $\mathbb{R}^2$ (`sumar`, `restar`, `multiplicarPorEscalar`, `productoPunto` y `calcularMagnitud`) sin recurrir a sobrecargas complejas de operadores.
* **`Particle` (Clase Entidad):** Almacena el estado dinámico (`id`, `pos`, `vel`, `mass`, `radius`) y una bandera booleana de actividad (`active`) para controlar la fusión[cite: 100].
* **`Obstacle` (Clase Entidad):** Define las fronteras geométricas de los cuatro cuadrados a partir de sus centros y longitudes de lado[cite: 76].
* **`CollisionsSimulation` (Clase Motor):** Centraliza el bucle temporal discreto ($\Delta_t = 0.01\text{ s}$)[cite: 25, 101]. Ejecuta la actualización cinemática, la resolución de colisiones secuenciales y el volcado estructurado de datos[cite: 101, 102, 103].

### Ruta del Archivo de Salida
El constructor del motor físico genera un archivo de texto plano de manera directa en la ruta absoluta local[cite: 103]:
`/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio_Git/Guia_5L/Code_L5/Code_L5/simulation_output.txt`

El formato de salida incluye:
* Un encabezado de tipo DataFrame (`Tiempo,ID,PosX,PosY,Masa,Activa`) para las trazas cinemáticas continuas[cite: 103, 109].
* Líneas de inyección de eventos explícitos (`[COLISIÓN FUSIÓN]`) que registran la masa de los cuerpos incidentes antes de ser consolidados[cite: 109].

---

## 3. Script de Visualización Científica (Python 3.12)

Para procesar y validar cuantitativamente los resultados obtenidos, se utiliza un script en Python optimizado para el IDE **Spyder**[cite: 44, 61].

### Características Técnicas del Script:
* **Lectura Eficiente:** Utiliza `pandas.read_csv()` configurado con el parámetro `comment='['` para filtrar e ignorar las líneas de texto informativas en milisegundos, importando la matriz numérica directamente.
* **Derivación Numérica:** Calcula las componentes instantáneas de las velocidades mediante diferencias finitas vectorizadas (`.diff()`) sobre las series de tiempo.
* **Diseño Dúo Horizontal:** Construye un lienzo gráfico simétrico de dos paneles interactivos con `matplotlib`[cite: 44, 61]:
  1. **Panel Izquierdo (Espacio de Fase $\mathbb{R}^2$):** Muestra las trayectorias continuas de las partículas, la ubicación geométrica de los 4 obstáculos fijos y burbujas terminales cuyo volumen es directamente proporcional a la masa final acumulada[cite: 44, 61, 76].
  2. **Panel Derecho (Conservación Cinética):** Grafica las sumas vectoriales del momento lineal ($\sum P_x$, $\sum P_y$ y $|\vec{P}_{\text{neto}}|$) a lo largo del eje temporal, demostrando que permanecen invariantes y validando la precisión física del modelo computacional[cite: 87].

---

## 4. Estructura del Directorio del Proyecto

```text
Code_L5/
├── Code_L5/
│   ├── main.cpp                 # Código fuente del simulador físico en C++
│   ├── main.py                  # Script de análisis y graficación en Python
│   └── simulation_output.txt    # Base de datos cinemáticos generada (CSV)
└── Practica_5.pdf               # Guía académica de la actividad de laboratorio
cd "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_4_Lab"
cd "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_5_Lab"
cat << 'EOF' > README.md
# Simulación de Sistemas Físicos: Colisiones Múltiples en Tiempo Discreto
## Práctica 5 - Informática II (Facultad de Ingeniería)
### Universidad de Antioquia (2026)

Este componente del repositorio contiene el desarrollo de la **Actividad 2.1** de la Práctica 5[cite: 1, 68]. El objetivo principal es el modelado cinemático y la simulación numérica en tiempo discreto de múltiples partículas interactuando dentro de un espacio cerrado bidimensional con obstáculos fijos[cite: 10, 11, 70]. El proyecto está diseñado en dos capas: un motor de cálculo físico desarrollado en C++ base y un script analítico de visualización en Python 3.12[cite: 12, 15, 61].

---

## 1. Características del Sistema Físico y Requerimientos

La simulación modela de manera computacional el comportamiento de $n$ partículas circulares, cada una dotada de una posición inicial, velocidad vectorial y masa cuantitativa, moviéndose en una caja rectangular[cite: 74, 75]. El entorno contiene **cuatro obstáculos cuadrados estáticos**[cite: 76].

El software gestiona de forma estricta tres leyes de interacción fundamentales:

1. **Colisión Partícula - Pared (Perfectamente Elástica):** Al impactar los límites de la caja, la componente de la velocidad perpendicular a la superficie se invierte ($e = 1.0$), conservando la energía cinética total del cuerpo[cite: 78, 79].
2. **Colisión Partícula - Obstáculo (Inelástica):** Al impactar contra las caras de los cuadrados estáticos, se determina el lado del impacto y se aplica un coeficiente de restitución ($e = 0.75$) exclusivamente a la componente perpendicular de la velocidad ($\vec{v}_{\perp}$), provocando una pérdida de energía simulada[cite: 80, 81, 82].
3. **Colisión Partícula - Partícula (Completamente Inelástica / Fusión):** Cuando dos cuerpos entran en contacto geométrico, se fusionan instantáneamente para dar origen a una nueva entidad macroscópica con masa $M = m_1 + m_2$[cite: 85, 86]. El nuevo estado cinemático se rige bajo la ley de **conservación del momento lineal global**[cite: 87]:
   $$\vec{v'} = \frac{m_1\vec{v_1} + m_2\vec{v_2}}{m_1 + m_2}$$
   La partícula absorbida se desactiva lógicamente para garantizar la consistencia de la masa invariante del sistema.

---

## 2. Arquitectura del Software (C++ Base)

La implementación en C++ se diseñó bajo el paradigma de **Programación Orientada a Objetos (POO)** con una sintaxis limpia y pedagógica, ideal para entornos de compilación estrictos como **Xcode** en macOS[cite: 10, 18].

### Componentes Principales:
* **`Vector2D` (Clase Matemática):** Encapsula los atributos coordenados privados `x` e `y`. Proporciona la interfaz algebraica para operaciones del espacio euclídeo $\mathbb{R}^2$ (`sumar`, `restar`, `multiplicarPorEscalar`, `productoPunto` y `calcularMagnitud`) sin recurrir a sobrecargas complejas de operadores.
* **`Particle` (Clase Entidad):** Almacena el estado dinámico (`id`, `pos`, `vel`, `mass`, `radius`) y una bandera booleana de actividad (`active`) para controlar la fusión[cite: 100].
* **`Obstacle` (Clase Entidad):** Define las fronteras geométricas de los cuatro cuadrados a partir de sus centros y longitudes de lado[cite: 76].
* **`CollisionsSimulation` (Clase Motor):** Centraliza el bucle temporal discreto ($\Delta_t = 0.01\text{ s}$)[cite: 25, 101]. Ejecuta la actualización cinemática, la resolución de colisiones secuenciales y el volcado estructurado de datos[cite: 101, 102, 103].

### Ruta del Archivo de Salida
El constructor del motor físico genera un archivo de texto plano de manera directa en la ruta absoluta local[cite: 103]:
`/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio_Git/Guia_5L/Code_L5/Code_L5/simulation_output.txt`

El formato de salida incluye:
* Un encabezado de tipo DataFrame (`Tiempo,ID,PosX,PosY,Masa,Activa`) para las trazas cinemáticas continuas[cite: 103, 109].
* Líneas de inyección de eventos explícitos (`[COLISIÓN FUSIÓN]`) que registran la masa de los cuerpos incidentes antes de ser consolidados[cite: 109].

---

## 3. Script de Visualización Científica (Python 3.12)

Para procesar y validar cuantitativamente los resultados obtenidos, se utiliza un script en Python optimizado para el IDE **Spyder**[cite: 44, 61].

### Características Técnicas del Script:
* **Lectura Eficiente:** Utiliza `pandas.read_csv()` configurado con el parámetro `comment='['` para filtrar e ignorar las líneas de texto informativas en milisegundos, importando la matriz numérica directamente.
* **Derivación Numérica:** Calcula las componentes instantáneas de las velocidades mediante diferencias finitas vectorizadas (`.diff()`) sobre las series de tiempo.
* **Diseño Dúo Horizontal:** Construye un lienzo gráfico simétrico de dos paneles interactivos con `matplotlib`[cite: 44, 61]:
  1. **Panel Izquierdo (Espacio de Fase $\mathbb{R}^2$):** Muestra las trayectorias continuas de las partículas, la ubicación geométrica de los 4 obstáculos fijos y burbujas terminales cuyo volumen es directamente proporcional a la masa final acumulada[cite: 44, 61, 76].
  2. **Panel Derecho (Conservación Cinética):** Grafica las sumas vectoriales del momento lineal ($\sum P_x$, $\sum P_y$ y $|\vec{P}_{\text{neto}}|$) a lo largo del eje temporal, demostrando que permanecen invariantes y validando la precisión física del modelo computacional[cite: 87].

---

## 4. Estructura del Directorio del Proyecto

```text
Code_L5/
├── Code_L5/
│   ├── main.cpp                 # Código fuente del simulador físico en C++
│   ├── main.py                  # Script de análisis y graficación en Python
│   └── simulation_output.txt    # Base de datos cinemáticos generada (CSV)
└── Practica_5.pdf               # Guía académica de la actividad de laboratorio
# 1. Registrar el archivo README.md en el área de preparación (Staging Area)
git add README.md

# 2. Confirmar los cambios con un mensaje descriptivo para el historial
git commit -m "Añadido el archivo README explicativo de la Práctica 5"

# 3. Subir los cambios a tu repositorio remoto en GitHub
# (Nota: si tu rama principal se llama 'master' en lugar de 'main', cambia la última palabra)
git push origin main
# Añade todos los archivos nuevos o modificados de la carpeta actual
git add .

# Confirma el conjunto de archivos
git commit -m "Estructura completa de la Actividad 2.1: C++, Python y README"

# Sube todo a GitHub
git push origin main
cd "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_5_Lab"
cat << 'EOF' > README.md
# Simulación de Sistemas Físicos: Colisiones Múltiples en Tiempo Discreto
## Práctica 5 - Informática II (Facultad de Ingeniería)
### Universidad de Antioquia (2026)

Este componente del repositorio contiene el desarrollo de la **Actividad 2.1** de la Práctica 5[cite: 1, 68]. El objetivo principal es el modelado cinemático y la simulación numérica en tiempo discreto de múltiples partículas interactuando dentro de un espacio cerrado bidimensional con obstáculos fijos[cite: 10, 11, 70]. El proyecto está diseñado en dos capas: un motor de cálculo físico desarrollado en C++ base y un script analítico de visualización en Python 3.12[cite: 12, 15, 61].

---

## 1. Características del Sistema Físico y Requerimientos

La simulación modela de manera computacional el comportamiento de $n$ partículas circulares, cada una dotada de una posición inicial, velocidad vectorial y masa cuantitativa, moviéndose en una caja rectangular[cite: 74, 75]. El entorno contiene **cuatro obstáculos cuadrados estáticos**[cite: 76].

El software gestiona de forma estricta tres leyes de interacción fundamentales:

1. **Colisión Partícula - Pared (Perfectamente Elástica):** Al impactar los límites de la caja, la componente de la velocidad perpendicular a la superficie se invierte ($e = 1.0$), conservando la energía cinética total del cuerpo[cite: 78, 79].
2. **Colisión Partícula - Obstáculo (Inelástica):** Al impactar contra las caras de los cuadrados estáticos, se determina el lado del impacto y se aplica un coeficiente de restitución ($e = 0.75$) exclusivamente a la componente perpendicular de la velocidad ($\vec{v}_{\perp}$), provocando una pérdida de energía simulada[cite: 80, 81, 82].
3. **Colisión Partícula - Partícula (Completamente Inelástica / Fusión):** Cuando dos cuerpos entran en contacto geométrico, se fusionan instantáneamente para dar origen a una nueva entidad macroscópica con masa $M = m_1 + m_2$[cite: 85, 86]. El nuevo estado cinemático se rige bajo la ley de **conservación del momento lineal global**[cite: 87]:
   $$\vec{v'} = \frac{m_1\vec{v_1} + m_2\vec{v_2}}{m_1 + m_2}$$
   La partícula absorbida se desactiva lógicamente para garantizar la consistencia de la masa invariante del sistema.

---

## 2. Arquitectura del Software (C++ Base)

La implementación en C++ se diseñó bajo el paradigma de **Programación Orientada a Objetos (POO)** con una sintaxis limpia y pedagógica, ideal para entornos de compilación estrictos como **Xcode** en macOS[cite: 10, 18].

### Componentes Principales:
* **`Vector2D` (Clase Matemática):** Encapsula los atributos coordenados privados `x` e `y`. Proporciona la interfaz algebraica para operaciones del espacio euclídeo $\mathbb{R}^2$ (`sumar`, `restar`, `multiplicarPorEscalar`, `productoPunto` y `calcularMagnitud`) sin recurrir a sobrecargas complejas de operadores.
* **`Particle` (Clase Entidad):** Almacena el estado dinámico (`id`, `pos`, `vel`, `mass`, `radius`) y una bandera booleana de actividad (`active`) para controlar la fusión[cite: 100].
* **`Obstacle` (Clase Entidad):** Define las fronteras geométricas de los cuatro cuadrados a partir de sus centros y longitudes de lado[cite: 76].
* **`CollisionsSimulation` (Clase Motor):** Centraliza el bucle temporal discreto ($\Delta_t = 0.01\text{ s}$)[cite: 25, 101]. Ejecuta la actualización cinemática, la resolución de colisiones secuenciales y el volcado estructurado de datos[cite: 101, 102, 103].

### Ruta del Archivo de Salida
El constructor del motor físico genera un archivo de texto plano de manera directa en la ruta absoluta local[cite: 103]:
`/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio_Git/Guia_5L/Code_L5/Code_L5/simulation_output.txt`

El formato de salida incluye:
* Un encabezado de tipo DataFrame (`Tiempo,ID,PosX,PosY,Masa,Activa`) para las trazas cinemáticas continuas[cite: 103, 109].
* Líneas de inyección de eventos explícitos (`[COLISIÓN FUSIÓN]`) que registran la masa de los cuerpos incidentes antes de ser consolidados[cite: 109].

---

## 3. Script de Visualización Científica (Python 3.12)

Para procesar y validar cuantitativamente los resultados obtenidos, se utiliza un script en Python optimizado para el IDE **Spyder**[cite: 44, 61].

### Características Técnicas del Script:
* **Lectura Eficiente:** Utiliza `pandas.read_csv()` configurado con el parámetro `comment='['` para filtrar e ignorar las líneas de texto informativas en milisegundos, importando la matriz numérica directamente.
* **Derivación Numérica:** Calcula las componentes instantáneas de las velocidades mediante diferencias finitas vectorizadas (`.diff()`) sobre las series de tiempo.
* **Diseño Dúo Horizontal:** Construye un lienzo gráfico simétrico de dos paneles interactivos con `matplotlib`[cite: 44, 61]:
  1. **Panel Izquierdo (Espacio de Fase $\mathbb{R}^2$):** Muestra las trayectorias continuas de las partículas, la ubicación geométrica de los 4 obstáculos fijos y burbujas terminales cuyo volumen es directamente proporcional a la masa final acumulada[cite: 44, 61, 76].
  2. **Panel Derecho (Conservación Cinética):** Grafica las sumas vectoriales del momento lineal ($\sum P_x$, $\sum P_y$ y $|\vec{P}_{\text{neto}}|$) a lo largo del eje temporal, demostrando que permanecen invariantes y validando la precisión física del modelo computacional[cite: 87].

---

## 4. Estructura del Directorio del Proyecto

```text
Code_L5/
├── Code_L5/
│   ├── main.cpp                 # Código fuente del simulador físico en C++
│   ├── main.py                  # Script de análisis y graficación en Python
│   └── simulation_output.txt    # Base de datos cinemáticos generada (CSV)
└── Practica_5.pdf               # Guía académica de la actividad de laboratorio
# 1. Registrar el archivo README.md en el área de preparación (Staging Area)
git add README.md

# 2. Confirmar los cambios con un mensaje descriptivo para el historial
git commit -m "Añadido el archivo README explicativo de la Práctica 5"

# 3. Subir los cambios a tu repositorio remoto en GitHub
# (Nota: si tu rama principal se llama 'master' en lugar de 'main', cambia la última palabra)
git push origin main
# Añade todos los archivos nuevos o modificados de la carpeta actual
git add .

# Confirma el conjunto de archivos
git commit -m "Estructura completa de la Actividad 2.1: C++, Python y README"

# Sube todo a GitHub
git push origin main
# 1. Navegar a la carpeta correcta de la Práctica 5
cd "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_5_Lab"

# 2. Generar el archivo README.md directamente en esta nueva ubicación
cat << 'EOF' > README.md
# Simulación de Sistemas Físicos: Colisiones Múltiples en Tiempo Discreto
## Práctica 5 - Informática II (Facultad de Ingeniería)
### Universidad de Antioquia (2026)

Este componente del repositorio contiene el desarrollo de la **Actividad 2.1** de la Práctica 5. El objetivo principal es el modelado cinemático y la simulación numérica en tiempo discreto de múltiples partículas interactuando dentro de un espacio cerrado bidimensional con obstáculos fijos. El proyecto está diseñado en dos capas: un motor de cálculo físico desarrollado en C++ base y un script analítico de visualización en Python 3.12.

---

## 1. Características del Sistema Físico y Requerimientos

La simulación modela de manera computacional el comportamiento de $n$ partículas circulares, cada una dotada de una posición inicial, velocidad vectorial y masa cuantitativa, moviéndose en una caja rectangular. El entorno contiene **cuatro obstáculos cuadrados estáticos**.

El software gestiona de forma estricta tres leyes de interacción fundamentales:

1. **Colisión Partícula - Pared (Perfectamente Elástica):** Al impactar los límites de la caja, la componente de la velocidad perpendicular a la superficie se invierte ($e = 1.0$), conservando la energía cinética total del cuerpo.
2. **Colisión Partícula - Obstáculo (Inelástica):** Al impactar contra las caras de los cuadrados estáticos, se determina el lado del impacto y se aplica un coeficiente de restitución ($e = 0.75$) exclusivamente a la componente perpendicular de la velocidad ($\vec{v}_{\perp}$), provocando una pérdida de energía simulada.
3. **Colisión Partícula - Partícula (Completamente Inelástica / Fusión):** Cuando dos cuerpos entran en contacto geométrico, se fusionan instantáneamente para dar origen a una nueva entidad macroscópica con masa $M = m_1 + m_2$. El nuevo estado cinemático se rige bajo la ley de **conservación del momento lineal global**:
   $$\vec{v'} = \frac{m_1\vec{v_1} + m_2\vec{v_2}}{m_1 + m_2}$$
   La partícula absorbida se desactiva lógicamente para garantizar la consistencia de la masa invariante del sistema.

---

## 2. Arquitectura del Software (C++ Base)

La implementación en C++ se diseñó bajo el paradigma de **Programación Orientada a Objetos (POO)** con una sintaxis limpia y pedagógica, ideal para entornos de compilación estrictos como **Xcode** en macOS.

### Componentes Principales:
* **`Vector2D` (Clase Matemática):** Encapsula los atributos coordenados privados `x` e `y`. Proporciona la interfaz algebraica para operaciones del espacio euclídeo $\mathbb{R}^2$ (`sumar`, `restar`, `multiplicarPorEscalar`, `productoPunto` y `calcularMagnitud`) sin recurrir a sobrecargas complejas de operadores.
* **`Particle` (Clase Entidad):** Almacena el estado dinámico (`id`, `pos`, `vel`, `mass`, `radius`) y una bandera booleana de actividad (`active`) para controlar la fusión.
* **`Obstacle` (Clase Entidad):** Define las fronteras geométricas de los cuatro cuadrados a partir de sus centros y longitudes de lado.
* **`CollisionsSimulation` (Clase Motor):** Centraliza el bucle temporal discreto ($\Delta_t = 0.01\text{ s}$). Ejecuta la actualización cinemática, la resolución de colisiones secuenciales y el volcado estructurado de datos.

### Archivo de Salida
El archivo de datos planos se generará directamente bajo el nombre `simulation_output.txt` en el directorio de ejecución para el análisis numérico correspondiente.

---

## 3. Script de Visualización Científica (Python 3.12)

Para procesar y validar cuantitativamente los resultados obtenidos, se utiliza un script en Python optimizado para el IDE **Spyder**.

### Características Técnicas del Script:
* **Lectura Eficiente:** Utiliza `pandas.read_csv()` configurado con el parámetro `comment='['` para filtrar e ignorar las líneas de texto informativas de los eventos de fusión, importando la matriz numérica directamente.
* **Derivación Numérica:** Calcula las componentes instantáneas de las velocidades mediante diferencias finitas vectorizadas (`.diff()`) sobre las series de tiempo.
* **Diseño Dúo Horizontal:** Construye un lienzo gráfico simétrico de dos paneles interactivos con `matplotlib`:
  1. **Panel Izquierdo (Espacio de Fase $\mathbb{R}^2$):** Muestra las trayectorias continuas de las partículas, la ubicación geométrica de los 4 obstáculos fijos y burbujas terminales cuyo volumen es directamente proporcional a la masa final acumulada.
  2. **Panel Derecho (Conservación Cinética):** Grafica las sumas vectoriales del momento lineal ($\sum P_x$, $\sum P_y$ y $|\vec{P}_{\text{neto}}|$) a lo largo del eje temporal, demostrando que permanecen invariantes y validando la precisión física del modelo computacional.

---

## 4. Instrucciones de Ejecución

### Motor Físico (C++):
1. Abra el archivo `main.cpp` en su entorno **Xcode**.
2. Compile y ejecute el programa para generar el volcado cinemático de datos.

### Graficación Analítica (Python):
1. Abra el archivo de visualización en su editor **Spyder**.
2. Asegúrese de actualizar la ruta interna hacia el archivo `simulation_output.txt` con la nueva ubicación.
3. Ejecute el script para desplegar los páneles interactivos de conservación cinética.
