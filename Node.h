//
// Created by dfmil on 11/24/2021.
//

#ifndef RACCOONBOT_NODE_H
#define RACCOONBOT_NODE_H


#include <string>

class Node {
public:
    std::string infoSet;
    int NUM_ACTIONS = 2;
    double *regretSum = new double[NUM_ACTIONS];
    double *strategy = new double[NUM_ACTIONS]; // a temporary strategy thing
    double *strategySum = new double[NUM_ACTIONS]; // adding all the strategies together
    double* getStrategy(double realizationWeight);
    double* getAverageStrategy();
    std::string toString();
};


#endif //RACCOONBOT_NODE_H
