#include<bits/stdc++.h>

// Verification : 
// Line Add Get Min - Library Checker (https://judge.yosupo.jp/submission/128025)
// Segment Add Get Min - Library Checker (https://judge.yosupo.jp/submission/128050)
struct LiChaoTree {
    struct Line {
        long long a, b; // 傾き,切片
        Line (long long _a, long long _b) : a(_a), b(_b) {}
        long long operator()(long long x) const { return a * x + b; }
        void swap(Line& other) {
            std::swap(a, other.a);
            std::swap(b, other.b);
        }
    };
    int n; // セグ木の最下層サイズ
    static constexpr long long inf   = 1LL << 30; // infll > max(a) * inf + max(b), inf > max(x[i])
    static constexpr long long infll = 1LL << 61; // infll > max(a) * inf + max(b)
    std::vector<long long> x; // x座標
    std::vector<Line> dat;    // セグ木のノード 1-indexed. dat[n+i] = x[i].

    LiChaoTree () {}
    // Constraints: |x[i]| <= 10^9
    // 制約が広い場合 inf と infll を増やせ.
    LiChaoTree (const std::vector<long long>& _x) {
        n = 1;
        while(n < (int)_x.size()) n <<= 1;
        x.assign(n, inf); // ここは多分大丈夫
        for(int i = 0; i < (int)_x.size(); ++i) x[i] = _x[i];
        dat.assign(2 * n, {0, infll});
    }

    // y = ax+b を追加
    // Constraints : |a| <= 10^9, |b| <= 10^18
    // 制約が広い場合 infll を増やせ. オーバーフロー気にしなくて良いので 8*10^9くらいでもおけそう
    void add_line(long long a, long long b) {
        add_line(a, b, 1, 0, n);
    }

    // y = ax+b を 区間 [x[l], x[r]) に追加
    // Constraints : |a| <= 10^9, |b| <= 10^18
    // 制約が広い場合 infll を増やせ. オーバーフロー気にしなくて良いので 8*10^9くらいでもおけそう
    void add_segment(long long a, long long b, int l, int r) {
        int nl = n + l, nr = n + r; // ノード番号
        int sz = 1; // 今見てるノードの幅
        // l, r は x[l], x[r] に対応してます 非再帰セグ木
        while(l < r) {
            if (nl & 1) {
                add_line(a, b, nl, l, l + sz);
                ++nl;
                l += sz;
            }
            if (nr & 1) {
                add_line(a, b, nr - 1, r - sz, r);
                --nr;
                r -= sz;
            }
            nl >>= 1, nr >>= 1;
            sz <<= 1;
        }
    }

    // y = ax+b を ノード番号nnode (左端l, 右端r）に追加
    void add_line(long long a, long long b, int nnode, int l, int r) {
        Line B(a, b);
        while(nnode < 2 * n) { // ノードが存在する限りトップダウンに更新を実行する
            int m = (l + r) >> 1;
            const auto& A = dat[nnode];

            bool ld = B(x[l]) < A(x[l]);
            bool md = B(x[m]) < A(x[m]);
            bool rd = B(x[r-1]) < A(x[r-1]);

            if(!ld && !rd) return;
            if(ld && rd) {
                dat[nnode] = B;
                return;
            }
            // lまたはrだけでBが下になるようにする
            if(md) dat[nnode].swap(B);

            if(ld == md) { // 右側
                nnode = (nnode << 1) + 1;
                l = m;
            }
            else { // 左側
                nnode = (nnode << 1);
                r = m;
            }
        }
    }

    // x[i] での最小値を答える
    long long min(int i) const {
        const auto xt = x[i];
        i += n;
        long long res = infll;
        while(i > 0) {
            res = std::min(res, dat[i](xt));
            i >>= 1;
        }
        return res;
    }
};