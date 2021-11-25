//
// Created by dfmil on 11/24/2021.
//

#ifndef RACCOONBOT_RACCOONTRAINER_H
#define RACCOONBOT_RACCOONTRAINER_H

#include <map>
#include <random>
#include "Node.h"
class RaccoonTrainer {
private:
    std::mt19937 generator;
public:
    const int PASS = 0, BET = 1, NUM_ACTIONS = 2;
    std::map<std::string, Node> *nodeMap = new std::map<std::string, Node>();
    RaccoonTrainer() {
        std::mt19937 generator (time(0));
    }
    double drand();
    int irand(int limit);
    void train(int iterations);
    int* shuffleCards(int* cards, int numCards);
    double cfr(int* cards, std::string history, double p0, double p1);
    int getPayoffForTerminalStates(int*cards, std::string history, int plays, int player, int opponent);
};

#endif //RACCOONBOT_RACCOONTRAINER_H
