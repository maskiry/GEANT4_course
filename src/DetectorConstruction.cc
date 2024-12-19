#include <vector>
#include <cmath>

#include "DetectorConstruction.hh"

#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolumeStore.hh"

DetectorConstruction::DetectorConstruction(){
  mode=1;
  sensDetRad = 0.866;
  aTrackerSD = 0;
  reinit=false;
}
DetectorConstruction::DetectorConstruction(int m){
  mode=m;
  sensDetRad = 0.866;
  aTrackerSD = 0;
  reinit=false;
}

DetectorConstruction::DetectorConstruction(int m, float rad){
  mode=m;
  sensDetRad = rad;
  aTrackerSD = 0;
  reinit=false;
}

G4RotationMatrix* DetectorConstruction::CalcRotation(const G4ThreeVector& targetPosition){

  G4ThreeVector directionVector = -targetPosition; 
  directionVector.setMag(1.0); 

  G4ThreeVector rotationAxis = directionVector.cross(G4ThreeVector(0, 0, 1));
  rotationAxis.setMag(1.0); 

  G4double rotationAngle = acos(directionVector.dot(G4ThreeVector(0, 0, 1)));

  G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
  rotationMatrix->rotate(rotationAngle, rotationAxis);
  return rotationMatrix;

}


G4VPhysicalVolume* DetectorConstruction::Construct(){
  if (reinit) return ReinitializeGeometry();
  reinit = true;
  auto nist = G4NistManager::Instance();

  //MAterials
  std::vector<G4double> photonEnergy = { 3.20 * eV, 3.30 * eV, 3.40 * eV };

  fLAB = new G4Material("LAB", 0.86 * g/cm3, 2);
  fLAB->AddElement(nist->FindOrBuildElement("C", false), 10); 
  fLAB->AddElement(nist->FindOrBuildElement("H", false), 22); 
  std::vector<G4double> LAB_RIND = { 1.51, 1.52, 1.53 };
  std::vector<G4double> LAB_ABSL = { 12.0 * m, 12.0 * m, 12 * m };
  G4MaterialPropertiesTable* LAB_mt = new G4MaterialPropertiesTable();
  LAB_mt->AddProperty("RINDEX", photonEnergy, LAB_RIND);
  LAB_mt->AddProperty("ABSLENGTH", photonEnergy, LAB_ABSL);
  fLAB->SetMaterialPropertiesTable(LAB_mt);

  PPO = new G4Material("PPO", 1.02 * g/cm3, 4); 
  PPO->AddElement(nist->FindOrBuildElement("C", false), 15);
  PPO->AddElement(nist->FindOrBuildElement("H", false), 11);
  PPO->AddElement(nist->FindOrBuildElement("N", false), 1);
  PPO->AddElement(nist->FindOrBuildElement("O", false), 1); 
  
  G4Material* gadolinium = new G4Material("Gadolinium", 7.90 * g/cm3, 1); 
  gadolinium->AddElement(nist->FindOrBuildElement("Gd", false), 1);

  fScintillator = new G4Material("Scintillator", 0.86 * g / cm3, 3);
  G4double ppoFraction = 3.0 / (860.0 + 3.0 + 0.5);
  G4double gadoliniumFraction = 0.5 / (860.0 + 3.0 + 0.5);
  fScintillator->AddMaterial(fLAB, 1.0 - ppoFraction - gadoliniumFraction); 
  fScintillator->AddMaterial(PPO, ppoFraction);
  fScintillator->AddMaterial(gadolinium, gadoliniumFraction);

  G4MaterialPropertiesTable* Scintillator_mt = new G4MaterialPropertiesTable();

  std::vector<G4double> Scintillator_SCINT = { 0.1, 0.8, 0.1 };
  std::vector<G4double> Scintillator_RIND  = { 1.51, 1.52, 1.53 };
  std::vector<G4double> Scintillator_ABSL  = { 5.0 * m, 5.0 * m, 5.0 * m };
  Scintillator_mt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, Scintillator_SCINT);
  Scintillator_mt->AddProperty("RINDEX", photonEnergy, Scintillator_RIND);
  Scintillator_mt->AddProperty("ABSLENGTH", photonEnergy, Scintillator_ABSL);
  Scintillator_mt->AddConstProperty("SCINTILLATIONYIELD", 5000. / MeV);
  Scintillator_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  Scintillator_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.0 * ns);

  fScintillator->SetMaterialPropertiesTable(Scintillator_mt);

  // StainlessSteel
  fStainlessSteel = new G4Material("StainlessSteel", 7.92*g/cm3, 5);  
  fStainlessSteel->AddElement(nist->FindOrBuildElement("Fe", false), 70. * perCent);   
  fStainlessSteel->AddElement(nist->FindOrBuildElement("Cr", false), 17.6 * perCent);  
  fStainlessSteel->AddElement(nist->FindOrBuildElement("Ni", false), 10. * perCent);  
  fStainlessSteel->AddElement(nist->FindOrBuildElement("Mn", false), 1.6 * perCent);   
  fStainlessSteel->AddElement(nist->FindOrBuildElement("Ti", false), 0.8 * perCent); 

  // PMMA (C502H8)
  fPMMA = new G4Material("PMMA", 1.19*g/cm3, 3);
  fPMMA -> AddElement(nist->FindOrBuildElement("C", false), 5);
  fPMMA -> AddElement(nist->FindOrBuildElement("O", false), 2);
  fPMMA -> AddElement(nist->FindOrBuildElement("H", false), 8);
  std::vector<G4double> PMMA_RIND = { 1.51, 1.52, 1.53 };
  std::vector<G4double> PMMA_ABSL = { 9.0 * m, 9.0 * m, 9.0 * m };
  G4MaterialPropertiesTable* PMMA_mt = new G4MaterialPropertiesTable();
  PMMA_mt->AddProperty("RINDEX", photonEnergy, PMMA_RIND);
  PMMA_mt->AddProperty("ABSLENGTH", photonEnergy, PMMA_ABSL);
  fPMMA->SetMaterialPropertiesTable(PMMA_mt);


  // World
  G4double worldSize = 2 * m;
  auto solidWorld = new G4Box("World", worldSize, worldSize, worldSize);
  auto logicWorld = new G4LogicalVolume(solidWorld, nist->FindOrBuildMaterial("G4_AIR"), "World"); 
  auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, fCheckOverlaps);  

  // Tank  
  G4double TankH = 1858.0 * mm;
  G4double tankR = 1858.0 * mm / 2;
  auto solidTank = new G4Tubs("Tank", 0.0, tankR, TankH / 2.0, 0.0, 360 * deg);
  auto logicTank = new G4LogicalVolume(solidTank, fStainlessSteel, "Tank"); 
  auto physTank = new G4PVPlacement(nullptr, G4ThreeVector(), logicTank, "Tank", logicWorld, false, 0, fCheckOverlaps);  

  // LAB inside Tank
  G4double LABH = TankH - 2 * mm;
  G4double LABR = tankR - 2 * mm;
  auto solidLAB = new G4Tubs("LAB", 0.0, LABR, LABH / 2.0, 0.0, 360 * deg);
  auto logicLAB = new G4LogicalVolume(solidLAB, fLAB, "LAB"); 
  auto physLAB = new G4PVPlacement(nullptr, G4ThreeVector(), logicLAB, "LAB", logicTank, false, 0, fCheckOverlaps);

  // Lamps
  G4double sphereRadius = 131 * mm; 
  G4double sliceStartAngle = 0.0 * deg; 
  G4double sliceEndAngle = 46.5 * deg; 
  G4double sliceStartPhi = 0.0 * deg; 
  G4double sliceEndPhi = 360.0 * deg;

  std::vector<G4ThreeVector> vertices;
  const double phi = (1 + sqrt(5)) / 2;

  if (mode==0) {
  vertices = std::vector<G4ThreeVector>({
    G4ThreeVector(1, 1, 1),
    G4ThreeVector(-1, 1, 1),
    G4ThreeVector(1, -1, 1),
    G4ThreeVector(1, 1, -1),
    G4ThreeVector(-1, -1, 1),
    G4ThreeVector(1, -1, -1),
    G4ThreeVector(-1, 1, -1),
    G4ThreeVector(-1, -1, -1),
    G4ThreeVector(0, phi, 1 / phi),
    G4ThreeVector(0, -phi, 1 / phi),
    G4ThreeVector(0, phi, -1 / phi),
    G4ThreeVector(0, -phi, -1 / phi),
    G4ThreeVector(1 / phi, 0, phi),
    G4ThreeVector(-1 / phi, 0, phi),
    G4ThreeVector(1 / phi, 0, -phi),
    G4ThreeVector(-1 / phi, 0, -phi),
    G4ThreeVector(phi, 1 / phi, 0),
    G4ThreeVector(-phi, 1 / phi, 0),
    G4ThreeVector(phi, -1 / phi, 0),
    G4ThreeVector(-phi, -1 / phi, 0)
  });
  }

  if (mode==1){
  vertices = std::vector<G4ThreeVector>({
    G4ThreeVector(0, 1, phi),
    G4ThreeVector(0, 1, -phi),
    G4ThreeVector(0, -1, phi),
    G4ThreeVector(0, -1, -phi),
    G4ThreeVector(1, phi, 0),
    G4ThreeVector(1, -phi, 0),
    G4ThreeVector(-1, phi, 0),
    G4ThreeVector(-1, -phi, 0),
    G4ThreeVector(phi, 0, 1),
    G4ThreeVector(-phi, 0, 1),
    G4ThreeVector(phi, 0, -1),
    G4ThreeVector(-phi, 0, -1)
  });
  }
  

  G4Tubs* solidPM = new G4Tubs("PhotoMultiplier", 0.0, sphereRadius, 1.0, sliceStartPhi, sliceEndPhi);
  G4LogicalVolume* logicPM = new G4LogicalVolume(solidPM, fLAB, "PhotoMultiplier");
  logicPM->SetVisAttributes(new G4VisAttributes(G4Colour(0.992, 0.867, 0.902)));
  for(G4ThreeVector& vec : vertices){
    vec.setMag(1.0);
    new G4PVPlacement(CalcRotation(vec), vec * sensDetRad * m, logicPM, "PhotoMultiplier", logicLAB, false, 0); 
  }

  // Spheres
  G4double internalR = 0.6203505 * m;
  G4double externalR = internalR + 10 * mm;

    // PMMA container
  auto solidPMMASphere = new G4Sphere("PMMASphere", 0.0, externalR, 0., 360 * deg, 0., 180 * deg);
  auto logicPMMASphere = new G4LogicalVolume(solidPMMASphere, fPMMA, "PMMASphere");
  auto physPMMASphere = new G4PVPlacement(nullptr, G4ThreeVector(), logicPMMASphere, "PMMASphere", logicLAB, false, 0, fCheckOverlaps);
   
    // Scint (inside PMMA container)
  auto solidScint = new G4Sphere("ScintSphere", 0.0, internalR, 0., 360 * deg, 0., 180 * deg);
  auto logicScint = new G4LogicalVolume(solidScint, fScintillator, "ScintSphere");
  logicScint->SetVisAttributes(new G4VisAttributes(G4Colour(0.78, 0.988, 0.925)));
  auto physScint = new G4PVPlacement(nullptr, G4ThreeVector(), logicScint, "ScintSphere", logicPMMASphere, false, 0, fCheckOverlaps);

  return physWorld;
}

