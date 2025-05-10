/*
utility::lazysegtree_utility seg(n):
すべて0で初期化されたn要素のsegtree beatsを宣言
seg.set(i, x):
ai = x
seg.chmin(l, r, x):
i ∈ [l, r) に対して ai = min(ai, x)
seg.chmax(l, r, x):
i ∈ [l, r) に対して ai = max(ai, x)
また、chminとchmaxをすることでi ∈ [l, r) に対して ai = x とすることも可能
seg.add(l, r, x):
i ∈ [l, r) に対して ai += x
seg.get(i):
aiを返す
seg.get_max(l, r):
[l, r)の最大値を返す
seg.get_min(l, r):
[l, r)の最小値を返す
seg.get_sum(l, r):
[l, r)の総和を返す
seg.max_right_max(l, x):
l <= rであって a[r] >= xであるような最小のr(存在しなければn)を返す
seg.max_right_min(l, x):
l <= rであって a[r] <= xであるような最小のr(存在しなければn)を返す
seg.max_right_sum_lq(l, x):
l <= rであって a[l] + a[l+1] ... a[r] <= xであるような最小のr(存在しなければn)を返す(累積和が単調じゃないと壊れる)
seg.max_right_sum_gq(l, x):
l <= rであって a[l] + a[l+1] ... a[r] >= xであるような最小のr(存在しなければn)を返す(累積和が単調じゃないと壊れる)
seg.min_left_max(r, x):
l <= rであって a[l-1] >= xであるような最大のl(存在しなければ0)を返す
seg.min_left_min(r, x):
l <= rであって a[l-1] <= xであるような最大のl(存在しなければ0)を返す
seg.min_left_sum_lq(r, x):
l <= rであって a[l-1] + a[l] ... a[r-1] <= xであるような最大のl(存在しなければ0)を返す(累積和が単調じゃないと壊れる)
seg.min_left_sum_gq(r, x):
l <= rであって a[l-1] + a[l] ... a[r-1] >= xであるような最大のl(存在しなければ0)を返す(累積和が単調じゃないと壊れる)
*/
namespace utility{
  const long long BINF = 1LL << 61;
  struct lazysegtree_utility{
    const long long second_lowest(long long a, long long a2, long long b, long long b2){
      if(a == b) return min(a2, b2);
      if(a2 <= b) return a2;
      if(b2 <= a) return b2;
      return max(a, b);
    }
    const long long second_highest(long long a, long long a2, long long b, long long b2){
      if(a == b) return max(a2, b2);
      if(a2 >= b) return a2;
      if(b2 >= a) return b2;
      return min(a, b);
    }
    struct S{
      long long lo, hi, lo2, hi2, sum;
      unsigned int sz, nlo, nhi;
      bool fail;
      S():lo(BINF), lo2(BINF), hi(-BINF), hi2(-BINF), sum(0), sz(0), nlo(0), nhi(0), fail(false){}
      S(long long x, unsigned int sz_ = 1):lo(x), lo2(BINF), hi(x), hi2(-BINF), sum(x*sz_), sz(sz_), nlo(sz_), nhi(sz_), fail(false){}
    };
    S e(){return S();}
    S op(S l, S r){
      S ret;
      ret.lo = min(l.lo, r.lo), ret.hi = max(l.hi, r.hi);
      ret.lo2 = second_lowest(l.lo, l.lo2, r.lo, r.lo2);
      ret.hi2 = second_highest(l.hi, l.hi2, r.hi, r.hi2);
      ret.sum = l.sum + r.sum, ret.sz = l.sz + r.sz;
      ret.nlo = l.nlo * (l.lo <= r.lo) + r.nlo * (r.lo <= l.lo);
      ret.nhi = l.nhi * (l.hi >= r.hi) + r.nhi * (r.hi >= l.hi);
      return ret;
    }
    struct F{
      long long lb, ub, bias;
      F(long long lb_ = -BINF, long long ub_ = BINF, long long bias_ = 0):lb(lb_), ub(ub_), bias(bias_){}
      static F chmin(long long x){
        return F(-BINF, x, 0LL);
      }
      static F chmax(long long x){
        return F(x, BINF, 0LL);
      }
      static F add(long long x){
        return F(-BINF, BINF, x);
      }
    };
    F composition(F a, F b){
      F ret;
      ret.lb = max(min(b.lb + b.bias, a.ub), a.lb) - b.bias;
      ret.ub = min(max(b.ub + b.bias, a.lb), a.ub) - b.bias;
      ret.bias = a.bias + b.bias;
      return ret;
    }
    F id(){return F();}
    S mapping(F f, S x){
      if(x.sz == 0) return e();
      if(x.lo == x.hi || f.lb == f.ub || f.lb >= x.hi || f.ub <= x.lo){
        return S(min(max(x.lo, f.lb), f.ub) + f.bias, x.sz);
      }
      if(x.lo2 == x.hi){
        x.lo = x.hi2 = max(x.lo, f.lb) + f.bias;
        x.hi = x.lo2 = min(x.hi, f.ub) + f.bias;
        x.sum = x.lo * x.nlo + x.hi * x.nhi;
        return x;
      }
      if(f.lb < x.lo2 && f.ub > x.hi2){
        long long nxt_lo = max(x.lo, f.lb), nxt_hi = min(x.hi, f.ub);
        x.sum += (nxt_lo - x.lo) * x.nlo + (nxt_hi - x.hi) * x.nhi + f.bias * x.sz;
        x.lo = nxt_lo + f.bias, x.hi = nxt_hi + f.bias, x.lo2 += f.bias, x.hi2 += f.bias;
        return x;
      }
      x.fail = true;
      return x;
    }
    lazysegtree_utility():lazysegtree_utility(0){}
    explicit lazysegtree_utility(int n):lazysegtree_utility(vector<S>(n, S(0, 1)){};
    explicit lazysegtree_utility(const vector<S> &v):n((int)v.size()){
      lg = 1;
      while((1 << lg) < n) lg++;
      size = 1 << lg;
      dat.assign(2*size, e());
      lazy.assign(2*size, id());
      for(int i = 0; i < n; i++) dat[i+size] = v[i];
      for(int i = size - 1; i > 0; i--) update(i);
    };
    void set(int p, S x){
      assert(0 <= p && p < n);
      p += size;
      for(int i = lg; i > 0; i--) push(p >> i);
      dat[p] = x;
      for(int i = 1; i <= lg; i++) update(p >> i);
    }
    S get(int p){
      assert(0 <= p && p < n);
      p += size;
      for(int i = lg; i > 0; i--) push(p >> i);
      return dat[p];
    }
    S prod(int l, int r){
      assert(0 <= l && l <= r && r <= n);
      if(l == r) return e();
      l += size;
      r += size;
      for(int i = lg; i >= 1; i--){
        if(((l >> i) << i) != l) push(l >> i);
        if(((r >> i) << i) != r) push((r - 1) >> i);
      }
      S sml = e(), smr = e();
      while(l < r){
        if(l & 1) sml = op(sml, dat[l++]);
        if(r & 1) smr = op(dat[--r], smr);
        l >>= 1;
        r >>= 1;
      }
      return op(sml, smr);
    }
    long long get_sum(int l, int r){
      assert(0 <= l && l <= r && r <= n);
      return prod(l, r).sum;
    }
    long long get_max(int l, int r){
      assert(0 <= l && l <= r && r <= n);
      return prod(l, r).hi;
    }
    long long get_min(int l, int r){
      assert(0 <= l && l <= r && r <= n);
      return prod(l, r).lo;
    }
    void apply(int p, F f){
      assert(0 <= p && p < n);
      p += size;
      for(int i = lg; i > 0; i--) push(p >> i);
      dat[p] = mapping(f, dat[p]);
      for(int i = 1; i <= lg; i++) update(p >> i);
    }
    void apply(int l, int r, F f){
      assert(0 <= l && l <= r && r <= n);
      if(l == r) return;
      l += size;
      r += size;
      for(int i = lg; i > 0; i--){
        if(((l >> i) << i) != l) push(l >> i);
        if(((r >> i) << i) != r) push((r - 1) >> i);
      }
      int l2 = l, r2 = r;
      while(l2 < r2){
        if(l2 & 1) all_apply(l2++, f);
        if(r2 & 1) all_apply(--r2, f);
        l2 >>= 1, r2 >>= 1;
      }
      for(int i = 1; i <= lg; i++){
        if(((l >> i) << i) != l) update(l >> i);
        if(((r >> i) << i) != r) update((r - 1) >> i);
      }
    }
    void chmin(int l, int r, long long x){
      assert(0 <= l && l <= r && r <= n);
      apply(l, r, F::chmin(x));
    }
    void chmin(int p, long long x){
      assert(0 <= p && p < n);
      apply(p, F::chmin(x));
    }
    void chmax(int l, int r, long long x){
      assert(0 <= l && l <= r && r <= n);
      apply(l, r, F::chmax(x));
    }
    void chmax(int p, long long x){
      assert(0 <= p && p < n);
      apply(p, F::chmax(x));
    }
    void add(int l, int r, long long x){
      assert(0 <= l && l <= r && r <= n);
      apply(l, r, F::add(x));
    }
    void add(int p, long long x){
      assert(0 <= p && p < n);
      apply(p, F::add(x));
    }
    template<bool (*g)(S)>
    int max_right(int l){
      return max_right(l, [](S x){return g(x);});
    }
    template<class G>
    int max_right(int l, G g){
      assert(0 <= l && l <= n);
      assert(g(e()));
      if(l == n) return n;
      l += size;
      for(int i = lg; i >= 1; i--) push(l >> i);
      S sm = e();
      do{
        while((l & 1) == 0) l >>= 1;
          if(!g(op(sm, dat[l]))){
            while(l < size){
              push(l);
              l *= 2;
              if(g(op(sm, dat[l]))){
                sm = op(sm, dat[l]);
                l++;
              }
            }
          return l - size;
        }
        sm = op(sm, dat[l]);
        l++;
      }while((l & -l) != l);
      return n;
    }
    template <bool (*g)(S)>
    int min_left(int r){
      return min_left(r, [](S x){return g(x);});
    }
    template<class G>
    int min_left(int r, G g){
      assert(0 <= r && r <= n);
      assert(g(e()));
      if(r == 0) return 0;
      r += size;
      for(int i = lg; i >= 1; i--) push((r - 1) >> i);
      S sm = e();
      do{
        r--;
        while(r > 1 && (r % 2)) r >>= 1;
        if(!g(op(dat[r], sm))){
            while(r < size){
              push(r);
              r *= 2;
              r++;
              if(g(op(dat[r], sm))){
                sm = op(dat[r], sm);
                r--;
              }
            }
          return r + 1 - size;
        }
        sm = op(dat[r], sm);
      }while((r & -r) != r);
      return 0;
    }
    int max_right_max(int l, long long target){
      return max_right(l, [&](S x){return target > x.hi;});
    }
    int max_right_min(int l, long long target){
      return max_right(l, [&](S x){return target < x.lo;});
    }
    int max_right_sum_lq(int l, long long target){
      return max_right(l, [&](S x){return target < x.sum;});
    }
    int max_right_sum_gq(int l, long long target){
      return max_right(l, [&](S x){return target > x.sum;});
    }
    int min_left_max(int r, long long target){
      return min_left(r, [&](S x){return target > x.hi;});
    }
    int min_left_min(int r, long long target){
      return min_left(r, [&](S x){return target < x.lo;});
    }
    int min_left_sum_lq(int r, long long target){
      return min_left(r, [&](S x){return target < x.sum;});
    }
    int min_left_sum_gq(int r, long long target){
      return min_left(r, [&](S x){return target > x.sum;});
    }
    private:
    void update(int k){dat[k] = op(dat[2 * k], dat[2 * k + 1]);}
    void all_apply(int k, F f){
      dat[k] = mapping(f, dat[k]);
      if(k < size){
        lazy[k] = composition(f, lazy[k]);
        if(dat[k].fail) push(k), update(k);
      }
    }
    void push(int k) {
      all_apply(2 * k, lazy[k]);
      all_apply(2 * k + 1, lazy[k]);
      lazy[k] = id();
    }
    int n, size, lg;
    vector<S> dat;
    vector<F> lazy;
  };
};
