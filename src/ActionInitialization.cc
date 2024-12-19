#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"



ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{G4cout << "Creating ActInit" << std::endl;}

ActionInitialization::~ActionInitialization()
{    G4cout << "Deleting ActionInit" << std::endl;}

void ActionInitialization::Build() const
{
    SetUserAction(userPrimaryGenerator);
    SetUserAction(userRunAction);
}

void ActionInitialization::RegisterRunAction(G4UserRunAction* run_act) 
{
   userRunAction = run_act; 
}

void ActionInitialization::RegisterPrimaryGeneratorAction(G4VUserPrimaryGeneratorAction* gen){
  userPrimaryGenerator = gen;
}