#ifndef DataStorage_h
#define DataStorage_h 1

#include <vector>
#include "G4AnyType.hh"


class DataStorage
{
public:
    DataStorage();
    ~DataStorage();
    void push_back(G4int n);
    std::vector<G4int> get();
    void clear();
private:
    std::vector<G4int> data;

};


#endif