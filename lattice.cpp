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
    std::vector<std::pair<int, int>> original_positions(all_agents.size());
    std::vector<std::pair<int, int>> intended_positions(all_agents.size());
    for (size_t i = 0; i < all_agents.size(); i++) {
        original_positions[i] = {all_agents[i].getX(), all_agents[i].getY()};
        all_agents[i].move(width, height); // move the agent to a new position
        intended_positions[i] = {all_agents[i].getX(), all_agents[i].getY()};
    }

    // detect conflicts
    // count how many agents intend to move to each cell
    std::vector<std::vector<int>> intended_counts(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < all_agents.size(); i++) {
        auto [ix, iy] = intended_positions[i];
        intended_counts[iy][ix]++;
    }
    //place agents
    for (size_t i = 0; i < all_agents.size(); i++) {
        auto [ix, iy] = intended_positions[i];
        auto [orig_x, orig_y] = original_positions[i];

        if (intended_counts[iy][ix] == 1) {
            grid[iy][ix] = all_agents[i]; // move the agent back to its original position
        }
        else if (intended_counts[iy][ix] > 1) { // if there is a conflict, move the agent back to its original position
            all_agents[i].setPosition(orig_x, orig_y); // move the agent back to its original position
            grid[orig_y][orig_x] = all_agents[i]; // place the agent back in the grid at its original position
        }   
    }
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