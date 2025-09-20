#include <conio.h>
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>
using namespace std;

class LifeGame;
class GameMode;

class GameSettings {
   private:
    int rows;
    int cols;
    int delay_ms;

   public:
    GameSettings(int r = 20, int c = 40, int d = 200) : rows(r), cols(c), delay_ms(d) {
    }

    int getRows() const {
        return rows;
    }
    int getCols() const {
        return cols;
    }
    int getDelay() const {
        return delay_ms;
    }

    void setRows(int r) {
        rows = r;
    }
    void setCols(int c) {
        cols = c;
    }
    void setDelay(int d) {
        delay_ms = d;
    }
};
class Grid {
   private:
    vector<vector<int>> cell;
    int rows;
    int cols;

   public:
    Grid(int r, int c) : rows(r), cols(c) {
        cell = vector<vector<int>>(rows, vector<int>(cols, 0));
    }
    int getRows() const {
        return rows;
    }
    int getCols() const {
        return cols;
    }

    int getCell(int x, int y) const {
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            return cell[x][y];
        }
        return 0;
    }

    void setCell(int x, int y, int value) {
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            cell[x][y] = value;
        }
    }

    void clear() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cell[i][j] = 0;
            }
        }
    }
    int countNeighbors(int x, int y) const {
        int count = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                count += getCell(nx, ny);
            }
        }
        return count;
    }

    void nextGeneration(Grid& nextGrid) const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int neighbors = countNeighbors(i, j);
                if (getCell(i, j) == 1) {
                    nextGrid.setCell(i, j, (neighbors == 2 || neighbors == 3) ? 1 : 0);
                } else {
                    nextGrid.setCell(i, j, (neighbors == 3) ? 1 : 0);
                }
            }
        }
    }
    void copyFrom(const Grid& other) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cell[i][j] = other.getCell(i, j);
            }
        }
    }
};

class GameMode {
   protected:
    LifeGame* game;

   public:
    GameMode(LifeGame* g) : game(g) {
    }
    virtual ~GameMode() = default;  // 虚函数析构函数
    virtual void run() = 0;
};
class RandomMode : public GameMode {
   public:
    RandomMode(LifeGame* g) : GameMode(g) {
    }

    void run() override {
        cout << "随机生成的地图\n";
        cout << "按任意键开始\n";
        _getch();
    }
};
class CustomMode : public GameMode {
   public:
    CustomMode(LifeGame* g) : GameMode(g) {
    }

    void run() override {
        cout << "自定义模式\n";
        cout << "按任意键开始\n";
        _getch();
    }
};

class SettingsMode : public GameMode {
   public:
    SettingsMode(LifeGame* g) : GameMode(g) {
    }

    void run() override {
        cout << "设置模式\n";
        cout << "按任意键开始\n";
        _getch();
    }
};

class AboutMode : public GameMode {
   public:
    AboutMode(LifeGame* g) : GameMode(g) {
    }

    void run() override {
        system("cls");
        cout << "Conway's Game of Life\n";
        cout << "====================\n\n";
        cout << "规则:\n";
        cout << "1. 任何拥有2或3个活邻居的活细胞会继续存活\n";
        cout << "2. 任何恰好有3个活邻居的死细胞会变为活细胞\n";
        cout << "3. 所有其他细胞会死亡或保持死亡状态\n\n";
        cout << "控制方式:\n";
        cout << "- ESC键: 在模拟过程中返回主菜单\n";
        cout << "- 在自定义模式中: 输入坐标来放置细胞\n\n";
        cout << "使用C++面向对象设计创建\n";
        cout << "\n按任意键返回菜单\n";
        _getch();
    }
};

class LifeGame {
   private:
    GameSettings settings;
    Grid currentGrid;
    Grid nextGrid;
    bool running;

    unique_ptr<RandomMode> randomMode;
    unique_ptr<CustomMode> customMode;
    unique_ptr<SettingsMode> settingsMode;
    unique_ptr<AboutMode> aboutMode;

   public:
    LifeGame()
        : settings(20, 40, 200),
          currentGrid(settings.getRows(), settings.getCols()),
          nextGrid(settings.getRows(), settings.getCols()),
          running(true) {
        // 初始化游戏模式
        randomMode = make_unique<RandomMode>(this);
        customMode = make_unique<CustomMode>(this);
        settingsMode = make_unique<SettingsMode>(this);
        aboutMode = make_unique<AboutMode>(this);
    }

    GameSettings& getSettings() {
        return settings;
    }
    Grid& getCurrentGrid() {
        return currentGrid;
    }
    Grid& getNextGrid() {
        return nextGrid;
    }
    bool isRunning() const {
        return running;
    }
    void stop() {
        running = false;
    }

    void initGrid() {
        currentGrid.clear();
        nextGrid.clear();
    }

    void update() {
        currentGrid.nextGeneration(nextGrid);
        currentGrid.copyFrom(nextGrid);
    }

    void printGrid() const {
        system("cls");
        cout << "Conway's Game of Life - Press ESC to return to menu\n";
        cout << "Speed: " << settings.getDelay() << "ms | Grid: " << settings.getRows() << "x" << settings.getCols()
             << "\n";
        cout << "+" << string(settings.getCols(), '-') << "+\n";

        for (int i = 0; i < settings.getRows(); i++) {
            cout << "|";
            for (int j = 0; j < settings.getCols(); j++) {
                cout << (currentGrid.getCell(i, j) ? '*' : '.');
            }
            cout << "|\n";
        }

        cout << "+" << string(settings.getCols(), '-') << "+\n";
    }

    void showMenu() {
        system("cls");
        cout << "=================================\n";
        cout << "    Conway's Game of Life\n";
        cout << "=================================\n";
        cout << "1. Random Mode\n";
        cout << "2. Custom Mode\n";
        cout << "3. Settings\n";
        cout << "4. About\n";
        cout << "0. Exit\n";
        cout << "=================================\n";
        cout << "Enter your choice: ";
    }

    void handleMenuChoice(int choice) {
        switch (choice) {
            case 1:
                randomMode->run();
                break;
            case 2:
                customMode->run();
                break;
            case 3:
                settingsMode->run();
                break;
            case 4:
                aboutMode->run();
                break;
            case 0:
                cout << "Thanks for playing!\n";
                stop();
                break;
            default:
                cout << "Invalid choice! Press any key to continue...\n";
                _getch();
        }
    }

    void run() {
        while (running) {
            showMenu();
            int choice;
            cin >> choice;
            handleMenuChoice(choice);
        }
    }
};

int main() {
    LifeGame game;
    game.run();
    return 0;
}