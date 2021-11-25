//
// Created by dfmil on 11/24/2021.
//

#include <random>
#include <iostream>
#include <functional>
#include "RaccoonTrainer.h"

double RaccoonTrainer::drand() {
 // Using the mersenne twister generator, generate a number from a distribution from 0 to 1
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(generator);
}
/// Runs cfr for given iterations
void RaccoonTrainer::train(int iterations) {
    //initialize card deck & utility variable
    const int NUM_CARDS = 3;
    int* cards = new int[NUM_CARDS]; // delete 26
    cards[0] = 1;
    cards[1] = 2;
    cards[2] = 3;
    double util = 0;

    for (int i = 0; i < iterations; ++i) {
    //shuffle cards
        cards = shuffleCards(cards, NUM_CARDS);
    //NOTE: p0 and p1 are just ways to figure out how to weight it

    // Train it, see exactly how good our thing is getting (util = trainer goodness)
        util += cfr(cards, "", 1, 1);
    }
    std::cout << "Average game value: " << std::to_string(util / iterations) << std::endl;
    // for each node/location in spacetime, print the strategy
    for (const auto& any : *nodeMap) {
        Node currNode = any.second;
        std::cout << currNode.toString();
    }
    delete[] cards;
}

double RaccoonTrainer::cfr(int *cards, std::string history, double p0, double p1) {
    // see who goes first (even actions = player, odd = opponent)
    int plays = history.length(); // number of actions taken THIS ITERATION (??)
    int player = plays % 2;
    int opponent = 1 - player; // either 1 or 0, whose turn it is?

    // handle end condition
    int payoffForTerminalStates = getPayoffForTerminalStates(cards, history, plays, player, opponent);
    // If iteration's over, return our payoff
    if (payoffForTerminalStates != 0) return payoffForTerminalStates;

    //Determine which node we're at (like getting a gps reading) (or something)
    std::string infoSet = std::to_string(cards[player]) + history;

    // get node if been here before, else make one
    Node node;
    try {
      node = nodeMap->at(infoSet);
    } catch (std::out_of_range) {
        //haven't seen this situation before, add to nodeMap
        node.infoSet = infoSet;
        nodeMap->insert ( std::pair<std::string, Node>(infoSet,node) );
    }

    //recursively call cfr for this iteration
    //For the node we've found ourselves at, find the strategy (like rapidly looking up a guidebook we wrote)
    //also is like rubik's cube, where we're in a state and we look up what we should do given this position
    double *strategy = node.getStrategy(player == 0? p0 : p1);
    double util[NUM_ACTIONS];
    double nodeUtil = 0;
    for (int i = 0; i < NUM_ACTIONS; ++i) {
    //Try every possible action (ignore the strategy).
        // However, since there's only 2 things we can do, condensed to just this
        std::string nextHistory = history + (i == 0? "p" : "b");
    // Find goodness of taking each action, weighted by the likelihood we take it
        if (player == 0) {
            util[i] = -cfr(cards, nextHistory, p0*strategy[i], p1);
        } else {
            util[i] = -cfr(cards, nextHistory, p0, p1*strategy[i]);
        }
    // nodeUtil says, given our current strategy, how good is this node to be in?
        // The "goodness" of this position is found by adding the goodness of future nodes * likelihood
        // we'll go that way
        nodeUtil += strategy[i] * util[i];
    }

    for (int i = 0; i < NUM_ACTIONS; ++i) {
        // regret for a path is just how good a path is - how good this node is
        double regret = util[i] - nodeUtil;
        // add the regret for taking an action, weighted by how likely we were to take it
        node.regretSum[i] += (player == 0? p1: p0) * regret;
    }
    //delete[] strategy;
    return nodeUtil;
}

int *RaccoonTrainer::shuffleCards(int *cards, int numCards) {
    for (int c1 = numCards-1; c1 > 0; --c1) {
        //pick random spot in deck, replace current card with it
        auto c2 = irand(c1);
        int temp = cards[c1];
        cards[c1] = cards[c2];
        cards[c2] = temp;
    }
    return cards;
}

int RaccoonTrainer::irand(int limit) {
    std::uniform_int_distribution<int> newdis(0,limit);
    return newdis(generator);
}

int RaccoonTrainer::getPayoffForTerminalStates(int *cards, std::string history, int plays, int player, int opponent) {
    if (plays > 1) {
    // if the last thing that happened was a pass, iteration's over
        bool terminalPass = history.at(plays-1) == 'p';
    // if last two actions were both bets, iteration's over
        bool doubleBet = history.substr(plays-2, plays) == "bb";
    // check if our card's better
        bool isPlayerCardHigher = cards[player] > cards[opponent];
    // Determine outcome for both players if there was a pass
        if (terminalPass) {
            // If both players just passed
            if (history == "pp") {
                return isPlayerCardHigher ? 1 : -1;
            } else {
                //It was just us maybe? or the opponent
                return 1;
            }
    // Determine outcome for both players if it was a doubleBet
        } else if (doubleBet) {
            //raise results
            return isPlayerCardHigher ? 2 : -2;
        }
    }
    //If game's still in play, return 0
    return 0;
}


