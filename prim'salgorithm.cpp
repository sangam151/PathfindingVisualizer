#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cstdlib> // for rand()
#include <ctime>   // for time()

using namespace std;

// Directions: up, right, down, left
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

// For walls: 0=top,1=right,2=bottom,3=left
struct Cell
{
    bool walls[4]; // true means wall exists
    bool visited;
    Cell()
    {
        for (int i = 0; i < 4; i++)
            walls[i] = true;
        visited = false;
    }
};

int rows = 10;
int cols = 10;

// Check if cell is inside grid
bool valid(int x, int y)
{
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Print maze as ASCII
void printMaze(const vector<vector<Cell>> &maze)
{
    // Print top border
    for (int i = 0; i < cols; i++)
        cout << "+---";
    cout << "+\n";

    for (int i = 0; i < rows; i++)
    {
        // Print left wall and spaces
        cout << "|";
        for (int j = 0; j < cols; j++)
        {
            cout << "   ";
            if (maze[i][j].walls[1])
                cout << "|"; // right wall
            else
                cout << " ";
        }
        cout << "\n";

        // Print bottom walls
        for (int j = 0; j < cols; j++)
        {
            cout << "+";
            if (maze[i][j].walls[2])
                cout << "---";
            else
                cout << "   ";
        }
        cout << "+\n";
    }
}

int main()
{
    srand(time(0));

    // Initialize maze grid
    vector<vector<Cell>> maze(rows, vector<Cell>(cols));

    // Priority queue stores edges: (weight, (x,y), direction)
    // direction means the wall from cell (x,y) to neighbor
    using Edge = tuple<int, int, int, int>; // weight, x, y, direction

    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;

    // Start from cell (0,0)
    maze[0][0].visited = true;

    // Push all walls of (0,0) into pq
    for (int d = 0; d < 4; d++)
    {
        int nx = 0 + dx[d];
        int ny = 0 + dy[d];
        if (valid(nx, ny))
        {
            int w = rand() % 100; // random weight
            pq.push({w, 0, 0, d});
        }
    }

    while (!pq.empty())
    {
        Edge edge = pq.top();
        pq.pop();

        int w = get<0>(edge);
        int x = get<1>(edge);
        int y = get<2>(edge);
        int dir = get<3>(edge);

        int nx = x + dx[dir];
        int ny = y + dy[dir];

        // If neighbor is inside and not visited, knock down wall
        if (valid(nx, ny) && !maze[nx][ny].visited)
        {
            maze[x][y].walls[dir] = false;
            // Remove opposite wall of neighbor
            int opposite = (dir + 2) % 4;
            maze[nx][ny].walls[opposite] = false;

            maze[nx][ny].visited = true;

            // Push neighbor's walls
            for (int nd = 0; nd < 4; nd++)
            {
                int nnx = nx + dx[nd];
                int nny = ny + dy[nd];
                if (valid(nnx, nny) && !maze[nnx][nny].visited)
                {
                    int nw = rand() % 100;
                    pq.push({nw, nx, ny, nd});
                }
            }
        }
    }

    printMaze(maze);

    return 0;
}
