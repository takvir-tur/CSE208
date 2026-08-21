#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "avl_tree.cpp"

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: ./avl_tree <input-file> <output-file>" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if(!fin.is_open() || !fout.is_open()){
        cerr << "Error: Could not open files." << endl;
        return 1;
    }

    AVL_tree tree;
    string command;

    while(fin >> command){
        if(command == "I"){
            int x;
            fin >> x;
            
            if(tree.insert(x)){
                fout << tree.getTree() << "\n";
            }else{
                fout << "duplicate\n";
            }

        }
        else if(command == "D"){
            int x;
            fin >> x;

            if(tree.erase(x)){
                fout << tree.getTree() << "\n";
            }else{
                fout << "not found\n";
            }

        }
        else if(command == "F"){
            int x;
            fin >> x;

            if(tree.find(x)){
                fout << "found\n";
            }else{
                fout << "not found\n";
            }

        }
        else if(command == "T"){
            vector<int> res = tree.traverse();
            
            if(res.empty()){
                fout << "\n";
            }
            else{
                for(size_t i = 0; i < res.size(); ++i){
                    fout << res[i] << (i == res.size() - 1 ? "" : " ");
                }
                fout << "\n";
            }
        }
    }

    fin.close();
    fout.close();

    return 0;
}