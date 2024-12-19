#include "Randomize.hh"
#include "SensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"

#include <iostream>

SensitiveDetector::SensitiveDetector(G4String name) : count(0), G4VSensitiveDetector(name) {}

void SensitiveDetector::Reset(){
  // hist.clear();
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *){
  G4Track *track = step->GetTrack();
  if(track->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
    if (G4UniformRand() < 0.28) count++;
  }
  track->SetTrackStatus(fStopAndKill);
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*){
  G4AnalysisManager *manager = G4AnalysisManager::Instance();
  G4int EvtId =  G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  std::cout << "EvtId: " << EvtId << " -> photon count: " << count << std::endl;

  const RunAction* runAction = dynamic_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
  if (runAction) {
    runAction->addCount(count);
  }

  count=0;
}