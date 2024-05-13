/*
LiChaoTree<long long, int> lc(-1e9, 1e9);
宣言
insert_line(a, b)
ax+bを挿入
get_min(x)
座標xでの最小値を返す
insert_segment(l, r, a, b)
ax+b(l <= x <= r)を挿入
*/
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
