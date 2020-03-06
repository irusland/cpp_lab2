//
// Created by Ruslan Sirazhetdinov on 06.03.2020.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct POINT {
    int x;
    int y;
    POINT() : x(-1), y(-1) {};
    POINT(int x, int y) : x(x), y(y) {};
    void Print() {
        cout << x << " " << y << endl;
    }
};
typedef vector<POINT> state;

void show_path(vector<vector<POINT>>& paths, POINT& end) {
    POINT prev = paths[end.x][end.y];
    stack<POINT> result;
    result.push(end);
    while (prev.x != -1 && prev.y != -1) {
        result.push(prev);
        prev = paths[prev.x][prev.y];
    }
    while(!result.empty()) {
        result.top().Print();
        result.pop();
    }
}


int check_point(POINT& p, vector<vector<POINT>>& paths, vector<vector<int>>& map, POINT& start) {
    if (p.x < 1 || p.x > map.size() || p.y < 1 || p.y > map[0].size()) return false; // за границу
    if (map[p.x - 1][p.y - 1] == 1) return false; // в стену
    if (p.x == start.x && p.y == start.y) // начало
        return false;
    POINT np = paths[p.x][p.y];
    return !(np.x != -1 && np.y != -1); // туда не ходили
}

vector<POINT> steps(POINT anchor_point, vector<vector<int>> &map) {
    vector<POINT> offsets {
            POINT(-1, 0),
            POINT(1, 0),
            POINT(0, -1),
            POINT(0, 1)
    };
    vector<POINT> new_points;
    for (auto offset : offsets) {
        POINT next_point;
        next_point.x = anchor_point.x + offset.x;
        next_point.y = anchor_point.y + offset.y;
        new_points.push_back(next_point);
    }
    return new_points;
}

vector<vector<POINT>> bfs(POINT start, POINT end, vector<vector<int>>& map) {
    queue<POINT> q;
    q.push(start);

    vector<vector<POINT>> paths(map.size() + 1, vector<POINT> (map[0].size() + 1));
    paths[start.x][start.y] = POINT();

    while (!q.empty()) {
        POINT anchor_point = q.front();
        q.pop();
        vector<POINT> next_points = steps(anchor_point, map);
        for(auto next_point : next_points) {
            if (check_point(next_point, paths, map, start)) {
                q.push(next_point);
                paths[next_point.x][next_point.y] = anchor_point;
                if (next_point.x == end.x && next_point.y == end.y) {
                    return paths;
                }
            }
        }
    }
    return paths;
}

int main(int argc, char **argv) {
    ifstream in(argv[1]);
    if (!in) {
        cout << "file '" << argv[1] << "' could not be opened" << argv[0] << argc << endl;
        return 1;
    }

    string str;
    getline(in, str);
    const int row_count = stoi(str);
    getline(in, str);
    const int column_count = stoi(str);
    vector<vector<int>> labyrinth(row_count , vector<int> (column_count));

    for (int r = 0; r < row_count; ++r) {
        getline(in, str);
        for (int c = 0; c < column_count; ++c) {
            labyrinth[r][c] = str[c * 2] == '1';
        }
    }

    getline(in, str);
    char strs[str.length()];
    strcpy(strs, str.c_str());
    char *pch = strtok(strs, " ,.");
    POINT start = POINT();
    start.x = stoi(pch);
    pch = strtok(nullptr, " ,.");
    start.y = stoi(pch);

    getline(in, str);
    char stre[str.length()];
    strcpy(stre, str.c_str());
    pch = strtok(stre, " ,.");
    POINT end = POINT();
    end.x = stoi(pch);
    pch = strtok(nullptr, " ,.");
    end.y = stoi(pch);

    vector<vector<POINT>> r = bfs(start, end, labyrinth);

    POINT end_tail = r[end.x][end.y];
    if (end_tail.x != -1 && end_tail.y != -1) {
        cout << "Y" << endl;
        show_path(r, end);
    } else {
        cout << "N" << endl;
    }

    in.close();
    return 0;
}
