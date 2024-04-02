/*
基本的な関数はdsuと同じ

undo() 1個前の操作を取り消す
snapshot() dsuの状態を保存する
rollback(int state = -1) 
  state = -1の時snapshot()で保存した状態に戻る
  state >= 0の時mergeがstate回呼ばれた時の状態に戻る
get_state() 現在のmergeが呼ばれた回数を返す
*/
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
