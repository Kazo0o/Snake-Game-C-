#include "../include/game.h"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>

Game::Game() : Game(WINDOW_WIDTH, WINDOW_HEIGHT)
{
  Setup();
  Init();
}

Game::Game(int width, int height)
{
  Setup();
  setWindowSize(width, height);
  Init();
}
/**
 *@brief This function Initialises the game window the game will run
 * @return void
 */
int Game::Init()
{
  // = sf::Window();
  _settings.antialiasingLevel = 4;
  _window.create(sf::VideoMode(_width, _height), "Snake-Game-C", sf::Style::Default, _settings);
  _window.setVerticalSyncEnabled(true);
  _window.setFramerateLimit(60); // capping our framerate at 60

  return SUCCESS;
}
void Game::Setup()
{
  _gameOver = false;
  _paused = false;
  _direction = RIGHT;
  _speed=5;
  
  // setting up the snake head
  _snakeHead.setPointCount(4.0f); // make a circle a diamond in sfml
  int x = WINDOW_WIDTH / 2;
  int y = WINDOW_HEIGHT / 2;

    sf::Color snakeColor(0, 255, 0);
  _snakeHead.setRadius(20.f);
  _snakeHead.setFillColor(snakeColor);
  _snakeHead.setPosition(x, y);
  
  //First tail value we have
  _tail.reserve(100);
  _tail.emplace_back(_snakeHead);
  // Setting up my fruits
  _fruit.setRadius(20.f);
  _fruit.setOutlineColor(sf::Color::Black);
  _fruit.setOutlineThickness(0.8f);
  _fruit.setFillColor(sf::Color::Red);


  //left corner of the circle is x. the bottom the cricle is s
  //so to get to the right side (edge-diameter)
  std::cout<<"x->"<<_fruit.getPosition().x<<"y->"<<_fruit.getPosition().y<<std::endl;
  std::cout<<"Screen Width -> "<<_width<<"Screen_height -> "<<_height<<std::endl;
  std::cout<<"Snake Head Location x "<<_snakeHead.getPosition().x<<std::endl;
  generateRandomFruitSpawnPoint(_fruit);
  _score = 0;
}

/*
 * @brief This function is responsible for running the game loop
 * @param window is the window in context that we've opened
 */
void Game::Run()
{
  // Initialise a window
  while (_window.isOpen())
  {
    sf::Event event;

    while (_window.pollEvent(event))
    {
        switch(event.type){
          case sf::Event::Closed:
            _window.close();
            break;
          case sf::Event::KeyPressed:
            //siwtching between the Keyboard inputs
            switch(event.key.code){
              case sf::Keyboard::A:
              case sf::Keyboard::Left:
                _direction = eDirection::LEFT;
                
                std::cout<<"A\n";
                break;
              case sf::Keyboard::W:
              case sf::Keyboard::Up:
                _direction = eDirection::UP;
              std::cout<<"W\n";
                break;
              case sf::Keyboard::D:
              case sf::Keyboard::Right:
                _direction = eDirection::RIGHT;
                std::cout<<"D\n";
                break;
              case sf::Keyboard::S:
              case sf::Keyboard::Down:
                _direction = eDirection::DOWN;
                std::cout<<"S\n";
                break;
            case sf::Keyboard::P:
                std::cout<<"Pause "<<_paused;
                setPause(!_paused);
                std::cout<<" paused "<<!_paused<<std::endl;
                break;  
            case sf::Keyboard::Escape:
                _window.close();
              default:
                //ignore all other possible keyboard events
                break;
          }
        default:
          //ignore all other possible events
          break;
      }
    }
    
    Logic();
    // First Clear the screen
    _window.clear();
    Draw();
    // Update the window
    _window.display();
  }

  // default behaviour is that the Escape key closes the window

  // Drawing starts here with this method or else you window will leak and not respond
}

