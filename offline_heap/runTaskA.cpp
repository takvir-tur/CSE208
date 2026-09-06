#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>

using namespace std;

int main(){
    for (int i = 1; i <= 10; ++i) {
        ostringstream oss;
        oss << setfill('0') << setw(2) << i;
        string testNum = oss.str();

        string inputPath = "binomial_heap_sample_io/test_" + testNum + "/input.txt";
        string outputPath = "binomial_heap_sample_io/test_" + testNum + "/my_output.txt";
        string expectedPath = "binomial_heap_sample_io/test_" + testNum + "/output.txt";

        // Redirect stdout and stderr of taskA to /dev/null so only the output file receives data
        string command = "./taskA \"" + inputPath + "\" \"" + outputPath + "\" > /dev/null 2>&1";
        int runStatus = system(command.c_str());

        if (runStatus != 0) {
            cerr << "test_" << testNum << ": CRASHED (Return code " << runStatus << ")\n\n";
            continue;
        }

        // Run diff directly to the terminal
        string diffCmd = "diff -u \"" + outputPath + "\" \"" + expectedPath + "\"";
        int diffStatus = system(diffCmd.c_str());

        if (diffStatus == 0) {
            cout << "test_" << testNum << ": PASSED\n";
        } else {
            cerr << "test_" << testNum << ": FAILED (Diff displayed above)\n\n";
        }
    }

    return 0;
}