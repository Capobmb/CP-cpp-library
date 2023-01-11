#include<bits/stdc++.h>


template<class C> struct Cost_tag{};
template<class S, class F, class G, class H, class C>
struct all_direction_tree_dp {
    int n; // 頂点数
    // S := モノイドをなすdpの型
    S e; // S の 単位元
    // C := 辺のコストの型
    
    // 以下、親をi, 子をjとする。
    F merge; // (S, S) -> S : 辺i-jを根とする部分木の結果同士をマージする演算
    G raise; // (S, ℤ, ℤ, C) -> S : 子jを根とする部分木の結果から 辺i-j(コストc)を根とする部分木の結果を得る演算
    H derive; // (S, ℤ) -> S : 辺i-jを根とする部分木の結果の総mergeから親iを根とする部分木の結果を得る演算
    std::vector<S> ans;
    std::vector<std::vector<S>> dp; //dp[i][j] : iのj番目の子を根とする部分木
    struct Edge{
        int to;
        C cost;
        Edge(int to, C cost) : to(to), cost(cost) {}
    };
    std::vector<std::vector<Edge>> edges;
    all_direction_tree_dp(int n, S e, F merge, G raise, H derive, Cost_tag<C>) : n(n), e(e), merge(merge), raise(raise), derive(derive), ans(n), dp(n), edges(n) {
        static_assert(std::is_same_v<S, decltype(merge(e, e))>, "ERROR: invalid merge function");
        static_assert(std::is_same_v<S, decltype(raise(e, 0, 0, C{}))>, "ERROR: invalid raise function");
        static_assert(std::is_same_v<S, decltype(derive(e, 0))>, "ERROR: invalid derive function");
    }

    std::pair<int,int> add_edge(int u, int v, C c) {
        edges[u].emplace_back(v, c);
        edges[v].emplace_back(u, c);
        return std::pair<int,int>((int)edges[u].size()-1, (int)edges[v].size()-1);
    }

    // 普通の木dpをする
    S dfs_sub(int now, int par) {
        S ret = e;
        int deg = edges[now].size();
        dp[now].assign(deg, e);
        for(int i=0; i<deg; ++i) {
            auto [to, cost] = edges[now][i];
            if(to == par) continue;
            dp[now][i] = dfs_sub(to, now);
            ret = merge(ret, raise(dp[now][i], now, to, cost));
        }
        return derive(ret, now);
    }

    void dfs_main(int now, int par, S par_v) {
        int deg = edges[now].size();

        // 半開区間 (i.e 1-indexed, 0-indexed)
        // left_prod[i+1] = merge(dp[now][0],...dp[now][i])
        // right_prod[i] = merge(dp[now][i],...dp[now][deg-1]);
        std::vector<S> left_prod(deg + 1, e), right_prod(deg + 1, e);

        for(int i=0; i<deg; ++i) {
            if(edges[now][i].to == par) dp[now][i] = par_v;
            left_prod[i+1] = merge(left_prod[i], raise(dp[now][i], now, edges[now][i].to, edges[now][i].cost));
        }
        for(int i=deg-1; i>=0; --i) {
            right_prod[i] = merge(right_prod[i+1], raise(dp[now][i], now, edges[now][i].to, edges[now][i].cost));
        }
        for(int i=0; i<deg; ++i) {
            if(edges[now][i].to == par) continue;
            dfs_main(edges[now][i].to, now, derive(merge(left_prod[i], right_prod[i+1]), now));
        }
        ans[now] = derive(right_prod[0], now);
    }

    std::vector<S> run() {
        dfs_sub(0, -1);
        dfs_main(0, -1, e);
        return ans;
    }
};
//  copy template below.
    // using S = ;
    // using C = ;
    // auto merge = [](S a, S b) {};
    // auto raise = [](S a, int i, int j, C c) {};
    // auto derive = [](S a, int i) {};
    // all_direction_tree_dp dp(n, (e of (S,merge)), merge, raise, derive, Cost_tag<C>{});
//


//  -------------------------------------------------------
//  -------------------------------------------------------
/*  
    verified by:
    ABC222-F Expensive Expense
    https://atcoder.jp/contests/abc222/submissions/34447758

    EDPC-V Subtree (older version)
    https://atcoder.jp/contests/dp/submissions/34444733

    ABC160-F Distributing Integers
    https://atcoder.jp/contests/abc160/submissions/34447575
*/

//  -------------------------------------------------------
//  -------------------------------------------------------


//  Example code : ABC222-F Expensive Expense
using namespace std;
int main() {
    int n; cin>>n; 
    vector<long long> d(n);

    using S = long long;
    using C = long long;
    auto merge = [](S a, S b) {return max(a, b);};
    auto raise = [&](S a, int i, int j, C c) {return max(a, d[j]) + c;};
    auto derive = [](S a, int i) {return a;};
    all_direction_tree_dp dp(n, 0LL, merge, raise, derive, Cost_tag<C>{});

    for(int i=0; i<n-1; i++) {
        int a,b,c; cin>>a>>b>>c; a--; b--;
        dp.add_edge(a, b, c);
    }
    for(int i=0; i<n; i++) cin>>d[i];

    auto res = dp.run();
    for(int i=0; i<n; i++) cout << res[i] << '\n';
}
