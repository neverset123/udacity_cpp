#include <iostream>
#include <fstream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  constexpr std::size_t numOfLife{3};

  std::string username;
  std::string username_2;
  std::string mode;

  std::cout << "Please select player mode(s for single player, d for double player, ai for ai opponent): ";
  std::getline(std::cin, mode);
  if(mode == "s" || mode == "ai")
  {
    std::cout << "Please enter username: ";
    std::getline(std::cin, username);
  }
  else if(mode == "d")
  {
    std::cout << "Please enter username 1: ";
    std::getline(std::cin, username);
    std::cout << "Please enter username 2: ";
    std::getline(std::cin, username_2);
  }
  else{
    std::cout << "invalid mode was selected: " << mode << "\n";
    return -1;
  }


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, kScreenWidth, kScreenHeight, numOfLife, mode);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";

  // save results to file
  std::ofstream file("./log.txt", std::ios::app);
  if (!file) {
    std::cerr << "Unable to open file for writing.\n";
    return -1;
  }
  std::string data = "user: " + username + ", score: " + std::to_string(game.GetScore(0)) + ", size: " + std::to_string(game.GetSize(0)) + "\n";
  std::cout << data;
  file << data;
  if(mode == "d")
  {
    std::string data_2 = "user: " + username_2 + ", score: " + std::to_string(game.GetScore(1)) + ", size: " + std::to_string(game.GetSize(1)) + "\n";
    std::cout << data_2;
    file << data_2;
  }

  file.close();
  return 0;
}