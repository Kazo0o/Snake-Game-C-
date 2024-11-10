#include <iostream>
#include <conio.h>
#include "include/game.h"
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
eDirection dir;

void Draw()
{
    system("cls");
    for (int c = 0; c < width + 2; c++)
    {
        cout << "#";
    }
    cout << endl;

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            if (c == 0)
            {
                cout << "#";
            }

            if (r == y && c == x)
            {
                cout << "O";
            }
            else if (r == fruitY && c == fruitX)
            {
                cout << "F";
            }
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == c && tailY[k] == r)
                    {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                {
                    cout << " ";
                }
            }

            if (c == width - 1)
            {
                cout << "#";
            }
        }
        cout << endl;
    }

    for (int c = 0; c < width + 2; c++)
    {
        cout << "#";
    }
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input(){

    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width)
    {
        x = 0;
    }
    else if (x < 0)
    {
        x = width - 1;
    }

    if (y >= height)
    {
        y = 0;
    }
    else if (y < 0)
    {
        y = height - 1;
    }

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            gameOver = true;
        }
    }

    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    Game snake_game;

    snake_game.Setup();
    snake_game.Run();

    /*while(!gameOver){
        Draw();
        Input();
        Logic();
        Sleep(15);
    }*/

    return 0;
}
