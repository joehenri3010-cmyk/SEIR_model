#include "lattice.h"
#include "agent.h"

Lattice::Lattice(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<std::optional<Agent>>(width));
}

// lattice.cpp
void Lattice::update() {
    // collect all current agents
    std::vector<Agent> all_agents;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j].has_value()) {
                all_agents.push_back(grid[i][j].value());
            }
        }
    }

    // clear the grid
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j].reset();
        }
    }

    // update and replace each agent
    for (auto& agent : all_agents) {
        agent.move(width, height);         // move agent
        agent.updateCompartment(all_agents); // update state based on neighbours
        
        // place agent in new position
        int nx = agent.getX();
        int ny = agent.getY();
        grid[ny][nx] = agent;
    }
}

void Lattice::addAgent(int x, int y, Agent::Compartment state, double sigma, double gamma) {
    // Add an agent to the lattice at the specified position and compartment
    grid[y][x] = Agent(x, y, state, sigma, gamma);
}



