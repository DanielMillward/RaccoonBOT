#include <iostream>
#include "RaccoonTrainer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    int iterations = 1000000;
    RaccoonTrainer trainer;
    trainer.train(iterations);
    return 0;
}
