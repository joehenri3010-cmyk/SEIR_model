#ifndef AGENT_H
#define AGENT_H

#include <random>
#include <vector>

// Define the class agent 
class Agent {
private:
    int x, y; // Position of the agent
    double sigma; // chance to become infected
    double gamma; // chance to recover
public:
    static std::mt19937 rng; // random number generator
    enum Compartment { SUSCEPTIBLE = 0, EXPOSED = 1, INFECTED = 2, RECOVERED = 3 }; // compartment of agent (0 = susceptible, 1 = exposed, 2 = infected, 3 = recovered)
    Compartment current_state; // current state of the agent
    Agent(int x, int y, Compartment current_state, double sigma, double gamma); // constructor
    void move(int width, int height); // function to move the agent
    Compartment getCompartment() const { return current_state; }; // function to get the compartment of the agent
    void updateCompartment(const std::vector<Agent>& other); // function to update the compartment of the agent
    int getX() const { return x; } // function to get the x position of the agent
    int getY() const { return y; } // function to get the y position of the agent    
};

#endif