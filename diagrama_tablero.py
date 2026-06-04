# ============================================================================
# SCRIPT DE ILUSTRACIÓN BASE: CONDICIONES INICIALES DEL MOTOR FÍSICO
# OBJETIVO: Reconstruir visualmente el diagrama estructural del escenario
# ============================================================================
import matplotlib.pyplot as plt

# 1. Lienzo Gráfico
# Creamos una figura de proporciones cuadradas (9x9 pulgadas) para que coincida 
# geométricamente con la simetría de la caja de 100x100 unidades.
fig, ax = plt.subplots(figsize=(9, 9))
ax.set_title("Diagrama Estructural: Marco Experimental y Condiciones Iniciales", fontsize=12, fontweight='bold')

# 2. Dibujo del Contenedor (Límites de la Caja 100x100)
# Trazamos el contorno exterior con una línea negra gruesa (linewidth=3)
ax.plot([0, 100, 100, 0, 0], [0, 0, 100, 100, 0], color='black', linewidth=3, label='Fronteras de la Caja (e = 1.0)')

# 3. Dibujo de los 4 Obstáculos Cuadrados (Cajas de Colisión AABB)
# Definimos los centros exactos y el tamaño del lado (10 unidades) especificados en C++
obstaculos = [(30, 30), (70, 30), (30, 70), (70, 70)]
num_obs = 1

for cx, cy in obstaculos:
    # Calculamos la esquina inferior izquierda restando el radio del bloque (10 / 2 = 5)
    rect = plt.Rectangle((cx - 5, cy - 5), 10, 10, color='#A9A9A9', edgecolor='#555555', alpha=0.8, hatch='//')
    ax.add_patch(rect)
    
    # Colocamos una etiqueta de texto indicando el centro numérico del bloque
    ax.text(cx, cy, f"Obs {num_obs}\n({cx}, {cy})", color='black', fontsize=9, 
            fontweight='bold', ha='center', va='center', bbox=dict(boxstyle="square,pad=0.3", fc="white", alpha=0.7))
    num_obs += 1

# 4. Inyección de las Partículas e Ilustración de Vectores de Velocidad (Física Inicial)
# Almacenamos las tuplas de datos: (ID, Posición_X, Posición_Y, Vel_X, Vel_Y, Radio)
datos_particulas = [
    (1, 10.0, 10.0, 15.0, 12.0, 2.5),  # P1: Esquina inferior izquierda
    (2, 90.0, 10.0, -12.0, 14.0, 3.0), # P2: Esquina inferior derecha
    (3, 10.0, 90.0, 11.0, -15.0, 2.0), # P3: Esquina superior izquierda
    (4, 90.0, 90.0, -14.0, -11.0, 2.8) # P4: Esquina superior derecha
]

colores = {1: '#1f77b4', 2: '#ff7f0e', 3: '#2ca02c', 4: '#d62728'}

for p_id, px, py, vx, vy, rad in datos_particulas:
    # A. Dibujar el cuerpo perimetral circular real de la partícula (escalado al radio)
    circulo = plt.Circle((px, py), rad, color=colores[p_id], alpha=0.8, edgecolor='black', zorder=4)
    ax.add_patch(circulo)
    
    # B. Dibujar la flecha del Vector de Velocidad utilizando ax.quiver()
    # Parámetros: posición (px, py), componentes (vx, vy). 
    # angles='xy', scale_units='xy', scale=1 garantiza que la flecha se dibuje en proporción directa a las unidades del plano.
    ax.quiver(px, py, vx, vy, angles='xy', scale_units='xy', scale=1, color=colores[p_id], width=0.005, zorder=5)
    
    # C. Texto identificador que detalla las magnitudes iniciales de cada entidad
    texto_info = f"P{p_id}\nv=({vx}, {vy})"
    # Desplazamos ligeramente el texto según la posición para que no se encima con los bordes
    desplazamiento_y = 4 if py < 50 else -7
    ax.text(px, py + desplazamiento_y, texto_info, color='black', fontsize=9, ha='center', fontweight='bold')

# 5. Punto de Referencia Central (Centro de Masa del Sistema Completo)
ax.scatter(50, 50, color='red', marker='+', s=150, linewidths=2, label='Centro Geométrico (50, 50)', zorder=3)

# 6. Configuración Estética y Rótulos Técnicos de los Ejes
ax.set_xlim(-5, 105)
ax.set_ylim(-5, 105)
ax.set_xlabel("Eje Coordenado Horizontal X (Unidades Espaciales)", fontsize=10)
ax_espacio = ax.set_ylabel("Eje Coordenado Vertical Y (Unidades Espaciales)", fontsize=10)

# Activación de rejilla tenue de tipo ingeniería
ax.grid(True, linestyle='--', alpha=0.5)

# Forzamos relación de aspecto igual (1:1) para que el plano de 100x100 no se distorsione en pantallas anchas
ax.set_aspect('equal')

# Posicionamiento de la leyenda de convenciones descriptivas
ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.07), ncol=2, shadow=True)

# Renderizar el lienzo en la GUI de Spyder
plt.show()