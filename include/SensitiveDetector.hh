#ifndef SENSITIVEDETECTOR_H
#define SENSITIVEDETECTOR_H

#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector{
public:
  SensitiveDetector(G4String);
  void Reset();
  ~SensitiveDetector() = default;

  void EndOfEvent(G4HCofThisEvent*) override;
  G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

private:
  G4int count;
  // std::vector<G4int> hist;
};

#endif // ELECTRON_SENSITIVEDETECTOR_H