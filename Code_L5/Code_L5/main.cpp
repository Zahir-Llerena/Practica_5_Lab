#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>

// ============================================================================
// 1. CLASE MATEMÁTICA: Vector2D
// ============================================================================
class Vector2D {
private:
    double x;
    double y;

public:
    Vector2D(double _x = 0.0, double _y = 0.0) {
        x = _x;
        y = _y;
    }

    // Funciones de acceso simples
    double getX() { return x; }
    double getY() { return y; }
    void setX(double _x) { x = _x; }
    void setY(double _y) { y = _y; }

    // Métodos algebraicos con nombres explícitos en lugar de símbolos
    Vector2D sumar(Vector2D otro) {
        return Vector2D(x + otro.getX(), y + otro.getY());
    }

    Vector2D restar(Vector2D otro) {
        return Vector2D(x - otro.getX(), y - otro.getY());
    }

    Vector2D multiplicarPorEscalar(double escalar) {
        return Vector2D(x * escalar, y * escalar);
    }
    
    double productoPunto(Vector2D otro) {
        return (x * otro.getX()) + (y * otro.getY());
    }
    
    double calcularMagnitud() {
        return std::sqrt(x * x + y * y);
    }
};

// ============================================================================
// 2. FUNCIONES GEOMÉTRICAS AUXILIARES
// ============================================================================

// Busca el punto más cercano en el borde del obstáculo cuadrado
Vector2D obtenerPuntoMasCercano(Vector2D posParticula, double minX, double maxX, double minY, double maxY) {
    double cercanoX = std::max(minX, std::min(posParticula.getX(), maxX));
    double cercanoY = std::max(minY, std::min(posParticula.getY(), maxY));
    return Vector2D(cercanoX, cercanoY);
}

// Determina hacia qué lado del cuadrado se rebotará
Vector2D calcularNormalDelObstaculo(Vector2D posParticula, double minX, double maxX, double minY, double maxY) {
    double distIzquierda = std::abs(posParticula.getX() - minX);
    double distDerecha   = std::abs(posParticula.getX() - maxX);
    double distAbajo     = std::abs(posParticula.getY() - minY);
    double distArriba    = std::abs(posParticula.getY() - maxY);
    
    double menorDistancia = std::min({distIzquierda, distDerecha, distAbajo, distArriba});
    
    if (menorDistancia == distIzquierda) return Vector2D(-1, 0); // Rebota a la izquierda
    if (menorDistancia == distDerecha)   return Vector2D(1, 0);  // Rebota a la derecha
    if (menorDistancia == distAbajo)     return Vector2D(0, -1); // Rebota abajo
    return Vector2D(0, 1);                                       // Rebota arriba
}

// ============================================================================
// 3. ENTIDADES DEL MUNDO FÍSICO
// ============================================================================
class Particle {
public:
    int id;
    Vector2D pos;
    Vector2D vel;
    double mass;
    double radius;
    bool active;

    Particle(int _id, Vector2D _pos, Vector2D _vel, double _mass, double _radius) {
        id = _id;
        pos = _pos;
        vel = _vel;
        mass = _mass;
        radius = _radius;
        active = true;
    }

    // Movimiento libre basado en ecuaciones de tiempo discreto
    void updatePosition(double dt) {
        if (active == false) return;
        
        // pos = pos + vel * dt
        Vector2D desplazamiento = vel.multiplicarPorEscalar(dt);
        pos = pos.sumar(desplazamiento);
    }
};

class Obstacle {
public:
    double minX, maxX, minY, maxY;

    // Modela los obstáculos cuadrados estáticos requeridos
    Obstacle(Vector2D centro, double longitudLado) {
        double mitadLado = longitudLado / 2.0;
        minX = centro.getX() - mitadLado;
        maxX = centro.getX() + mitadLado;
        minY = centro.getY() - mitadLado;
        maxY = centro.getY() + mitadLado;
    }
};

// ============================================================================
// 4. MOTOR DE SIMULACIÓN Y TRATAMIENTO DE EVENTOS
// ============================================================================
class CollisionsSimulation {
private:
    double boxWidth;
    double boxHeight;
    double dt;
    double restitutionCoef;
    std::vector<Particle> particles;
    std::vector<Obstacle> obstacles;
    std::ofstream outputFile;

