#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;
#define pb push_back
#define bame_heavy(n) (b_factor(n) > 1)
#define dane_heavy(n) (b_factor(n) < -1)
 class node{
    
    public:

    int height,data;
    node* left;
    node* right;
    node(int val){
        data=val;    
        height=0;
        left=NULL;
        right=NULL;
    }

 };


 node *root=NULL; 

 int height(node *a){
     if(a==NULL) return -1;
     return a->height;
 }
 int b_factor(node *a){
     if(a==NULL) return 0;
     int balance_f=height(a->left)-height(a->right);
     return balance_f;
 }
 void update_height(node *n){
     if(n==NULL) return;
     int max_h_child=max(height(n->left),height(n->right));
     n->height=max_h_child+1;
 }
 node*  rotate_left(node *a){
     node *b=a->right;
     node *temp=b->left;
     b->left=a;
     a->right=temp;
     update_height(a);
     update_height(b);
     return b;
 }
 node* rotate_right(node *a){
     node *b=a->left;
     node *temp=b->right;
     b->right=a;
     a->left=temp;
     update_height(a);
     update_height(b);
     return b;
 }
node* find_min(node *a){
    while(a->left!=NULL){
        a=a->left;
    }
    return a;
}
node* rebalance(node *a) {
    if (a == NULL) return NULL;

    update_height(a);
    //int bf = b_factor(a);

    if (bame_heavy(a)) {     // here im doing LL and lr
        node* l_child = a->left;
        if (b_factor(l_child) < 0) {
            a->left = rotate_left(l_child);
        }
        return rotate_right(a);
    }

    if (dane_heavy(a)) {  //here rr,rl
        node* r_child = a->right;
        if (b_factor(r_child) > 0) {
            a->right = rotate_right(r_child);
        }
        return rotate_left(a);
    }

    return a;
}
bool find(int key)
{
    node *current = root;

    while (current != NULL)
    {
        if (key == current->data)
            return true;

        if (key < current->data)
            current = current->left;
        else
            current = current->right;
    }

    return false;
}
node *inserting_process(node*a,bool &insert_done,int key){  //recirsion for insert
    if(a==NULL){
        insert_done=true;
        return new node(key);
    }
    if(key<a->data)a->left=inserting_process(a->left,insert_done,key);
    else if(key>a->data)a->right=inserting_process(a->right,insert_done,key);
    else{
        insert_done=false;
        return a;
    }
    return rebalance(a);
}
bool insert (int key){
    bool insert_done=false;
    root=inserting_process(root,insert_done,key);
    return insert_done;
}
void inorder_traversing(node *a,vector<int> &v){
    if(a==NULL) return;
    inorder_traversing(a->left,v);
    v.pb(a->data);
    inorder_traversing(a->right,v);
}

vector<int> traverse()
{
    vector<int> v;
    inorder_traversing(root, v);
    return v;
}


node* deleting_process(node*a,bool &delete_done,int key){
    if(a==NULL)return NULL;
    if(key<a->data)a->left=deleting_process(a->left,delete_done,key);
    else if(key>a->data)a->right=deleting_process(a->right,delete_done,key);
    else{
        delete_done=true;
        if(a->left==NULL && a->right==NULL){
            delete a;
            return NULL;
        }
        if(a->left==NULL){
            node *temp=a->right;
            delete a;
            return temp;
        }
        if(a->right==NULL){
            node *temp=a->left;
            delete a;
            return temp;
        }
        node *successor=find_min(a->right);   
        a->data=successor->data;
        a->right=deleting_process(a->right,delete_done,successor->data);
    }
    return rebalance(a);
}
bool DELETE(int key){
    bool delete_done=false;
    root=deleting_process(root,delete_done,key);
    return delete_done;
}
string printing_format(node *a){
    if(a==NULL) return "";
    if(a->left==NULL && a->right==NULL){
        return to_string(a->data);
    }
    return to_string(a->data)+"("
            +printing_format(a->left)+","
            +printing_format(a->right)+")";
}
void free_memory(node *node)
{
    if (node == nullptr)return;
    free_memory(node->left);
    free_memory(node->right);
    delete node;
}
struct timer_calcualtion {
    long long c= 0;
    long long total = 0; //tatal in nanosecond

    double average() {
        if(c==0) return 0;
        return (double)total / c;
    }
    void add_count(long long t) {
        c++;
        total += t;
    }

};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Invalid arguments.";
        return 1;
    }

    ifstream input(argv[1]);
    ofstream output(argv[2]);
     timer_calcualtion time_for_insert;
 timer_calcualtion traversal_time;  
timer_calcualtion time_for_delete;
timer_calcualtion time_for_find;
    char input_command;
    int C;

    while (input>>input_command)
    {
        if(input_command=='I')
        {
            input >> C;
            auto start_time = chrono::steady_clock::now();
            
            bool result=insert(C);
            auto end_time = chrono::steady_clock::now();
            time_for_insert.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());   
            if(result)output << printing_format(root) << '\n';
            else
                output << "duplicate\n";
            
        }

        else if(input_command=='D')
        {
            input >> C;
            auto start_time = chrono::steady_clock::now();
            bool result=DELETE(C);
            auto end_time = chrono::steady_clock::now();
            time_for_delete.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());   
            if(result)output << printing_format(root) << '\n';
            else
                output << "not found\n";
        }

        else if(input_command == 'F')
        {
            input>>C;
            auto start_time = chrono::steady_clock::now();
            bool result=find(C);
            auto end_time = chrono::steady_clock::now();
            time_for_find.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
            if (result)output << "found\n";
            else
                output <<"not found\n";
        }

        else if(input_command=='T')
        {
            auto start_time = chrono::steady_clock::now();
        
            vector<int> v=traverse();
        auto end_time = chrono::steady_clock::now();
        traversal_time.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
            int size=v.size();
            for (int i=0;i<size; i++)
            {
                if (i>0)
                    output <<" ";

                output <<v[i];
            }

            output <<'\n';
        }
    }
cout<<"operation,count,total_ns,average_ns\n";

cout<<"insert,"
    <<time_for_insert.c<<","
    <<time_for_insert.total<<","
    <<time_for_insert.average()<<"\n";

cout<<"delete,"
    <<time_for_delete.c<<","
    <<time_for_delete.total<<","
    <<time_for_delete.average()<<"\n";

cout<<"find,"
    <<time_for_find.c<<","
    <<time_for_find.total<<","
    <<time_for_find.average()<<"\n";

cout<<"traverse,"
    <<traversal_time.c<<","
    <<traversal_time.total<<","
    <<traversal_time.average()<<"\n";
    free_memory(root);

    return 0;
}