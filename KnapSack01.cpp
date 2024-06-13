#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Star {
    string name;
    int x, y, z, weight, profit;
};

struct Edge {
    Star source, ending;
    int distance;
};

// knapsack algorithm
void knapsack(Star stars[], int n, int W, string filename) {
    vector<vector<int>> dp(n+1, vector<int>(W+1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            // check if the weight of current star is less than or equal to to the current capacity,w;
            if (stars[i-1].weight <= w) {
                // compare to visit the current star or do not using max
                dp[i][w] = max(dp[i-1][w], dp[i-1][w - stars[i-1].weight] + stars[i-1].profit);
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    // identify the visited stars in the optimal solution
    vector<Star> selectedStars;
    int w = W;
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selectedStars.push_back(stars[i - 1]);
            w -= stars[i - 1].weight;
        }
    }

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return;
    }

    file << "Knapsack Dynamic Programming Matrix:\n";
    // number of capacity
    file << "\t";
    for (int j = 0; j <= W; j++) {
            file << j << "\t";
    }
    file << endl;

    file << "\t";
    for (int i = 0; i <= n; i++) {
        
        for (int j = 0; j <= W; j++) {
            file << dp[i][j] << "\t";
        }
        file << endl;
        file << stars[i].name << "\t";
    }

    int totalWeight = 0; 
    int totalProfit = 0;
    file << "\nSelected Stars:\n";
    file << "Name\tWeight\tProfit\n";
    for (const auto& star : selectedStars) {
        file << "Star " << star.name << "\t" << star.weight << "\t" << star.profit << "\n";
        totalWeight += star.weight;
        totalProfit += star.profit;
    }
    file << endl;
    file << "Total Weight of visited stars: " << totalWeight << "\n";
    file << "Total Profit of visited stars: " << totalProfit << "\n";
    file.close();

    cout << "Data saved to " << filename << endl;
}

// function to load star attributes and adjacency list from file
void loadFromFile(Star listStar[], Edge listEdge[], const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return;
    }

    string line;
    int starCount = 0;

    // read the star attributes
    while (getline(file, line)) {
        if (line.find("Star Attributes:") != string::npos) {
            // skip header line
            getline(file, line);
            while (getline(file, line) && !line.empty() && starCount < 20) {
                stringstream ss(line);
                string token;

                getline(ss, token, '\t');
                listStar[starCount].name = token;
                getline(ss, token, '\t');
                listStar[starCount].x = stoi(token);
                getline(ss, token, '\t');
                listStar[starCount].y = stoi(token);
                getline(ss, token, '\t');
                listStar[starCount].z = stoi(token);
                getline(ss, token, '\t');
                listStar[starCount].weight = stoi(token);
                getline(ss, token, '\t');
                listStar[starCount].profit = stoi(token);

                starCount++;
            }
        }

        int edgeCount = 0;
        if (line.find("Adjacency List") != string::npos) {
            // skip header line
            getline(file, line);
            while (getline(file, line) && !line.empty() && edgeCount < 54) {
                stringstream ss(line);
                string source, dest;
                int dist;

                getline(ss, source, '\t');
                getline(ss, dest, '\t');
                ss >> dist;

                for (int k = 0; k < 20; k++) {
                    if (listStar[k].name == source) {
                        listEdge[edgeCount].source = listStar[k];
                    }
                }
                for (int k = 0; k < 20; k++) {
                    if (listStar[k].name == dest) {
                        listEdge[edgeCount].ending = listStar[k];
                    }
                }

                listEdge[edgeCount].distance = dist;
                edgeCount++;
            }
        }
        
    }
    file.close();
}

int main() {
    int numStar = 20;
    Star listStar[numStar];
    Edge listEdge[54];
    loadFromFile(listStar, listEdge, "stars_data.txt");

    int maxWeight = 800;  // Spaceship capacity
    knapsack(listStar, numStar, maxWeight, "knapsack_output.txt");
}