    // REQUERIMIENTO 1: Paredes de la caja (Perfectamente elásticas)
    void checkWallCollisions(Particle& p) {
        // Colisión Pared Izquierda / Derecha
        if (p.pos.getX() - p.radius <= 0.0) {
            p.pos.setX(p.radius);
            p.vel.setX(-p.vel.getX()); // Invierte velocidad en X
        } else if (p.pos.getX() + p.radius >= boxWidth) {
            p.pos.setX(boxWidth - p.radius);
            p.vel.setX(-p.vel.getX()); // Invierte velocidad en X
        }

        // Colisión Pared Inferior / Superior
        if (p.pos.getY() - p.radius <= 0.0) {
            p.pos.setY(p.radius);
            p.vel.setY(-p.vel.getY()); // Invierte velocidad en Y
        } else if (p.pos.getY() + p.radius >= boxHeight) {
            p.pos.setY(boxHeight - p.radius);
            p.vel.setY(-p.vel.getY()); // Invierte velocidad en Y
        }
    }

    // REQUERIMIENTO 2: Obstáculos cuadrados (Inelásticas con coeficiente 'e')
    void checkObstacleCollisions(Particle& p) {
        for (int i = 0; i < obstacles.size(); i++) {
            Obstacle obs = obstacles[i];
            
            Vector2D puntoCercano = obtenerPuntoMasCercano(p.pos, obs.minX, obs.maxX, obs.minY, obs.maxY);
            Vector2D vectorDistancia = p.pos.restar(puntoCercano);
            double distancia = vectorDistancia.calcularMagnitud();

            // Si la distancia al cuadrado es menor que el radio, hay impacto
            if (distancia < p.radius) {
                Vector2D normal;
                if (distancia > 0.0001) {
                    normal = vectorDistancia.multiplicarPorEscalar(1.0 / distancia);
                } else {
                    normal = calcularNormalDelObstaculo(p.pos, obs.minX, obs.maxX, obs.minY, obs.maxY);
                }

                double vNormal = p.vel.productoPunto(normal);
                
                if (vNormal < 0) { // Se mueve hacia adentro del obstáculo
                    Vector2D velNormal = normal.multiplicarPorEscalar(vNormal);
                    Vector2D velTangential = p.vel.restar(velNormal);

                    // Ecuación del documento: rebote aplicando 'e' a la componente perpendicular
                    p.vel = velTangential.restar(velNormal.multiplicarPorEscalar(restitutionCoef));
                    p.pos = puntoCercano.sumar(normal.multiplicarPorEscalar(p.radius)); // Separación física
                }
            }
        }
    }

    // REQUERIMIENTO 3: Choque entre partículas (Completamente Inelástica / Fusión)
    void checkParticleCollisions(int i, int j, double currentTime) {
        Particle& p1 = particles[i];
        Particle& p2 = particles[j];

        if (p1.active == false || p2.active == false) return;

        Vector2D vectorDistancia = p2.pos.restar(p1.pos);
        double distancia = vectorDistancia.calcularMagnitud();
        double sumaRadios = p1.radius + p2.radius;

        if (distancia <= sumaRadios) {
            double m1 = p1.mass;
            double m2 = p2.mass;
            double mTotal = m1 + m2; // M = m1 + m2
            
            // FÓRMULA DE CONSERVACIÓN DEL MOMENTO: v' = (m1*v1 + m2*v2) / (m1+m2)
            Vector2D m1_v1 = p1.vel.multiplicarPorEscalar(m1);
            Vector2D m2_v2 = p2.vel.multiplicarPorEscalar(m2);
            Vector2D nuevaVelocidad = (m1_v1.sumar(m2_v2)).multiplicarPorEscalar(1.0 / mTotal);
            
            // Corrección geométrica para evitar que queden unidas de forma traslapada
            double overlap = sumaRadios - distancia;
            Vector2D normalDireccion = (distancia > 0.0) ? vectorDistancia.multiplicarPorEscalar(1.0 / distancia) : Vector2D(1.0, 0.0);
            Vector2D posCorregida = p1.pos.restar(normalDireccion.multiplicarPorEscalar(overlap * (m2 / mTotal)));
            
            // Nuevo radio conservando las propiedades físicas del nuevo cuerpo [cite: 91]
            double nuevoRadio = std::sqrt((p1.radius * p1.radius) + (p2.radius * p2.radius));

            // Registro explícito solicitado en la guía [cite: 109]
            outputFile << "[COLISIÓN FUSIÓN],Tiempo=" << currentTime
                       << ",ID_Absorbente=" << p1.id << ",Masa_m1=" << m1
                       << ",ID_Absorbida=" << p2.id << ",Masa_m2=" << m2
                       << ",Nueva_Masa_M=" << mTotal << "\n";

            // Modificar las propiedades de p1 para que actúe como el nuevo cuerpo combinado
            p1.pos = posCorregida;
            p1.vel = nuevaVelocidad;
            p1.mass = mTotal;
            p1.radius = nuevoRadio;

            // Desactivar la partícula p2 absorbida
            p2.active = false;
        }
    }

public:
    CollisionsSimulation(double ancho, double alto, double pasoTiempo, double e) {
        boxWidth = ancho;
        boxHeight = alto;
        dt = pasoTiempo;
        restitutionCoef = e;
        
        std::string path = "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_5_Lab/simulation_output.txt";
        outputFile.open(path);
    }

