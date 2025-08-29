#include <bits/stdc++.h>
using namespace std;



//need to add hashmap implementation and also have to add latest modification time and accordingly change the use of heapify.

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
    unordered_map<T*, int> indexMap; // Map to track indices of nodes
public:
    MaxHeap():size(0){}
    void insertKey(T* node){
        arr.push_back(node);
        indexMap[node] = size;
        size++;//added in the array
        int curr=size-1;
        while(curr>0){
            int parent=(curr-1)/2;
            if(Comparator()(arr[parent],arr[curr])){
                swap(arr[curr],arr[parent]);
                indexMap[arr[curr]] = curr;
                indexMap[arr[parent]] = parent;
                curr=parent;
            }
            else break;
        }
    }
    T* getMax(){
        if(size>0)return arr[0];
        else return nullptr;
    }
    void heapify(T* node){
        int index = -1;
        auto it = indexMap.find(node);
        if (it != indexMap.end()) {
            index = it->second;
        }
        if(index==-1){
            cout<<"Node not found"<<endl;return;
        }
        // Heapify up
        int parent = (index - 1) / 2;
        while (index > 0 && Comparator()(arr[parent], arr[index])) {
            swap(arr[parent], arr[index]);
            indexMap[arr[parent]] = parent;
            indexMap[arr[index]] = index;
            index = parent;
            parent = (index - 1) / 2;
        }
        // Heapify down
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;
            if (leftChild < size && Comparator()(arr[largest], arr[leftChild])) {
                largest = leftChild;
            }
            if (rightChild < size && Comparator()(arr[largest], arr[rightChild])) {
                largest = rightChild;
            }
            if (largest != index) {
                swap(arr[index], arr[largest]);
                indexMap[arr[index]] = index;
                indexMap[arr[largest]] = largest;
                index = largest;
            } else {
                break;
            }
        }
    }
    void deleteKey(int i){//at index i
        if(i<0 or i>=size){
            cout<<"invalid_index"<<endl;return;
        }
        indexMap.erase(arr[i]);
        if(i != size-1){
            swap(arr[i],arr[size-1]);
            indexMap[arr[i]] = i;
        }
        size--;
        arr.pop_back();
        if(i<size){
            int childL=2*i+1;int childR=2*i+2;
            while(i<size){
                if(childL>=size)break;//no children
                if(childL==size-1){
                    if(Comparator()(arr[i],arr[childL])){
                        swap(arr[i],arr[childL]);
                        indexMap[arr[i]] = i;
                        indexMap[arr[childL]] = childL;
                        i=childL;
                    }
                    break;
                }
                else if(Comparator()(arr[i],arr[childL]) or Comparator()(arr[i],arr[childR])){
                    if(Comparator()(arr[childL],arr[childR])){
                        swap(arr[i],arr[childR]);
                        indexMap[arr[i]] = i;
                        indexMap[arr[childR]] = childR;
                        i=childR;
                    }
                    else{
                        swap(arr[i],arr[childL]);
                        indexMap[arr[i]] = i;
                        indexMap[arr[childL]] = childL;
                        i=childL;
                    }
                }
                else break;
                childL=2*i+1;childR=2*i+2;
            }
            int parent = (i - 1) / 2;
            // Heapify up
            while (parent >= 0 && Comparator()(arr[parent], arr[i])) {
                swap(arr[parent], arr[i]);
                indexMap[arr[parent]] = parent;
                indexMap[arr[i]] = i;
                i = parent;
                parent = (i - 1) / 2;
            }
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