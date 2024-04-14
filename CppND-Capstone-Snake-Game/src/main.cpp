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

  std::string username;

  std::cout << "Enter username: ";
  std::getline(std::cin, username);

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "User: " << username << "\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  // save results to file
  std::string data = "user: " + username + ", score: " + std::to_string(game.GetScore()) + ", size: " + std::to_string(game.GetSize()) + "\n";
  std::ofstream file("./log.txt", std::ios::app);
  if (!file) {
      std::cerr << "Unable to open file for writing.\n";
      return -1;
  }
  file << data;
  file.close();

  return 0;
}