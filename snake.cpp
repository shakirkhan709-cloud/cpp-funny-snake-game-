#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

int width = 40, height = 20;
int x, y, foodX, foodY, score, speed = 100;
vector<pair<int, int>> tail;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;

void Setup()
{
    dir = STOP;
    x = width / 2;
    y = height / 2;
    foodX = rand() % (width - 2) + 1;
    foodY = rand() % (height - 2) + 1;
    score = 0;
    tail.clear();
    gameOver = false;
}

void Draw()
{
    system("cls");
    for (int i = 0; i < width; i++) cout << "*";
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0 || j == width - 1)
                cout << "*";
            else if (i == y && j == x)
                cout << "@";
            else if (i == foodY && j == foodX)
                cout << ".";
            else
            {
                bool printed = false;
                for (auto segment : tail)
                {
                    if (segment.first == j && segment.second == i)
                    {
                        cout << "o";
                        printed = true;
                        break;
                    }
                }
                if (!printed)
                    cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width; i++) cout << "*";
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input()
{
    if (_kbhit())
    {
        int ch = _getch();
        if (ch == 224)  // Arrow key prefix
        {
            ch = _getch();  // Actual key code
            switch (ch)
            {
            case 72: dir = UP; break;    // ↑
            case 80: dir = DOWN; break;  // ↓
            case 75: dir = LEFT; break;  // ←
            case 77: dir = RIGHT; break; // →
            }
        }
        else if (ch == 'x' || ch == 'X')
        {
            gameOver = true;
        }
    }
}


void Logic()
{
    if (!tail.empty())
    {
        tail.insert(tail.begin(), { x, y });
        tail.pop_back();
    }

    switch (dir)
    {
    case LEFT:  x--; break;
    case RIGHT: x++; break;
    case UP:    y--; break;
    case DOWN:  y++; break;
    }

    if (x <= 0 || x >= width - 1 || y < 0 || y >= height)
        gameOver = true;

    for (auto segment : tail)
        if (segment.first == x && segment.second == y)
            gameOver = true;

    if (x == foodX && y == foodY)
    {
        score += 10;
        foodX = rand() % (width - 2) + 1;
        foodY = rand() % (height - 2) + 1;
        tail.push_back({ x, y });
    }
}

void SaveHighScore()
{
    int highScore = 0;
    ifstream in("highscore.txt");
    if (in.is_open()) in >> highScore;
    in.close();

    if (score > highScore)
    {
        ofstream out("highscore.txt");
        out << score;
        out.close();
        cout << " New High Score: " << score << "  " << endl;
    }
    else
    {
        cout << "Your Score: " << score << endl;
        cout << "High Score: " << highScore << endl;
    }
}

void ShowHighScore()
{
    system("cls");
    int highScore = 0;
    ifstream in("highscore.txt");
    if (in.is_open()) in >> highScore;
    in.close();

    cout << "\n===== High Score =====\n";
    cout << "High Score: " << highScore << "\n";
    cout << "======================\n\n";
    system("pause");
}

void SelectDifficulty()
{
    system("cls");
    int choice;
    cout << "\nChoose Difficulty:\n";
    cout << "1. Easy\n2. Medium\n3. Hard\nChoice: ";
    cin >> choice;
    switch (choice)
    {
        case 1: speed = 120; break;
        case 2: speed = 80; break;
        case 3: speed = 40; break;
        default: speed = 100;
    }
}

void ShowInstructions()
{
    system("cls");
    cout << "\n========== Instructions ==========\n";
    cout << "Use Arrow Keys (← ↑ ↓ →) to move snake\n";
    cout << "Eat '.' to grow, avoid wall and self\n";
    cout << "Press X anytime to exit\n";
    cout << "Avoid crashing into your own tail!\n";
    cout << "==================================\n\n";
    system("pause");
}

void ClearScreen()
{
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written, cells;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console, &csbi);
    cells = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(console, ' ', cells, topLeft, &written);
    FillConsoleOutputAttribute(console, csbi.wAttributes, cells, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void Run()
{
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(speed);
    }

    ClearScreen();
    cout << "\n\n\t GAME OVER \n";
    SaveHighScore();
    cout << "\nPress any key to return to menu...";
    getch();
}

int main()
{
     system("chcp 65001 > nul"); // Enable UTF-8 encoding for arrow keys

    while (true)
    {
        system("cls");
        cout << "=========== SNAKE GAME DADA EDITION ===========\n";
        cout << "1. Start Game\n";
        cout << "2. Select Difficulty\n";
        cout << "3. View High Score\n";
        cout << "4. Instructions\n";
        cout << "5. Exit\n";
        cout << "===============================================\n";
        cout << "Enter your choice: ";
        int ch;
        cin >> ch;

        switch (ch)
        {
            case 1: Run(); break;
            case 2: SelectDifficulty(); break;
            case 3: ShowHighScore(); break;
            case 4: ShowInstructions(); break;
            case 5: exit(0);
            default: cout << "Invalid choice.\n"; Sleep(1000);
        }
    }
}
