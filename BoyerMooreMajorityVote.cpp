// returns {whether majority exists, majority element, size of majority}
template<class T>
std::tuple<bool, T, int> boyer_moore_majority_vote(const std::vector<T>& v) {
    T maj{};
    int counter{};

    for(auto&& elem : v) {
        if(counter == 0) maj = elem;
        
        if(maj == elem) ++counter;
        else --counter;
    }

    int counted = std::count(v.begin(), v.end(), maj);
    return {(int)v.size() < counted * 2, maj, counted};
}