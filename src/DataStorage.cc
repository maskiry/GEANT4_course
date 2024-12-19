#include "DataStorage.hh"
#include <iostream>


DataStorage::DataStorage(){
};

DataStorage::~DataStorage(){
};

void DataStorage::push_back(G4int n){
    data.push_back(n);
}

std::vector<G4int> DataStorage::get(){
    return data;
}

void DataStorage::clear(){
    if (data.size())
        data.clear();
}