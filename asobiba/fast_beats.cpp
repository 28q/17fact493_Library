#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

namespace utility{
  const long long BINF = 1LL << 61;

  struct lazysegtree_utility{
    struct S{
      long long lo, hi, lo2, hi2, sum;
      unsigned int sz, nlo, nhi;
      bool fail;
      S():lo(BINF), lo2(BINF), hi(-BINF), hi2(-BINF), sum(0), sz(0), nlo(0), nhi(0), fail(false){}
      S(long long x, unsigned int sz_ = 1):lo(x), lo2(BINF), hi(x), hi2(-BINF), sum(x*sz_), sz(sz_), nlo(sz_), nhi(sz_), fail(false){}
    };

    struct F{
      long long lb, ub, bias;
      F(long long lb_ = -BINF, long long ub_ = BINF, long long bias_ = 0):lb(lb_), ub(ub_), bias(bias_){}
      static F chmin(long long x){ return F(-BINF, x, 0LL); }
      static F chmax(long long x){ return F(x, BINF, 0LL); }
      static F add(long long x){ return F(-BINF, BINF, x); }
    };

    int n, size, lg;
    vector<S> dat;
    vector<F> lazy;

    lazysegtree_utility():lazysegtree_utility(0){}
    explicit lazysegtree_utility(int n_):lazysegtree_utility(vector<S>(n_, S(0,1))){}
    explicit lazysegtree_utility(const vector<S> &v):n((int)v.size()){
      lg = 1;
      while((1 << lg) < n) lg++;
      size = 1 << lg;
      dat.assign(2*size, S());
      lazy.assign(2*size, F());
      for(int i = 0; i < n; i++) dat[i+size] = v[i];
      for(int i = size - 1; i > 0; i--) update(i);
    }

    S e() const{ return S(); }
    F id() const{ return F(); }

    S op(const S &l, const S &r) const{
      S ret;
      ret.lo = min(l.lo, r.lo);
      ret.hi = max(l.hi, r.hi);
      ret.lo2 = second_lowest(l.lo, l.lo2, r.lo, r.lo2);
      ret.hi2 = second_highest(l.hi, l.hi2, r.hi, r.hi2);
      ret.sum = l.sum + r.sum;
      ret.sz = l.sz + r.sz;
      ret.nlo = l.nlo * (l.lo <= r.lo) + r.nlo * (r.lo <= l.lo);
      ret.nhi = l.nhi * (l.hi >= r.hi) + r.nhi * (r.hi >= l.hi);
      return ret;
    }

    F composition(const F &a, const F &b) const{
      F ret;
      ret.lb = max(min(b.lb + b.bias, a.ub), a.lb) - b.bias;
      ret.ub = min(max(b.ub + b.bias, a.lb), b.ub) - b.bias;
      ret.bias = a.bias + b.bias;
      return ret;
    }

    S mapping(const F &f, const S &x) const{
      if(x.sz == 0) return e();
      if(x.lo == x.hi || f.lb == f.ub || f.lb >= x.hi || f.ub <= x.lo){
        long long v = min(max(x.lo, f.lb), f.ub) + f.bias;
        return S(v, x.sz);
      }
      if(x.lo2 == x.hi){
        S y = x;
        y.lo = y.hi2 = max(y.lo, f.lb) + f.bias;
        y.hi = y.lo2 = min(y.hi, f.ub) + f.bias;
        y.sum = y.lo * y.nlo + y.hi * y.nhi;
        return y;
      }
      if(f.lb < x.lo2 && f.ub > x.hi2){
        long long nxt_lo = max(x.lo, f.lb);
        long long nxt_hi = min(x.hi, f.ub);
        S y = x;
        y.sum += (nxt_lo - x.lo) * x.nlo + (nxt_hi - x.hi) * x.nhi + f.bias * x.sz;
        y.lo = nxt_lo + f.bias;
        y.hi = nxt_hi + f.bias;
        y.lo2 += f.bias;
        y.hi2 += f.bias;
        return y;
      }
      S y = x;
      y.fail = true;
      return y;
    }

    void set(int p, S x){
      assert(0 <= p && p < n);
      p += size;
      for(int i = lg; i > 0; i--) push(p >> i);
      dat[p] = x;
      for(int i = 1; i <= lg; i++) update(p >> i);
    }
    void set(int i, long long x){ set(i, S(x,1)); }

    void apply(int p, const F &f){
      assert(0 <= p && p < n);
      p += size;
      for(int i = lg; i > 0; i--) push(p >> i);
      dat[p] = mapping(f, dat[p]);
      for(int i = 1; i <= lg; i++) update(p >> i);
    }

    void apply(int l, int r, const F &f){
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
        l2 >>= 1;
        r2 >>= 1;
      }
      for(int i = 1; i <= lg; i++){
        if(((l >> i) << i) != l) update(l >> i);
        if(((r >> i) << i) != r) update((r - 1) >> i);
      }
    }

    void chmin(int l, int r, long long x){ apply(l, r, F::chmin(x)); }
    void chmin(int p, long long x){ apply(p, F::chmin(x)); }
    void chmax(int l, int r, long long x){ apply(l, r, F::chmax(x)); }
    void chmax(int p, long long x){ apply(p, F::chmax(x)); }
    void add(int l, int r, long long x){ apply(l, r, F::add(x)); }
    void add(int p, long long x){ apply(p, F::add(x)); }

    void set(int l, int r, long long x){
      assert(0 <= l && l <= r && r <= n);
      apply(l, r, F::chmin(x));
      apply(l, r, F::chmax(x));
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

    long long get_sum(int l, int r){ return prod(l, r).sum; }
    long long get_max(int l, int r){ return prod(l, r).hi; }
    long long get_min(int l, int r){ return prod(l, r).lo; }

  private:
    void update(int k){ dat[k] = op(dat[2*k], dat[2*k+1]); }
    void all_apply(int k, const F &f){
      dat[k] = mapping(f, dat[k]);
      if(k < size){
        lazy[k] = composition(f, lazy[k]);
        if(dat[k].fail){ push(k); update(k); }
      }
    }
    void push(int k){
      all_apply(2*k, lazy[k]);
      all_apply(2*k+1, lazy[k]);
      lazy[k] = id();
    }

    long long second_lowest(long long a, long long a2, long long b, long long b2) const{
      if(a == b) return min(a2, b2);
      if(a2 <= b) return a2;
      if(b2 <= a) return b2;
      return max(a, b);
    }
    long long second_highest(long long a, long long a2, long long b, long long b2) const{
      if(a == b) return max(a2, b2);
      if(a2 >= b) return a2;
      if(b2 >= a) return b2;
      return min(a, b);
    }
  };
}

