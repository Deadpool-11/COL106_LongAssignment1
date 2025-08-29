#include <ctime>
#include "classes.hpp"
#include "structs.hpp"
using namespace std;

//some global variables for ease.
map<string, file*> fileMap;
MaxHeap<file,Comparator_time> timeHeap;
MaxHeap<file,Comparator_vcount> versionHeap;

void create(string filename){
    file* newFile=new file(filename);
    fileMap[filename]=newFile;
    timeHeap.insertKey(newFile);
    versionHeap.insertKey(newFile);
}

void read(string filename){
    if(fileMap.find(filename)==fileMap.end()){
        cout<<"File not found"<<endl;
    }
    else{
        cout<<fileMap[filename]->active_version->get_content()<<endl;
    }
}

void update(string filename, string newContent){
    if(fileMap.find(filename)==fileMap.end()){
        cout<<"File not found"<<endl;return;
    }
    file* currFile=fileMap[filename];
    if(currFile->active_version->get_snapshot_timestamp()!=-1){
        TreeNode* newNode=new TreeNode(currFile->total_versions,time(0),newContent);
        currFile->total_versions++;
        versionHeap.heapify(currFile);// maintaining heap even if the total_version is changed.
        currFile->version_map[currFile->total_versions-1]=newNode;
        newNode->parent=currFile->active_version;
        currFile->active_version->children.push_back(newNode);
        //currFile->active_version->set_snapshot_timestamp(time(0));
        currFile->active_version=newNode;
        timeHeap.heapify(currFile); // maintain timeHeap (file was updated)
    }
    else{
        currFile->active_version->set_content(newContent);
        timeHeap.heapify(currFile); // maintain timeHeap (file was updated)
    }
}

void snapshot(string filename, string message){
    if(fileMap.find(filename)==fileMap.end()){
        cout<<"File not found"<<endl;
        return;
    }
    file* currFile=fileMap[filename];
    currFile->active_version->set_message(message);
    currFile->active_version->set_snapshot_timestamp(time(0));
    timeHeap.heapify(currFile); // maintain timeHeap (snapshot time updated)
}

void rollback(string filename,int version_id){
    //its possible to have no version_id, i will be tackling the problem by equating with -1
    file* currfile=fileMap[filename];
    if(currfile->active_version->parent==nullptr){
        cout<<"NO PARENT"<<endl;return;
    }
    if(currfile->active_version->get_snapshot_timestamp()==-1){
        currfile->active_version->set_snapshot_timestamp(time(0));
        timeHeap.heapify(currfile); // maintain timeHeap (snapshot time updated)
    }
    // something that i added from my side. snapshoting before going to do the rollback

    if(version_id==-1){
        //rollback to the parent.
        currfile->active_version=currfile->active_version->parent;
    }
    else{
        currfile->active_version=currfile->version_map[version_id];
    }
    //assuming once it is snapshoted, it can never be changed by mutation.
    timeHeap.heapify(currfile); 
}

void history(string filename){
    file* currfile=fileMap[filename];
    map<int, TreeNode*> mapping=currfile->version_map;
    for(int i=0;i<currfile->total_versions;i++){
        if(mapping[i]->get_snapshot_timestamp()!=-1){
            cout<<"VERSION_ID: "<<i
            <<" CREATION_TIMESTAMP: "<<mapping[i]->get_created_timestamp()
            <<" SNAPSHOT_TIMESTAMP: "<<mapping[i]->get_snapshot_timestamp()
            <<" MESSAGE: "<<mapping[i]->get_message()<<endl;
        }
    }
}

void recentfiles(){
    MaxHeap<file,Comparator_time> tHeap_copy=timeHeap;
    while(!tHeap_copy.isEmpty()){
        file* currFile=tHeap_copy.extractMax();
        cout<<currFile->fileName<<endl;
    }
}

void biggesttrees(){
    MaxHeap<file,Comparator_vcount> vHeap_copy=versionHeap;
    while(!vHeap_copy.isEmpty()){
        file* currFile=vHeap_copy.extractMax();
        cout<<currFile->fileName<<endl;
    }
}

void getCommand(string line){
    stringstream ss(line);
    string command;
    try{
        ss>>command;
        if(command=="CREATE"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                create(filename);
            }
        }
        else if(command=="READ"){
            string filename;ss>>filename;
            if(fileMap.find(filename)!=fileMap.end()){
            read(filename);
            }
            else cout<<"File not found"<<endl;
        }
        else if(command=="INSERT"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                cout<<"File not found"<<endl;return;
            }
            string newContent;getline(ss,newContent);
            newContent=newContent.substr(1);
            newContent=fileMap[filename]->active_version->get_content()+newContent;
            cout<<newContent<<endl;
            update(filename,newContent);
        }
        else if(command=="UPDATE"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                cout<<"File not found"<<endl;return;
            }
            string newContent;getline(ss,newContent);
            newContent=newContent.substr(1);
            cout<<newContent<<endl;
            update(filename,newContent);
        }
        else if(command=="SNAPSHOT"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                cout<<"File not found"<<endl;return;
            }
            string message;getline(ss,message);
            message=message.substr(1);
            snapshot(filename,message);
        }
        else if(command=="ROLLBACK"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                cout<<"File not found"<<endl;return;
            }
            int version_id;
            if (!(ss >> version_id)) {
                version_id = -1;
            }
            rollback(filename,version_id);
        }
        else if(command=="HISTORY"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                cout<<"File not found"<<endl;return;
            }
            history(filename);
        }
        else if(command=="RECENT_FILES"){
            recentfiles();
        }
        else if(command=="BIGGEST_TREES"){
            biggesttrees();
        }
        else if(command=="ACTIVE_VERSION"){
            string filename;ss>>filename;
            if(fileMap.find(filename)==fileMap.end()){
                cout<<"File not found"<<endl;return;
            }
            cout<<"ACTIVE_VERSION: "<<fileMap[filename]->active_version->get_version_id()<<endl;
        }
        else{
            cout<<"INVALID INPUT"<<endl;
        }
    }
    catch(exception& e){
        cerr<<"Error: "<<e.what()<<endl;
    }
}

int main(){
    string line;
    while(getline(cin,line)){
        if(line=="EXIT")break;
        getCommand(line);
    }
    // you have to destroy all the dynamically allocated memory.
}