#include <iomanip>
#include "hash.cpp"
#include <unordered_set>

const int c1 = 7;
const int c2 = 13;

int main(){
    srand(time(0));
    unordered_set<string> uniqueWords;
    vector<string> words;

    while(uniqueWords.size() < wordCount){
        string w = randomWord(wordLength);
        if(uniqueWords.find(w) == uniqueWords.end()){
            uniqueWords.insert(w);
            words.push_back(w);
        }
    }

    vector<string> searchWords;
    for(int i = 0; i < 1000; i++){
        searchWords.push_back(words[i * 10]); 
    }

    chainHash<string, int> ch1(1), ch2(2);
    openHash<string, int> dh1(1, 1), dh2(2, 1);
    openHash<string, int> cp1(1, 2, c1, c2), cp2(2, 2, c1, c2);

    for(int i = 0; i < wordCount; i++){
        ch1.insert(words[i], i + 1); ch2.insert(words[i], i + 1);
        dh1.insert(words[i], i + 1); dh2.insert(words[i], i + 1);
        cp1.insert(words[i], i + 1); cp2.insert(words[i], i + 1);
    }

    cout << "\n--------------------------------------------------------------------------------------\n";
    cout << left << setw(20) << "Method" 
         << "| " << setw(28) << "Hash1" 
         << "| " << "Hash2\n";
    cout << left << setw(20) << "" 
         << "| " << setw(10) << "Collisions" << " | " << setw(15) << "Average Hits" 
         << "| " << setw(10) << "Collisions" << " | " << setw(15) << "Average Hits\n";
    cout << "--------------------------------------------------------------------------------------\n";
    
    cout << left << setw(20) << "Chaining Method" 
         << "| " << setw(10) << ch1.getCollisions() << " | " << setw(15) << fixed << setprecision(3) << ch1.getAvgHits(searchWords)
         << "| " << setw(10) << ch2.getCollisions() << " | " << setw(15) << ch2.getAvgHits(searchWords) << "\n";
         
    cout << left << setw(20) << "Double Hashing" 
         << "| " << setw(10) << dh1.getCollisions() << " | " << setw(15) << dh1.getAvgHits(searchWords)
         << "| " << setw(10) << dh2.getCollisions() << " | " << setw(15) << dh2.getAvgHits(searchWords) << "\n";
         
    cout << left << setw(20) << "Custom Probing" 
         << "| " << setw(10) << cp1.getCollisions() << " | " << setw(15) << cp1.getAvgHits(searchWords)
         << "| " << setw(10) << cp2.getCollisions() << " | " << setw(15) << cp2.getAvgHits(searchWords) << "\n";
    cout << "--------------------------------------------------------------------------------------\n";

    unordered_set<long long> uniqueH1;
    unordered_set<long long> uniqueH2;
    
    int testSize = nextSize(wordCount); 

    for(const string& w : words){
        uniqueH1.insert(Hash1(w, testSize));
        uniqueH2.insert(Hash2(w, testSize));
    }

    double percentH1 = (uniqueH1.size() / (double)wordCount) * 100;
    double percentH2 = (uniqueH2.size() / (double)wordCount) * 100;

    cout << "\nUnique Hash percent:\n";
    cout << "Hash1: " << fixed << setprecision(2) << percentH1 
         << "%" << "\n";
    cout << "Hash2: " << fixed << setprecision(2) << percentH2 
         << "%" << "\n";


    //  ---INTEGER KEY TESTING
    // unordered_set<int> uniqueInts;
    // vector<int> intKeys;

    // while (uniqueInts.size() < wordCount) {
    //     int num = abs((rand() << 15) ^ rand()); 
    //     if (uniqueInts.find(num) == uniqueInts.end()) {
    //         uniqueInts.insert(num);
    //         intKeys.push_back(num);
    //     }
    // }

    // vector<int> searchInts;
    // for (int i = 0; i < 1000; i++) {
    //     searchInts.push_back(intKeys[i * 10]);
    // }

    // chainHash<int, int> ch1_int(1), ch2_int(2);
    // openHash<int, int> dh1_int(1, 1), dh2_int(2, 1);
    // openHash<int, int> cp1_int(1, 2, 7, 11), cp2_int(2, 2, 7, 11); 

    // for (int i = 0; i < wordCount; i++) {
    //     ch1_int.insert(intKeys[i], i + 1); ch2_int.insert(intKeys[i], i + 1);
    //     dh1_int.insert(intKeys[i], i + 1); dh2_int.insert(intKeys[i], i + 1);
    //     cp1_int.insert(intKeys[i], i + 1); cp2_int.insert(intKeys[i], i + 1);
    // }

    // cout << "\n================[ INTEGER KEYS ]======================================================\n";
    // cout << left << setw(20) << "Method" 
    //      << "| " << setw(28) << "Hash1" 
    //      << "| " << "Hash2\n";
    // cout << left << setw(20) << "" 
    //      << "| " << setw(10) << "Collisions" << " | " << setw(15) << "Average Hits" 
    //      << "| " << setw(10) << "Collisions" << " | " << setw(15) << "Average Hits\n";
    // cout << "--------------------------------------------------------------------------------------\n";
    
    // cout << left << setw(20) << "Chaining Method" 
    //      << "| " << setw(10) << ch1_int.getCollisions() << " | " << setw(15) << fixed << setprecision(3) << ch1_int.getAvgHits(searchInts)
    //      << "| " << setw(10) << ch2_int.getCollisions() << " | " << setw(15) << ch2_int.getAvgHits(searchInts) << "\n";
         
    // cout << left << setw(20) << "Double Hashing" 
    //      << "| " << setw(10) << dh1_int.getCollisions() << " | " << setw(15) << dh1_int.getAvgHits(searchInts)
    //      << "| " << setw(10) << dh2_int.getCollisions() << " | " << setw(15) << dh2_int.getAvgHits(searchInts) << "\n";
         
    // cout << left << setw(20) << "Custom Probing" 
    //      << "| " << setw(10) << cp1_int.getCollisions() << " | " << setw(15) << cp1_int.getAvgHits(searchInts)
    //      << "| " << setw(10) << cp2_int.getCollisions() << " | " << setw(15) << cp2_int.getAvgHits(searchInts) << "\n";
    // cout << "--------------------------------------------------------------------------------------\n";


    return 0;
}