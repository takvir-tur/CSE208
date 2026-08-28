#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <list>

using namespace std;

const int minTableSize = 13;
const double maxLoadFactor = 0.5;
const double minLoadFactor = 0.25;
const int wordLength = 10;
const int wordCount = 10000;

bool isPrime(int n){
    for(int i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }

    return true;
}

int nextSize(int currSize){
    int i = currSize * 2 + 1;
    while(true){
        if(isPrime(i)) return i;
        i++;
    }
}

int prevSize(int currSize){
    int i = currSize / 2 - 1;
    while(true){
        if(isPrime(i)) return i;
        i--;
    }
}



string randomWord(size_t length) {
    string word;
    word.reserve(length); 
    
    for(size_t i = 0; i < length; ++i){
        word += (char)('a' + (rand() % 26));
    }
    
    return word;
}

int Hash1(string k, int tableSize){
    int h = 0;
    int p = 31;
    for(char c : k){
        h = (h * p + c) % tableSize;
    }

    return h;
}
int Hash1(int k, int tableSize){
    int h = 0;
    int p = 31;
    h = (h * p + k) % tableSize;

    return h;
}

int Hash2(string k, int tableSize){
    unsigned int h = 2166136261;

    for(char c : k){
        h ^= c;
        h *= 16777619;
    }

    return h % tableSize;
}
int Hash2(int k, int tableSize){
    unsigned int h = 2166136261;

    h ^= k;
    h *= 16777619;

    return h % tableSize;
}

int auxHash(string k, int tableSize){
    int h = 0;
    int p = 109;
    for(char c : k){
        h = (h * p + c) % (tableSize - 1);
    }

    return h + 1;
}
int auxHash(int k, int tableSize){
    int h = 0;
    int p = 109;
    h = (h * p + k) % (tableSize - 1);

    return h + 1;
}

template <typename K, typename V>
class chainHash{
    vector<list<pair<K, V>>> hashTable;
    int tableSize;
    int count;
    int hashFunc;
    int collisions;
    int deleteCount;
    int insertCount;
    int lastCount;

    void resize(int newSize) {
        vector<list<pair<K, V>>> oldTable = hashTable;
        
        hashTable.assign(newSize, list<pair<K, V>>());
        tableSize = newSize;
        
        insertCount = 0;
        deleteCount = 0;
        
        for(auto bucket : oldTable){
            for (const auto& pair : bucket) {
                unsigned int h = (hashFunc == 1) ? Hash1(pair.first, tableSize) : Hash2(pair.first, tableSize);
                hashTable[h].push_back(pair);
            }
        }
        
        lastCount = count;
    }
public:
    chainHash(int func) : tableSize(minTableSize), count(0), hashFunc(func), collisions(0), deleteCount(0), insertCount(0), lastCount(0) {
        hashTable.resize(tableSize);
    }

    int getCollisions(){
        return collisions;
    }

    double getAvgHits(const vector<K>& words){
        long long totalHits = 0;
        for(const K& w : words){
            totalHits += search(w);
        }

        return (double)totalHits / words.size();
    }

    void insert(K key, V value){
        double loadFactor = (double)(count + 1) / tableSize;
        if(loadFactor > maxLoadFactor && insertCount >= lastCount / 2){
            resize(nextSize(tableSize * 2 + 1));
        }

        int h = (hashFunc == 1) ? Hash1(key, tableSize) : Hash2(key, tableSize);
        if(!hashTable[h].empty()) collisions++;

        for(auto pair : hashTable[h]){
            if(pair.first == key) return;
        }

        hashTable[h].push_back({key, value});
        count++;
        insertCount++;
    }

    int search(K key){
        int h = (hashFunc == 1) ? Hash1(key, tableSize) : Hash2(key, tableSize);
        int probes = 1;
        for(auto pair : hashTable[h]){
            if(pair.first == key) return probes;
            probes++;
        }

        return probes;
    }

    void remove(K key) {
        int h = (hashFunc == 1) ? Hash1(key, tableSize) : Hash2(key, tableSize);
        
        for(auto it = hashTable[h].begin(); it != hashTable[h].end(); it++){
            if(it->first == key){
                hashTable[h].erase(it);
                count--;
                deleteCount++;

                double loadFactor = (double)count / tableSize;
                if(tableSize > minTableSize && loadFactor < minLoadFactor && deleteCount >= lastCount / 2){
                    int newSize = prevSize(tableSize / 2);
                    
                    if(newSize < minTableSize){
                        newSize = minTableSize;
                    }
                    
                    if(newSize < tableSize){
                        resize(newSize);
                    }
                }
                
                
                return;
            }
        }
    }
};

template <typename K, typename V>
class openHash {
    struct Entry {
        K key;
        V value;
        bool isOccupied = false;
        bool isDeleted = false;
    };

