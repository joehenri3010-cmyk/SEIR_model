#ifndef AGENT_H
#define AGENT_H

#include <random>

// Define the class agent 
class Agent {
private:
    int x, y; // Position of the agent
    enum Compartment { SUSCEPTIBLE = 0, EXPOSED = 1, INFECTED = 2, RECOVERED = 3 }; // compartment of agent (0 = susceptible, 1 = exposed, 2 = infected, 3 = recovered)
    Compartment current_state; // current state of the agent
public:
    Agent(int x, int y, Compartment current_state); // constructor
    void move(); // function to move the agent
    bool isInfected(); // function to check if the agent is infected 
    Compartment getCompartment() const { return current_state; }; // function to get the compartment of the agent
    void updateCompartment(const Agents& other); // function to update the compartment of the agent
    int getX() const { return x; } // function to get the x position of the agent
    int getY() const { return y; } // function to get the y position of the agent    
};

#endif