#ifndef FPrimaryGeneratorAction_h
#define FPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"

# define M_PIl 3.14159265358979323846 

class G4GeneralParticleSource;
class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:

  PrimaryGeneratorAction();
  PrimaryGeneratorAction(int m);
  ~PrimaryGeneratorAction();
  void GeneratePrimaries(G4Event*);
  void setMode(int m);
  // const G4GeneralParticleSource* GetParticleGun() const { return fParticleGun; }

private:
  int mode=1; //mode=1 - x, mode=2 - y, mode=3 - z , mode=4 - uniform
  
  G4ParticleGun*  fParticleGun; 
  G4Tubs* fEnvelopeBox;

  G4ParticleDefinition* electron;
  G4ParticleDefinition* positron;
};
#endif