    ~CollisionsSimulation() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }

    void addParticle(Vector2D pos, Vector2D vel, double mass, double radius) {
        int nuevoId = (int)particles.size() + 1;
        Particle p(nuevoId, pos, vel, mass, radius);
        particles.push_back(p);
    }

    void addObstacle(Vector2D centro, double longitudLado) {
        if (obstacles.size() < 4) {
            Obstacle obs(centro, longitudLado);
            obstacles.push_back(obs);
        }
    }

    void runSimulation(double tiempoTotal) {
        double currentTime = 0.0;
        outputFile << "Tiempo,ID,PosX,PosY,Masa,Activa\n"; // Encabezado

        while (currentTime <= tiempoTotal) {
            // 1. Volcar el estado cinemático actual
            for (int i = 0; i < particles.size(); i++) {
                Particle p = particles[i];
                outputFile << currentTime << "," << p.id << "," << p.pos.getX() << ","
                           << p.pos.getY() << "," << p.mass << "," << (p.active ? 1 : 0) << "\n";
            }

            // 2. Actualizar posiciones (Ecuaciones de movimiento libre)
            for (int i = 0; i < particles.size(); i++) {
                particles[i].updatePosition(dt);
            }

            // 3. Resolver interacciones con el entorno cerrado (Paredes)
            for (int i = 0; i < particles.size(); i++) {
                checkWallCollisions(particles[i]);
            }

            // 4. Resolver interacciones con obstáculos fijos
            for (int i = 0; i < particles.size(); i++) {
                checkObstacleCollisions(particles[i]);
            }
            
            // 5. Resolver colisiones mutuas partícula-partícula
            for (int i = 0; i < particles.size(); i++) {
                for (int j = i + 1; j < particles.size(); j++) {
                    checkParticleCollisions(i, j, currentTime);
                }
            }
            
            currentTime += dt; // Avanzar el reloj discreto
        }
        std::cout << "Simulación finalizada. Archivo escrito en la carpeta directorio de Zahir Llerena.\n";
    }
};

// ============================================================================
// 5. PROGRAMA PRINCIPAL
// ============================================================================
int main() {
    CollisionsSimulation sim(100.0, 100.0, 0.01, 0.75);

    // Inicializar los 4 obstáculos de la práctica
    sim.addObstacle(Vector2D(30.0, 30.0), 10.0);
    sim.addObstacle(Vector2D(70.0, 30.0), 10.0);
    sim.addObstacle(Vector2D(30.0, 70.0), 10.0);
    sim.addObstacle(Vector2D(70.0, 70.0), 10.0);

    // Registrar 4 partículas concurrentes
    sim.addParticle(Vector2D(10.0, 10.0), Vector2D(15.0, 12.0), 2.0, 2.5);
    sim.addParticle(Vector2D(90.0, 10.0), Vector2D(-12.0, 14.0), 3.0, 3.0);
    sim.addParticle(Vector2D(10.0, 90.0), Vector2D(11.0, -15.0), 1.5, 2.0);
    sim.addParticle(Vector2D(90.0, 90.0), Vector2D(-14.0, -11.0), 2.5, 2.8);

    sim.runSimulation(15.0);
    return 0;
}



// Version preliminar del algoritmo de simulacionde coliciones por Zahir Llerena
