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
    void run() override;
};
class CustomMode : public GameMode {
   public:
    CustomMode(LifeGame* g) : GameMode(g) {
    }
    void run() override;
};
class SettingsMode : public GameMode {
   public:
    SettingsMode(LifeGame* g) : GameMode(g) {
    }
    void run() override;
};
class AboutMode : public GameMode {
   public:
    AboutMode(LifeGame* g) : GameMode(g) {
    }
    void run() override {
        system("cls");
        cout << "康威生命游戏\n";
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
        cout << "康威生命游戏 - 按ESC键返回菜单\n";
        cout << "速度: " << settings.getDelay() << "毫秒 | 网格: " << settings.getRows() << "x" << settings.getCols()
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
        cout << "       康威生命游戏\n";
        cout << "=================================\n";
        cout << "1. 随机模式\n";
        cout << "2. 自定义模式\n";
        cout << "3. 设置\n";
        cout << "4. 关于\n";
        cout << "0. 退出\n";
        cout << "=================================\n";
        cout << "请输入你的选择: ";
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
                cout << "感谢游玩!\n";
                stop();
                break;
            default:
                cout << "无效的选择! 按任意键继续...\n";
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
    void runSimulation() {
        bool simulationRunning = true;
        while (simulationRunning) {
            printGrid();
            if (_kbhit()) {
                char key = _getch();
                if (key == 27) {
                    simulationRunning = false;
                    break;
                }
            }
            update();
            this_thread::sleep_for(chrono::milliseconds(settings.getDelay()));
        }
    }
    void reinitializeGrid() {
        currentGrid = Grid(settings.getRows(), settings.getCols());
        nextGrid = Grid(settings.getRows(), settings.getCols());
    }
};
void RandomMode::run() {
    system("cls");
    cout << "随机模式\n";
    cout << "请输入初始细胞密度(0-100): ";

    int density;
    while (!(cin >> density) || density < 0 || density > 100) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "请输入0-100之间的数字: ";
    }

    game->initGrid();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100);

    auto& grid = game->getCurrentGrid();
    for (int i = 0; i < grid.getRows(); i++) {
        for (int j = 0; j < grid.getCols(); j++) {
            grid.setCell(i, j, dis(gen) < density ? 1 : 0);
        }
    }

    cout << "\n初始化完成! 按任意键开始模拟...\n";
    _getch();

    game->runSimulation();
}
void CustomMode::run() {
    system("cls");
    game->initGrid();
    auto& grid = game->getCurrentGrid();

    cout << "自定义模式\n";
    cout << "输入坐标来放置活细胞 (格式: 行 列)\n";
    cout << "输入-1 -1结束输入并开始模拟\n";
    cout << "坐标范围: 0-" << grid.getRows() - 1 << " 0-" << grid.getCols() - 1 << "\n\n";

    int x, y;
    while (true) {
        cout << "输入坐标: ";
        if (cin >> x >> y) {
            if (x == -1 && y == -1) break;
            if (x >= 0 && x < grid.getRows() && y >= 0 && y < grid.getCols()) {
                grid.setCell(x, y, 1);
                cout << "已放置细胞在 (" << x << ", " << y << ")\n";
            } else {
                cout << "坐标越界!\n";
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "无效输入!\n";
        }
    }

    cout << "\n初始化完成! 按任意键开始模拟...\n";
    _getch();

    game->runSimulation();
}
void SettingsMode::run() {
    system("cls");
    cout << "游戏设置\n";
    cout << "=================================\n";
    cout << "当前设置:\n";
    cout << "1. 行数: " << game->getSettings().getRows() << "\n";
    cout << "2. 列数: " << game->getSettings().getCols() << "\n";
    cout << "3. 更新延迟: " << game->getSettings().getDelay() << " 毫秒\n";
    cout << "0. 返回主菜单\n";
    cout << "=================================\n";
    cout << "请选择要修改的项目: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: {
            cout << "输入新的行数 (10-50): ";
            int rows;
            if (cin >> rows && rows >= 10 && rows <= 50) {
                game->getSettings().setRows(rows);
                game->reinitializeGrid();
                cout << "行数已更新!\n";
            } else {
                cout << "无效的行数!\n";
            }
            break;
        }
        case 2: {
            cout << "输入新的列数 (10-80): ";
            int cols;
            if (cin >> cols && cols >= 10 && cols <= 80) {
                game->getSettings().setCols(cols);
                game->reinitializeGrid();
                cout << "列数已更新!\n";
            } else {
                cout << "无效的列数!\n";
            }
            break;
        }
        case 3: {
            cout << "输入新的延迟时间 (50-1000 毫秒): ";
            int delay;
            if (cin >> delay && delay >= 50 && delay <= 1000) {
                game->getSettings().setDelay(delay);
                cout << "延迟已更新!\n";
            } else {
                cout << "无效的延迟时间!\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "无效的选择!\n";
    }

    cout << "按任意键继续...\n";
    _getch();
}
int main() {
    system("chcp 65001");
    LifeGame game;
    game.run();
    return 0;
}