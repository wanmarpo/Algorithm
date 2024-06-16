#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> // C++17 feature 

using namespace std;
namespace fs = std::filesystem;

// Heapify function for max heap
void heapify(vector<int>& arr, int N, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < N && arr[l] > arr[largest])
        largest = l;

    if (r < N && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, N, largest);
    }
}

// Main function to do heap sort
void heapSort(vector<int>& arr) {
    int N = arr.size();

    // Build heap
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(arr, N, i);

    for (int i = N - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Print the current working directory
int main() {

    for (int i = 1; i < 7; i++) {   
        vector<int> arr;
        string filename = "dataset_" + to_string(i) + ".txt"; // able to read and be able to execute all 6 datasets automatically

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Unable to open input file: " << filename << endl;
            continue;
        }

        // Read data from file
        int num;
        while (file >> num) 
        {
            arr.push_back(num);
        }
        file.close();

        
        // Start timing
        auto start_timer = chrono::high_resolution_clock::now();


        // Perform heap sort
        heapSort(arr);

         // Stop timing
        auto stop_timer = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = stop_timer - start_timer;

        // Print time taken
        cout << "Time taken for heap sort of dataset " << i << ": " << duration.count() << " seconds" << endl;

        // Write sorted array to a new file
        ofstream outputFile("Heapsort_dataset_" + to_string(i) + ".txt");
        if (outputFile.is_open()) {     
            for (int num : arr) {
                outputFile << num << " ";
            }
            
            outputFile << endl;
            outputFile.close();
            cout << "Written sorted array of number  " << i << endl;
        } else {
            cerr << "Unable to open output file: " << i << endl;
        }
    }
}
