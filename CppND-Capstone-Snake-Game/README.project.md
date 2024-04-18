## Instructions
build and run:
```
mkdir build && cd build
cmake .. && make
./SnakeGame
```

## new features
1. user could type in their name before starting the game, and game results(username, score, size) will be logged in log.text file
2. add fodd type attribute and snake motion strategy after eating the food.
3. add life counter in snake
4. add single play, multiplayer and ai agent Mode, ai agent opponent is controlled by a star algorithm
5. add food placing multithreads depending on food type

## rubric points
### Loops, Functions, I/O
1. The project accepts user input and processes the input in main.cpp line 20-37
2. the program writes data to a file in main.cpp line 47-62.

### Object Oriented Programming
1. One class Food in food.cpp and food.h are added to the project with appropriate access specifiers for class members.
2. Class constructors utilize member initialization lists in food.h line 13-14.
3. Class Food abstracts implementation details from their interfaces in food.h

### Memory Management
1. The project uses destructors appropriately in game.cpp line 12-15.
2. The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate in game.cpp line 5-10.
3. The project uses smart pointers instead of raw pointers in game.h line 47 50 61.

### Concurrency
1. The project uses multithreading in game.cpp line 25 26
2. A mutex or lock is used in the project in game.cpp line 88-113

