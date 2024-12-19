#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <math.h>
#include <fstream>
#include <iostream>


void PrimaryGeneratorAction::setMode(int m){
  mode = m;
}

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeBox(0),
  mode(1)
{
  G4cout << "Creating Generator" << std::endl;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* electron
    = particleTable->FindParticle("e-");
  G4ParticleDefinition* positron
    = particleTable->FindParticle("e+");

  fParticleGun = new G4ParticleGun();
}

PrimaryGeneratorAction::PrimaryGeneratorAction(int m)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeBox(0),
  mode(m)
{
  G4cout << "Creating Generator" << std::endl;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* electron
    = particleTable->FindParticle("e-");
  G4ParticleDefinition* positron
    = particleTable->FindParticle("e+");

  fParticleGun = new G4ParticleGun();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  G4cout << "Deleting GeneratorAction" << std::endl;

  delete fParticleGun;
  delete fEnvelopeBox;
  delete electron;
  delete positron;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (mode==4){
    G4double theta = acos(1 - 2 * G4UniformRand());
    G4double phi = 2 * M_PIl * G4UniformRand();
    G4double r = 0.6203505 * pow(G4UniformRand(), 1.0 / 3.0) * m;

    fParticleGun->SetParticlePosition(G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta))*r);
  }
  G4double angle_theta = acos(1 - 2 * G4UniformRand());
  G4double phiOfTrajectory = 2 * M_PIl * G4UniformRand();
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(cos(phiOfTrajectory) * sin(angle_theta), sin(phiOfTrajectory)* sin(angle_theta), cos(angle_theta)));
  
  fParticleGun->GeneratePrimaryVertex(anEvent);

}