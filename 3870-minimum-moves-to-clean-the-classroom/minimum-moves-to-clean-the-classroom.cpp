class Solution {
public:
    int minMoves(vector<string>& classroom, int energy) {
        
        int m = classroom.size();
        int n = classroom[0].size();

        vector<vector<int>> id(m, vector<int>(n, -1));

        int sr = 0, sc = 0;
        int litter = 0;

        // Find starting point and give every litter a number
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                if (classroom[i][j] == 'S') {
                    sr = i;
                    sc = j;
                }

                if (classroom[i][j] == 'L') {
                    id[i][j] = litter++;
                }
            }
        }

        // No litter
        if (litter == 0)
            return 0;

        int totalMask = (1 << litter) - 1;

        // visited[row][col][energy][mask]
        vector<vector<vector<vector<bool>>>> visited(
            m,
            vector<vector<vector<bool>>>(
                n,
                vector<vector<bool>>(
                    energy + 1,
                    vector<bool>(1 << litter, false)
                )
            )
        );

        queue<tuple<int, int, int, int>> q;

        // row, col, current energy, collected litter mask
        q.push({sr, sc, energy, 0});

        visited[sr][sc][energy][0] = true;

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        int moves = 0;

        while (!q.empty()) {

            int size = q.size();

            while (size--) {

                auto [x, y, e, mask] = q.front();
                q.pop();

                // All litter collected
                if (mask == totalMask)
                    return moves;

                // No energy left
                if (e == 0)
                    continue;

                for (int d = 0; d < 4; d++) {

                    int nx = x + dx[d];
                    int ny = y + dy[d];

                    // Outside grid
                    if (nx < 0 || nx >= m ||
                        ny < 0 || ny >= n)
                        continue;

                    // Obstacle
                    if (classroom[nx][ny] == 'X')
                        continue;

                    int newEnergy = e - 1;

                    // Reset energy
                    if (classroom[nx][ny] == 'R')
                        newEnergy = energy;

                    int newMask = mask;

                    // Collect litter
                    if (classroom[nx][ny] == 'L') {
                        newMask |= (1 << id[nx][ny]);
                    }

                    if (!visited[nx][ny][newEnergy][newMask]) {

                        visited[nx][ny][newEnergy][newMask] = true;

                        q.push({
                            nx,
                            ny,
                            newEnergy,
                            newMask
                        });
                    }
                }
            }

            moves++;
        }

        return -1;
    }
};