#include "ResonanceType.h"
#include <iostream>

double ResonanceType::GetWidth() const { return Width_; }

void ResonanceType::Print() const
{
    ParticleType::Print();
    std::cout << "WIDTH: " << GetWidth() << '\n';
}

ResonanceType::ResonanceType(std::string Name, double Mass, int Charge, double Width) 
: ParticleType{Name, Mass, Charge}, Width_{Width} {}
