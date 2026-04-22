#ifndef LATTICE_H
#define LATTICE_H

#include "agent.h"
#include <vector>

class Lattice {
private:
    int width, height;
    std::vector<std::vector<std::optional<Agent>>> grid;
public:
    Lattice(int width, int height);
    void update();
    void addAgent(int x, int y, Agent::Compartment state);
    const std::vector<std::vector<std::optional<Agent>>>& getGrid() const { return grid; }
};

#endif