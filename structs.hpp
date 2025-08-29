#include <bits/stdc++.h>
using namespace std;

struct TreeNode{
private:
    int version_id ;
    string content ;
    string message ; // Empty if not a snapshot
    time_t created_timestamp ;
    time_t snapshot_timestamp ; // -1 if not a snapshot
public:
    TreeNode * parent ;
    vector <TreeNode*> children ;
    TreeNode(int version_id, time_t created_timestamp, string content="", string message="")
        : version_id(version_id), content(content), message(message), created_timestamp(created_timestamp), snapshot_timestamp(-1), parent(nullptr) {}
    void addChild(TreeNode* child){
        children.push_back(child);
        child->parent=this;
    }
    int get_version_id(){return version_id;}
    string get_content(){return content;}
    time_t get_created_timestamp(){return created_timestamp;}
    time_t get_snapshot_timestamp(){return snapshot_timestamp;}
    string get_message(){return message;}
    void set_content(string new_content){
        if(snapshot_timestamp==-1){
            content=new_content;
        }
        else{
            cout<<"IMMUTABLE"<<endl;
        }
    }
    void set_snapshot_timestamp(time_t timestamp){
        if(snapshot_timestamp!=-1){
            cout<<"ALREADY A SNAPSHOT"<<endl;
            return;
        }
        snapshot_timestamp=timestamp;
    }
    void set_message(string new_message){
        if(snapshot_timestamp==-1){
            message=new_message;
        }
        else{
            cout<<"IMMUTABLE"<<endl;
        }
    }
};


struct file{
    string fileName;
    TreeNode * root ; // Your implementation of the tree
    TreeNode * active_version ;
    map <int , TreeNode *> version_map ; // Your implementation of the HashMap
    int total_versions ;
    file(string fileName):fileName(fileName){
        TreeNode* newNode=new TreeNode(0,time(0));
        version_map[0]=newNode;
        root=newNode;
        active_version=newNode;
        total_versions=1;
    }
    
};



struct Comparator_time{
    bool operator()(file* a, file* b) const {
        return a->active_version->get_created_timestamp() < b->active_version->get_created_timestamp();
    }
};
struct Comparator_vcount{
    bool operator()(file* a, file* b) const {
        return a->total_versions < b->total_versions;
    }
};