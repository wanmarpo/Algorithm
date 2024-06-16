#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <climits>
#include <chrono>
#include <unordered_map>

using namespace std;

struct Star {
    string name;
    int x, y, z, weight, profit;
};

struct Edge {
    int source, destination, distance;
};

struct GraphData {
    unordered_map<string, pair<int, int>> nodePositions;
    vector<pair<string, string>> edges;
};

GraphData generateGraphData(const vector<Star>& listStar, const vector<Edge>& listEdge, const vector<int>& distances, const vector<int>& parents, int source) {
    GraphData graphData;

    // Store node positions
    for (const auto& star : listStar) {
        graphData.nodePositions[star.name] = make_pair(star.x, star.y);
    }

    // Store edges with their source and destination node names
    for (const auto& edge : listEdge) {
        string source = string(1, 'A' + edge.source);
        string destination = string(1, 'A' + edge.destination);
        graphData.edges.emplace_back(source, destination);
    }

    // Store all shortest paths as edges
    for (int i = 0; i < distances.size(); ++i) {
        if (distances[i] != INT_MAX) {
            vector<int> path;
            for (int v = i; v != -1; v = parents[v])
                path.push_back(v);
            reverse(path.begin(), path.end());

            string currentNode = string(1, 'A' + source);
            for (int j = 1; j < path.size(); ++j) {
                string nextNode = string(1, 'A' + path[j]);
                graphData.edges.emplace_back(currentNode, nextNode);
                currentNode = nextNode;
            }
        }
    }

    return graphData;
}
void generatePathDOTFile(const GraphData& graphData, const vector<int>& path, int pathIndex, const string& filename) {
    ofstream dotFile(filename);
    if (dotFile.is_open()) {
        dotFile << "graph G {\n";
        dotFile << "  node [shape=circle, fontsize=10];\n";
        dotFile << "  edge [fontsize=8];\n";

        // Write node positions
        for (const auto& node : graphData.nodePositions) {
            dotFile << "  " << node.first << " [pos=\"" << node.second.first << "," << node.second.second << "!\"];\n";
        }

        // Write regular edges
        dotFile << "  // Regular edges\n";
        dotFile << "  edge [color=black, style=solid];\n";
        for (const auto& edge : graphData.edges) {
            dotFile << "  " << edge.first << " -- " << edge.second << ";\n";
        }

        // Write the current path as a separate edge
        dotFile << "  // Path " << pathIndex << "\n";
        dotFile << "  edge [color=red, style=dashed];\n";
        string currentNode = string(1, 'A' + path[0]);
        for (int i = 1; i < path.size(); ++i) {
            string nextNode = string(1, 'A' + path[i]);
            dotFile << "  " << currentNode << " -- " << nextNode << " [constraint=false];\n";
            currentNode = nextNode;
        }

        dotFile << "}\n";
        dotFile.close();
    } else {
        cerr << "Unable to open DOT file" << endl;
    }
}
void generateDOTFile(const GraphData& graphData, const string& filename) {
    ofstream dotFile(filename);
    if (dotFile.is_open()) {
        dotFile << "graph G {\n";
        dotFile << "  node [shape=circle, fontsize=10];\n";
        dotFile << "  edge [fontsize=8];\n";

        // Write node positions
        for (const auto& node : graphData.nodePositions) {
            dotFile << "  " << node.first << " [pos=\"" << node.second.first << "," << node.second.second << "!\"];\n";
        }

        // Write regular edges
        dotFile << "  // Regular edges\n";
        dotFile << "  edge [color=black, style=solid];\n";
        for (const auto& edge : graphData.edges) {
            dotFile << "  " << edge.first << " -- " << edge.second << ";\n";
        }

        // Write shortest path edges
        dotFile << "  // Shortest path edges\n";
        dotFile << "  edge [color=red, style=dashed];\n";
        for (const auto& edge : graphData.edges) {
            dotFile << "  " << edge.first << " -- " << edge.second << " [constraint=false];\n";
        }

        dotFile << "}\n";
        dotFile.close();
    } else {
        cerr << "Unable to open DOT file" << endl;
    }
}

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

// Dijkstra's algorithm to find the shortest path from source to all other nodes
void dijkstra(const vector<vector<pair<int, int>>>& adj, int numVertices, int source, vector<int>& distances, vector<int>& parents) {
    distances.assign(numVertices, INT_MAX);
    parents.assign(numVertices, -1);
    distances[source] = 0;

    using P = pair<int, int>; // Pair (distance, vertex)
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (dist > distances[u])
            continue;

        for (const auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                parents[v] = u;
                pq.push({distances[v], v});
            }
        }
    }
}

