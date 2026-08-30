#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

using ll = unsigned long long;

const int size_init = 13;
const double highest_load = 0.50;
const double minimum_load = 0.25;
bool isPrime(int n) {
    if (n<2)return false;
    for(int i=2;1LL*i*i<=n;i++)
        if(n%i==0) return false;
    return true;
}
int nextPrime(int number) {
    number++;
    while (!isPrime(number)) {
        number++;
    }
    return number;
}
int previousPrime(int n) {
    while (!isPrime(n)) n--;
    return n;
}


ll hash1(const string& word) {                      
    ll hashValue = 5381;
    for (char ch : word) hashValue = hashValue * 41 + ch;
    return hashValue;
}

ll hash2(const string& word) {                    
    ll hashValue = 14695981039346656037ULL;
    for (char ch : word) {
        hashValue ^= (unsigned char)ch;
        hashValue *= 1099511628211ULL;
    }
    return hashValue;
}

enum Method{CHAINING,DOUBLE_HASHING,CUSTOM_PROBING};
enum slotState {EMPTY,OCCUPIED,DELETED};
template<class Key,class Value>
class HashTable {
    struct slot {
        Key key;
        Value value;
        slotState state;
        slot() : state(EMPTY) {}  //every new slot  become empty
    };
    typedef ll (*HashFunction)(const Key&);
    Method method;
    HashFunction hash_function;
    vector<list<pair<Key, Value>>>chainTable;
    vector<slot> table_probe;

    int count_item;
    long long totalCollisions;
    int insertsSinceGrow;
    int deletesSinceShrink;
    int insertsNeededToGrow;
    int deletesNeededToShrink;

    int get_step(const Key& key) const {
        ll value = hash_function(key);
        return 1 + (value >> 16) % (capacity() - 1);
    }

    int get_prob_index(const Key& key, int attempt) const {
        int m = capacity();
        ll initial_pos = hash_function(key) % m;
        ll step = get_step(key);

        if (method == DOUBLE_HASHING)
            return (initial_pos + 1ULL * attempt * step) % m;

        const int C1 = 17,C2 = 31;
        return (initial_pos+C1*1ULL*attempt*step
                +C2*1ULL*attempt*attempt) % m;
    }
    bool reinsert(const Key& key, const Value& value) {
        if (method==CHAINING) {
            int index = hash_function(key) % capacity();
            chainTable[index].push_back(make_pair(key, value));
            return true;
        }
         for(int i=0;i<capacity();i++) {
            slot& slot= table_probe[get_prob_index(key, i)];
            if (slot.state != OCCUPIED) {
                slot.key = key;
                slot.value = value;
                slot.state = OCCUPIED;
                return true;
            }
        }
        return false;
    }

    void table_resize(int newSize) {
        vector<pair<Key, Value>> oldItems;
        if (method == CHAINING){
            for (auto& bucket:chainTable)
                for (auto& item:bucket) oldItems.push_back(item);

            chainTable.clear();
            chainTable.resize(newSize);
        }else{
            for (auto& slot:table_probe)
                if(slot.state==OCCUPIED)
                    oldItems.push_back(make_pair(slot.key, slot.value));

                 while (true) {
                table_probe.clear();
                table_probe.resize(newSize);
                bool success = true;
                for (auto& item:oldItems) {
                    if (!reinsert(item.first, item.second)){
                        success = false;
                        break;
                    }
                }
                if (success) return;
                newSize=nextPrime(newSize*2);
            }
        }

        for (auto& item : oldItems)
            reinsert(item.first, item.second);
    }

    int capacity()const {
        if (method == CHAINING)return (int)chainTable.size();
        return (int)table_probe.size();
    }
    double loadFactor() const {
        return (double)count_item / capacity();
    }
    void table_shrink() {
        int newSize = previousPrime(capacity()/2);
        if (newSize < size_init) newSize = size_init;
        table_resize(newSize);
        deletesNeededToShrink = max(1, count_item/2);
        deletesSinceShrink = 0;
    }
    void shrink_check() {
        if (capacity() > size_init && loadFactor() < minimum_load &&
        deletesSinceShrink >= deletesNeededToShrink)
        table_shrink();
    }
    void table_grow() {
        table_resize(nextPrime(capacity() * 2));
        insertsNeededToGrow = max(1, count_item / 2);
        insertsSinceGrow = 0;
    }
    
    void grow_check() {
        if (loadFactor() > highest_load &&
            insertsSinceGrow >= insertsNeededToGrow)
            table_grow();
    }


public:
    HashTable(Method chosenMethod, HashFunction chosenHash) {
        method = chosenMethod;
        hash_function = chosenHash;
        count_item = 0;
        totalCollisions = 0;
        insertsSinceGrow = deletesSinceShrink = 0;
        insertsNeededToGrow = deletesNeededToShrink = 0;

        if (method == CHAINING) chainTable.resize(size_init);
        else table_probe.resize(size_init);
    }


    long long collisions() const {
        return totalCollisions;
    }

