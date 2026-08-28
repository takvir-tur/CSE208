#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <climits>
#include <chrono>
using namespace std;
#define pb push_back
#define bame_heavy(n) (b_factor(n) > 1)
#define dane_heavy(n) (b_factor(n) < -1)
 class node{
    
    public:

    int start,end,id;
    int maxend;
    int height;
    node* left;
    node* right;
    node(int i,int s,int e){
        left=NULL;
        right=NULL;
        start=s;
        end=e;
        id=i;
        maxend=e;
        height=0;
    }

 };
 node *root=NULL;
 unordered_map<int, pair<int,int>> interval_map;


 int height(node *a){
     if(a==NULL) return -1;
     return a->height;
 }
 int b_factor(node *a){
     if(a==NULL) return 0;
     int balance_f=height(a->left)-height(a->right);
     return balance_f;
 }
 int get_max_end(node *a){
     if(a==NULL) return INT_MIN;
     return a->maxend;
 }
 void update_height_maxend(node *n){
     if(n==NULL) return;
     int max_h_child=max(height(n->left),height(n->right));
     n->height=max_h_child+1;
     n->maxend=max(n->end,max(get_max_end(n->left),get_max_end(n->right)));

 }
 node*  rotate_left(node *a){
     node *b=a->right;
     node *temp=b->left;
     b->left=a;
     a->right=temp;
     update_height_maxend(a);
     update_height_maxend(b);
     return b;
 }
 node* rotate_right(node *a){
     node *b=a->left;
     node *temp=b->right;
     b->right=a;
     a->left=temp;
     update_height_maxend(a);
     update_height_maxend(b);
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

    update_height_maxend(a);
    //int bf = b_factor(a);

    if (bame_heavy(a)) {
        node* l_child = a->left;
        if (b_factor(l_child) < 0) {
            a->left = rotate_left(l_child);
        }
        return rotate_right(a);
    }

    if (dane_heavy(a)) {
        node* r_child = a->right;
        if (b_factor(r_child) > 0) {
            a->right = rotate_right(r_child);
        }
        return rotate_left(a);
    }

    return a;
}
// bool find(int key)
// {
//     node *temp = root;

//     while (temp != NULL)
//     {
//         if (key == temp->data)
//             return true;

//         if (key < temp->data)
//             temp = temp->left;
//         else
//             temp = temp->right;
//     }

//     return false;
// }
bool is_smaller(int a,int b,int c,int d){
    if(a<c) return true;
    if(a>c) return false;
    return b<d;
}
bool is_overlap(node *a,int s,int e){
    return a->start<e && s<a->end;
}
node* inserting_process(node *a, int id, int start, int end)
{
    if(a == NULL)
    {
        return new node(id, start, end);
    }

    if(is_smaller(start, id, a->start, a->id))
    {
        a->left = inserting_process(a->left, id, start, end);
    }
    else
    {
        a->right = inserting_process(a->right, id, start, end);
    }
    return rebalance(a);
}
int curr_id=1;
int  insert (int start, int end){
    int id=curr_id;
    curr_id++;

    root=inserting_process(root, id, start, end);
    interval_map[id]={start,end};
    return id;
}
// void inorder_traversing(node *a,vector<int> &v){
//     if(a==NULL) return;
//     inorder_traversing(a->left,v);
//     v.pb(a->data);
//     inorder_traversing(a->right,v);
// }

// vector<int> traverse()
// {
//     vector<int> v;
//     inorder_traversing(root, v);
//     return v;
// }

