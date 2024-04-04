//problems: https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_B
//solution: https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=9053488#1
#include<bits/stdc++.h>
using namespace std;
template<class T>
struct potential_dsu{
  potential_dsu():n(0), cnt(0){}
  explicit potential_dsu(int _n):n(_n), cnt(_n), p(_n, -1), weight(_n, T()){}
  int merge(int a, int b, T pt){
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    pt += potential(b) - potential(a);
    a = leader(a), b = leader(b);
    if(a == b) return a;
    cnt--;
    if(p[a] < p[b]) swap(a, b), pt = -pt;
    p[b] += p[a];
    p[a] = b;
    weight[a] = pt;
    return a;
  }
  bool same(int a, int b){
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    return leader(a) == leader(b);
  }
  int size(int a){
    assert(0 <= a && a < n);
    return -p[leader(a)];
  }
  int leader(int a){
    assert(0 <= a && a < n);
    if(p[a] < 0) return a;
    int r = leader(p[a]);
    weight[a] += weight[p[a]];
    return p[a] = r;
  }
  T potential(int a){
    leader(a);
    return weight[a];
  }
  T diff(int a, int b){
    return potential(a) - potential(b);
  }
  vector<vector<int>> groups(){
    vector<int> lb(n), gs(n);
    for(int i = 0; i < n; i++){
      lb[i] = leader(i);
      gs[lb[i]]++;
    }
    vector<vector<int>> res(n), res2;
    for(int i = 0; i < n; i++) res[i].reserve(gs[i]);
    for(int i = 0; i < n; i++) res[lb[i]].push_back(i);
    for(auto x:res) if(x.size() > 0) res2.push_back(x);
    return res2;
  }
  int count(){
    return cnt;
  }
  private:
  int n, cnt;
  vector<int> p;
  vector<T> weight;
};
int main(){
  int n, q;
  cin >> n >> q;
  potential_dsu<long long> d(n);
  while(q--){
    int t;
    cin >> t;
    if(t == 0){
      int x, y, z;
      cin >> x >> y >> z;
      d.merge(y, x, z);
    }else{
      int x, y;
      cin >> x >> y;
      if(!d.same(x, y)) cout << "?" << '\n';
      else cout << d.diff(y, x) << '\n';
    }
  }
}
