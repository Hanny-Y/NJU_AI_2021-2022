//
//  main.cpp
//  Make Pair
//
//  Created by 雍语涵 on 2021/10/20.
//

#include <iostream>
#include <queue>
#include <map>
using namespace std;

struct Node{
    int i;
    int j;
    int sum;
};

struct cmp{
    bool operator() (const Node& a, const Node& b) {
        return a.sum > b.sum;
    }
};

map<pair<int, int>, bool> judge;

priority_queue<Node, vector<Node>, cmp> MinHeap;

vector<Node> kthSmall(int arr1[], int arr2[], int n, int k){
    vector<Node> res;
    Node fir;
    fir.i = 0;
    fir.j = 0;
    fir.sum = arr1[0] + arr2[0];
    MinHeap.push(fir);
    judge[pair<int, int>(0, 0)] = true;
    while (k--) {
        Node cur = MinHeap.top();
        MinHeap.pop();
        res.push_back(cur);
        Node ismall;
        Node jsmall;
        if(cur.i + 1 < n){
            ismall.i = cur.i + 1;
            ismall.j = cur.j;
            ismall.sum = arr1[ismall.i] + arr2[ismall.j];
            if(judge[pair<int, int>(ismall.i, ismall.j)] != true){
                MinHeap.push(ismall);
                judge[pair<int, int>(ismall.i, ismall.j)] = true;
            }
        }
        if(cur.j + 1 < n){
            jsmall.i = cur.i;
            jsmall.j = cur.j + 1;
            jsmall.sum = arr1[jsmall.i] + arr2[jsmall.j];
            if(judge[pair<int, int>(jsmall.i, jsmall.j)] != true){
                MinHeap.push(jsmall);
                judge[pair<int, int>(jsmall.i, jsmall.j)] = true;
            }
        }
    }
    return res;
}

//合并函数
void Merge(int arr[], int low, int mid, int high){
    int i = low;
    int j = mid + 1;
    int k = 0;
    int *temp = new int[high - low + 1];
    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        }else{
            temp[k++] = arr[j++];
        }
    }
    while(i <= mid){
        temp[k++] = arr[i++];
    }
    while(j <= high){
        temp[k++] = arr[j++];
    }
    for(i = low, k = 0; i <= high; i++, k++){
        arr[i] = temp[k];
    }
    delete [] temp;
}


//归并排序
void merge_sort(int arr[], int low, int high){
    if(low >= high){
        return;
    }
    int mid = low + (high - low) / 2;
    merge_sort(arr, low, mid);
    merge_sort(arr, mid+1, high);
    Merge(arr, low, mid, high);
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
    merge_sort(A, 0, n-1);
    merge_sort(B, 0, n-1);
    vector<Node> res = kthSmall(A, B, n, k);
    for(int i = 0; i < k; i++){
        cout << res[i].sum << " ";
    }
    cout << res[k-1].sum << endl;
    return 0;
}
