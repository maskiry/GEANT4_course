#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "SensitiveDetector.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  DetectorConstruction(int m);
  DetectorConstruction(int m, float rad);
  ~DetectorConstruction() = default;

  G4VPhysicalVolume* Construct() override;

  void ConstructSDandField();
  G4VPhysicalVolume* ReinitializeGeometry();
  void setSDPos(G4float r);

private:
  G4RotationMatrix* CalcRotation(const G4ThreeVector& targetPosition);
  G4bool fCheckOverlaps = true;
  int mode = 1; //0- ico, 1 -dodec
  float sensDetRad;
  SensitiveDetector* aTrackerSD;
  G4float sdPos;
  G4Material* fLAB;
  G4Material* PPO;
  G4Material* fStainlessSteel;
  G4Material* fPMMA;
  G4Material* fScintillator;
  bool reinit;

};

#endif // DETECTOR_CONSTRUCTION_H