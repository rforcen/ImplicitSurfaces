//
//  Function.hpp
//  Polygonizer
//
//  Created by asd on 12/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Function_hpp
#define Function_hpp

#include <math.h>

class AuxMath {
public:
    double sphere(double x, double y, double z) {    return x*x + y*y + z*z;    }
    double sqr(double x) { return x*x; }
    double sqr3(double x) { return x*x*x; }
    double cube(double x) { return x*x*x; }
    double sqr4(double x) { return x*x*x*x; }
    double pow4(double x) { return x*x*x*x; }
    double orth(double x, double y, double z) {
        double a = 0.06, b = 2;
        return     (sqr(x*x + y*y - 1) + z*z)
        *(sqr(y*y + z*z - 1) + x*x)
        *(sqr(z*z + x*x - 1) + y*y)
        - a*a*(1 + b*(x*x + y*y + z*z));
    }
};


class Function : public AuxMath { // base class
public:
    virtual double evaluate(double x, double y, double z)=0;
};

class Mandelbox : public Function {
    double evaluate(double x, double y, double z) { return mandelbrot(x,y,z)-1; }
    double mandelbrot(double x, double y, double z) {
        int s = 7;        x *= s;        y *= s;        z *= s;
        
        double posX = x, posY = y, posZ = z;
        double dr = 1.0;
        double r = 0.0, scale = 2, minRadius2 = 0.25, fixedRadius2 = 1;
        
        for (int n = 0; n < 50; n++) {
            // Reflect
            if (x > 1.0) x = 2.0 - x;
            else if (x < -1.0)    x = -2.0 - x;
            if (y > 1.0)        y = 2.0 - y;
            else if (y < -1.0)    y = -2.0 - y;
            if (z > 1.0)        z = 2.0 - z;
            else if (z < -1.0)    z = -2.0 - z;
            
            // Sphere Inversion
            double r2 = x * x + y * y + z * z;
            
            if (r2 < minRadius2) {
                x = x * fixedRadius2 / minRadius2;
                y = y * fixedRadius2 / minRadius2;
                z = z * fixedRadius2 / minRadius2;
                dr = dr * fixedRadius2 / minRadius2;
            } else if (r2 < fixedRadius2) {
                x = x * fixedRadius2 / r2;
                y = y * fixedRadius2 / r2;
                z = z * fixedRadius2 / r2;
                fixedRadius2 *= fixedRadius2 / r2;
            }
            
            x = x * scale + posX;
            y = y * scale + posY;
            z = z * scale + posZ;
            dr *= scale;
        }
        r = sqrt(x * x + y * y + z * z);
        return r;
    }
};

class Sphere : public Function {
    double r=1;
    double  evaluate(double x, double y, double z) {    return sphere(x,y,z)-r;    }
};
class Blob : public Function {
    double evaluate(double x, double y, double z) { return 4. - sphere(x + .5, y - .5, z - .5) - sphere(x - .5, y + .5, z - .5) - sphere(x - .5, y - .5, z + .5); }
};
class NordstarndWeird : public Function {
    double evaluate(double x, double y, double z) {
        return 25.*(x*x*x*(y + z) + y*y*y*(x + z) + z*z*z*(x + y))
        + 50*(x*x*y*y + x*x*z*z + y*y*z*z)
        - 125*(x*x*y*z + y*y*x*z + z*z*x*y)
        + 60*x*y*z
        - 4*(x*y + y*z + z*x);
    }
};
class DecoCube : public Function {
    double evaluate(double x, double y, double z) {
        double a = 0.95, b = 0.01;
        return (sqr(x*x + y*y - a*a) + sqr(z*z - 1))*(sqr(y*y + z*z - a*a) + sqr(x*x - 1))
        *(sqr(z*z + x*x - a*a) + sqr(y*y - 1))
        - b;
    }
};
class Cassini : public Function {
    double evaluate(double x, double y, double z) {
        double a = 0.3;
        return    (sqr((x -a)) + z*z) * (sqr((x + a)) + z*z) - pow4(y); // ( (x-a)^2 + y^2) ((x+a)^2 + y^2) = z^4; a = 0.5;
    }
};
class Orth : public Function {
//    double a = 0.06, b = 2;
    double evaluate(double x, double y, double z) {
        return    orth(x,y,z);
    }
};

class Orth3 : public Function {
    double evaluate(double x, double y, double z) {
        return 4. - orth(x + .5, y - .5, z - .5) - orth(x - .5, y + .5, z - .5) - orth(x - .5, y - .5, z + .5);
    }
};
class Pretzel : public Function {
    double aa = 1.6;
    double evaluate(double x, double y, double z) {
        return sqr( ((x -1)*(x - 1) + y*y - aa*aa) * ((x + 1)*(x + 1) + y*y - aa*aa)) + z*z*10 - 1;
    }
};
class Tooth : public Function {
    double evaluate(double x, double y, double z) {
        return sqr4(x) + sqr4(y) + sqr4(z) - sqr(x) - sqr(y) - sqr(z);
    }
};