void Game::Logic()
{
  sf::Vector2f prevPos;

  // This should make it easier to work with the position of the snake head
  int snakeX = _snakeHead.getPosition().x;
  int snakeY = _snakeHead.getPosition().y;

  prevPos.x = _tail.at(0).getPosition().x;
  prevPos.y = _tail.at(0).getPosition().y;

  sf::Vector2f prevPos2; // previous previous position
  // we start off the head at the head
  _tail.at(0).setPosition(snakeX, snakeY);

  for (auto it = _tail.begin() + 1; it != _tail.end(); ++it)
  {
    // This should move the 2nd last frame to make it the last frame
    prevPos2.x = (*it).getPosition().x;
    prevPos2.y = (*it).getPosition().y;

    // this should set the current frame position of tail to the last previous position
    (*it).setPosition(prevPos.x, prevPos.y);

    prevPos = prevPos2;
  }

  switch (_direction)
  {
    // Where no incrementing or decrementing means position doesnt
    // change in that direction
  case LEFT:
    _snakeHead.setPosition(--snakeX, snakeY);
    break;
  case RIGHT:
    _snakeHead.setPosition(++snakeX, snakeY);
    break;
  case UP:
    _snakeHead.setPosition(snakeX, --snakeY);
    break;
  case DOWN:
    _snakeHead.setPosition(snakeX, ++snakeY);
    break;
  default:
    // Do Nothing here at all
    break;
  }

  wallWarping();
  Scoring();  //execute the scoring system
}

// no need to copy these values, they're already references to the snake head location
//
void Game::wallWarping()
{
  sf::Vector2f position =_snakeHead.getPosition();
  float x= position.x;
  float y = position.y;
  float diameter=2*_snakeHead.getRadius();

  if (x >= _width-diameter)
  {
    //x = 0;
    _snakeHead.setPosition(0,y);
  }
  else if (x < 0)
  {
    //x = _width - diameter
    _snakeHead.setPosition(_width-diameter,y);
  }

  if (y >= _height-diameter)
  {
    //y = 0;
    _snakeHead.setPosition(x,0);
  }
  else if (y < 0)
  {
    //y = _height -diameter;
    _snakeHead.setPosition(x,_height-diameter);
  }
}

void Game::Scoring()
{ 
  for(auto iterator=_tail.begin();iterator!=_tail.end();iterator++){
    
    if((*iterator).getPosition().x==_snakeHead.getPosition().x &&
      (*iterator).getPosition().y==_snakeHead.getPosition().y)
    {
      _gameOver=true;
    }
  }

  if (_fruit.getPosition().x == _snakeHead.getPosition().x &&
      _fruit.getPosition().y == _snakeHead.getPosition().y)
  {
    _score += 10;
    // Randomly generate another fruit
    generateRandomFruitSpawnPoint(_fruit);
    // add a tail
    _tail.emplace_back(sf::CircleShape(20.f, 2.0f)); // we're not copying into the vector
  //Adding speed to the snake
    int speedX=_snakeHead.getPosition().x;
    int speedY=_snakeHead.getPosition().y;

    _snakeHead.setPosition(speedX+_speed,speedY+_speed);
  }
 
}
void Game::Draw()
{
  // Draw the fruit first
  // generateRandomFruitSpawnPoint(_fruit);
  _window.draw(_fruit); // draws an apple
  _window.draw(_snakeHead); //draws the snake tail

  for(auto iterator=_tail.begin();iterator!=_tail.end();iterator++){
    _window.draw((*iterator));
  }


}
bool Game::enforceVectorRange(sf::Vector2f &v, float MIN, float MAX)
{
  return true; // TODO LATER
}

void Game::generateRandomFruitSpawnPoint(sf::CircleShape &v)
{
  //result = random num [(max-min)+min]
  //where max -> _width-diameter
  //      min -> _height-diameter
  int maxX =_width-2*v.getRadius();
  int maxY = _height-2*v.getRadius();
  float x = (std::rand()%(maxX-0+1))+0;
  float y = (std::rand()%(maxY-0+1))+0; 
   
  v.setPosition(x,y);

}
///////////////////////////////////////
//    GETTTERS                                  //
//////////////////////////////////////
bool Game::isGameOver() const
{
  return _gameOver;
};
////////////////////////////////////////////
//  SETTERS                                               //
///////////////////////////////////////////
void Game::setWindowSize(int w, int h)
{
  _width = w;
  _height = h;
}
void Game::setGameOver(bool gameover)
{
  _gameOver = gameover;
}

void Game::setPause(bool pause)
{
  _paused = pause;
}
