#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "interval_schedule.cpp"

using namespace std;

void printVector(const vector<int>& res, ofstream& fout){
    if(res.empty()){
        fout << "none\n";
    }else{
        for(size_t i = 0; i < res.size(); ++i){
            fout << res[i] << (i == res.size() - 1 ? "" : " ");
        }
        fout << "\n";
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: ./interval_scheduler <input-file> <output-file>" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if(!fin.is_open() || !fout.is_open()){
        cerr << "Error: Could not open files." << endl;
        return 1;
    }

    AVL_tree_interval tree;
    string command;

    while(fin >> command){
        if(command == "ADD"){
            int s, e;
            fin >> s >> e;
            tree.add(s, e);
            fout << tree.getTree() << "\n";

        }
        else if(command == "REMOVE"){
            int id;
            fin >> id;
            if(tree.remove(id)){
                fout << tree.getTree() << "\n";
            }else{
                fout << "not found\n";
            }

        }
        else if(command == "UPDATE"){
            int id, s, e;
            fin >> id >> s >> e;
            if(tree.update(id, s, e)){
                fout << tree.getTree() << "\n";
            }else{
                fout << "not found\n";
            }

        }
        else if(command == "CONFLICT"){
            int s, e;
            fin >> s >> e;
            if(tree.conflict(s, e)){
                fout << "yes\n";
            }else{
                fout << "no\n";
            }

        }
        else if(command == "OVERLAPS"){
            int s, e;
            fin >> s >> e;
            vector<int> res = tree.overlaps(s, e);
            printVector(res, fout);

        }
        else if(command == "AT"){
            int t;
            fin >> t;
            vector<int> res = tree.at(t);
            printVector(res, fout);

        }
        else if(command == "NEXT"){
            int t;
            fin >> t;
            Node* nextEvent = tree.next(t);
            
            if(nextEvent != nullptr){
                fout << nextEvent->id << " " << nextEvent->start << " " << nextEvent->end << "\n";
            }else{
                fout << "none\n";
            }
        }
    }

    fin.close();
    fout.close();

    return 0;
}