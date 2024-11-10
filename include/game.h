#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include <vector>
#include <string>

#define SUCCESS 0
static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

enum eDirection
{
  STOP = 0,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// All our game logic goes into here
class Game
{
public:
  Game(); // uses defaults
  Game(int width, int height);

  void Run();
  void Setup();
  int Init();
  void Logic();
  void Draw();
  void DrawFruit(sf::CircleShape &fruit);
  void wallWarping();
  // getters
  bool isPaused() const;
  bool isGameOver() const;

  // setters
  void setPause(bool pause);
  void setGameOver(bool gameover);
  void setWindowSize(int w, int h);
  
private:
  void generateRandomFruitSpawnPoint(sf::CircleShape &v);
  bool enforceVectorRange(sf::Vector2f &v, float MIN, float MAX);
  
  void Scoring();
  bool _gameOver;
  bool _paused;
  int _width, _height;
  eDirection _direction = STOP;
  int _score;
  sf::CircleShape _snakeHead;
  // figure out why we have a collections of tails
  std::vector<sf::CircleShape> _tail;
  sf::CircleShape _fruit;
  sf::RenderWindow _window;
  sf::ContextSettings _settings; // this will be used for Anti/Aliasing
  int _speed;
};
