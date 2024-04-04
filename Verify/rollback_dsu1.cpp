//problems: https://judge.yosupo.jp/problem/persistent_unionfind
//solution: https://judge.yosupo.jp/submission/200162
#include<bits/stdc++.h>
using namespace std;
struct rollback_dsu{
  rollback_dsu():n(0), cnt(0), inner_snap(0){}
  explicit rollback_dsu(int _n):n(_n), cnt(_n), inner_snap(0), p(_n, -1){}
  int merge(int a, int b){
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    int x = leader(a), y = leader(b);
    history.emplace(x, p[x]);
    history.emplace(y, p[y]);
    history2.emplace(cnt);
    if(x == y) return x;
    cnt--;
    if(-p[x] < -p[y]) swap(x, y);
    p[x] += p[y];
    p[y] = x;
    return x;
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
    return p[a] = leader(p[a]);
  }
  void undo(){
    p[history.top().first] = history.top().second;
    history.pop();
    p[history.top().first] = history.top().second;
    history.pop();
    cnt = history2.top();
    history2.pop();
  }
  void snapshot(){
    inner_snap = (int)history2.size();
  }
  int get_state(){
    return (int)history2.size();
  }
  void rollback(int state = -1){
    if(state == -1) state = inner_snap;
    assert(state <= (int)history2.size());
    while(state < (int)history2.size()) undo();
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
  int inner_snap;
  vector<int> p;
  stack<pair<int, int>> history;
  stack<int> history2;
};
int main(){
  int n, q;
  cin >> n >> q;
  vector<vector<int>> Q(q, vector<int>(4));
  vector<vector<vector<int>>> g1(q+1);
  vector<vector<vector<int>>> g2(q+1);
  for(int i = 0; i < q; i++){
    for(int j = 0; j < 4; j++){
      cin >> Q[i][j];
    }
    Q[i][1]++;
    if(Q[i][0] == 0){
      g1[Q[i][1]].push_back({i+1, Q[i][2], Q[i][3]});
    }else{
      g2[Q[i][1]].push_back({i+1, Q[i][2], Q[i][3]});
    }
  }
  vector<int> ans(q+1, -1);
  rollback_dsu d(n);
  auto dfs = [&](auto f, int s) -> void{
    for(auto x:g2[s]){
      ans[x[0]] = d.same(x[1], x[2]);
    }
    for(auto x:g1[s]){
      d.merge(x[1], x[2]);
      f(f, x[0]);
      d.undo();
    }
  };
  dfs(dfs, 0);
  for(int i = 0; i < q; i++) if(ans[i+1] != -1) cout << ans[i+1] << '\n';
}
