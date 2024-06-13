#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ctime>
#include <cstdlib>
using namespace std;

// digits in leader_id
const set<int> valid_digits = {0, 1, 2, 3, 4};

// sizes of the datasets
const vector<int> sizes = {100, 1000, 10000, 100000, 500000, 1000000};

// function to check if a number has only valid digits
bool has_only_valid_digits(int number) {
    while (number > 0) {
        int digit = number % 10;
        if (valid_digits.find(digit) == valid_digits.end()) {
            return false;
        }
        number /= 10;
    }
    return true;
}

// function to generate datasets
vector<int> generate_datasets(int size) {
    vector<int> dataset;
    while (dataset.size() < size) {
        int n = rand() % 32000;
        if (has_only_valid_digits(n)) {
            dataset.push_back(n);
        }
    }
    return dataset;
}

int main() {
    srand(time(0));

    // Generate the datasets
    vector<vector<int>> datasets;
    for (int size : sizes) {
        datasets.push_back(generate_datasets(size));
    }

    // Save the datasets to files
    for (size_t i = 0; i < datasets.size(); ++i) {
        ofstream file("dataset_" + to_string(i + 1) + ".txt");
        if (file.is_open()) {
            for (int number : datasets[i]) {
                file << number << "\n";
            }
            file.close();
        } else {
            cerr << "Unable to open file dataset_" << i + 1 << ".txt" << endl;
        }
    }
}
