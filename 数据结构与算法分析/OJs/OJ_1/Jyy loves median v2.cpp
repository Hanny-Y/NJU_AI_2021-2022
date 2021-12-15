//
//  main.cpp
//  Jyy Loves Median1 v2
//
//  Created by 雍语涵 on 2021/10/15.
//

#include <iostream>
#include <queue>
using namespace std;

//定义大顶堆和小顶堆
priority_queue<int, vector<int>, greater<int>> min_heap;
priority_queue<int, vector<int>, less<int>> max_heap;

//对输入的数据进行处理
void add_data(int x){
    if(max_heap.empty()){
       max_heap.push(x);
        return;
    }
    if(x < max_heap.top()){
        max_heap.push(x);
    }else{
        min_heap.push(x);
    }
    while(min_heap.size() > max_heap.size() + 1){
        max_heap.push(min_heap.top());
        min_heap.pop();
    }
    while(min_heap.size() + 1 < max_heap.size()){
        min_heap.push(max_heap.top());
        max_heap.pop();
    }
}

//main函数
int main(){
    int n;
    cin >> n;
    vector<int> all_data;
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        add_data(x);
        if(i % 2 == 0){
            if(min_heap.size() > max_heap.size()){
                all_data.push_back(min_heap.top());
            }else{
                all_data.push_back(max_heap.top());
            }
        }
    }
    for(int i = 0; i < all_data.size(); i++){
        cout << all_data[i] << " ";
    }
    return 0;
}
