//problems: https://judge.yosupo.jp/problem/line_add_get_min
//solution: https://judge.yosupo.jp/submission/156890
#include<bits/stdc++.h>
using namespace std;
namespace IO{
  template <typename T>
  using is_broadly_integral = 
  typename conditional_t<
  is_integral_v<T>
  || is_same_v<T, __int128_t>
  || is_same_v<T, __uint128_t>,
  true_type, false_type>::type; 
  template <typename T>
  using is_broadly_signed =
  typename conditional_t<
  is_signed_v<T>
  || is_same_v<T, __int128_t>,
  true_type, false_type>::type;
  template <typename T>
  using is_broadly_unsigned =
  typename conditional_t<
  is_unsigned_v<T>
  || is_same_v<T, __uint128_t>,
  true_type, false_type>::type;
  #define ENABLE_VALUE(x) \
  template <typename T> \
  constexpr bool x##_v = x<T>::value;
    ENABLE_VALUE(is_broadly_integral);
    ENABLE_VALUE(is_broadly_signed);
    ENABLE_VALUE(is_broadly_unsigned);
  #undef ENABLE_VALUE
  #define ENABLE_HAS_TYPE(var)                                              \
  template <class, class = void>                                         \
  struct has_##var : std::false_type {};                                 \
  template <class T>                                                     \
  struct has_##var<T, std::void_t<typename T::var>> : std::true_type {}; \
  template <class T>                                                     \
  constexpr auto has_##var##_v = has_##var<T>::value;
  static constexpr int SZ = 1 << 17;
  static constexpr int offset = 64;
  char inbuf[SZ], outbuf[SZ];
  int in_left = 0, in_right = 0, out_right = 0;
  struct Pre{
    char num[40000];
    constexpr Pre() : num() {
      for (int i = 0; i < 10000; i++) {
        int n = i;
        for (int j = 3; j >= 0; j--) {
          num[i * 4 + j] = n % 10 + '0';
          n /= 10;
        }
      }
    }
  } constexpr pre;
  void load() {
    int len = in_right - in_left;
    memmove(inbuf, inbuf + in_left, len);
    in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
    in_left = 0;
  }
  void flush() {
    fwrite(outbuf, 1, out_right, stdout);
    out_right = 0;
  }
  void skip_space() {
    if (in_left + offset > in_right) load();
    while (inbuf[in_left] <= ' ') in_left++;
  }
  void single_read(char& c) {
    if (in_left + offset > in_right) load();
    skip_space();
    c = inbuf[in_left++];
  }
  void single_read(string& S) {
    skip_space();
    while (true) {
      if (in_left == in_right) load();
      int i = in_left;
      for (; i != in_right; i++) {
        if (inbuf[i] <= ' ') break;
      }
      copy(inbuf + in_left, inbuf + i, back_inserter(S));
      in_left = i;
      if (i != in_right) break;
    }
  }
  template <typename T, enable_if_t<is_broadly_integral_v<T>>* = nullptr>
  void single_read(T& x) {
    if (in_left + offset > in_right) load();
    skip_space();
    char c = inbuf[in_left++];
    [[maybe_unused]] bool minus = false;
    if constexpr (is_broadly_signed_v<T>) {
      if (c == '-') minus = true, c = inbuf[in_left++];
    }
    x = 0;
    while (c >= '0') {
      x = x * 10 + (c & 15);
      c = inbuf[in_left++];
    }
    if constexpr (is_broadly_signed_v<T>) {
      if (minus) x = -x;
    }
  }
  void rd() {}
  template <typename Head, typename... Tail>
  void rd(Head& head, Tail&... tail) {
    single_read(head);
    rd(tail...);
  }
  void single_write(const char& c) {
    if (out_right > SZ - offset) flush();
    outbuf[out_right++] = c;
  }
  void single_write(const bool& b) {
    if (out_right > SZ - offset) flush();
    outbuf[out_right++] = b ? '1' : '0';
  }
  void single_write(const string& S) {
    flush(), fwrite(S.data(), 1, S.size(), stdout);
  }
  void single_write(const char* p) { flush(), fwrite(p, 1, strlen(p), stdout); }
  template <typename T, enable_if_t<is_broadly_integral_v<T>>* = nullptr>
  void single_write(const T& _x) {
    if (out_right > SZ - offset) flush();
    if (_x == 0) {
      outbuf[out_right++] = '0';
      return;
    }
    T x = _x;
    if constexpr (is_broadly_signed_v<T>) {
      if (x < 0) outbuf[out_right++] = '-', x = -x;
    }
    constexpr int buffer_size = sizeof(T) * 10 / 4;
    char buf[buffer_size];
    int i = buffer_size;
    while (x >= 10000) {
      i -= 4;
      memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
      x /= 10000;
    }
    if(x < 100){
      if(x < 10){
        outbuf[out_right] = '0' + x;
        ++out_right;
      }else{
        uint32_t q = (uint32_t(x) * 205) >> 11;
        uint32_t r = uint32_t(x) - q * 10;
        outbuf[out_right] = '0' + q;
        outbuf[out_right + 1] = '0' + r;
        out_right += 2;
      }
    }else{
      if(x < 1000){
        memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
        out_right += 3;
      }else{
        memcpy(outbuf + out_right, pre.num + (x << 2), 4);
        out_right += 4;
      }
    }
    memcpy(outbuf + out_right, buf + i, buffer_size - i);
    out_right += buffer_size - i;
  }
  void wt() {}
  template <typename Head, typename... Tail>
  void wt(const Head& head, const Tail&... tail) {
    single_write(head);
    wt(forward<const Tail>(tail)...);
  }
  template <typename... Args>
  void wtn(const Args&... x) {
    wt(forward<const Args>(x)...);
    wt('\n');
  }
  struct Dummy{
    Dummy() { atexit(flush); }
  } dummy;
};
template<typename T, typename U, size_t sz = (1LL<<20), U eps = 1>
struct LiChaoTree{
  template <class P, class Q>
  inline bool chmax(P &a, const Q &b) {
    return (a < b ? a = b, 1 : 0);
  }
  template <class P, class Q>
  inline bool chmin(P &a, const Q &b) {
    return (a > b ? a = b, 1 : 0);
  }
  struct Line{
    T a, b;
    constexpr Line(T a, T b):a(a), b(b){};
    constexpr Line():Line(0, numeric_limits<T>::max()){};
    T operator()(U x) const {return a*x+b;};
  };
  struct Node{
    Line f;
    Node *l, *r;
  };
  int index;
  Node *NODE, *root;
  U L, R;
  LiChaoTree(U L, U R):index(0),root(nullptr),L(L),R(R){
    NODE = new Node[sz];
  };
  Node *new_node(){
  	NODE[index].f = Line();
    return &(NODE[index++]);
  }
  void insert_segment(U xl, U xr, T a, T b){
  	assert(L <= xl && xl < xr && xr <= R);
    Line f(a, b);
    if(!root) root = new_node();
    insert_segment_sub(root, xl, xr, f, L, R);
  }
  void insert_line(T a, T b){insert_segment(L, R, a, b);}
  T get_min(U x){
  	assert(L <= x && x < R);
    if(!root) return numeric_limits<T>::max();
    return min_sub(root, x, L, R);
  }
private:
  void insert_segment_sub(Node *rt, U xl, U xr, const Line &f, U l, U r){
  	chmax(xl, l);
    chmin(xr, r);
    if(xl >= xr) return;
    if(l < xl || xr < r){
      U mid = (l+r)/2;
      if(!rt->l) rt->l = new_node();
      if(!rt->r) rt->r = new_node();
      insert_segment_sub(rt->l, xl, xr, f, l, mid);
      insert_segment_sub(rt->r, xl, xr, f, mid, r);
    }else{
      insert_line_sub(rt, f, l, r);
    }
  }
  void insert_line_sub(Node *rt, const Line &f, U l, U r){
    T fl = f(l), fr = f(r-eps);
    Line g = rt->f;
    T gl = g(l), gr = g(r-eps);
    if(fl >= gl && fr >= gr) return;
    if(fl <= gl && fr <= gr){
      rt->f = f;
      return;
    }
    U mid = (l+r)/2;
    T fm = f(mid), gm = g(mid);
    if(fm <= gm){
      rt->f = f;
      if(fl < gl){
        if(!rt->r) rt->r = new_node();
        insert_line_sub(rt->r, g, mid, r);
      }else{
        if(!rt->l) rt->l = new_node();
        insert_line_sub(rt->l, g, l, mid);
      }
    }else{
      if(gl < fl){
        if(!rt->r) rt->r = new_node();
        insert_line_sub(rt->r, f, mid, r);
      }else{
        if(!rt->l) rt->l = new_node();
        insert_line_sub(rt->l, f, l, mid);
      }
    }
  }
  T min_sub(Node *rt, U x, U l, U r){
    T ret = rt->f(x);
    U mid = (l+r)/2;
    if(x < mid && rt->l) return min(ret, min_sub(rt->l, x, l, mid));
    if(x >= mid && rt->r) return min(ret, min_sub(rt->r, x, mid, r));
    return ret;
  }
};
int main(){
  int n, q;
  IO::rd(n, q);
  LiChaoTree<long long, int> lc(-1001001001, 1001001001);
  bool t;
  int p, a;
  long long b;
  while(n--){
    IO::rd(a, b);
    lc.insert_line(a, b);
  }
  while(q--){
    IO::rd(t);
    if(t){
      IO::rd(p);
      IO::wtn(lc.get_min(p));
    }else{
      IO::rd(a, b);
      lc.insert_line(a, b);
    }
  }
}