    bool search(const Key& key, Value& foundValue, int& hits) const {
        hits = 0;

        if (method == CHAINING) {
            int index = hash_function(key) % capacity();
            for (const auto& item : chainTable[index]) {
                hits++;
                if (item.first == key) {
                    foundValue = item.second;
                    return true;
                }
            }
            return false;
        }

        for (int i = 0; i < capacity(); i++) {
            const slot& slot = table_probe[get_prob_index(key, i)];
            hits++;
            if (slot.state==EMPTY)return false;
            if (slot.state==OCCUPIED && slot.key==key){
                foundValue=slot.value;
                return true;
            }
        }
        return false;
    }

    bool insert(const Key& key, const Value& value){
        int hits;
        Value oldValue;
        if(search(key, oldValue, hits)) return false;    // Duplicate key

        if(method == CHAINING){
            int index = hash_function(key)%capacity();
            if (!chainTable[index].empty())totalCollisions++;
            chainTable[index].push_back(make_pair(key, value));
        } else {
            bool inserted=false;
                while (!inserted) {
                for (int i = 0;i<capacity();i++) {
                    int position = get_prob_index(key, i);
                    slot& slot = table_probe[position];

                    if (slot.state == OCCUPIED) {
                        totalCollisions++;
                    } else {
                        slot.key=key;
                        slot.value=value;
                        slot.state=OCCUPIED;
                        inserted=true;
                        break;
                    }
                }

                if (!inserted)table_grow();
            }
        }

        count_item++;
        insertsSinceGrow++;
        grow_check();
        return true;
    }

    bool erase(const Key& key) {
        if (method == CHAINING) {
            int index = hash_function(key) % capacity();
            auto& bucket = chainTable[index];

            for (auto it = bucket.begin(); it != bucket.end(); it++) {
                if (it->first == key) {
                    bucket.erase(it);
                    count_item--;
                    deletesSinceShrink++;
                    shrink_check();
                    return true;
                }
            }
            return false;
        }

        for (int i = 0; i < capacity(); i++) {
            slot& slot = table_probe[get_prob_index(key, i)];
            if (slot.state == EMPTY) return false;

            if (slot.state == OCCUPIED && slot.key == key) {
                slot.state = DELETED;
                count_item--;
                deletesSinceShrink++;
                shrink_check();
                return true;
            }
        }
        return false;
    }
};

string randomWord(int length) {
    const string letters = "abcdefghijklmnopqrstuvwxyz";
    string word = "";

    for (int i = 0; i < length; i++)
        word += letters[rand() % 26];
    return word;
}

vector<string> unique_word_generator(int amount, int length) {
    vector<string> words;
    unordered_set<string> usedWords;

    while ((int)words.size() < amount) {
        string word = randomWord(length);
        if (usedWords.find(word) == usedWords.end()) {
            usedWords.insert(word);
            words.push_back(word);
        }
    }
    return words;
}

struct Result {
    long long collisions;
    double averageHits;
};

Result evaluate(const vector<string>& words, Method method,
                ll (*hash_function)(const string&), mt19937& generator) {
    HashTable<string, int> table(method, hash_function);

    for (int i = 0; i < (int)words.size(); i++)
        table.insert(words[i], i + 1);

    vector<int> randomOrder(words.size());
    for (int i = 0; i < (int)words.size(); i++) randomOrder[i] = i;
    shuffle(randomOrder.begin(), randomOrder.end(), generator);

    long long totalHits = 0;
    for (int i = 0; i < 1000; i++) {
        int foundValue, hits;
        table.search(words[randomOrder[i]], foundValue, hits);
        totalHits += hits;
    }

    Result result;
    result.collisions = table.collisions();
    result.averageHits = totalHits / 1000.0;
    return result;
}

double uniqueHashPercentage(const vector<string>& words,
                            ll (*hash_function)(const string&)) {
    int testSize = nextPrime(2 * (int)words.size() + 1);  
    unordered_set<ll> uniqueHashes;

    for (const string& word : words)
        uniqueHashes.insert(hash_function(word) % testSize);  

    return uniqueHashes.size() * 100.0 / words.size();
}

int main() {
    srand((unsigned)time(nullptr));
    random_device seed;
    mt19937 generator(seed());

    vector<string> reportWords = unique_word_generator(10000, 10);

        cout << fixed << setprecision(3);
        cout << "Hash1 unique percentage: "
            << uniqueHashPercentage(reportWords, hash1) << "%\n";
        cout << "Hash2 unique percentage: "
            << uniqueHashPercentage(reportWords, hash2) << "%\n\n";

    Method collisionMethods[3] = {CHAINING, DOUBLE_HASHING, CUSTOM_PROBING};
    string methodNames[3] = {"Chaining", "Double hashing", "Custom probing"};

    cout << left << setw(18) << "Method"
         << right << setw(18) << "Hash1 collisions" << setw(16) << "Avg. hits"
         << setw(20) << "Hash2 collisions" << setw(16) << "Avg. hits\n";
    cout << string(88, '-') << '\n';
    for (int i = 0; i < 3; i++) {
        Result hash1Result = evaluate(reportWords, collisionMethods[i], hash1, generator);
        Result hash2Result = evaluate(reportWords, collisionMethods[i], hash2, generator);

        cout << left << setw(18) << methodNames[i] << right
             << setw(18) << hash1Result.collisions << setw(16) << hash1Result.averageHits
             << setw(20) << hash2Result.collisions << setw(16) << hash2Result.averageHits
             << '\n';
    }

    return 0;
}