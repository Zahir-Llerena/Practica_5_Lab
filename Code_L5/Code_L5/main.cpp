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




// Version preliminar del algoritmo de simulacionde coliciones por Zahir Llerena
