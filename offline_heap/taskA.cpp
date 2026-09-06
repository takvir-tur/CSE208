#include "binomial_heap.cpp"

int main(int argc, char* argv[]){
    if (argc < 2) {
        cerr << "Error: No input file specified.\n";
        cerr << "Usage: " << argv[0] << " <input_file> [output_file]\n";
        return 1;
    }

    string inputFileName = argv[1];
    string outputFileName = (argc >= 3) ? argv[2] : "output.txt";

    ifstream inFile(inputFileName);
    ofstream outFile(outputFileName);

    if(!inFile.is_open()){
        cerr << "Error: Cannot open " << inputFileName << endl;
        return 1;
    }

    // Index 1 maps to H1, Index 2 maps to H2
    BinomialHeap heaps[3];
    string command;

    while (inFile >> command){
        if(command == "I"){
            int h, x;
            inFile >> h >> x;
            heaps[h].insert(x);
        } 
        else if(command == "F"){
            int h;
            inFile >> h;
            int minVal = heaps[h].findMin();
            
            string outMsg = "Find Min returned: " + to_string(minVal) + "\n";
            cout << outMsg; 
            outFile << outMsg;
        }
        else if(command == "E"){
            int h;
            inFile >> h;
            int minVal = heaps[h].extractMin();
            
            string outMsg = "Extract Min returned: " + to_string(minVal) + "\n";
            cout << outMsg; 
            outFile << outMsg;
        } 
        else if(command == "D"){
            int h, x, y;
            inFile >> h >> x >> y;
            heaps[h].decreaseKey(x, y);
        } 
        else if (command == "R") {
            int h, x;
            inFile >> h >> x;
            heaps[h].remove(x);
        } 
        else if(command == "U"){
            int h1, h2;
            inFile >> h1 >> h2;
            heaps[h1].unionHeap(&heaps[h2]);
        } 
        else if(command == "P"){
            int h;
            inFile >> h;
            heaps[h].print(h, outFile);
        }
    }

    inFile.close();
    outFile.close();
    return 0;
}