/*
node* deleting_process(node *a, int start, int id)
{
    if(a == NULL)
        return NULL;

    if(smaller(start, id, a->start, a->id))
    {
        a->left = deleting_process(a->left, start, id);
    }

    else if(smaller(a->start, a->id, start, id))
    {
        a->right = deleting_process(a->right, start, id);
    }

    else
    {
        // node found
    }

    return rebalance(a);
}*/
node* deleting_process(node*a,int start,int id){
    if(a==NULL)return NULL;
    if(is_smaller(start, id, a->start, a->id))a->left=deleting_process(a->left,start,id);
    else if(is_smaller(a->start, a->id, start, id))a->right=deleting_process(a->right,start,id);
    else{
        if(a->left==NULL && a->right==NULL){
            delete a;
            return NULL;
        }
        else if(a->left==NULL){
            node *temp=a->right;
            delete a;
            return temp;
        }
        else if(a->right==NULL){
            node *temp=a->left;
            delete a;
            return temp;
        }
        else{
            node *successor=find_min(a->right);
            a->start=successor->start;
            a->end=successor->end;
            a->id=successor->id;
            a->right=deleting_process(a->right,successor->start,successor->id);
        }
    }
    return rebalance(a);
}
bool DELETE(int id){
    if(interval_map.find(id)==interval_map.end()) return false;
    int start=interval_map[id].first;
    root=deleting_process(root,start,id);
    interval_map.erase(id);
    return true;
}
bool update(int id,int u_start,int u_end){
    if(interval_map.find(id)==interval_map.end()) return false;
    int start=interval_map[id].first;
    root=deleting_process(root,start,id);
    root=inserting_process(root,id,u_start,u_end);
    interval_map[id]={u_start,u_end};
    return true;
}
bool conflict(int s,int e){
    node *current=root;

    while(current!=NULL){
        if(is_overlap(current,s,e))
            return true;

        if(current->left!=NULL &&
           current->left->maxend>s)
            current=current->left;
        else
            current=current->right;
    }

    return false;
}
node* next(int start_time){
    node *current=root;
    node* result=NULL;
    while(current!=NULL){
        if(current->start>=start_time){
            result=current;
            current=current->left;
        }
        else{
            current=current->right;
        }
    }
    return result;
}
void overlaps_process(node *a,int start,int end,vector<int> &ovrlap_vector){
    if(a==NULL) return;
    if(a->left!=NULL && a->left->maxend>start) overlaps_process(a->left,start,end,ovrlap_vector);
    if(is_overlap(a,start,end)) ovrlap_vector.pb(a->id);
    if(a->right!=NULL && a->right->maxend>start) overlaps_process(a->right,start,end,ovrlap_vector);
}
vector<int> overlaps(int start,int end){
    vector<int> ovrlap_vector;
    overlaps_process(root,start,end,ovrlap_vector);
    return ovrlap_vector;
}
void at_process(node *a,int time,vector<int> &at_vector){
    if(a==NULL) return;
    if(a->left!=NULL && a->left->maxend>time) at_process(a->left,time,at_vector);
    if(a->start<=time && time<a->end) at_vector.pb(a->id);
    if(a->right!=NULL && a->right->maxend>time ) at_process(a->right,time,at_vector);
}
vector<int> at(int time){
    vector<int> at_vector;
    at_process(root,time,at_vector);
    return at_vector;
}

