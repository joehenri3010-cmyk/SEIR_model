#include "lattice.h"
#include "agent.h"

Lattice::Lattice(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<std::optional<Agent>>(width));
}

void Lattice::update() {
    // Update the lattice with the new position and compartment of the agents
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j].has_value()) {
                grid[i][j]->updatelattice(*this);
            }
        }
    }
}

void Lattice::addAgent(int x, int y, Agent::Compartment state, double sigma, double gamma) {
    // Add an agent to the lattice at the specified position and compartment
    grid[y][x] = Agent(x, y, state, sigma, gamma);
}



