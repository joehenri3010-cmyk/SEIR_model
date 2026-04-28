#include "lattice.h"
#include "agent.h"
#include <iostream>

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

    // store positions and move all agents to new positions
    int skipped = 0; // DEBUG: track skipped agents
    for (auto& agent : all_agents) {

        // store original position in case the new position is occupied
        int orig_x = agent.getX();
        int orig_y = agent.getY();

        // move agent to new position
        agent.move(width, height);

        int nx = agent.getX();
        int ny = agent.getY();

       if (!grid[ny][nx].has_value()) {
           grid[ny][nx] = agent;
        }
        else if (!grid[orig_y][orig_x].has_value()) { // if the new position is occupied and the original position is empty, move the agent back to its original position
            agent.setPosition(orig_x, orig_y);
            grid[orig_y][orig_x] = agent;
        } else {
            // if both the new position and original position are occupied, skip moving this agent for this update
            // debug start
            skipped++;
            std::cout << "SKIPPED: (" << orig_x << "," << orig_y << ") -> (" << nx << "," << ny << ")\n";
            // debug finish
        }
    }
    std::cout << "Total skipped agents: " << skipped << "\n"; // debugging output to track skipped agents
     // recollect all current agents after move to update their compartments based on their new positions
    all_agents.clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j].has_value()) {
                all_agents.push_back(grid[i][j].value());
            }
        }
    }

    // find neighbors for the agent within a 1 cell radius
    for (auto& agent : all_agents) {
        std::vector<Agent> neighbors;
        for (const auto &other : all_agents) {
            int dx = std::abs(agent.getX() - other.getX());
            int dy = std::abs(agent.getY() - other.getY());

            // account for wrap-around distance
            dx = std::min(dx, width - dx);
            dy = std::min(dy, height - dy);

            if (dx <= 1 && dy <= 1) { // if the other agent is within a 1 cell radius
                neighbors.push_back(other);
            }
        }
        agent.updateCompartment(neighbors); // update the compartment of the agent based on its neighbors  
        grid[agent.getY()][agent.getX()] = agent; // place the agent back in the grid with its updated compartment 
    }
}

void Lattice::addAgent(int x, int y, Agent::Compartment state, double sigma, double gamma) {
    // Add an agent to the lattice at the specified position and compartment
    grid[y][x] = Agent(x, y, state, sigma, gamma);
}

void Lattice::addAgentRandom(Agent::Compartment state, double sigma, double gamma) {
    std::uniform_int_distribution<int> disx(0, width - 1); // assuming lattice size is width x height
    std::uniform_int_distribution<int> disy(0, height - 1);
    int x, y;
    do {
        x = disx(Agent::rng);
        y = disy(Agent::rng);
    } while (grid[y][x].has_value()); // keep generating random positions until an empty cell is found
    grid[y][x] = Agent(x, y, state, sigma, gamma); // add the agent to the lattice at the random position
}


