#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <cmath>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <unordered_set>
using namespace std;

// struct star's attribute
struct Star {
    string name;
    int x, y, z, weight, profit;
};

// struct edge's attribute
struct Edge
{
    Star source, ending;
    int distance;
};

// function to check if a number has (0, 2, 3, 4, 5, 6, 7)
bool numberChecker(int number) {
    const set<int> id_digits = {0, 2, 3, 4, 5, 6, 7};
    while (number > 0) {
        int num = number % 10;
        if (id_digits.find(num) == id_digits.end()) {
            return false;
        }
        number /= 10;
    }
    return true;
}

// function to generate star attribute values
int generateValue(int count) {
    int n;
    if (count == 0) {
        while (true) {
            n = rand() % 100 + 1;
            if (numberChecker(n)) {
                break;
            }
        } return n;
    }

    else if (count == 1) {
        while (true) {
            n = rand() % 1000;
            if (numberChecker(n)) {
                break;
            }
        } return n;
    }   
}

// function to assign attributes to stars
void starAttribute(Star s[20], char starName[20]) {
    for (int k = 0; k < 20; k++) {
        s[k].name = starName[k];
        s[k].x = generateValue(1);
        s[k].y = generateValue(1);
        s[k].z = generateValue(1);
        s[k].weight = generateValue(0);
        s[k].profit = generateValue(0);
    }
}

// function to add edges
void addEdge(vector<vector<int>>& adjacencyList, int src, int dest) {
        adjacencyList[src].push_back(dest);
        adjacencyList[dest].push_back(src);
}

bool isConnected(vector<vector<int>>& adjList, int A, int B) {
    // Check if B is in the adjacency list of A
    for(int i = 0; i < adjList[A].size(); i++) {
        if(adjList[A][i] == B) {
            return true;
        }
    } return false;
}

// function to calculate distance between two stars
double distanceCalculator(const Star& a, const Star& b) {
    return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2) + pow((b.z - a.z), 2));
}

// function to generate the star's edges
void edgeAttribute(Edge listEdge[], Star listStar[], vector<vector<int>>& adjacencyList) {
    int sum = 0;
    // assign 3 edge to each star
    for (int src = 0; src < 20; src++){
        while (adjacencyList[src].size() < 3){
            int end = rand() % 20;
            if (src != end && !isConnected(adjacencyList, src, end)){
                addEdge(adjacencyList, src, end);
                sum++;
            }
        }
    }

    // assign edge to random star until 54
    while (sum < 54) {
        int srcStar = rand() % 20;
        int endStar = rand() % 20;
        if (srcStar != endStar && !isConnected(adjacencyList, srcStar, endStar)) {
            addEdge(adjacencyList, srcStar, endStar);
            sum++;
        }
    }
    
    // key into listEdge
    sum = 0;
    for (int i = 0; i < 20; i++) {
        for (int connectedStar : adjacencyList[i]) {
            if (i < connectedStar) {
                listEdge[sum].source = listStar[i];
                listEdge[sum].ending = listStar[connectedStar];
                listEdge[sum].distance = distanceCalculator(listEdge[sum].source, listEdge[sum].ending);
                sum++;
            }
        }
    }
}

// function to save star attributes and adjacency list to file
void saveToFile(const Star listStar[], const Edge listEdge[], const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return;
    }

    // save stars' attributes
    file << "Star Attributes:\n";
    file << "Name\t X\t Y\t Z\t Weight\t Profit\n";
    for (int i = 0; i < 20; i++) {
        file << listStar[i].name << "\t" << listStar[i].x << "\t" << listStar[i].y << "\t" << listStar[i].z << "\t" 
        << listStar[i].weight << "\t" << listStar[i].profit << endl;
    }
    file << endl;

    // save edge list with distances
    file << "Edge List:\n";
    file << "Source\t Destination\t Distance\n";
    for (int i = 0; i < 54; i++) {
        file << listEdge[i].source.name << "\t" << listEdge[i].ending.name << "\t" << listEdge[i].distance << endl;
    }
    file.close();
    cout << "Data saved to " << filename << endl;
}

int main() {
    long long seed = 1211102060LL + 1221303348LL + 1221300839LL;
    srand(seed);
    // star names (A-T)
    char starName[] = {'A', 'B', 'C', 'D', 'E', 
                        'F', 'G', 'H', 'I', 'J', 
                        'K', 'L', 'M', 'N', 'O', 
                        'P', 'Q', 'R', 'S', 'T'};
    Star listStar[20];
    starAttribute(listStar, starName);
    vector<vector<int>> adjacencyList;
    adjacencyList.resize(20);
    Edge listEdge[54];
    edgeAttribute(listEdge, listStar, adjacencyList);
    // save data to file
    saveToFile(listStar, listEdge, "stars_data.txt");
}
