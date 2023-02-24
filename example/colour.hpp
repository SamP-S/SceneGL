#include "la.hpp"
#include <vector>

class colour {
    
    public:
        colour() :
            _r(1.0f), _g(1.0f), _b(1.0f), _a(1.0f) {}
        colour(float r, float g, float b) :
            _r(r), _g(g), _b(b), _a(1.0f) {}
        colour(float r, float g, float b, float a) :
        _r(r), _g(g), _b(b), _a(a) {}

        static colour Red() {
            return colour(1.0f, 0.0f, 0.0f);
        }

        static colour Green() {
            return colour(0.0f, 1.0f, 0.0f);
        }

        static colour Blue() {
            return colour(0.0f, 0.0f, 1.0f);
        }

        static colour White() {
            return colour();
        }

        static colour Black() {
            return colour(0.0f, 0.0f, 0.0f);
        }

        static colour Grey() {
            return colour(0.5f, 0.5f, 0.5f);
        }

        // add operator functions
        // linear interpolation between colours

    private:
        float _r, _g, _b, _a;
};
