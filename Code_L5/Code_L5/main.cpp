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



// Version preliminar del algoritmo de simulacionde coliciones por Zahir Llerena
