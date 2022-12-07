#include "ParticleType.h"
#include "ResonanceType.h"
#include "Particle.h"
#include "TRandom.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include <vector>
R__LOAD_LIBRARY(ParticleType_cpp.so);
R__LOAD_LIBRARY(ResonanceType_cpp.so);
R__LOAD_LIBRARY(Particle_cpp.so);

void generation()
{
  std::vector<Particle> EventParticle;
  std::vector<Particle> DecayEventParticle;
  Particle::AddParticleType("Pione+", 0.13957, +1);
  Particle::AddParticleType("Pione-", 0.13957, -1);
  Particle::AddParticleType("Kaone+", 0.49367, +1);
  Particle::AddParticleType("Kaone-", 0.49367, -1);
  Particle::AddParticleType("Protone+", 0.93827, +1);
  Particle::AddParticleType("Protone-", 0.93827, -1);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);

  TCanvas *c1 = new TCanvas("c1", "canvas1", 2000, 2000);
  TCanvas *c2 = new TCanvas("c2", "canvas2", 2000, 2000);

  c1->Divide(3, 2);
  c2->Divide(3, 3);

  // TFile *file = new TFile("particle.root", "RECREATE");
  TH1F *ParticleDistribution = new TH1F("h1", "tipi particelle", 7, 0, 7);
  TH1F *PhiDistribution = new TH1F("h2", "distribuzione angolp phi", 1000, 0, 2 * TMath::Pi());
  TH1F *ThetaDistribution = new TH1F("h3", "distribuzione angolp theta", 1000, 0, TMath::Pi());
  TH1F *ImpulseDistribution = new TH1F("h4", "impulso", 1000, 0, 4);
  TH1F *TraverseImpulseDistribution = new TH1F("h5", "impulso traverso", 1000, 0, 4);
  TH1F *EnergyDistribution = new TH1F("h6", "energia", 1000, 0, 2);
  TH1F *TotalInvMass = new TH1F("h7", "inv massa totale", 1000, 0, 4);
  // h7->Sumw2();
  TH1F *invMassOppositeCharge = new TH1F("h8", "inv massa segno discorde", 1000, 0, 200);
  // h8->Sumw2();
  TH1F *invMassSameCharge = new TH1F("h9", "inv massa segno concorde", 1000, 0, 200);
  // h9->Sumw2();
  TH1F *invMassKPOppositeCharge = new TH1F("h10", "inv massa pione+/Kaone-", 1000, 0, 200);
  // h10->Sumw2();
  TH1F *invMassPKOppositeCharge = new TH1F("h11", "inv massa pione-/Kaone+", 1000, 0, 200);
  // h11->Sumw2();
  TH1F *invMassKPPSameCharge = new TH1F("h12", "inv massa pione+/Kaone+", 1000, 0, 200);
  // h12->Sumw2();
  TH1F *invMassKPNSameCharge = new TH1F("h13", "inv massa pione-/Kaone-", 1000, 0, 200);
  // h13->Sumw2();
  TH1F *invMassDaughter = new TH1F("h14", "inv massa figlie", 1000, 0, 200);
  // h14->Sumw2();
  TF1 *uniform_f = new TF1("f1", "[0]");
  TF1 *exp_f = new TF1("f2", "[0]*exp(x*[1])");
  TF1 *gaus_f = new TF1("f3", "gaus", 0.3, 1);
  invMassOppositeCharge->Sumw2();
  invMassSameCharge->Sumw2();
  invMassKPOppositeCharge->Sumw2();
  invMassPKOppositeCharge->Sumw2();
  invMassKPPSameCharge->Sumw2();
  invMassKPNSameCharge->Sumw2();
  // TF1 *f = new TF1("f", "gaus", 0, 200);
  // f->SetParameters(0.89166827, 0.050);
  // file->Write();

  gRandom->SetSeed();
  int MaxEvent = 100;

  for (int i = 0; i < 1E5; i++)
  {
    EventParticle.clear();

    for (int j = 0; j < MaxEvent; j++)
    {
      Particle particle;

      double Phi = gRandom->Uniform(0, 2 * TMath::Pi());
      double Theta = gRandom->Uniform(0, TMath::Pi());
      double Impulse = gRandom->Exp(1);

      double Px = Impulse * sin(Theta) * cos(Phi);
      double Py = Impulse * sin(Theta) * sin(Phi);
      double Pz = Impulse * cos(Theta);

      particle.SetP(Px, Py, Pz);

      double random = gRandom->Rndm();
      if (random < 0.4)
      {
        particle.SetIndex("Pione+");
      }
      else if (random < 0.8)
      {
        particle.SetIndex("Pione-");
      }
      else if (random < 0.85)
      {
        particle.SetIndex("Kaone+");
      }
      else if (random < 0.90)
      {
        particle.SetIndex("Kaone-");
      }
      else if (random < 0.945)
      {
        particle.SetIndex("Protone+");
      }
      else if (random < 0.99)
      {
        particle.SetIndex("Protone-");
      }
      else
      {
        particle.SetIndex("K*");
      }

      if (particle.GetIndex() == 6)
      {
        Particle p;
        Particle k;
        double RandomDecay = gRandom->Rndm();

        if (RandomDecay < 0.5)
        {
          p.SetIndex("Pione+");
          k.SetIndex("Kaone-");
        }
        else
        {
          p.SetIndex("Pione-");
          k.SetIndex("Kaone+");
        }
        particle.Decay2body(p, k);
        DecayEventParticle.push_back(p);
        DecayEventParticle.push_back(k);
      }

      EventParticle.push_back(particle);

      int decaysize = DecayEventParticle.size();
      for (i = 0; i < decaysize; ++i)
      {
        EventParticle.push_back(DecayEventParticle[i]);
      }

      ParticleDistribution->Fill(particle.GetIndex());}
      
      PhiDistribution->Fill(Phi);
      ThetaDistribution->Fill(Theta);
      ImpulseDistribution->Fill(Impulse);
      Float_t impulsotraverso = sqrt(pow(Px, 2) + pow(Py, 2));
      TraverseImpulseDistribution->Fill(impulsotraverso);
      EnergyDistribution->Fill(particle.GetTotalEnergy());
      invMassDaughter->Fill(particle.InvMass());
  }
