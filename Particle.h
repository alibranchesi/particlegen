#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>
#include <string>
#include "ParticleType.h"

class Particle
{
public:
    Particle(); //default constructor
    Particle(std::string Name, double Px, double Py, double Pz);
    static void AddParticleType(std::string Name, double Mass, int Charge,
                                double Width = 0); 
    int GetCharge() const;
    double GetPx() const;
    double GetPy() const;
    double GetPz() const;
    void SetP(double Px=0, double Py=0, double Pz=0);
    double GetPNorm() const;
    double GetMass() const;
    double GetTotalEnergy() const;
    double GetParticleCharge() const;
    int GetIndex() const;
    double InvMass(Particle &other) const;
    void SetIndex(int Index);
    void SetIndex(std::string Name);
    void PrintIndex() const; 
    static void PrintTable();
    int Decay2body(Particle &dau1,Particle &dau2) const;

private:    
    double Px_;
    double Py_;
    double Pz_;
    int Index_;
    static std::vector<ParticleType*> Table_;
    static int FindParticle(std::string Name);
    void Boost(double Bx, double By, double Bz);
};
#endif