#ifndef FActionInitialization_h
#define FActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Обязательный класс, который должен быть объявлен в проекте Geant4
/// Имя класса может быть другим, и он должен наследоваться от
/// класса G4VUserActionInitialization

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();//Конструктор
    virtual ~ActionInitialization();//Деструктор
    virtual void Build() const;//Создание источника первичных частиц
    void RegisterRunAction(G4UserRunAction* run_act);
    void RegisterPrimaryGeneratorAction(G4VUserPrimaryGeneratorAction* gen);
private:
  G4UserRunAction* userRunAction;
  G4VUserPrimaryGeneratorAction* userPrimaryGenerator;
};

#endif