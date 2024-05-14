//problems: https://atcoder.jp/contests/abc353/tasks/abc353_e
//solution: https://atcoder.jp/contests/abc353/submissions/53470465
#include<bits/stdc++.h>
using namespace std;
template<typename T>
struct Trie{
  struct Node{
    int sum = 0;
    map<T, Node*> m;
  };
  Node* root = new Node;
  void mk(Node*& t){
    if(!t){
      t = new Node;
      cnt++;
    }
  }
  void insert_vector(const vector<T>& s){
    Node* c = root;
    c->sum++;
    for(const T &x:s){
      mk(c->m[x]);
      c = c->m[x];
      c->sum++;
    }
  }
  void insert_string(const string& s){
    if(!is_same<T, char>::value) assert(false);
    Node* c = root;
    c->sum++;
    for(const T &x:s){
      mk(c->m[x]);
      c = c->m[x];
      c->sum++;
    }
  }
  int find_vector(const vector<T>& s){
    Node* c = root;
    for(const T &x:s){
      mk(c->m[x]);
      c = c->m[x];
    }
    return c->sum;
  }
  int find_string(const string& s){
    Node* c = root;
    for(const T &x:s){
      mk(c->m[x]);
      c = c->m[x];
    }
    return c->sum;
  }
  long long solve(const string& s){
    long long ret = 0;
    Node* c = root;
    for(const T &x:s){
      mk(c->m[x]);
      c = c->m[x];
      ret += c->sum;
    }
    return ret;
  }
  int count(){
    return cnt;
  }
  private:
  int cnt = 1;
};
int main(){
  int n;
  cin >> n;
  Trie<char> t;
  long long ans = 0;
  for(int i = 0; i < n; i++){
    string s;
    cin >> s;
    ans += t.solve(s);
    t.insert_string(s);
  }
  cout << ans << endl;
}
