#include <bits/stdc++.h>
using namespace std;





//the max heap implementation
//array representation
// arr[(i-1)/2]	Returns the parent node
// arr[(2*i)+1]	Returns the left child node
// arr[(2*i)+2]	Returns the right child node
template <typename T,typename Comparator=std::less<T>>
class MaxHeap{
private:
    vector<T*> arr;
    int size;
public:
    MaxHeap():size(0){}
    void insertKey(T* node){
        arr.push_back(node);
        size++;//added in the array
        int curr=size-1;
        while(curr>0){
            int parent=(curr-1)/2;
            if(Comparator()(arr[parent],arr[curr])){
                swap(arr[curr],arr[parent]);
                curr=parent;
            }
            else break;
        }
    }
    T* getMax(){
        if(size>0)return arr[0];
        else return nullptr;
    }
    void deleteKey(int i){//at index i
        if(i<0 or i>=size){
            cout<<"invalid_index"<<endl;return;
        }
        swap(arr[i],arr[size-1]);
        size--;
        while(i<size){
            int childL=2*i+1;int childR=2*i+2;
            if(childL>=size)break;//no children
            if(childL==size-1){
                if(Comparator()(arr[i],arr[childL])){
                    swap(arr[i],arr[childL]);
                    i=childL;
                }
                break;
            }
            else if(Comparator()(arr[i],arr[childL]) or Comparator()(arr[i],arr[childR])){
                if(Comparator()(arr[childL],arr[childR])){
                    swap(arr[i],arr[childR]);
                    i=childR;
                }
                else{
                    swap(arr[i],arr[childL]);
                    i=childL;
                }
            }
            else break;
        }
    }
    T* extractMax(){
        T* maxNode=getMax();
        deleteKey(0);
        return maxNode;
    }
    T* right(int i){return (2*i+2<size)?arr[2*i+2]:nullptr;}
    T* left(int i){return (2*i+1<size)?arr[2*i+1]:nullptr;}
    int getSize(){return size;}
    bool isEmpty(){return size==0;}
};

// for min heap, you have to just invert the comparator.

