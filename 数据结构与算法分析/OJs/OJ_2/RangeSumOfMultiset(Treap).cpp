//
//  main.cpp
//  Range sum of multiset
//  op 0: insert x; op 1: find x and return count of x; op 2: range sum.
//  Created by 雍语涵 on 2021/12/16.
//

#include <iostream>
#include <random>
#include <vector>
using namespace std;

struct treap_node{
    int key; //node的值
    int lsum, rsum; //左边的节点和和右边的节点和
    int l, r;
    int p, cnt; //分配的priority和cnt（相同的值有几个copy,主要是为了操作2）
};

struct treap_rotate{
    treap_node node[100000 + 1];
    int root, sign;
    
     //左旋
    void left_rotate(int &k){
        int t = node[k].r;
        node[k].r = node[t].l;
        node[t].l = k;
        node[t].lsum += node[k].lsum;
        node[k].rsum = node[t].lsum - node[t].cnt * node[t].key + node[k].cnt * node[k].key;
        k = t;
    }
    
    //右旋
    void right_rotate(int &k){
        int t = node[k].l;
        node[k].l = node[t].r;
        node[t].r = k;
        node[t].rsum += node[k].rsum;
        node[k].lsum = node[t].rsum - node[t].cnt * node[t].key + node[k].cnt * node[k].key;
        k = t;
    }
    
    void _insert(int &k, int x){
        //如果接口（根节点）不存在,x作为下一次的接口节点的值
        if(!k){
            k = ++sign;
            node[sign].key = node[sign].lsum = node[sign].rsum = x;
            node[sign].p = rand();
            node[sign].cnt = 1;
            node[sign].l = node[k].r = 0;
        }else{
            if(node[k].key == x){
                node[k].lsum += x;
                node[k].rsum += x;
                node[k].cnt++;
            }else if(node[k].key < x){
                node[k].rsum += x;
                _insert(node[k].r, x);
                if(node[node[k].r].p < node[k].p)
                    left_rotate(k);
            }else{
                node[k].lsum += x;
                _insert(node[k].l, x);
                if(node[node[k].l].p < node[k].p)
                    right_rotate(k);
            }
        }
    }
    
    void insert(int x){
        _insert(root, x);
    }
    
    int _find(int &k, int x){
        if(!k){
            return 0;
        }else{
            if(x == node[k].key){
                return 1;
            }
            if(x < node[k].key){
                return _find(node[k].l, x);
            }else{
                return _find(node[k].r, x);
            }
        }
    }
    
    int find(int x){
        return _find(root, x);
    }
    
    int _remove(int &k, int x){
        if(!k)
            return 0;
        if(node[k].key == x){
            if(node[k].cnt > 1){
                node[k].cnt--;
                node[k].lsum -= x;
                node[k].rsum -= x;
                return node[k].cnt + 1;
            }else{
                if(node[k].l == 0 || node[k].r == 0){
                    k = node[k].l + node[k].r;
                    return 1;
                }else if(node[node[k].l].p < node[node[k].r].p){
                    right_rotate(k);
                    return _remove(k, x);
                }else{
                    left_rotate(k);
                    return _remove(k, x);
                }
            }
        }else if(node[k].key < x){
            node[k].rsum -= x;
            return _remove(node[k].r, x);
        }else{
            node[k].lsum -= x;
            return _remove(node[k].l, x);
        }
        return 0;
    }
    
    int remove(int x){
        if(find(x))
            return _remove(root, x);
        else
            return 0;
    }
    
    int _rangesum(int &k, int x){
        if(!k) return 0;
        if(node[k].key <= x){
            return node[k].lsum + _rangesum(node[k].r, x);
        }else{
            return _rangesum(node[k].l, x);
        }
    }
    
    int range_sum(int l, int r){
        return _rangesum(root, r) - _rangesum(root, l);
    }
};

int main(){
    treap_rotate S;
    int Q, mod;
    cin >> Q >> mod;
    
    int lans = 0;
    int y, x, l, r, u, v, q;
    int cnt;
    vector<int> ans;

    for(int i = 0; i < Q; i++){
        cin >> q;
        switch(q){
            case 0:
                cin >> y;
                x = (y + lans) % mod;
                //cout << x << endl;
                S.insert(x);
                break;
            case 1:
                cin >> y;
                x = (y + lans) % mod;
                //cout << x << endl;
                //cout << S.root << endl;
                //cout << S._find(S.root, x) << endl;
                //cout << S._remove(S.root, x) << endl;
                cout << S.node[S.root].key << endl;
                cnt = S.remove(x);
                ans.push_back(cnt);
                //cout << cnt << endl;
                break;
            case 2:
                cin >> u >> v;
                l = (u + lans) % mod;
                r = (v + lans) % mod;
                if(l > r) swap(l, r);
                lans = S.range_sum(l, r);
                ans.push_back(lans);
                break;
        }
    }
    vector<int>::iterator ite = ans.begin();
    for(; ite != ans.end(); ite++){
        cout << *ite << endl;
    }
    //cout << lans << endl;
    return 0;
}

