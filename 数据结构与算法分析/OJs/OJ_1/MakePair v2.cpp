//
//  main.cpp
//  Make Pair 2.0
//
//  Created by 雍语涵 on 2021/10/20.
//

#include <iostream>
#include <queue>
#include <map>
using namespace std;

struct node{
    int i;   // arr1的下标
    int j;
    int sum;   // 2维数组中的元素值 arr1[i] + arr2[j]
    };
// 优先级队列的比较函数
struct cmp{
    bool operator() (const node& a, const node& b) {
        return a.sum > b.sum;
    }
};

//map<pair<int, int>, bool> judge;

vector<int> KthSmallest(int arr1[], int arr2[], int n, int k){
    vector<int> res;
    if(arr1==NULL || arr2==NULL || n<=0 || k<=0)
        return res;
  
    //int *bidx = new int[n];
    //for(int i=0; i<n; i++)
        //bidx[i] = 0; // arr2的下标
  
    priority_queue<node, vector<node>, cmp> MinHeap; //优先级队列
    for(int i=0; i<n; i++) {  //二维数组第一列入队列
        node t;
        t.i = i;//arr1下标
        t.j = 0;
        t.sum = arr1[i] + arr2[0]; // arr1[i] + arr2[0]
        MinHeap.push(t);
        //judge[pair<int, int>(i, 0)] = true;
    }
  
    while(k--){
        node cur = MinHeap.top();
        MinHeap.pop();
        res.push_back(cur.sum);
        node next;
        if(cur.j + 1 < n){
            next.i = cur.i;
            next.j = cur.j + 1;
            next.sum = arr1[next.i] + arr2[next.j];
            //if(judge[pair<int, int>(next.i, next.j)] != true)
            MinHeap.push(next);  //对应行的下一个元素入队列，arr1[t.i] + arr2[++d[t.i]]
                //judge[pair<int, int>(next.i, next.j)] = true;
        }
    }
    return res;
}


void quickSort(int s[], int l, int r)
{
    if (l< r)
    {
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j]>= x) // 从右向左找第一个小于x的数
                j--;
            if(i < j)
                s[i++] = s[j];
            while(i < j && s[i]< x) // 从左向右找第一个大于等于x的数
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quickSort(s, l, i - 1); // 递归调用
        quickSort(s, i + 1, r);
    }
}




int main(){
    int n, k;
    cin >> n >> k;
    int A[n], B[n];
    for(int i = 0; i < n; i++){
        cin >> A[i];
    }
    for(int i = 0; i < n; i++){
        cin >> B[i];
    }
    quickSort(A, 0, n-1);
    quickSort(B, 0, n-1);
    vector<int> res = KthSmallest(A, B, n, k);
    //for(int i = 0; i < k; i++){
        //cout << res[i] << " ";
    //}
    cout << res[k-1] << endl;
}
