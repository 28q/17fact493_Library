template <class S, auto op, auto e>
struct segtree{
    static_assert(is_convertible_v<decltype(op), function<S(S, S)>>, "op must work as S(S, S)");
    static_assert(is_convertible_v<decltype(e), function<S()>>, "e must work as S()");
    int n, size;
    vector<S> seg; 
    segtree() : segtree(0){}
    explicit segtree(int m) : segtree(vector<S>(m, e())){}
    explicit segtree(const vector<S> & A) : n(int(A.size()))
    {
        size = 1;
        while(size < n){
            size *= 2;
        }
        seg.assign(2 * size, e());
        for(int i = 0;i < n;i++){
            seg[size + i] = A[i];
        }
        for(int i = size - 1;i >= 1;i--){
            seg[i] = op(seg[i * 2], seg[i * 2 + 1]);
        }
    }
    void set(int p, S x){
        assert(0 <= p && p <= n);
        p += size;
        seg[p] = x;
        p >>= 1;
        while(p){
            seg[p] = op(seg[p * 2], seg[p * 2 + 1]);
            p >>= 1;
        }
    }
    S get(int p){
        assert(0 <= p && p <= n);
        return seg[p + size];
    }
    S prod(int l, int r){
        assert(0 <= l && l <= r && r <= n);
        S L = e(), R = e();
        l += size;
        r += size;
        while(l != r){
            if(l & 1){
                L = op(L, seg[l]);
                l++;
            }
            if(r & 1){
                r--;
                R = op(seg[r], R);
            }
            l >>= 1;
            r >>= 1;
        }
        return op(L, R);
    }
    S all_prod(){
        return seg[1];
    }
    template <bool (*f)(S)>
    int max_right(int l){
        return max_right(l, [](S x){return f(x);});
    }
    template <class F>
    int max_right(int l, F f){
        assert(0 <= l && l <= n);
        assert(f(e()));
        if(l == n)return n;
        l += size;
        S sum = e();
        do{
            while(l % 2 == 0){
                l >>= 1;
            }
            if(!f(op(sum, seg[l]))){
                while(l < size){
                    l *= 2;
                    if(f(op(sum, seg[l]))){
                        sum = op(sum, seg[l]);
                        l++;
                    }
                }
                return l - size;
            }
            l++;
        }while((l & -l) != l);
        return n;
    }
    template <bool (*f)(S)>
    int min_left(int r){
        return min_left(r, [](S x){return f(x);});
    }
    template <class F> int min_left(int r, F f){
        assert(0 <= r && r <= n);
        assert(f(e()));
        if(r == 0)return 0;
        r += size;
        S sum = e();
        do{
            r--;
            while(r > 1 && (r % 2)){
                r >>= 1;
            }
            if(!f(op(seg[r], sum))){
                while(r < size){
                    r *= 2;
                    r++;
                    if(f(op(seg[r], sum))){
                        sum = op(seg[r], sum);
                        r--;
                    }
                }
                return r - size + 1; 
            }
            sum = op(seg[r], sum);
        }while((r & -r) != r);
        return 0;
    }
};