class Pilz : public Function {
    double a = 0.05, b = -0.1;
    double evaluate(double x, double y, double z) {
        return    sqr( sqr(x*x + y*y - 1) + sqr(z - 0.5))
        * (  sqr(y*y / a*a + sqr(z + b) - 1) + x*x)
        - a * (1 + a*sqr(z - 0.5));
    }
};
class Bretzel : public Function {
    double evaluate(double x, double y, double z) {
        double a = 0.003, b = 0.7;
        return sqr(x*x*(1 - x*x) - y*y)   + 0.5*z*z - a*(1 + b*(x*x + y*y + z*z));
    }
};
class BarthDecic : public Function {
    double evaluate(double x, double y, double z) {
        double GR = 1.6180339887; // Golden ratio
        double GR2 = GR * GR;
        double GR4 = GR2 * GR2;
        double w = 0.3;
        return 8*(x*x - GR4*y*y)*(y*y - GR4*z*z)*(z*z - GR4*x*x)
        *(x*x*x*x + y*y*y*y + z*z*z*z - 2*x*x*y*y - 2*x*x*z*z - 2*y*y*z*z)
        + (3 + 5*GR)*sqr((x*x + y*y + z*z - w*w))
        *sqr((x*x + y*y + z*z - (2 - GR)*w*w))*w*w;
    }
};
class Clebsch0  : public Function {
    double evaluate(double x, double y, double z) {
        return 81*(cube(x) + cube(y) + cube(z)) - 189*(sqr(x)*y + sqr(x)*z + sqr(y)*x + sqr(y)*z + sqr(z)*x + sqr(z)*y) +
        54*(x*y*z) + 126*(x*y + x*z + y*z) - 9*(sqr(x) + sqr(y) + sqr(z)) - 9*(x + y + z) + 1;
    }
};
class Clebsch : public Function {
    double evaluate(double x, double y, double z) {
        return
        16. * cube(x) + 16 * cube(y) - 31 * cube(z) + 24 * sqr(x) * z - 48 * sqr(x) * y - 48 * x * sqr(y)
        + 24 * sqr(y) * z - 54 * sqrt(3) * sqr(z) - 72 * z;
    }
};
class Chubs : public Function {
    double evaluate(double x, double y, double z) {
        return pow4(x) + pow4(y) + pow4(z) - sqr(x) - sqr(y) - sqr(z) + 0.5;
        // x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + 0.5 = 0;
    }
};
class Chair : public Function {
    double evaluate(double x, double y, double z) {
        double k = 5, a = 0.95, b = 0.8;
        return sqr(sqr(x) + sqr(y) + sqr(z) - a*sqr(k)) - b*((sqr((z - k)) - 2*sqr(x))*(sqr((z + k)) - 2*sqr(y)));
        // (x^2+y^2+z^2-a*k^2)^2-b*((z-k)^2-2*x^2)*((z+k)^2-2*y^2)=0,       with k=5, a=0.95 and b=0.8.
    }
};
class Roman : public Function {
    double evaluate(double x, double y, double z) {
        double r=2;
        return sqr(x)*sqr(y) + sqr(y)*sqr(z) + sqr(z)*sqr(x) - r*x*y*z;
    }
};
class Sinxyz : public Function {
    double evaluate(double x, double y, double z) {
        return sin(x)*sin(y)*sin(z);
    }
};
class F001 : public Function {
    double evaluate(double x, double y, double z) {
        return     sqr3(x)+sqr3(y)+sqr4(z)-10; // x^3 + y^3 + z^4 -10 = 0
    }
};
class TangleCube : public Function {
    double evaluate(double x, double y, double z) {
        return     sqr4(x) - 5*sqr(x) + sqr4(y) - 5*sqr(y) + sqr4(z) - 5*sqr(z) +11.8;
    }
};
class Goursat : public Function {
    double evaluate(double x, double y, double z) {
        double a=0, b=0, c=-1;
        return sqr4(x)+sqr4(y)+sqr4(z)+ a*sqr(sqr(x)+sqr(y)+sqr(z)) + b*(sqr(x)+sqr(y)+sqr(z)) + c;
        // (x^4 + y^4 + z^4) + a * (x^2 + y^2 + z^2)^2 + b * (x^2 + y^2 + z^2) + c = 0
    }
};

extern vector<string> fNames;
extern vector<Function*>funcs;

#endif /* Function_hpp */
