cmake_minimum_required(VERSION 3.15)
project(PokerEngine)

find_package(pybind11 REQUIRED)

pybind11_add_module(poker_env 
    bindings.cpp 
    card.cpp 
    deck.cpp 
    evaluator.cpp
)