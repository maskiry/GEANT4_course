#include "G4AnalysisManager.hh"
#include "RunAction.hh"

#include<fstream>


RunAction::RunAction(){
    hist = new DataStorage();
}

RunAction::RunAction(G4String file){
    file_path = file;
    hist = new DataStorage();
}

void RunAction::BeginOfRunAction(const G4Run *run){
    hist->clear();
    // std::ofstream file;
    // file.open("../data/energy.csv");
    // file.close();
}

void RunAction::EndOfRunAction(const G4Run *run){
    std::ofstream file;
    file.open(file_path, std::ios::app);
    for (auto c: hist->get()){
        file << c << ",";
    }
    file << "\n";
    file.close();
}

void RunAction::addCount(G4int cnt) const{
    hist->push_back(cnt);
}

RunAction::~RunAction(){
    delete hist;
}