#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

void selectionsort(vector<int>& arr) {
    int i, j, min_idx, temp;
    
    for (i = 0; i < arr.size() - 1; i++) {
        min_idx = i;
        temp = arr[i];
        
        for (j = i + 1; j < arr.size(); j++) {
            if (arr[j] < temp) {
                min_idx = j;
                temp = arr[j];
            }
        }
        
        if (min_idx!= i) {
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

int main() {


    for (int i = 1; i < 7; i++) {
        vector<int> arr;
        string filename = "dataset_" + to_string(i) + ".txt";
        ifstream File(filename); // open the input file
        if (!File) {
            cerr << "Error opening file: " << filename << endl;
            continue;
        }

        int num;
        while (File >> num) {
            arr.push_back(num); // read numbers from file and store in vector
        }

        File.close(); // close the input file


        // Start timing
        auto time_start = chrono::high_resolution_clock::now();
        
        
        // Debugging output: size of the array
        cout << "Array size: " << arr.size() << endl;

        // Using std::sort instead of selectionsort so that it is faster 
        selectionsort(arr);

        // Stop timing
        auto time_stop = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = time_stop - time_start;

        
        // print out the time taken
        cout << "Time taken for selection sort of dataset " << i << ": " << duration.count() << " seconds" << endl;

        // Write the sorted array to a file
        string outputFilename = "sorted_dataset_" + to_string(i) + ".txt";
        ofstream outputFile(outputFilename);
        if (!outputFile) {
            cerr << "Error in file: " << outputFilename << endl;
            continue;
        }


        for (int i : arr) {
            outputFile << i << " ";
        }
        outputFile << endl;

        outputFile.close(); // close the output file


    }

    // Inform of the end of the output
    cout << "done" << endl;

}
