#include<bits/stdc++.h>


// Verification : 
// Line Add Get Min - Library Checker (// https://judge.yosupo.jp/submission/132907)
// Segment Add Get Min - Library Checker (https://judge.yosupo.jp/submission/132915)
// note : ↑の制約 : |a|,|x| <= 10^9, |b| <= 10^18

template<typename T, bool(*comp)(T, T), T(*e)(), T(*xub)()>
struct LiChaoTree {
    struct Line {
        T a, b; // 傾き,切片
        Line (T _a, T _b) : a(_a), b(_b) {}
        T operator()(T x) const { return a * x + b; }   
    };
    int n;                    // セグ木の最下層サイズ
    std::vector<T> x;         // x座標
    std::vector<Line> dat;    // セグ木のノード 1-indexed. dat[n+i] = x[i].

    LiChaoTree () {}
    // max(x) < xub() , comp(max(a) * xub() + max(b), e()) = false.
    template<typename S>
    LiChaoTree (const std::vector<S>& _x) {
        n = 1;
        while(n < (int)_x.size()) n <<= 1;
        x.assign(n, xub());
        for(int i = 0; i < (int)_x.size(); ++i) x[i] = _x[i];
        dat.assign(2 * n, {0, e()});
    }

    // y = ax+b を追加
    void add_line(T a, T b) {
        add_line(a, b, 1, 0, n);
    }

    // y = ax+b を 区間 [x[l], x[r]) に追加
    void add_segment_at(T a, T b, int l, int r) {
        int nl = n + l, nr = n + r; // ノード番号
        int sz = 1;                 // 今見てるノードの幅
        // l, r は x[l], x[r] に対応. cf.非再帰セグ木
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

    // y = ax+b を 区間 [lval, rval) に追加 ;余計な定数倍アリ
    void add_segment(T a, T b, int lval, int rval) {
        add_segment_at(a, b,
            std::lower_bound(x.begin(), x.end(), lval) - x.begin(),
            std::lower_bound(x.begin(), x.end(), rval) - x.begin()
        );
    }

    // x[i] での最小値
    T query_at(int index) const {
        const auto xt = x[index];
        index += n;
        T res = e();
        while(index > 0) {  // note:ここはforloopにできます
            chval(res, dat[index](xt));
            index >>= 1;
        }
        return res;
    }
    // x = val での最小値 ;余計な定数倍アリ
    T query(T val) const {
        return query_at(std::lower_bound(x.begin(), x.end(), val) - x.begin());
    }

  private:
    void chval(T& a, T b) const { // chminみたいな
        if(comp(b, a)) a = b;
    }
    // y = ax+b を ノード番号 nnode (左端l, 右端r）に追加
    void add_line(T a, T b, int nnode, int l, int r) {
        Line B(a, b);
        while(nnode < 2 * n) {  // ノードが存在する限りトップダウンに更新を実行する
            int m = (l + r) >> 1;
            const auto& A = dat[nnode];

            bool ld = comp(B(x[l]), A(x[l]));
            bool md = comp(B(x[m]), A(x[m]));
            bool rd = comp(B(x[r-1]), A(x[r-1]));

            if(md) {
                std::swap(dat[nnode], B);
                ld = !ld, md = !md, rd = !rd;
            }
            if(ld == rd) return;

            if(ld) { // 左側
                nnode = (nnode << 1);
                r = m;
            }
            else {   // 右側
                nnode = (nnode << 1) + 1;
                l = m;
            }
        }
    }
};

// using T = long long;
// bool comp(T a, T b) {return a < b;}
// constexpr T e() {return 1LL<<62;}
// constexpr T xub() {return 1LL<<31;}
// LiChaoTree<T, comp, e, xub> lct(x);