// Each time it takes O(1) to randomly pop an element from the set.
template <typename T>
class RandomSet {
 private:
  std::vector<T> vec;

 public:
  void insert(T v) { vec.push_back(v); }
  T pop() {
    int index = Util::random(0, vec.size());
    T rtv = vec[index];
    vec[index] = vec[vec.size() - 1];
    vec.pop_back();
    return rtv;
  }
  bool empty() {
    return vec.empty();
  }
  u64 size() {
    return vec.size();
  }
  void clear() {
    vec.clear();
  }
};