/*
  int size = EventParticle.size();

  for (int a = 0; a < size; ++a)
  {
    for (int b = 1; b < size; ++b)
    {
      h7->Fill(EventParticle[a].InvMass(EventParticle[b]));
    }

    for (int a = 0; a < size; ++a)
    {
      for (int b = 1; b < size; ++b)
      {
        if ((EventParticle[a].GetParticleCharge()) * (EventParticle[b].GetParticleCharge()) < 0)
        {
          h8->Fill(EventParticle[a].InvMass(EventParticle[b]));
        }
      }
    }

    for (int a = 0; a < size; ++a)
    {
      for (int b = 1; b < size; ++b)
      {
        if ((EventParticle[a].GetParticleCharge()) * (EventParticle[b].GetParticleCharge()) > 0)
        {
          h9->Fill(EventParticle[a].InvMass(EventParticle[b]));
        }
      }
    }

    for (int a = 0; a < size; ++a)
    {
      for (int b = 1; b < size; ++b)
      {
        if (EventParticle[a].GetIndex() == 0)
        {
          if (EventParticle[b].GetIndex() == 3)
          {
            h10->Fill(EventParticle[a].InvMass(EventParticle[b]));
          }
          if (EventParticle[b].GetIndex() == 2)
          {
            h12->Fill(EventParticle[a].InvMass(EventParticle[b]));
          }
        }
      }
    }

    for (int a = 0; a < size; ++a)
    {
      for (int b = 1; b < size; ++b)
      {
        if (EventParticle[a].GetIndex() == 1)
        {
          if (EventParticle[b].GetIndex() == 2)
          {
            h11->Fill(EventParticle[a].InvMass(EventParticle[b]));
          };
          if (EventParticle[b].GetIndex() == 3)
          {
            h12->Fill(EventParticle[a].InvMass(EventParticle[b]));
          };
        }
      }
    }
    int decaysize = DecayEventParticle.size();
    for (int a = 0; a < (decaysize) / 2; a + 2)
    {
      h14->Fill(DecayEventParticle[a].InvMass(DecayEventParticle[a + 1]));
    }
    // h14->Fit("f");

    // c1->cd(1);
    // c1->cd(2);
    EventParticle.clear();
    DecayEventParticle.clear();
  }
}
  */

c1->cd(1);
ParticleDistribution->Draw();
}