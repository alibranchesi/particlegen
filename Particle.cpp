#include "ParticleType.h"
#include "ResonanceType.h"
#include "Particle.h"
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>

std::vector<ParticleType *> Particle::Table_{};

Particle::Particle(std::string Name, double Px, double Py, double Pz)
    : Px_{Px}, Py_{Py}, Pz_{Pz}
{
    Index_ = Particle::FindParticle(Name);
}

Particle::Particle() : Px_{0},
                       Py_{0},
                       Pz_{0},
                       Index_{-1} {};

int Particle::FindParticle(std::string Name)
{
    for (int i = 0; i < static_cast<int>(Table_.size()); i++)
        if (Table_[i]->GetName() == Name)
            return i;
    std::cout << "\n ** PARTICLE " << Name << " NOT FOUND ** \n";
    return -1;
}

void Particle::AddParticleType(std::string Name, double Mass, int Charge,
                               double Width)
{
    ParticleType *new_particle;
    if (Width > 0)
        new_particle = new ResonanceType{Name, Mass, Charge, Width};
    else
        new_particle = new ParticleType{Name, Mass, Charge};

    Table_.push_back(new_particle);
}

double Particle::GetPx() const { return Px_; }

double Particle::GetPy() const { return Py_; }

double Particle::GetPz() const { return Pz_; }

void Particle::SetP(double Px, double Py, double Pz)
{
    Px_ = Px;
    Py_ = Py;
    Pz_ = Pz;
}

double Particle::GetPNorm() const
{
    return GetPx() * GetPx() + GetPy() * GetPy() + GetPz() * GetPz();
}

double Particle::GetMass() const { return Table_[Index_]->GetMass(); }

double Particle::GetTotalEnergy() const
{
    return std::sqrt(GetMass() * GetMass() + GetPNorm() * GetPNorm());
}

double Particle::GetParticleCharge() const
{
    return Table_[Index_]->GetCharge();
}

double Particle::InvMass(Particle &other) const
{
    return std::sqrt(pow((GetTotalEnergy() + other.GetTotalEnergy()), 2) -
                     (pow((GetPx() - other.GetPx()), 2) +
                      pow((GetPy() - other.GetPy()), 2) +
                      pow((GetPz() - other.GetPz()), 2)));
}

void Particle::SetIndex(int Index) { Index_ = Index; }

void Particle::SetIndex(std::string Name)
{
    if (FindParticle(Name) != -1)
    {
        Index_ = FindParticle(Name);
    }
    else
    {
        std::cerr << '\n'
                  << " ** CANNOT ASSIGN INDEX ** " << '\n';
    }
}

void Particle::PrintIndex() const
{
    std::cout << "INDEX: " << Index_ << '\n';
    std::cout << "Px: " << Px_ << '\n';
    std::cout << "Py: " << Py_ << '\n';
    std::cout << "Pz: " << Pz_ << '\n';
}
void Particle::PrintTable()
{
    std::cout << "TABLE: " << '\n';
    for (int i = 0; i < static_cast<int>(Table_.size()); i++)
    {
        Table_[i]->Print();
        std::cout << '\n';
    }
}

int Particle::GetIndex() const { return Index_; }

int Particle::Decay2body(Particle &dau1, Particle &dau2) const
{
    if (GetMass() == 0.0)
    {
        std::cout << '\n'
                  << " ** DECAYMENT CANNOT BE PERFORMED IF MASS IS ZERO ** " << '\n';
        return 1;
    }

    double massMot = GetMass();
    double massDau1 = dau1.GetMass();
    double massDau2 = dau2.GetMass();

    if (Index_ > -1)
    { // add width effect

        // gaussian random numbers

        float x1, x2, w, y1, y2;

        double invnum = 1. / RAND_MAX;
        do
        {
            x1 = 2.0 * rand() * invnum - 1.0;
            x2 = 2.0 * rand() * invnum - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2.0 * log(w)) / w);
        y1 = x1 * w;
        y2 = x2 * w;

        massMot += Table_[Index_]->GetWidth() * y1;
    }

    if (massMot < massDau1 + massDau2)
    {
        std::cerr << '\n'
                  << " ** DECAYMENT CANNOT BE PERFORMED BECAUSE MASS IS TOO LOW IN THIS CHANNEL ** " << '\n';
        return 2;
    }

    double pout = sqrt((massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) / massMot * 0.5;

    double norm = 2 * M_PI / RAND_MAX;

    double phi = rand() * norm;
    double theta = rand() * norm * 0.5 - M_PI / 2.;
    dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi), pout * cos(theta));
    dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi), -pout * cos(theta));

    double energy = sqrt(Px_ * Px_ + Py_ * Py_ + Pz_ * Pz_ + massMot * massMot);

    double bx = Px_ / energy;
    double by = Py_ / energy;
    double bz = Pz_ / energy;

    dau1.Boost(bx, by, bz);
    dau2.Boost(bx, by, bz);

    return 0;
}

void Particle::Boost(double bx, double by, double bz)
{

    double energy = GetTotalEnergy();

    // Boost this Lorentz vector
    double b2 = bx * bx + by * by + bz * bz;
    double gamma = 1.0 / sqrt(1.0 - b2);
    double bp = bx * Px_ + by * Py_ + bz * Pz_;
    double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

    Px_ += gamma2 * bp * bx + gamma * bx * energy;
    Py_ += gamma2 * bp * by + gamma * by * energy;
    Pz_ += gamma2 * bp * bz + gamma * bz * energy;
}
