#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "interval_schedule.cpp"

using namespace std;
using namespace std::chrono;

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

void printTiming(const string& operation, int count, long long total_ns){
    cout << operation << "," << count << "," << total_ns << ",";
    if(count == 0) cout << "N/A\n";
    else cout << (total_ns / count) << "\n";
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: ./interval_scheduler <input-file> <output-file>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if(!fin.is_open() || !fout.is_open()){
        cerr << "Error: Could not open files.\n";
        return 1;
    }

    AVL_tree_interval tree;
    string command;

    long long time_ADD = 0, time_REMOVE = 0, time_UPDATE = 0;
    long long time_CONFLICT = 0, time_OVERLAPS = 0, time_AT = 0, time_NEXT = 0;
    int count_ADD = 0, count_REMOVE = 0, count_UPDATE = 0;
    int count_CONFLICT = 0, count_OVERLAPS = 0, count_AT = 0, count_NEXT = 0;

    while(fin >> command){
        if(command == "ADD"){
            int s, e;
            fin >> s >> e;

            auto start = steady_clock::now();
            tree.add(s, e);
            auto end = steady_clock::now();

            time_ADD += duration_cast<nanoseconds>(end - start).count();
            count_ADD++;

            fout << tree.getTree() << "\n";

        }
        else if(command == "REMOVE"){
            int id;
            fin >> id;

            auto start = steady_clock::now();
            bool deleted = tree.remove(id);
            auto end = steady_clock::now();

            time_REMOVE += duration_cast<nanoseconds>(end - start).count();
            count_REMOVE++;

            if(deleted){
                fout << tree.getTree() << "\n";
            }else{
                fout << "not found\n";
            }

        }
        else if(command == "UPDATE"){
            int id, s, e;
            fin >> id >> s >> e;

            auto start = steady_clock::now();
            bool updated = tree.update(id, s, e);
            auto end = steady_clock::now();

            time_UPDATE += duration_cast<nanoseconds>(end - start).count();
            count_UPDATE++;

            if(updated){
                fout << tree.getTree() << "\n";
            }else{
                fout << "not found\n";
            }

        }
        else if(command == "CONFLICT"){
            int s, e;
            fin >> s >> e;

            auto start = steady_clock::now();
            bool has_conflict = tree.conflict(s, e);
            auto end = steady_clock::now();

            time_CONFLICT += duration_cast<nanoseconds>(end - start).count();
            count_CONFLICT++;

            if(has_conflict){
                fout << "yes\n";
            }else{
                fout << "no\n";
            }

        }
        else if(command == "OVERLAPS"){
            int s, e;
            fin >> s >> e;

            auto start = steady_clock::now();
            vector<int> res = tree.overlaps(s, e);
            auto end = steady_clock::now();

            time_OVERLAPS += duration_cast<nanoseconds>(end - start).count();
            count_OVERLAPS++;

            printVector(res, fout);

        }
        else if(command == "AT"){
            int t;
            fin >> t;

            auto start = steady_clock::now();
            vector<int> res = tree.at(t);
            auto end = steady_clock::now();

            time_AT += duration_cast<nanoseconds>(end - start).count();
            count_AT++;

            printVector(res, fout);

        }
        else if(command == "NEXT"){
            int t;
            fin >> t;

            auto start = steady_clock::now();
            Node* nextEvent = tree.next(t);
            auto end = steady_clock::now();

            time_NEXT += duration_cast<nanoseconds>(end - start).count();
            count_NEXT++;
            
            if(nextEvent != nullptr){
                fout << nextEvent->id << " " << nextEvent->start << " " << nextEvent->end << "\n";
            }
            else{
                fout << "none\n";
            }
        }
    }

    fin.close();
    fout.close();

    cout << "\nInterval Scheduler\n";
    cout << "operation,count,total_ns,average_ns\n";
    printTiming("ADD", count_ADD, time_ADD);
    printTiming("REMOVE", count_REMOVE, time_REMOVE);
    printTiming("UPDATE", count_UPDATE, time_UPDATE);
    printTiming("CONFLICT", count_CONFLICT, time_CONFLICT);
    printTiming("OVERLAPS", count_OVERLAPS, time_OVERLAPS);
    printTiming("AT", count_AT, time_AT);
    printTiming("NEXT", count_NEXT, time_NEXT);

    return 0;
}