G4VPhysicalVolume* DetectorConstruction::ReinitializeGeometry() {
  auto nist = G4NistManager::Instance();

  // World
  G4double worldSize = 2 * m;
  auto solidWorld = new G4Box("World", worldSize, worldSize, worldSize);
  auto logicWorld = new G4LogicalVolume(solidWorld, nist->FindOrBuildMaterial("G4_AIR"), "World"); 
  auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, fCheckOverlaps);  

  // Tank  
  G4double TankH = 1858.0 * mm;
  G4double tankR = 1858.0 * mm / 2;
  auto solidTank = new G4Tubs("Tank", 0.0, tankR, TankH / 2.0, 0.0, 360 * deg);
  auto logicTank = new G4LogicalVolume(solidTank, fStainlessSteel, "Tank"); 
  auto physTank = new G4PVPlacement(nullptr, G4ThreeVector(), logicTank, "Tank", logicWorld, false, 0, fCheckOverlaps);  

  // LAB inside Tank
  G4double LABH = TankH - 2 * mm;
  G4double LABR = tankR - 2 * mm;
  auto solidLAB = new G4Tubs("LAB", 0.0, LABR, LABH / 2.0, 0.0, 360 * deg);
  auto logicLAB = new G4LogicalVolume(solidLAB, fLAB, "LAB"); 
  auto physLAB = new G4PVPlacement(nullptr, G4ThreeVector(), logicLAB, "LAB", logicTank, false, 0, fCheckOverlaps);

  // Lamps
  G4double sphereRadius = 131 * mm; 
  G4double sliceStartAngle = 0.0 * deg; 
  G4double sliceEndAngle = 46.5 * deg; 
  G4double sliceStartPhi = 0.0 * deg; 
  G4double sliceEndPhi = 360.0 * deg;

  std::vector<G4ThreeVector> vertices;
  const double phi = (1 + sqrt(5)) / 2;

  if (mode==0) {
  vertices = std::vector<G4ThreeVector>({
    G4ThreeVector(1, 1, 1),
    G4ThreeVector(-1, 1, 1),
    G4ThreeVector(1, -1, 1),
    G4ThreeVector(1, 1, -1),
    G4ThreeVector(-1, -1, 1),
    G4ThreeVector(1, -1, -1),
    G4ThreeVector(-1, 1, -1),
    G4ThreeVector(-1, -1, -1),
    G4ThreeVector(0, phi, 1 / phi),
    G4ThreeVector(0, -phi, 1 / phi),
    G4ThreeVector(0, phi, -1 / phi),
    G4ThreeVector(0, -phi, -1 / phi),
    G4ThreeVector(1 / phi, 0, phi),
    G4ThreeVector(-1 / phi, 0, phi),
    G4ThreeVector(1 / phi, 0, -phi),
    G4ThreeVector(-1 / phi, 0, -phi),
    G4ThreeVector(phi, 1 / phi, 0),
    G4ThreeVector(-phi, 1 / phi, 0),
    G4ThreeVector(phi, -1 / phi, 0),
    G4ThreeVector(-phi, -1 / phi, 0)
  });
  }

  if (mode==1){
  vertices = std::vector<G4ThreeVector>({
    G4ThreeVector(0, 1, phi),
    G4ThreeVector(0, 1, -phi),
    G4ThreeVector(0, -1, phi),
    G4ThreeVector(0, -1, -phi),
    G4ThreeVector(1, phi, 0),
    G4ThreeVector(1, -phi, 0),
    G4ThreeVector(-1, phi, 0),
    G4ThreeVector(-1, -phi, 0),
    G4ThreeVector(phi, 0, 1),
    G4ThreeVector(-phi, 0, 1),
    G4ThreeVector(phi, 0, -1),
    G4ThreeVector(-phi, 0, -1)
  });
  }
  

  G4Tubs* solidPM = new G4Tubs("PhotoMultiplier", 0.0, sphereRadius, 1.0, sliceStartPhi, sliceEndPhi);
  G4LogicalVolume* logicPM = new G4LogicalVolume(solidPM, fLAB, "PhotoMultiplier");
  logicPM->SetVisAttributes(new G4VisAttributes(G4Colour(0.992, 0.867, 0.902)));
  for(G4ThreeVector& vec : vertices){
    vec.setMag(1.0);
    new G4PVPlacement(CalcRotation(vec), vec * sensDetRad * m, logicPM, "PhotoMultiplier", logicLAB, false, 0); 
  }

  // Spheres
  G4double internalR = 0.6203505 * m;
  G4double externalR = internalR + 10 * mm;

    // PMMA container
  auto solidPMMASphere = new G4Sphere("PMMASphere", 0.0, externalR, 0., 360 * deg, 0., 180 * deg);
  auto logicPMMASphere = new G4LogicalVolume(solidPMMASphere, fPMMA, "PMMASphere");
  auto physPMMASphere = new G4PVPlacement(nullptr, G4ThreeVector(), logicPMMASphere, "PMMASphere", logicLAB, false, 0, fCheckOverlaps);
   
    // Scint (inside PMMA container)
  auto solidScint = new G4Sphere("ScintSphere", 0.0, internalR, 0., 360 * deg, 0., 180 * deg);
  auto logicScint = new G4LogicalVolume(solidScint, fScintillator, "ScintSphere");
  logicScint->SetVisAttributes(new G4VisAttributes(G4Colour(0.78, 0.988, 0.925)));
  auto physScint = new G4PVPlacement(nullptr, G4ThreeVector(), logicScint, "ScintSphere", logicPMMASphere, false, 0, fCheckOverlaps);

  return physWorld;
}

void DetectorConstruction::setSDPos(G4float r){
  sensDetRad = r;
}

void DetectorConstruction::ConstructSDandField()
{
  if (aTrackerSD) return;
  std::cout << aTrackerSD << " *************\n";
  G4String trackerChamberSDname = "DetectorSD";

  aTrackerSD = new SensitiveDetector(trackerChamberSDname);
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

  SetSensitiveDetector("PhotoMultiplier", aTrackerSD, true);
}