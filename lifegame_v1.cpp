#include <bits/stdc++.h>
#include <windows.h>

#include <chrono>
#include <thread>
using namespace std;

const int rows = 20;
const int cols = 40;
vector<vector<int>> grid(rows, vector<int>(cols));
vector<vector<int>> next_grid(rows, vector<int>(cols));

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

void printgrid() {
    system("cls");
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

int main() {
    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = rand() % 2;
        }
    }
    while (1) {
        printgrid();
        nextgeneration();
        Sleep(200);
    }
}
