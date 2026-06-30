#ifndef REQTANGULAR_H
#define REQTANGULAR_H

#include <cmath>
class Reqtangular {
    float epsilon_r, epsilon_eff, width, length, h, delta_L, real_L;
    float g_width, g_length;
    float c0 = 3e8;
    double frequency;
    
    public:
        Reqtangular(float f, int number_of_helz, float er, float h);
        void calculate_width();
        void calculate_length();
        void calculate_delta_L();
        void calculate_real_L();
        void calculate_g();
        float get_width() const;
        float get_length() const;
        float get_g_width() const;
        float get_g_length() const;
};
#endif