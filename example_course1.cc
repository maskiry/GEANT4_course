//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4OpticalPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(0);
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  physicsList->RegisterPhysics(opticalPhysics);

  // auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  G4RunManager* runManager = new G4RunManager;
  runManager->SetUserInitialization(physicsList);
  
  ActionInitialization* actionInit = new ActionInitialization;
  PrimaryGeneratorAction* gen = new PrimaryGeneratorAction();
  actionInit->RegisterPrimaryGeneratorAction(gen);
  G4String file_path = "../data/data.csv";
  RunAction* runAction = new RunAction(file_path);
  actionInit->RegisterRunAction(runAction);
  runManager->SetUserInitialization(actionInit);

  runManager->SetUserInitialization(new DetectorConstruction());

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
  UImanager->ApplyCommand("/control/execute init_vis.mac");
  UImanager->ApplyCommand("/vis/scene/endOfEventAction refresh");

  std::ofstream file;
  file.open(file_path);
  file.close();

  //---------------e- with energy 1 MeV---------------
  for (float x_pos=-0.9; x_pos<1; x_pos+=0.1){
    file.open(file_path, std::ios::app);
    file << "e-_x_"+std::to_string(x_pos)+",";
    file.close();
    G4String command = "/gun/position " + std::to_string(x_pos) +" 0 0 m"; 
    UImanager->ApplyCommand("/gun/particle e-");
    UImanager->ApplyCommand("/gun/energy 1 MeV");
    UImanager->ApplyCommand(command);
    UImanager->ApplyCommand("/run/beamOn 50");
  }

  for (float y_pos=-0.9; y_pos<1; y_pos+=0.1){
    file.open(file_path, std::ios::app);
    file << "e-_y_"+std::to_string(y_pos)+",";
    file.close();
    G4String command = "/gun/position 0 " + std::to_string(y_pos) +" 0 m"; 
    UImanager->ApplyCommand("/gun/particle e-");
    UImanager->ApplyCommand("/gun/energy 1 MeV");
    UImanager->ApplyCommand(command);
    UImanager->ApplyCommand("/run/beamOn 50");
  }

  for (float z_pos=-0.9; z_pos<1; z_pos+=0.1){
    file.open(file_path, std::ios::app);
    file << "e-_z_"+std::to_string(z_pos)+",";
    file.close();
    G4String command = "/gun/position 0 0 " + std::to_string(z_pos) +" m"; 
    UImanager->ApplyCommand("/gun/particle e-");
    UImanager->ApplyCommand("/gun/energy 1 MeV");
    UImanager->ApplyCommand(command);
    UImanager->ApplyCommand("/run/beamOn 50");
  }

  gen->setMode(4);
  file.open(file_path, std::ios::app);
  file << "e-_uniform_,";
  file.close();
  UImanager->ApplyCommand("/gun/particle e-");
  UImanager->ApplyCommand("/gun/energy 1 MeV");
  UImanager->ApplyCommand("/gun/position 0 0 0 m");
  UImanager->ApplyCommand("/run/beamOn 50");

  //---------------e+ with energy 3 MeV---------------
  gen->setMode(1);
  for (float x_pos=-0.9; x_pos<1; x_pos+=0.1){
    file.open(file_path, std::ios::app);
    file << "e+_x_"+std::to_string(x_pos)+",";
    file.close();
    G4String command = "/gun/position " + std::to_string(x_pos) +" 0 0 m"; 
    UImanager->ApplyCommand("/gun/particle e+");
    UImanager->ApplyCommand("/gun/energy 3 MeV");
    UImanager->ApplyCommand(command);
    UImanager->ApplyCommand("/run/beamOn 50");
  }

  for (float y_pos=-0.9; y_pos<1; y_pos+=0.1){
    file.open(file_path, std::ios::app);
    file << "e+_y_"+std::to_string(y_pos)+",";
    file.close();
    G4String command = "/gun/position 0 " + std::to_string(y_pos) +" 0 m"; 
    UImanager->ApplyCommand("/gun/particle e+");
    UImanager->ApplyCommand("/gun/energy 3 MeV");
    UImanager->ApplyCommand(command);
    UImanager->ApplyCommand("/run/beamOn 50");
  }

  for (float z_pos=-0.9; z_pos<1; z_pos+=0.1){
    file.open(file_path, std::ios::app);
    file << "e+_z_"+std::to_string(z_pos)+",";
    file.close();
    G4String command = "/gun/position 0 0 " + std::to_string(z_pos) +" m"; 
    UImanager->ApplyCommand("/gun/particle e+");
    UImanager->ApplyCommand("/gun/energy 3 MeV");
    UImanager->ApplyCommand(command);
    UImanager->ApplyCommand("/run/beamOn 50");
  }
  gen->setMode(4);
  file.open(file_path, std::ios::app);
  file << "e+_uniform_,";
  file.close();
  UImanager->ApplyCommand("/gun/particle e+");
  UImanager->ApplyCommand("/gun/energy 3 MeV");
  UImanager->ApplyCommand("/gun/position 0 0 0 m");
  UImanager->ApplyCommand("/run/beamOn 50");

  
  ui->SessionStart();

  delete ui;
  delete visManager;
  delete runManager;
  return 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
