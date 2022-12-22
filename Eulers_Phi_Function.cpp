/**
 * @struct Euler Phi
 * @brief Euler Phi Function
 */
struct euler_phi {
    std::vector<int> sieve;

    /**
     * @brief Construct a new euler phi object. Time Complexity: O(MAXloglogMAX)
     * @param MAX sqrt(x_max) < MAX <= 10^7
     */
    euler_phi(int MAX) : sieve(MAX) {
        for (int i = 0; i < MAX; ++i) sieve[i] = i;
        for (int i = 2; i < MAX; ++i) {
            if (sieve[i] < i) continue;
            for (int j = i; j < MAX; j += i) {
                if (i < sieve[j]) sieve[j] = i;
            }
        }
    }

    /**
     * @brief return φ(x). Time Complexity: O(sqrt(x))
     * @return φ(x)
     */
    long long operator()(long long x) const {
        long long ret = 1;
        for (long long i = 2, b = x; i * i <= b; ++i) {
            if (sieve[i] < i) continue;
            if (x % i) continue;

            x /= i;
            ret *= i - 1;
            while (x % i == 0) ret *= i, x /= i;
        }
        if (x > 1) ret *= x - 1;
        return ret;
    }
};
