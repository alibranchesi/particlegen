#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H
#include "ParticleType.h"
#include <string>

class ResonanceType: public ParticleType{
    public:
    ResonanceType(std::string Name, double Mass, int Charge, double Width);
    double GetWidth() const override;
    virtual void Print() const override;
    
    private:
    double const Width_;
};

#endif