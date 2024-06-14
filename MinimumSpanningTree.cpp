#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Star {
    string name;
    int x, y, z, weight, profit;
};

struct Edge {
    int source, destination, distance;
};

// Function to load star attributes and edge list from file
void loadFromFile(vector<Star>& listStar, vector<Edge>& listEdge, const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return;
    }

    string line;

    // Read star attributes
    while (getline(file, line)) {
        if (line.find("Star Attributes:") != string::npos) {
            // Skip header line
            getline(file, line);
            while (getline(file, line) && !line.empty()) {
                stringstream ss(line);
                Star star;
                string token;

                getline(ss, star.name, '\t');
                getline(ss, token, '\t');
                star.x = stoi(token);
                getline(ss, token, '\t');
                star.y = stoi(token);
                getline(ss, token, '\t');
                star.z = stoi(token);
                getline(ss, token, '\t');
                star.weight = stoi(token);
                getline(ss, token, '\t');
                star.profit = stoi(token);

                listStar.push_back(star);
            }
        }
        if (line.find("Edge List:") != string::npos) {
            // Skip header line
            getline(file, line);
            while (getline(file, line) && !line.empty()) {
                stringstream ss(line);
                Edge edge;
                string source, destination;
                int dist;

                getline(ss, source, '\t');
                getline(ss, destination, '\t');
                ss >> dist;

                edge.source = source[0] - 'A'; 
                edge.destination = destination[0] - 'A'; 
                edge.distance = dist;

                listEdge.push_back(edge);
            }
        }
    }
    file.close();
}

// Comparator function to sort edges by distance
bool compareEdge(Edge e1, Edge e2) {
    return e1.distance < e2.distance;
}

// Function to find the subset of an element i
int find(vector<int>& parent, int i) {
    if (parent[i] != i)
        parent[i] = find(parent, parent[i]);
    return parent[i];
}

// Function to do union of two subsets
void Union(vector<int>& parent, vector<int>& rank, int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}


// Function to perform Kruskal's algorithm and find the MST
void kruskalMST(vector<Star>& listStar, vector<Edge>& listEdge) {
    // Sort edges in increasing order of distance
    sort(listEdge.begin(), listEdge.end(), compareEdge);

    int V = listStar.size();
    vector<int> parent(V);
    vector<int> rank(V, 0);

    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    vector<Edge> mst;
    for (auto& edge : listEdge) {
        int root1 = find(parent, edge.source);
        int root2 = find(parent, edge.destination);

        if (root1 != root2) {
            mst.push_back(edge);
            Union(parent, rank, root1, root2);
        }

        if (mst.size() == V - 1) {
            break;
        }
    }

    // Save the MST to a file
    ofstream outFile("mst_output.txt");
    if (!outFile.is_open()) {
        cerr << "Unable to open output file" << endl;
        return;
    }

    outFile << "MST Edges:\n";
    for (const auto& edge : mst) {
        outFile << listStar[edge.source].name << "\t" 
                << listStar[edge.destination].name << "\t" 
                << edge.distance << "\n";
    }
    outFile.close();
}

int main() {
    vector<Star> listStar;
    vector<Edge> listEdge;
    loadFromFile(listStar, listEdge, "stars_data.txt");

    kruskalMST(listStar, listEdge);

    cout << "MST generated and saved to mst_output.txt" << endl;

    return 0;
}
