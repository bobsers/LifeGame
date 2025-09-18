#include <conio.h>
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

// 全局变量
int rows = 20;
int cols = 40;
int delay_ms = 200;
vector<vector<int>> grid;
vector<vector<int>> next_grid;

// 初始化网格
void init_grid() {
    grid = vector<vector<int>>(rows, vector<int>(cols, 0));
    next_grid = vector<vector<int>>(rows, vector<int>(cols, 0));
}

// 计算邻居数量
int count_neighbors(int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                count += grid[nx][ny];
            }
        }
    }
    return count;
}

// 下一代
void nextgeneration() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int neighbors = count_neighbors(i, j);
            if (grid[i][j] == 1) {
                next_grid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                next_grid[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
    grid = next_grid;
}

// 打印网格
void printgrid() {
    system("cls");
    cout << "Conway's Game of Life - Press ESC to return to menu\n";
    cout << "Speed: " << delay_ms << "ms | Grid: " << rows << "x" << cols << "\n";
    cout << "+" << string(cols, '-') << "+\n";
    for (int i = 0; i < rows; i++) {
        cout << "|";
        for (int j = 0; j < cols; j++) {
            cout << (grid[i][j] ? '*' : '.');
        }
        cout << "|\n";
    }
    cout << "+" << string(cols, '-') << "+\n";
}

// 清空网格
void clear_grid() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = 0;
        }
    }
}

// 随机模式
void random_mode() {
    clear_grid();
    srand(time(0));

    cout << "Enter cell density (0.0-1.0, default 0.3): ";
    cin.ignore();
    double density = 0.3;
    if (cin.peek() == '\n') {
        cin.ignore();
    } else {
        cin >> density;
        cin.ignore();
        if (density < 0 || density > 1) density = 0.3;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = (rand() / (double)RAND_MAX) < density ? 1 : 0;
        }
    }

    while (true) {
        printgrid();
        nextgeneration();

        if (_kbhit()) {
            char key = _getch();
            if (key == 27) break;
        }

        Sleep(delay_ms);
    }
}

// 自定义模式
void custom_mode() {
    clear_grid();

    cout << "Custom Mode - Place cells by coordinates\n";
    cout << "Grid size: " << rows << "x" << cols << " (0-based indexing)\n";
    cout << "Enter coordinates (row col), -1 -1 to finish:\n";
    cout << "Example: 10 20 (places cell at row 10, column 20)\n\n";

    printgrid();

    int r, c;
    while (true) {
        cout << "Enter coordinates (row col): ";
        cin >> r >> c;

        if (r == -1 && c == -1) break;

        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            grid[r][c] = 1;
            printgrid();
            cout << "Cell placed at (" << r << ", " << c << ")\n";
        } else {
            cout << "Invalid coordinates! Must be within 0-" << rows - 1 << " and 0-" << cols - 1 << "\n";
        }
    }

    cout << "\nPress any key to start simulation...\n";
    _getch();

    while (true) {
        printgrid();
        nextgeneration();

        if (_kbhit()) {
            char key = _getch();
            if (key == 27) break;
        }

        Sleep(delay_ms);
    }
}
// 设置模式
void settings_mode() {
    cout << "Settings\n";
    cout << "========\n";
    cout << "Current settings:\n";
    cout << "- Grid size: " << rows << "x" << cols << "\n";
    cout << "- Update speed: " << delay_ms << "ms\n\n";

    cout << "1. Change grid size\n";
    cout << "2. Change update speed\n";
    cout << "3. Reset to defaults\n";
    cout << "0. Back to main menu\n";

    int choice;
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
        case 1: {
            cout << "Enter new grid size (rows cols): ";
            int new_rows, new_cols;
            cin >> new_rows >> new_cols;

            if (new_rows < 5 || new_cols < 5 || new_rows > 50 || new_cols > 100) {
                cout << "Invalid size! Must be between 5x5 and 50x100\n";
                cout << "Press any key to continue...\n";
                _getch();
                break;
            }

            rows = new_rows;
            cols = new_cols;
            init_grid();
            cout << "Grid size updated to " << rows << "x" << cols << "\n";
            cout << "Press any key to continue...\n";
            _getch();
            break;
        }

        case 2: {
            cout << "Enter update delay in milliseconds (50-2000): ";
            int new_delay;
            cin >> new_delay;

            if (new_delay < 50 || new_delay > 2000) {
                cout << "Invalid delay! Must be between 50 and 2000\n";
                cout << "Press any key to continue...\n";
                _getch();
                break;
            }

            delay_ms = new_delay;
            cout << "Update speed set to " << delay_ms << "ms\n";
            cout << "Press any key to continue...\n";
            _getch();
            break;
        }

        case 3:
            rows = 20;
            cols = 40;
            delay_ms = 200;
            init_grid();
            cout << "Settings reset to defaults\n";
            cout << "Press any key to continue...\n";
            _getch();
            break;
    }
}

// 显示菜单
void show_menu() {
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

// 关于信息
void show_about() {
    system("cls");
    cout << "Conway's Game of Life\n";
    cout << "====================\n\n";
    cout << "Rules:\n";
    cout << "1. Any live cell with 2 or 3 live neighbors survives\n";
    cout << "2. Any dead cell with exactly 3 live neighbors becomes alive\n";
    cout << "3. All other cells die or remain dead\n\n";
    cout << "Controls:\n";
    cout << "- ESC: Return to main menu during simulation\n";
    cout << "- In custom mode: Enter coordinates to place cells\n\n";
    cout << "Created with C++\n";
    cout << "\nPress any key to return to menu...\n";
    _getch();
}

int main() {
    init_grid();

    while (true) {
        show_menu();

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                random_mode();
                break;

            case 2:
                custom_mode();
                break;

            case 3:
                settings_mode();
                break;

            case 4:
                show_about();
                break;

            case 0:
                cout << "Thanks for playing!\n";
                return 0;

            default:
                cout << "Invalid choice! Press any key to continue...\n";
                _getch();
        }
    }

    return 0;
}