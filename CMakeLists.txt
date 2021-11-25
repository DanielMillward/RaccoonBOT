cmake_minimum_required(VERSION 3.16)
project(RaccoonBOT)

set(CMAKE_CXX_STANDARD 14)

add_executable(RaccoonBOT main.cpp RaccoonTrainer.h RaccoonTrainer.cpp Node.cpp Node.h)
