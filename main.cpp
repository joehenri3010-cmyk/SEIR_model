#include <iostream>
#include <vector>
#include "lattice.h"
#include "agent.h"

int main() {
    // simulation parameters
    int width = 100;
    int height = 100;
    int total_agents = 200;
    int initial_infected = 5;
    int time_steps = 20;
    double sigma = 0.3; // rate of exposed to infected transition
    double gamma = 0.1; // rate of infected to recovered transition

    // initialise lattice
    Lattice lattice(width, height);

    // Initialize agents
    for (int i = 0; i < total_agents-initial_infected; ++i) {
        lattice.addAgentRandom(Agent::SUSCEPTIBLE, sigma, gamma);
    }
    // Initialize infected agents
    for (int i = 0; i < initial_infected; ++i) {
        lattice.addAgentRandom(Agent::INFECTED, sigma, gamma);
    }
    // Run the simulation for a number of steps
    for (int step = 0; step < time_steps; step++) {
        std::cout << "Step " << step << ":\n";
        int susceptible_count = 0, exposed_count = 0, infected_count = 0, recovered_count = 0;
        const auto& grid = lattice.getGrid();
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                if (cell.has_value()) {
                    switch (cell->getCompartment()) {
                        case Agent::SUSCEPTIBLE:
                            susceptible_count++;
                            break;
                        case Agent::EXPOSED:
                            exposed_count++;
                            break;
                        case Agent::INFECTED:
                            infected_count++;
                            break;
                        case Agent::RECOVERED:
                            recovered_count++;
                            break;
                    }  
                }
            }
        }
        // Print counts
        int total_count = susceptible_count + exposed_count + infected_count + recovered_count; 

        std::cout << "S: " << susceptible_count 
                      << " E: " << exposed_count 
                      << " I: " << infected_count 
                      << " R: " << recovered_count
                      << " Total: " << total_count;
        std::cout << "\n";
        // Update the lattice for the next time step
        lattice.update();
    }
    return 0;
}