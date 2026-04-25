#include <agent.h>
#include <lattice.h>

Agent::Agent(int x, int y, Compartment current_state, double sigma, double gamma) : x(x), y(y), current_state(current_state), sigma(sigma), gamma(gamma) {}

void Agent::move() {
    // Randomly move the agent to one of the 8 neighboring positions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1, 1);
    
    x += dis(gen);
    y += dis(gen);

    // wrap around the edges of the lattice
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= lattice.getWidth()) x = lattice.getWidth() - 1;
    if (y >= lattice.getHeight()) y = lattice.getHeight() - 1;

}

void Agent::updateCompartment(const Agents& other) {
    // If the agent is susceptible and has at least one infected neighbor, it becomes exposed
    if (current_state == SUSCEPTIBLE) {
        for (const auto& neighbor : other) {
            if (neighbor.isInfected()) {
                current_state = EXPOSED;
                break;
            }
        }
    }
    // If the agent is exposed, it becomes infected
    else if (current_state == EXPOSED) {
        // add a random chance for the agent to become infected
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        if (dis(gen) < sigma ) { // sigma is the chance to become infected
            current_state = INFECTED;
        }
    }
    // If the agent is infected, it recovers
    else if (current_state == INFECTED) {
        // add a random chance for the agent to recover
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        if (dis(gen) < gamma ) { // gamma is the chance to recover
            current_state = RECOVERED;
        }
    }
}