    vector<Entry> hashTable;
    int tableSize;
    int count;
    int hashFunc;
    int probeType; 
    int C1, C2;
    int collisions;
    int deleteCount;
    int insertCount;
    int lastCount;

    void resize(int newSize){
        vector<Entry> oldTable = hashTable;
        hashTable.assign(newSize, Entry());
        tableSize = newSize;
        
        count = 0;
        insertCount = 0;
        deleteCount = 0;
        
        for(const auto& entry : oldTable){
            if (entry.isOccupied && !entry.isDeleted) {
                long long h = (hashFunc == 1) ? Hash1(entry.key, tableSize) : Hash2(entry.key, tableSize);
                long long aux = auxHash(entry.key, tableSize);
                
                for(int i = 0; i < tableSize; i++){
                    long long idx;
                    if(probeType == 1){ 
                        idx = (h + i * aux) % tableSize;
                    }
                    else{ 
                        idx = (h + C1 * i * aux + C2 * i * i) % tableSize;
                    }

                    if(!hashTable[idx].isOccupied){
                        hashTable[idx].key = entry.key;
                        hashTable[idx].value = entry.value;
                        hashTable[idx].isOccupied = true;
                        hashTable[idx].isDeleted = false;
                        count++;
                        break;
                    }
                }
            }
        }
        lastCount = count;
    }

public:
    openHash(int func, int proType, int c1 = 1, int c2 = 1) 
        : tableSize(minTableSize), count(0), hashFunc(func), probeType(proType), 
          C1(c1), C2(c2), collisions(0), deleteCount(0), insertCount(0), lastCount(0) {
        hashTable.resize(tableSize);
    }

    int getCollisions(){
        return collisions;
    }

    double getAvgHits(const vector<K>& words){
        long long totalHits = 0;
        for(const K& w : words){
            totalHits += search(w);
        }

        return (double)totalHits / words.size();
    }

    void insert(K key, V value){
        double loadFactor = (double)(count + 1) / tableSize;
        if(loadFactor > maxLoadFactor && insertCount >= lastCount / 2){
            resize(nextSize(tableSize * 2 + 1));
        }

        long long h = (hashFunc == 1) ? Hash1(key, tableSize) : Hash2(key, tableSize);
        long long aux = auxHash(key, tableSize);
        
        for(int i = 0; i < tableSize; i++){
            long long idx;
            if(probeType == 1){ 
                idx = (h + i * aux) % tableSize;
            }
            else{ 
                idx = (h + C1 * i * aux + C2 * i * i) % tableSize;
            }

            if(!hashTable[idx].isOccupied || hashTable[idx].isDeleted){
                if (i > 0) collisions++;
                hashTable[idx].key = key;
                hashTable[idx].value = value;
                hashTable[idx].isOccupied = true;
                hashTable[idx].isDeleted = false;
                
                count++;
                insertCount++;
                return;
            }
            else if(hashTable[idx].key == key) return;
        }
    }

    int search(K key){
        long long h = (hashFunc == 1) ? Hash1(key, tableSize) : Hash2(key, tableSize);
        long long aux = auxHash(key, tableSize);
        
        for(int i = 0; i < tableSize; i++){
            long long idx;
            if(probeType == 1){
                idx = (h + i * aux) % tableSize;
            }
            else{
                idx = (h + C1 * i * aux + C2 * i * i) % tableSize;
            }

            if(!hashTable[idx].isOccupied && !hashTable[idx].isDeleted) return i + 1;
            if(hashTable[idx].isOccupied && !hashTable[idx].isDeleted && hashTable[idx].key == key) return i + 1;
        }

        return tableSize;
    }

    void remove(K key){
        long long h = (hashFunc == 1) ? Hash1(key, tableSize) : Hash2(key, tableSize);
        long long aux = auxHash(key, tableSize);
        
        for(int i = 0; i < tableSize; i++){
            long long idx;
            if(probeType == 1){
                idx = (h + i * aux) % tableSize;
            }
            else{
                idx = (h + C1 * i * aux + C2 * i * i) % tableSize;
            }

            if(!hashTable[idx].isOccupied) return; 

            if(hashTable[idx].isOccupied && !hashTable[idx].isDeleted && hashTable[idx].key == key){
                hashTable[idx].isDeleted = true;
                count--;
                deleteCount++;
                
                double loadFactor = (double)count / tableSize;
                if(tableSize > minTableSize && loadFactor < minLoadFactor && deleteCount >= lastCount / 2){
                    int newSize = prevSize(tableSize / 2);
                    if (newSize < minTableSize) newSize = minTableSize;
                    if (newSize < tableSize) resize(newSize);
                }

                return;
            }
        }
    }
};