struct rolling_hash {
    // Ref : https://qiita.com/keymoon/items/11fac5627672a6d6a9f6 (Last viewed 2023-01-09)
    int n;

    using ull = unsigned long long;
    static constexpr ull MOD = (1ull << 61) - 1;
    // static constexpr int base = 9973;
    static inline const ull base = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
    std::vector<ull> hash, power;

    // a*b mod 2^61-1 (最後にModを取らない) / note: result < 4*MOD
    static ull mul(ull a, ull b) {
        static constexpr ull MASK30 = (1ull << 30) - 1;
        static constexpr ull MASK31 = (1ull << 31) - 1;
        ull au = a >> 31;
        ull ad = a & MASK31;
        ull bu = b >> 31;
        ull bd = b & MASK31;
        ull mid = ad * bu + au * bd;
        ull midu = mid >> 30;
        ull midd = mid & MASK30;
        return au * bu * 2 + midu + (midd << 31) + ad * bd;
    }

    // X mod 2^61-1
    static ull take_mod(ull x) {
        static constexpr ull MASK61 = MOD;
        ull xu = x >> 61;
        ull xd = x & MASK61;
        ull res = xu + xd;
        if (res >= MOD) res -= MOD;
        return res;
    }

    rolling_hash(const std::string &s) : n(s.size()), hash(n+1), power(n+1) {
        hash[0] = 0;
        power[0] = 1;
        for(int j = 0; j < n; j++) {
            power[j+1] = take_mod(mul(power[j], base));
            hash[j+1] = take_mod(mul(hash[j], base) + s[j]);
        }
    }

    // get hash of S[l:r) in O(1)
    ull gethash(int l, int r) const {
        static constexpr ull POSITIVIZER = MOD << 2;
        return take_mod(hash[r] + POSITIVIZER - mul(hash[l], power[r-l]));
    }

    // return S[l1:r1) == S[l2:r2) in O(1)
    bool is_same(int l1, int r1, int l2, int r2) const {
        return gethash(l1, r1) == gethash(l2, r2);
    }

    // get length of LCP of S[l1:r1) and S[l2:r2) in O(logN)
    int get_lcp(int l1, int r1, int l2, int r2) const {
        int len = std::min(r1 - l1, r2 - l2);
        int ok = 0, ng = len + 1, mid{};
        while(ng - ok > 1) {
            mid = (ok + ng) >> 1;
            (is_same(l1, l1 + mid, l2, l2 + mid) ? ok : ng) = mid;
        }
        return ok;
    }

    // concatenate hash of S[l1:r1) and S[l2:r2) in this order
    ull concat(int l1, int r1, int l2, int r2) const {
        auto lh = gethash(l1, r1);
        auto rh = gethash(l2, r2);
        auto res = take_mod(mul(lh, power[r2-l2])) + rh;
        if(res >= MOD) res -= MOD;
        return res;
    }
};
using RH = rolling_hash;
unsigned long long concat(RH& a, int l1, int r1, RH& b, int l2, int r2) {
    auto ah = a.gethash(l1, r1);
    auto bh = b.gethash(l2, r2);
    auto res = RH::take_mod(RH::mul(ah, b.power[r2 - l2])) + bh;
    if(res >= RH::MOD) res -= RH::MOD;
    return res;
}
int get_lcp(RH& a, int l1, int r1, RH& b, int l2, int r2) {
    int ok = 0, ng = std::min(r1 - l1, r2 - l2) + 1, mid{};
    while(ng - ok > 1) {
        mid = (ok + ng) >> 1;
        (a.gethash(l1, l1 + mid) == b.gethash(l2, l2 + mid) ? ok : ng) = mid;
    }
    return ok;
}
// s[l1, r1) < t[l2, r2)
bool is_less(string& s, string& t, RH& sh, int l1, int r1, RH& th, int l2, int r2) {
    int lcp_len = get_lcp(sh, l1, r1, th, l2, r2);
    if(lcp_len == r1 - l1 || lcp_len == r2 - l2) return r1 - l1 < r2 - l2;
    return s[l1 + lcp_len] < t[l2 + lcp_len];
}
// s[l1, r1) > t[l2, r2)
bool is_greater(string& s, string& t, RH& sh, int l1, int r1, RH& th, int l2, int r2) {
    return !is_less(s, t, sh, l1, r1, th, l2, r2);
}
