## Instructions
build and run:
```
mkdir build && cd build
cmake .. && make
./SnakeGame
```

## new features
1. user could type in their name before starting the game, and game results(username, score, size) will be logged in log.text file
2. add Food class to handle food generation and update
3. Food and Snake object in game class are changed to pointer and smart pointer. Food resource is allocated in initializaiton list and destroyed in destructor.
4. a thread for checking if score has reached multiplies of 10 consistently, and congratulation result is shown on console.
