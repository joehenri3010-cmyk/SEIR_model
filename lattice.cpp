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

        // store original position in case the new position is occupied
        int orig_x = agent.getX();
        int orig_y = agent.getY();

        // move agent to new position
        agent.move(width, height);

        // find neighbors for the agent within a 1 cell radius
        std::vector<Agent> neighbors;
        for (const auto &other : all_agents) {
            if (other.getX() >= agent.getX() - 1 && other.getX() <= agent.getX() + 1 && // check if other agent is within 1 cell radius
                other.getY() >= agent.getY() - 1 && other.getY() <= agent.getY() + 1) { // check if other agent is within 1 cell radius
                    neighbors.push_back(other);
                }
            }
        agent.updateCompartment(neighbors); // update the compartment of the agent based on its neighbors   

        // place agent in new position
        int nx = agent.getX();
        int ny = agent.getY();

        if (!grid[ny][nx].has_value()) { // only place the agent if the new position is empty
            grid[ny][nx] = agent;
        }
        else {
            agent.setPosition(orig_x, orig_y); // if the new position is occupied, keep the agent in its current position
            grid[orig_y][orig_x] = agent; // place the agent back in its original position
        }
    }
}

void Lattice::addAgent(int x, int y, Agent::Compartment state, double sigma, double gamma) {
    // Add an agent to the lattice at the specified position and compartment
    grid[y][x] = Agent(x, y, state, sigma, gamma);
}