string printing_format(node *a){
    if(a==NULL) return "";
    if(a->left==NULL && a->right==NULL){
        return to_string(a->id);
    }
    return to_string(a->id )+"("
            +printing_format(a->left)+","
            +printing_format(a->right)+")";
}
void free_memory(node *node)
{
    if (node == nullptr)
        return;
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
    if(argc != 3)
    {
        cout << "INVALID INPUT\n   ";
        return 1;
    }

    
    string input_command;
    ofstream output(argv[2]);
    ifstream input(argv[1]);
timer_calcualtion time_for_add;
timer_calcualtion time_for_remove;
timer_calcualtion time_for_update;
timer_calcualtion time_for_conflict;
timer_calcualtion time_for_overlaps;
timer_calcualtion time_for_at;
timer_calcualtion time_for_next;
    while(input>>input_command)
    {
        if(input_command=="ADD")
        {
            int start,end;
            input >> start >> end;
            auto start_time = chrono::steady_clock::now();
            insert(start, end);
            auto end_time = chrono::steady_clock::now();
            time_for_add.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
            output << printing_format(root) << '\n';
        }
        
        
        else if(input_command=="CONFLICT")
        {
            int start,end;
            input >> start >> end;
            auto start_time = chrono::steady_clock::now();
            bool conflicted=conflict(start, end);
            auto end_time = chrono::steady_clock::now();
            time_for_conflict.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
            if(conflicted)
            output<< "yes\n";
            else
            output << "no\n";
        }
        
        else if(input_command=="OVERLAPS")
        {
            int s,e;
            input >>s>>e;
            auto start_time = chrono::steady_clock::now();
            vector<int> v = overlaps(s, e);
            auto end_time = chrono::steady_clock::now();
            time_for_overlaps.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
            
            
            if(v.empty())
            {
                output << "none\n";
            }
            else
            {
                int size=v.size();
                for(int i = 0; i < size; i++)
                {
                    if(i > 0)
                    output << " ";
                    
                    output << v[i];
                }
                
                output << '\n';
            }
        }
        else if(input_command == "REMOVE")
        {
            int target_id;
            input >> target_id;
            auto start_time = chrono::steady_clock::now();
            bool removed=DELETE(target_id);
            auto end_time = chrono::steady_clock::now();
            time_for_remove.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
            if(removed)
            output << printing_format(root) << '\n';
            else
                output << "not found\n";
        }

        else if(input_command == "UPDATE")
        {
            int id, s, e;
            input >> id >> s >> e;
            auto start_time = chrono::steady_clock::now();
            bool updated=update(id, s, e);
            auto end_time = chrono::steady_clock::now();
            time_for_update.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());

            if(updated)
                output << printing_format(root) << '\n';
            else
                output << "not found\n";
        }

        else if(input_command == "AT")
        {
            int t;
            input >> t;
auto start_time = chrono::steady_clock::now();
            vector<int> v = at(t);
auto end_time = chrono::steady_clock::now();
            time_for_at.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time-start_time).count());
            if(v.empty())
            {
                output << "none\n";
            }
            else
            {
                int size = v.size();
                for(int i = 0; i < size; i++)
                {
                    if(i > 0)
                        output << " ";

                    output << v[i];
                }

                output << '\n';
            }
        }

        else if(input_command=="NEXT")
        {
            int t;
            input>>t;
            auto start_time = chrono::steady_clock::now();
            node *ans=next(t);
            auto end_time = chrono::steady_clock::now();
            time_for_next.add_count(chrono::duration_cast<chrono::nanoseconds>(end_time-start_time).count());

            if(ans==NULL)
            {
                output<<"none\n";
            }
            else
            {
                output << ans->id <<" "<< ans->start <<" "<< ans->end << '\n';
            }
        }
    }
 cout<<"operation,count,total_ns,average_ns\n";

cout<<"add,"
<<time_for_add.c<<","
<<time_for_add.total<<","
<<time_for_add.average()<<"\n";

cout<<"remove,"
<<time_for_remove.c<<","
<<time_for_remove.total<<","
<<time_for_remove.average()<<"\n";

cout<<"update,"
<<time_for_update.c<<","
<<time_for_update.total<<","
<<time_for_update.average()<<"\n";

cout<<"conflict,"
<<time_for_conflict.c<<","
<<time_for_conflict.total<<","
<<time_for_conflict.average()<<"\n";

cout<<"overlaps,"
<<time_for_overlaps.c<<","
<<time_for_overlaps.total<<","
<<time_for_overlaps.average()<<"\n";

cout<<"at,"
<<time_for_at.c<<","
<<time_for_at.total<<","
<<time_for_at.average()<<"\n";

cout<<"next,"
<<time_for_next.c<<","
<<time_for_next.total<<","
<<time_for_next.average()<<"\n";

    free_memory(root);

    return 0;
}
