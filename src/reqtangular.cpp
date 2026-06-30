#include <iostream>
#include "../include/reqtangular.h"

Reqtangular::Reqtangular(float f, int number_of_chose, float er, float h_get):epsilon_r(er){
    if (number_of_chose ==1)
    {
        frequency = f * pow(10,6); //MHz to Hz
    }
    else if (number_of_chose ==2)
    {
        frequency = f * pow(10,9); //GHz to Hz
    }
    h=h_get/1000; //mm to m
    calculate_width();
    calculate_length();
    calculate_delta_L();
    calculate_real_L();
    calculate_g();
};

void Reqtangular::calculate_width(){
    width=c0/((2*frequency)*(sqrt((1+epsilon_r)/2)));
}

void Reqtangular::calculate_length(){
    float term1=(epsilon_r+1)/2;
	float term2=(epsilon_r-1)/2;
	float term3=pow(1+(12 * (h/width)),(-0.5));
	epsilon_eff=term1 + (term2 * term3);
	length=c0 / ((2*frequency) * sqrt(epsilon_eff));
}

void Reqtangular::calculate_delta_L(){
    float term1 = h * 0.412;
	float term2 = (epsilon_eff + 0.3) * ((width/h) + 0.264);
	float term3 = (epsilon_eff-0.258) * ((width/h) + 0.8) ;
	delta_L = term1 * (term2/term3);
}

void Reqtangular::calculate_real_L(){
    real_L = length - (2 * delta_L);
}

void Reqtangular::calculate_g(){
    g_width = (6 * h) + width;
	g_length = (6 * h) + real_L;
}

float Reqtangular::get_width() const {
    return width;
}

float Reqtangular::get_length() const {
    return length;
}

float Reqtangular::get_g_width() const {
    return g_width;
}

float Reqtangular::get_g_length() const {
    return g_length;
}
