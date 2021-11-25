//
// Created by dfmil on 11/24/2021.
//

#include "Node.h"

/// Normalizes/positivizes/cleans up the (cumulative?) regrets to give a semi-valid strategy (sum != 1)
/// \param realizationWeight How much you want this to compare to other strategies in strategySum (normally 1)
double *Node::getStrategy(double realizationWeight) {
    double normalizingSum = 0;
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        // if there's a positive value for regret for that action already, set it to that otherwise it's 0.
        strategy[i] = regretSum[i] > 0 ? regretSum[i] : 0;
        normalizingSum += strategy[i];
    }
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        // normalize it
        if (normalizingSum > 0) {
            strategy[i] /= normalizingSum;
        } else {
            //if no regret, make it basically random
            strategy[i] = 1.0 / NUM_ACTIONS;
        }
        strategySum[i] += realizationWeight * strategy[i];
    }
    return strategy;
}

/// Normalizes/positivizes/cleans up the TOTAL ADDED CUMULATIVE strategies into a usable one
double *Node::getAverageStrategy() {
    double *avgStrategy = new double[NUM_ACTIONS]; // delete 49
    double normalizingSum = 0;
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        normalizingSum += strategySum[i];
    }
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        if (normalizingSum > 0) {
            avgStrategy[i] = strategySum[i] / normalizingSum;
        } else {
            avgStrategy[i] = 1.0 / NUM_ACTIONS;
        }
    }
    return avgStrategy;
}

std::string Node::toString() {
    double *avgStrat = getAverageStrategy();
    std::string outstring = infoSet + ":\n" + "Pass: " + std::to_string(avgStrat[0]) + ",  Bet: " + std::to_string(avgStrat[1]) + "\n";
    delete[] avgStrat;
    return  outstring;
}
