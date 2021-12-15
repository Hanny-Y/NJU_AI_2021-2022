//
//  main.cpp
//  Gold Miner Game
//
//  Created by 雍语涵 on 2021/10/15.
//

#include <iostream>
#include <queue>
using namespace std;
struct step{
    vector<int> x;
    vector<int> y;
    void expand(int size, bool flag){
        int length = int(x.size());
        int tem;
        if(flag){
            for(int i = 0; i < length; i++){
                tem = x[i];
                x.push_back(tem + size);
            }
            int length_ = int(y.size());
            for(int i = 0; i < length_; i++){
                tem = y[i];
                y.push_back(tem + size);
            }
        }else{
            for(int i = 0; i < length; i++){
                tem = x[0];
                x.erase(x.begin());
                x.push_back(tem + 1);
                x.push_back(tem + size + 1);
            }
            int length_ = int(y.size());
            for(int i = 0; i < length_; i++){
                tem = y[0];
                y.erase(y.begin());
                y.push_back(tem + 1);
                y.push_back(tem + size + 1);
            }
        }
    }
    void Print(){
        unsigned long length = x.size();
        if(length > 0){
            for(int i = 0; i < length; i++){
                cout << x[i] << " ";
            }
            cout << endl;
        }
        length = y.size();
        if(length > 0){
            for(int i = 0; i < length; i++){
                cout << y[i] << " ";
            }
            cout << endl;
        }
    }
};

void gprint(vector<step> T){
    unsigned long length = T.size();
    step s;
    for(int i = 0; i < length; i++){
        s = T[i];
        s.Print();
    }
}

vector<step> resolve(int n, vector<step> T){
    unsigned long l;
    step s;
    vector<int> xy;
    if(n == 2){
        step tmp;
        tmp.x.push_back(1);
        tmp.y.push_back(2);
        T.push_back(tmp);
    }else if(n == 3){
        step fir, sec, thi;
        fir.x.push_back(1);
        fir.y.push_back(2);
        fir.y.push_back(3);
        sec.x.push_back(2);
        sec.y.push_back(3);
        thi.x.push_back(3);
        thi.y.push_back(1);
        T.push_back(fir);
        T.push_back(sec);
        T.push_back(thi);
    }else{
        bool flag = n%2==0;
        n = n / 2;
        T = resolve(n, T);
        l = T.size();
        s = T[0];
        vector<int> tmp = s.y;
        if(T[0].y.size() == 1){
            T.pop_back();
            l--;
        }else{
            T[0].y.pop_back();
            T[0].expand(n, flag);
        }
        for(int i = 1; i < l; i++){
            T[i].expand(n, flag);
        }
        if(flag){
            step S;
            S.y.push_back(1);
            for(int i = 2; i <= n; i++){
                S.x.push_back(i+n);
                S.y.push_back(i);
            }
            T.insert(T.begin(), S);
            step st;
            st.x.push_back(n+1);
            st.y.push_back(1);
            st.y.push_back(2*n);
            T.insert(T.begin(), st);
            step ste;
            ste.y.push_back(n+1);
            for(int i = 2; i <= n; i++){
                ste.x.push_back(i);
                ste.y.push_back(i+n);
            }
            T.insert(T.begin(), ste);
            step Step;
            Step.x.push_back(1);
            Step.y.push_back(n);
            for(int i = 1; i <= n; i++){
                Step.y.push_back(n+i);
            }
            T.insert(T.begin(), Step);
        }else{
            n = n + 1;
            step S;
            S.y.push_back(1);
            S.y.push_back(2);
            for(int i = 3; i <= n; i++){
                S.x.push_back(n + i - 1);
                S.y.push_back(i);
            }
            T.insert(T.begin(), S);
            step st;
            st.x.push_back(n+1);
            st.y.push_back(1);
            st.y.push_back(2);
            st.y.push_back(2*n-1);
            T.insert(T.begin(), st);
            
            step ste;
            ste.y.push_back(1);
            ste.y.push_back(n+1);
            for(int i = 3; i <= n; i++){
                ste.x.push_back(i);
                ste.y.push_back(n+i-1);
            }
            T.insert(T.begin(), ste);
            
            step Step;
            Step.x.push_back(2);
            Step.y.push_back(n);
            for(int i = 1; i < n; i++){
                Step.y.push_back(n+i);
            }
            T.insert(T.begin(), Step);
            
            step STep;
            STep.x.push_back(1);
            STep.y.push_back(2);
            for(int i = 3; i < 2*n; i++){
                STep.y.push_back(i);
            }
            T.insert(T.begin(), STep);
        }
    }
    return T;
}

void solve(int n){
    vector<step> T;
    T = resolve(n, T);
    gprint(T);
}

int main(){
    //输入行、列
    int n;
    cin >> n;
    
    solve(n);
    return 0;
}
