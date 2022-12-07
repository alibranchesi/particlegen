#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H
#include <string>

class ParticleType
{
public:
    ParticleType(std::string Name, double Mass, int Charge);
    std::string GetName() const;
    double GetMass() const;
    int GetCharge() const;
    virtual void Print() const;
    virtual double GetWidth() const;
    
private:
    std::string const Name_;
    double const Mass_;
    int const Charge_;
};
#endif