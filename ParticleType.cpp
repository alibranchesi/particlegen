#include "ParticleType.h"
#include <iostream>
#include <string>

ParticleType::ParticleType(std::string Name, double Mass, int Charge) : Name_(Name),
                                                                        Mass_(Mass),
                                                                        Charge_(Charge) {}

std::string ParticleType::GetName() const { return Name_; }
// const char* ParticleType::GetName() const {return Name_;}

double ParticleType::GetMass() const { return Mass_; }

int ParticleType::GetCharge() const { return Charge_; }

double ParticleType::GetWidth() const {return 0; }

void ParticleType::Print() const
{
    std::cout << "NAME: " << GetName() << '\n';
    std::cout << "MASS: " << GetMass() << '\n';
    std::cout << "CHARGE: " << GetCharge() << '\n';
}
