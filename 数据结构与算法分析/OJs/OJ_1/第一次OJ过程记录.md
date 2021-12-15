#### 一、Jyy Loves Median 1

看到这个题的名字，笑死我算了。

题意大概是，从键盘给一个数组，然后按要求输出子数组的中位数。

第一次提交收获了零分，原因是，中位数是排序之后的中间数！！！不是简单的位置中间的数！（怎么会有人这么蠢呐？！）

时间复杂度要求是O(nlgn)，这个我熟，MergeSort的时间复杂度，于是就有了这一版代码：

```c++
//self_made_fst_version
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

```

感觉真是合情合理呢（但其实已经超时了）。version_1通过了第一阶段的测试，其他的都是TLE，也就是说这一版代码对于$n\leq 10^3$都是适用的，而扩大n使得$n\leq 10^5$以及对n没有更多限制时都是有问题的。

初步分析下来，我觉得还是i的遍历太原始了。要想使得整个算法的时间复杂度都是O(nlgn)，也许可以尝试对merge函数做一点修改？？

然后修改了merge_sort函数，试图每次排序一个左边的顶格子数组就输出中位数，进行自己的实验之后，好像也不可以。（是存在明显的脑瘫错误啦！）

目前看来，排序部分的算法应该是没有什么问题的。

---

纠结了一下午，我在stfw的时候误入了StackOverflow论坛（程序员快乐老家），看到了上面提到了一个经典问题，也就是"Running Median"，好形象的名字，可比什么jyy's median妙多了（狗头）

这个经典问题的描述是这样的：给你n个数，当数字个数为奇数时输出此时的中位数。

最经典的思路是这样的：以第一个数作为mid值，建立一个**大顶堆**一个**小顶堆**，大于mid的加入小顶堆，小于mid 的加入大顶堆若两堆相差2个数更新mid。

想象两个堆为一个三角形和一个倒三角形，组成的沙漏形状，即为对顶堆，两个堆相接之处则为答案

维护一个大根堆一个小根堆，保持这两个堆均分数组数据。询问时输出对顶即可。

使用优先队列的方法建立对顶堆，保持两个二叉堆的数据个数相差不大于1。
当输入个数为奇数时，输出数据个数较大的那一个的top元素。
两个二叉堆的堆顶其中之一必为中位数，因为小根堆小到大，大根堆从大到小，将堆顶放在一起相当于是将一个有序序列从中间断开，断开位置即为中位数。

先去了解一下优先队列（不太记得大一的时候cjj有没有讲过惹）

- 头文件：#include < queue >

- 基本操作（与队列基本相同）：

  - top：访问队头元素
  - empty：队列是否为空
  - size：返回队列内元素个数
  - push：插入元素到队尾
  - emplace：原地构造一个元素并插入队列
  - pop：弹出队头元素
  - swap：交换内容

- 定义：priority_queue<Type, Container, Functional>

  Type 就是数据类型，Container 就是容器类型（Container必须是用数组实现的容器，比如vector,deque等等，但不能用 list。STL里面默认用的是vector），Functional 就是比较的方式，当需要用自定义的数据类型时才需要传入这三个参数，使用基本数据类型时，只需要传入数据类型，默认是大顶堆

  ```c++
  //升序队列
  priority_queue <int,vector<int>,greater<int> > q;
  //降序队列
  priority_queue <int,vector<int>,less<int> >q;
  
  //greater和less是std实现的两个仿函数（就是使一个类的使用看上去像一个函数。其实现就是类中实现一个operator()，这个类就有了类似函数的行为，就是一个仿函数类了）
  ```

搞清楚一个大概之后，就开始写了。对顶堆的方法呢，就是为了这个running median设计的。

```c++
//version 2
#include <iostream>
#include <queue>
using namespace std;

//定义大顶堆和小顶堆
priority_queue<int, vector<int>, greater<int>> min_heap;
priority_queue<int, vector<int>, less<int>> max_heap;

//对输入的数据进行处理
void add_data(int x){
    if(max_heap.empty()){
       max_heap.push(x);//我的设定是第一个数存进大顶堆
        return; //第一个bug，如果大顶堆为空应该放进去之后立刻终止函数
    }
  //如果比大顶堆的顶部大的话，就把它存进小顶堆
    if(x < max_heap.top()){
        max_heap.push(x);
    }else{
        min_heap.push(x);
    }
  //平衡两个堆的大小，相差不能超过1
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
  //读入数据
    int n;
    cin >> n;
    vector<int> medians;
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        add_data(x);
      //此时总的数应该是奇数个的，所以中位数肯定是规模大的堆的顶，联想沙漏
        if(i % 2 == 0){
            if(min_heap.size() > max_heap.size()){
                medians.push_back(min_heap.top());
            }else{
                medians.push_back(max_heap.top());
            }
        }
    }
  //输出medians内容
    for(int i = 0; i < medians.size(); i++){
        cout << medians[i] << " ";
    }
    return 0;
}
```

提交之后，全部ac了，斯莱特林加100分！！！

但是，oj给了我一个warning：

