cmake_minimum_required(VERSION 3.10)
project(Assignment7)

set(CMAKE_CXX_STANDARD 11)

add_executable(poker main.cpp CardPile.cpp Card.cpp Deck.cpp Hand.cpp Game.cpp)