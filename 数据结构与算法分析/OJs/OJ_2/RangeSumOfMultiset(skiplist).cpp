//
//  main.cpp
//  SkipListSolution
//
//  Created by 雍语涵 on 2021/12/17.
//

#include <iostream>
#include <climits>
#include <random>
#define MAXN 500000
using namespace std;


const int max_level = 15;
struct skip_node {
    int64_t key;
    int w, level;
    skip_node **nxt;
    int64_t *d_sum;
    skip_node(int64_t k, int level) : key(k), level(level) {
        nxt = new skip_node *[level];
        d_sum = new int64_t[level];
        for (int i = 0; i < level; ++i) {
            nxt[i] = nullptr;
            d_sum[i] = 0;
        }
        w = 1;
    }
};

class skip_list {
   private:
    skip_node *head, *tail;
    
    int random_level() {
        int random_level = 1;
        static int seed = time(NULL);
        static default_random_engine e(seed);
        static uniform_int_distribution<int> u(0, 1);
        while (u(e) % 2 && random_level < max_level)
            random_level++;
        return random_level;
    };
    
    skip_node *find_des(int64_t x) {
        skip_node *tmp = head;
        for (int i = max_level - 1; i >= 0; i--) {
            while (tmp->nxt[i] != nullptr && tmp->nxt[i]->key < x)
                tmp = tmp->nxt[i];
        }
        return tmp;
    }
    
    int64_t sum(int64_t x) {
        skip_node *tmp = head;
        int64_t sum = 0;
        for (int i = max_level - 1; i >= 0; i--) {
            while (tmp->nxt[i] != nullptr && tmp->nxt[i]->key <= x) {
                tmp = tmp->nxt[i];
                sum += tmp->d_sum[i];
            }
        }
        return sum;
    }

   public:
    skip_list() {
        head = new skip_node(LLONG_MIN, max_level);
        tail = new skip_node(LLONG_MAX, max_level);
        for (int i = 0; i < max_level; i++) {
            head->nxt[i] = tail;
        }
    }
    
    void insert(int64_t x){
        skip_node *node = find_des(x)->nxt[0];
        if (node->key == x) {
            node->w++;
            for (int i = 0; i < node->level; ++i) node->d_sum[i] += x;
            skip_node *tmp = head;
            for (int i = max_level - 1; i >= node->level; i--){
                while (tmp->nxt[i] != nullptr && tmp->nxt[i]->key < x)
                    tmp = tmp->nxt[i];
                tmp->nxt[i]->d_sum[i] += x;
            }
        } else {
            int r_level = random_level();
            skip_node *new_node = new skip_node(x, r_level);
            skip_node *tmp = head;
            for (int i = max_level - 1; i >= 0; i--) {
                int64_t acc_sum = 0;
                while (tmp->nxt[i] != nullptr && tmp->nxt[i]->key < x) {
                    tmp = tmp->nxt[i];
                    acc_sum += tmp->d_sum[i];
                }
                if (i < r_level) {
                    if (i + 1 < r_level) new_node->d_sum[i + 1] = acc_sum;
                    new_node->nxt[i] = tmp->nxt[i];
                    tmp->nxt[i] = new_node;
                } else {
                    tmp->nxt[i]->d_sum[i] += x;
                }
            }
            new_node->d_sum[0] = x;
            for (int i = 1; i < new_node->level; i++) {
                new_node->d_sum[i] += new_node->d_sum[i - 1];
                new_node->nxt[i]->d_sum[i] -= new_node->d_sum[i] - x;
            }
        }
    }
    
    int remove(int64_t x){
        skip_node *node = find_des(x)->nxt[0];
        if (node->key != x){
            return 0;
        } else {
            if (node->w > 1){
                node->w--;
                for (int i = 0; i < node->level; i++) node->d_sum[i] -= x;
                skip_node *tmp = head;
                for (int i = max_level - 1; i >= node->level; i--) {
                    while (tmp->nxt[i] != nullptr && tmp->nxt[i]->key < x)
                        tmp = tmp->nxt[i];
                    tmp->nxt[i]->d_sum[i] -= x;
                }
                return node->w + 1;
            } else {
                skip_node *tmp = head;
                for (int i = max_level - 1; i >= 0; i--) {
                    while (tmp->nxt[i] != nullptr && tmp->nxt[i]->key < x)
                        tmp = tmp->nxt[i];
                    if (i < node->level) {
                        tmp->nxt[i]->nxt[i]->d_sum[i] +=
                            tmp->nxt[i]->d_sum[i] - x;
                        tmp->nxt[i] = tmp->nxt[i]->nxt[i];
                    } else {
                        tmp->nxt[i]->d_sum[i] -= x;
                    }
                }
                delete node;
                return 1;
            }
        }
    }
    
    int64_t rangesum(int64_t l, int64_t r){
        return sum(r) - sum(l);
        
    }
};

skip_list S;
int64_t Q, mod, lans, y, x, l, r, u, v, cnt;
int q;

int main() {
    cin >> Q >> mod;
    for (int i = 0; i < Q; i++) {
        cin >> q;
        switch (q) {
            case 0:
                cin >> y;
                x = (y + lans) % mod;
                S.insert(x);
                break;
            case 1:
                cin >> y;
                x = (y + lans) % mod;
                cnt = S.remove(x);
                cout << cnt << endl;
                break;
            case 2:
                cin >> u >> v;
                l = (u + lans) % mod;
                r = (v + lans) % mod;
                if (l > r) swap(l, r);
                lans = S.rangesum(l, r);
                cout << lans << endl;
                break;
        }
    }
    return 0;
}