```c++
jyylovesmedian1cpp.cpp: In function ‘int main()’:
jyylovesmedian1cpp.cpp:47:22: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]
   47 |     for(int i = 0; i < all_data.size(); i++){
      |                    ~~^~~~~~~~~~~~~~~~~
```

呜呜呜，已经在难过了，int和unsigned int的比较是有可能出现问题的（为什么？建议去翻翻ics课本捏），幸好不是ics课呜呜，感谢oj不杀之恩。

那么，下一个任务！

#### Gold Miner Game

大概题意：给一个$n\times n$的棋盘，要求给出50个以内的两个集合，可以使得从这两个集合中各取一个数作为访问棋盘单元的横纵坐标，要避开规定的单元并访问剩下的所有单元。

要求使用分治法。

看上去方向很明确。可是，我完全不知道从哪里开始啊啊啊啊！

oj真的好难啊！对于我这种算法废物来说，比pa难多啦！

再次感谢互联网，提供了一点思路。看完发现，这种看上去很牛x的题目，其实就是一道浪费生命的水题，没有太高的技术含量（可幸好没有）。

第一次整了个粗略的代码上去，结果脑瘫地不知道咋的把语言设置成了java8，然后出发了oj的隐藏彩蛋。

```
编译错误
You are a troll. Trolls are not welcome. As a judge, I sentence your code to death.
```

智障oj怀疑我钓鱼（流汗黄豆）

调了语言再交一次，以为这把可以一边过，结果第二个点就wa了，第一个点合理猜测是n=3，呵呵。

由于这种题思路不错应该不会wa，所以一定是犯了脑瘫错误有弱智bug。

几次提交wa的bug如下：

- 忘了扩展其他节点了！
- 开一个新变量改值又不会改变原来队列里的值！
- 分治法之外的最后几步连x，y都分不清粗心写错了！

什么就为了这些，浪费了我一天的时间？！

事实上，de弱智bug就是这样一个过程，好想de点高级bug啊，可是废物连高级bug都写不出来。

代码有亿点丑陋，以后也许会改，也许...

```c++
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
```



#### Make Pair

听人说这是腾讯面试题，助教，真有你的。

给两个数组，要求取出两两相加之后的结果中最小的前k个数，时间复杂度也提出了要求。

又看到了熟悉的$O(nlgn)$，....

对时间复杂度这么高的要求，又是含有比较大小的过程的题，所以考虑用堆来写。

第一版代码：

```c++
#include <iostream>
#include <queue>
using namespace std;

struct node{
    int aidx;   // arr1的下标
    int value;   // 2维数组中的元素值 arr1[i] + arr2[j]
    };
// 优先级队列的比较函数
struct cmp{
    bool operator() (const node& a, const node& b) {
        return a.value > b.value;
    }
};
  
vector<int> KthSmallest(int arr1[], int arr2[], int n, int k){
    vector<int> res;
    if(arr1==NULL || arr2==NULL || n<=0 || k<=0)
        return res;
  
    int *bidx = new int[n];
    for(int i=0; i<n; i++)
        bidx[i] = 0; // arr2的下标
  
    node t;
    priority_queue<node, vector<node>, cmp> Q; //优先级队列
    for(int i=0; i<n; i++) {  //二维数组第一列入队列
        t.aidx = i;          //arr1下标
        t.value = arr1[i] + arr2[bidx[i]]; // arr1[i] + arr2[0]
        Q.push(t);
    }
  
    while(k--){
        t = Q.top();
        Q.pop();
        res.push_back(t.value);
        bidx[t.aidx]++;
        t.value = arr1[t.aidx] + arr2[bidx[t.aidx]];
        Q.push(t);  //对应行的下一个元素入队列，arr1[t.i] + arr2[++d[t.i]]
    }
    return res;
}

void quickSort(int s[], int l, int r){
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
```

测试的结果我一整个哽住了，通过了测试四，测试一直接wa了，测试二tle，而我本地测怎么测也不会wa。开始坐牢了。

算法课的时候写出了第二版。

稍微改了一下思路，第一版代码考虑还是差了点东西，找出最小堆里的顶部元素之后，仅仅是加入了下一行的一个元素。其实在把数组排序之后，已经可以确定最小的元素就是A数组和B数组的第一个元素的和然后只要以这个点为起点来扩展就可以了。

```c++
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
    //for(int i = 0; i < k; i++){
        //cout << res[i].sum << " ";
    //}
    cout << res[k-1].sum << endl;
    return 0;
}
```

第二次测试的结果终于正常点了，第一个点都过了，第二个以后的都tle了。

晚上的时候问了问计科一个学长，还是没弄清为什么会tle但是在学长解释下看了hint差不多弄明白了第二版代码不是oj想要的代码。反而是第一版更合理。

所以在第一版做了一些修改，添加了越界的判断之后，通过了第一个点和第三个点...

```c++
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
```

目前还没想出什么解决的办法，先放着坑吧，ddl之前应该是填不上了，以后有时间（肯定没有）再填这个坑和bonus的题吧。
