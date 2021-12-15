//
//  main.cpp
//  JYY loves Median
//
//  Created by 雍语涵 on 2021/10/12.
//

#include <iostream>
using namespace std;

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
    //输入数组的总长度
    int n;
    cin >> n;
    //输入待操作的数组
    int a[n];
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    //对各个子数组进行一个序的排
    for(int i = 0; i < n; i=i+2){
        merge_sort(a, 0, i);
        cout << a[i/2] << " ";
    }
  

    return 0;
}

