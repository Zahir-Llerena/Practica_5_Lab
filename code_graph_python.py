# Sistema de grafica para el motor de coliciones

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 1. Carga Inteligente de Datos 
ruta_archivo = "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_5_Lab/simulation_output.txt"

df = pd.read_csv(ruta_archivo, comment='[')

# Filtrar únicamente los estados de partículas vivas en la simulación
df_vivas = df[df['Activa'] == 1].copy()

# 2. Cálculo Vectorial de Velocidades y Momento Lineal
# Agrupamos por ID y calculamos el diferencial instantáneo: v = dr / dt
df_vivas['VelX'] = df_vivas.groupby('ID')['PosX'].diff() / df_vivas.groupby('ID')['Tiempo'].diff()
df_vivas['VelY'] = df_vivas.groupby('ID')['PosY'].diff() / df_vivas.groupby('ID')['Tiempo'].diff()

# Rellenar los valores iniciales (NaN) del primer instante de tiempo
df_vivas['VelX'] = df_vivas['VelX'].bfill()
df_vivas['VelY'] = df_vivas['VelY'].bfill()

# Cantidad de movimiento mecánico (p = m * v) para cada eje coordenado
df_vivas['Px'] = df_vivas['Masa'] * df_vivas['VelX']
df_vivas['Py'] = df_vivas['Masa'] * df_vivas['VelY']

# Agrupación por instante de tiempo para evaluar el comportamiento del sistema global
sistema_global = df_vivas.groupby('Tiempo').agg({'Px': 'sum', 'Py': 'sum'}).reset_index()
sistema_global['P_Neto'] = np.sqrt(sistema_global['Px']**2 + sistema_global['Py']**2)

# ============================================================================
# 3. CONSTRUCCIÓN DEL PANEL GRÁFICO 
# ============================================================================
fig, axs = plt.subplots(1, 2, figsize=(16, 7), constrained_layout=True)
colores = {1: '#1f77b4', 2: '#ff7f0e', 3: '#2ca02c', 4: '#d62728'}

# ---------- PANEL 1: ESPACIO DE FASE R^2 (Izquierda) ----------
ax_espacio = axs[0]
ax_espacio.set_title("Espacio de Fase R²: Trayectorias y Evolución Geométrica", fontsize=12, fontweight='bold')

# Dibujar límites de la caja (100x100 unidades) y los 4 obstáculos fijos simétricos 
ax_espacio.plot([0, 100, 100, 0, 0], [0, 0, 100, 100, 0], color='black', linewidth=2, label='Límites de la Caja')
obstaculos = [(30, 30), (70, 30), (30, 70), (70, 70)] 
for cx, cy in obstaculos:
    ax_espacio.add_patch(plt.Rectangle((cx - 5, cy - 5), 10, 10, color='#A9A9A9', alpha=0.7, hatch='//'))

# Graficar curvas continuas y marcar los tamaños terminales proporcionales a la masa 
for p_id, datos in df_vivas.groupby('ID'):
    ax_espacio.plot(datos['PosX'], datos['PosY'], color=colores[p_id], alpha=0.6, linewidth=1.5)
    
    # Estado final de la partícula (Acreción de volumen visual) 
    ultimo = datos.iloc[-1]
    ax_espacio.scatter(ultimo['PosX'], ultimo['PosY'], color=colores[p_id], s=ultimo['Masa'] * 60, 
                       edgecolors='black', label=f'Partícula {p_id} (M_f = {ultimo["Masa"]} u)', zorder=5)

ax_espacio.set_xlim(-2, 102)
ax_espacio.set_ylim(-2, 102)
ax_espacio.set_xlabel("Coordenada X")
ax_espacio.set_ylabel("Coordenada Y")
ax_espacio.grid(True, linestyle='--', alpha=0.5)
ax_espacio.legend(loc='upper right', shadow=True)

# ---------- PANEL 2: CONSERVACIÓN DEL MOMENTO LINEAL TOTAL (Derecha) ----------
ax_momento = axs[1]
ax_momento.set_title("Verificación Física: Conservación del Momento Lineal", fontsize=12, fontweight='bold')
ax_momento.plot(sistema_global['Tiempo'], sistema_global['Px'], label=r'$\sum P_x$', color='#4B0082', linewidth=2)
ax_momento.plot(sistema_global['Tiempo'], sistema_global['Py'], label=r'$\sum P_y$', color='#008080', linewidth=2)
ax_momento.plot(sistema_global['Tiempo'], sistema_global['P_Neto'], label=r'$|P_{neto}|$', color='#D2691E', linestyle=':', linewidth=2)

ax_momento.set_xlabel("Tiempo (s)")
ax_momento.set_ylabel("Cantidad de Movimiento (p)")
ax_momento.grid(True, linestyle='--', alpha=0.5)
ax_momento.legend(loc='lower left', shadow=True)

plt.show()


# Funcion preliminar de grafica del sistema de colisiones por Zahir Llerena 

