#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "avl_tree.cpp" 

using namespace std;
using namespace std::chrono;

void printTiming(const string& operation, int count, long long total_ns){
    cout << operation << "," << count << "," << total_ns << ",";
    if(count == 0) cout << "N/A\n";
    else cout << (total_ns / count) << "\n";
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: ./avl_tree <input-file> <output-file>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if(!fin.is_open() || !fout.is_open()){
        cerr << "Error: Could not open files.\n";
        return 1;
    }

    AVL_tree tree;
    string command;

    long long time_I = 0, time_D = 0, time_F = 0, time_T = 0;
    int count_I = 0, count_D = 0, count_F = 0, count_T = 0;

    while(fin >> command){
        if(command == "I"){
            int x;
            fin >> x;
            
            auto start = steady_clock::now();
            bool success = tree.insert(x);
            auto end = steady_clock::now();
            
            time_I += duration_cast<nanoseconds>(end - start).count();
            count_I++;

            if(success){
                fout << tree.getTree() << "\n";
            }else{
                fout << "duplicate\n";
            }

        }else if(command == "D"){
            int x;
            fin >> x;

            auto start = steady_clock::now();
            bool deleted = tree.erase(x);
            auto end = steady_clock::now();
            
            time_D += duration_cast<nanoseconds>(end - start).count();
            count_D++;

            if(deleted){
                fout << tree.getTree() << "\n";
            }else{
                fout << "not found\n";
            }

        }else if(command == "F"){
            int x;
            fin >> x;

            auto start = steady_clock::now();
            bool found = tree.find(x);
            auto end = steady_clock::now();
            
            time_F += duration_cast<nanoseconds>(end - start).count();
            count_F++;

            if(found){
                fout << "found\n";
            }else{
                fout << "not found\n";
            }

        }else if(command == "T"){
            auto start = steady_clock::now();
            vector<int> res = tree.traverse();
            auto end = steady_clock::now();
            
            time_T += duration_cast<nanoseconds>(end - start).count();
            count_T++;
            
            if(res.empty()){
                fout << "\n";
            }else{
                for(size_t i = 0; i < res.size(); ++i){
                    fout << res[i] << (i == res.size() - 1 ? "" : " ");
                }
                fout << "\n";
            }
        }
    }

    fin.close();
    fout.close();

    cout << "AVL\n";
    cout << "operation,count,total_ns,average_ns\n";
    printTiming("I", count_I, time_I);
    printTiming("D", count_D, time_D);
    printTiming("F", count_F, time_F);
    printTiming("T", count_T, time_T);

    return 0;
}