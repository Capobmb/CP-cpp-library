template<typename T>
struct sum_array_2d {
    int r, c;
    std::vector<std::vector<T>> sum;
    sum_array_2d(const std::vector<std::vector<T>>& a) : r(a.size()), c(a.front().size()), sum(r+1, std::vector<T>(c+1, T{})) {
        for(int i = 0; i < r; i++) {
            for(int j = 0; j < c; j++) {
                sum[i+1][j+1] = sum[i][j+1] + sum[i+1][j] - sum[i][j] + a[i][j];
            }
        }
    }

    // return [x1, x2) x [y1, c2), 0-indexed
    T get(int x1, int x2, int y1, int y2) const {
        return sum[x2][y2] - sum[x2][y1] - sum[x1][y2] + sum[x1][y1];
    }
};
