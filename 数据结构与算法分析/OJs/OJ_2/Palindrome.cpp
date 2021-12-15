//
//  main.cpp
//  Palindrome
//
//  Created by 雍语涵 on 2021/12/11.
//

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
using namespace std;

vector<string> wordsRev;
unordered_map<string_view, int> indices;


//判断一个字符串是否是回文串
bool ispalind(string& s)
    {
        int l = 0;
        unsigned long r = s.size()-1;
        while(l < r)
            if(s[l++] != s[r--])
                return false;
        return true;
    }

int palindromePairs(vector<string>& words) {
    int cnt  = 0;
    unordered_map<string, int> w_id; //表格
    set<unsigned long> wdLen; //存储字符串的长度
    for(int i = 0; i < words.size(); ++i) {
        w_id[words[i]] = i;//字符串idx
        wdLen.insert(words[i].size());//每个字符串长度
    }
    string front, back, revword; //前半部分，后半部分，翻转
    for(int i = 0; i < words.size()-1; ++i){
        revword = words[i];//逆序的字符串
        reverse(revword.begin(),revword.end());
        if((w_id.count(revword) > 0) && (w_id[revword] != i)){
            cnt++;
            cout << words[i] << endl;
        }
        unsigned long len = words[i].size(); //此时的字符串的长度
        for(auto it = wdLen.begin(); *it < len; ++it){
            front = words[i].substr(0, *it);
            reverse(front.begin(),front.end());
            back = words[i].substr(*it);
            if((w_id.count(front)>0) && (ispalind(back)== true)){//前缀的逆存在
                cnt++;
                cout << words[i] << endl;
                //cout << front << endl;
            }
        }
        for(auto it = wdLen.begin(); *it < len; ++it){
            front = revword.substr(0, *it);
            back = revword.substr(*it);
            if((w_id.count(front)>0) && (ispalind(back) == true)){//后缀的逆存在
                cout << words[i] << endl;
                cnt++;
            }
        }
    }
    return cnt;
}

int main() {
    int n;
    cin >> n;
    vector<string> words;
    for(int i = 0; i < n; i++){
        string s;
        cin >> s;
        words.push_back(s);
    }
    int res = palindromePairs(words);
    cout << res << endl;
    return 0;
}