// Function to print the shortest path
void printShortestPath(const vector<int>& parents, int start, int goal, ostream& os) {
    vector<int> path;
    for (int v = goal; v != -1; v = parents[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] == goal) {
        os << "No path from " << char('A' + start) << " to " << char('A' + goal) << "\n";
    } else {
        os << "Shortest path from " << char('A' + start) << " to " << char('A' + goal) << ": ";
        for (int v : path)
            os << char('A' + v) << " ";
        os << endl;
    }
}

// Function to print all shortest paths from the source
void printAllShortestPaths(const vector<int>& distances, const vector<int>& parents, int source, ostream& os) {
    os << "Shortest distances from " << char('A' + source) << ":\n";
    for (int i = 0; i < distances.size(); ++i) {
        os << char('A' + source) << " to " << char('A' + i) << " = ";
        if (distances[i] == INT_MAX) {
            os << "∞ (unreachable)\n";
        } else {
            os << distances[i] << "\n";
        }
        printShortestPath(parents, source, i, os);
        if (distances[i] != INT_MAX) {
            os << "Total distance: " << distances[i] << "\n\n";
        }
    }
}

// Function to calculate and print the space complexity
void calculateSpaceComplexity(const vector<vector<pair<int, int>>>& adj, const vector<int>& distances, const vector<int>& parents, ostream& os) {
    size_t adjSize = 0;
    for (const auto& edges : adj) {
        adjSize += edges.size() * sizeof(pair<int, int>);
    }
    size_t distancesSize = distances.size() * sizeof(int);
    size_t parentsSize = parents.size() * sizeof(int);

    os << "Space complexity:\n";
    os << "Adjacency list size: " << adjSize << " bytes\n";
    os << "Distances vector size: " << distancesSize << " bytes\n";
    os << "Parents vector size: " << parentsSize << " bytes\n";
    os << "Total space used: " << (adjSize + distancesSize + parentsSize) << " bytes\n";
}


int main() {
    vector<Star> listStar;
    vector<Edge> listEdge;
    loadFromFile(listStar, listEdge, "stars_data.txt");

    cout << "Stars loaded: " << listStar.size() << endl;
    for (const auto& star : listStar) {
        cout << "Star: " << star.name << " (" << star.x << ", " << star.y << ", " << star.z << ")\n";
    }

    cout << "Edges loaded: " << listEdge.size() << endl;
    for (const auto& edge : listEdge) {
        cout << "Edge: " << char('A' + edge.source) << " -> " << char('A' + edge.destination) << " with distance " << edge.distance << endl;
    }

    int numVertices = listStar.size();
    vector<vector<pair<int, int>>> adj(numVertices);

    for (const auto& edge : listEdge) {
        adj[edge.source].emplace_back(edge.destination, edge.distance);
        adj[edge.destination].emplace_back(edge.source, edge.distance);
    }

    vector<int> distances;
    vector<int> parents;

    int source = 0; // Assuming 'A' is the starting node

    auto start = chrono::high_resolution_clock::now();
    dijkstra(adj, numVertices, source, distances, parents);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    ofstream outputFile("path_output.txt");
    if (outputFile.is_open()) {
        printAllShortestPaths(distances, parents, source, outputFile);
        outputFile << "Time complexity: " << duration.count() << " seconds\n";
        calculateSpaceComplexity(adj, distances, parents, outputFile);
        outputFile.close();
    } else {
        cerr << "Unable to open output file" << endl;
    }

    // Generate graph data
    GraphData graphData = generateGraphData(listStar, listEdge, distances, parents, source);

    // Generate DOT files for each path
    int pathIndex = 1;
    for (int i = 0; i < distances.size(); ++i) {
        if (distances[i] != INT_MAX) {
            vector<int> path;
            for (int v = i; v != -1; v = parents[v])
                path.push_back(v);
            reverse(path.begin(), path.end());

            string filename = "path_" + to_string(pathIndex) + ".dot";
            generatePathDOTFile(graphData, path, pathIndex, filename);
            cout << "Path " << pathIndex << " DOT file generated as " << filename << endl;
            pathIndex++;
        }
    }
    // Generate DOT file
    generateDOTFile(graphData, "graph.dot");

    cout << "Shortest paths generated and saved to path_output.txt" << endl;
    cout << "Graph DOT file generated as graph.dot" << endl;

    return 0;
}