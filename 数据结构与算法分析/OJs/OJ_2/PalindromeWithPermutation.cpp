//
//  main.cpp
//  Palindrome including permutation
//
//  Created by 雍语涵 on 2021/12/15.
//

#include <iostream>
#include <algorithm>
#include <string>
#define MAXN 10000000
using namespace std;

int dp[MAXN] = {0};

int main(){
    int k;
    string str;
    unsigned long long ans = 0;
    cin >> k;
    
    for(int i = 0; i < k; i++){
        cin >> str;
        for(int j = 0; j < str.size(); j++){
            dp[i] ^= 1 << (str[j] - 'a');
        }
    }
    
    sort(dp, dp + k);
    for(int i = 0; i < k; i++){
        int *l, *r;
        l = lower_bound(dp, dp + k, dp[i]);
        r = upper_bound(dp, dp + k, dp[i]);
        ans += r - l - 1;
        
        for(int j = 0; j < 26; j++){
            l = lower_bound(dp, dp + k, dp[i] ^ (1 << j));
            r = upper_bound(dp, dp + k, dp[i] ^ (1 << j));
            ans += r - l;
        }
    }
    
    cout << ans / 2 << endl;
    return 0;
}
