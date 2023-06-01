#include <queue> // for priority_queue
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <limits>
#include <chrono>
#include <functional>
#include <set>
#include <string> 
#include <random>

using namespace std;

struct Edge {
    int destination;
    int weight;

    Edge(int dest, int w) : destination(dest), weight(w) {}
};

struct Graph {
    vector<vector<Edge>> graphList;
    vector<vector<int>> graphMatrix;

    Graph(int V) : graphList(V), graphMatrix(V, vector<int>(V, 0)) {}
};

void addEdgeToList(Graph& graph, int source, int destination, int weight) {
    graph.graphList[source].push_back(Edge(destination, weight));
}

void addEdgeToMatrix(Graph& graph, int source, int destination, int weight) {
    graph.graphMatrix[source][destination] = weight;
}

// Function to perform Dijkstra's algorithm using adjacency list
void dijkstraBasicFormList(Graph& graph, int source) {
    int V = graph.graphList.size(); // Number of vertices

    // Create a priority queue to store vertices that are being processed
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Create a vector to store distances from the source vertex
    vector<int> dist(V, numeric_limits<int>::max());

    // Initialise the distance of the source vertex to 0
    dist[source] = 0;

    // Push the source vertex into the priority queue
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        // Extract the minimum distance vertex from the priority queue
        int u = pq.top().second;
        pq.pop();

        // Iterate through all adjacent vertices of u
        for (const Edge& edge : graph.graphList[u]) {
            int v = edge.destination;
            int weight = edge.weight;

            // If a shorter path is found, update the distance and push the vertex into the priority queue
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
}

// Function to perform Dijkstra's algorithm using adjacency matrix
void dijkstraBasicFormMatrix(Graph& graph, int source) {
    int V = graph.graphMatrix.size(); // Number of vertices

    // Create a vector to store distances from the source vertex
    vector<int> dist(V, numeric_limits<int>::max());

    // Create a vector to track visited vertices
    vector<bool> visited(V, false);

    // Initialise the distance of the source vertex to 0
    dist[source] = 0;

    // Find shortest paths for all vertices
    for (int count = 0; count < V - 1; ++count) {
        // Find the vertex with the minimum distance value among the unvisited vertices
        int minDist = numeric_limits<int>::max();
        int u = -1;
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        // If all vertices are visited or distances to all vertices are infinity,
        // then u will be -1 and we should break the loop
        if (u == -1) {
            break;
        }

        // Mark the selected vertex as visited
        visited[u] = true;

        // Update distances of the adjacent vertices
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && graph.graphMatrix[u][v] != 0 && dist[u] != numeric_limits<int>::max() &&
                dist[u] + graph.graphMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph.graphMatrix[u][v];
            }
        }
    }
}

// Function to perform Dijkstra's algorithm using binary heap and adjacency list
void dijkstraBinaryHeapList(Graph& graph, int source) {
    int V = graph.graphList.size(); // Number of vertices

    // Create a vector to store distances from the source vertex
    vector<int> dist(V, numeric_limits<int>::max());

    // Create a priority queue (min heap) to store vertices and their distances
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Initialise the distance of the source vertex to 0
    dist[source] = 0;

    // Insert the source vertex into the priority queue
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        // Extract the vertex with the minimum distance
        int u = pq.top().second;
        pq.pop();

        // Process each adjacent vertex of u
        for (const auto& edge : graph.graphList[u]) {
            int v = edge.destination;
            int weight = edge.weight;

            // If a shorter path to v is found, update the distance
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
}

// Function to perform Dijkstra's algorithm using binary heap and adjacency matrix
void dijkstraBinaryHeapMatrix(Graph& graph, int source) {
    int V = graph.graphMatrix.size(); // Number of vertices

    // Create a vector to store distances from the source vertex
    vector<int> dist(V, numeric_limits<int>::max());

    // Create a priority queue (min heap) to store vertices and their distances
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Initialise the distance of the source vertex to 0
    dist[source] = 0;

    // Insert the source vertex into the priority queue
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        // Extract the vertex with the minimum distance
        int u = pq.top().second;
        pq.pop();

        // Process each adjacent vertex of u
        for (int v = 0; v < V; ++v) {
            // If there is an edge from u to v
            if (graph.graphMatrix[u][v] > 0) {
                int weight = graph.graphMatrix[u][v];

                // If a shorter path to v is found, update the distance
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }
    }
}

// Function to perform Dijkstra's algorithm using balanced BST (set) and adjacency list
void dijkstraBalancedBSTList(Graph& graph, int source) {
    int V = graph.graphList.size(); // Number of vertices

    // Create a set to store vertices that are being processed
    set<pair<int, int> > bst;

    // Create a vector to store distances from the source vertex
    vector<int> dist(V, numeric_limits<int>::max());

    // Initialise the distance of the source vertex to 0
    dist[source] = 0;

    // Insert the source vertex into the set
    bst.insert({ dist[source], source });

    while (!bst.empty()) {
        // Extract the vertex with the minimum distance
        int u = bst.begin()->second;
        bst.erase(bst.begin());

        // Process each adjacent vertex of u
        for (const auto& edge : graph.graphList[u]) {
            int v = edge.destination;
            int weight = edge.weight;

            // If a shorter path to v is found, update the distance and update the set
            if (dist[u] + weight < dist[v]) {
                // Remove the old distance-vertex pair from the set
                bst.erase({ dist[v], v });

                // Update the distance of v
                dist[v] = dist[u] + weight;

                // Insert the updated distance-vertex pair into the set
                bst.insert({ dist[v], v });
            }
        }
    }
}

// Function to perform Dijkstra's algorithm using balanced BST (set) and adjacency matrix
void dijkstraBalancedBSTMatrix(Graph& graph, int source) {
    int V = graph.graphMatrix.size(); // Number of vertices

    // Create a set to store vertices that are being processed
    set<pair<int, int>> bst;

    // Create a vector to store distances from the source vertex
    vector<int> dist(V, numeric_limits<int>::max());

    // Initialise the distance of the source vertex to 0
    dist[source] = 0;

    // Insert the source vertex into the set
    bst.insert({ dist[source], source });

    while (!bst.empty()) {
        // Extract the vertex with the minimum distance
        int u = bst.begin()->second;
        bst.erase(bst.begin());

        // Process each adjacent vertex of u
        for (int v = 0; v < V; ++v) {
            // If there is an edge from u to v
            if (graph.graphMatrix[u][v] > 0) {
                int weight = graph.graphMatrix[u][v];

                // If a shorter path to v is found, update the distance and update the set
                if (dist[u] + weight < dist[v]) {
                    // Remove the old distance-vertex pair from the set
                    bst.erase({ dist[v], v });

                    // Update the distance of v
                    dist[v] = dist[u] + weight;

                    // Insert the updated distance-vertex pair into the set
                    bst.insert({ dist[v], v });
                }
            }
        }
    }
}



// Function template to measure the execution time of a function
template <typename Func, typename... Args>
auto measureExecutionTime(Func&& func, Args&&... args) {
    auto start = chrono::steady_clock::now();
    std::forward<Func>(func)(std::forward<Args>(args)...);
    auto end = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::duration<float>>(end - start).count();
}

void dijkstraBasicFormList(Graph& graph, int source);
void dijkstraBasicFormMatrix(Graph& graph, int source);
void dijkstraBinaryHeapList(Graph& graph, int source);
void dijkstraBinaryHeapMatrix(Graph& graph, int source);
void dijkstraBalancedBSTList(Graph& graph, int source);
void dijkstraBalancedBSTMatrix(Graph& graph, int source);

std::vector<std::vector<double>> allExecutionTimes(8);
std::vector<std::string> columnNames = {"Graph Size", "Density", "Dijkstra Basic Form List", "Dijkstra Basic Form Matrix", "Dijkstra Binary Heap List", "Dijkstra Binary Heap Matrix", "Dijkstra Balanced BST List", "Dijkstra Balanced BST Matrix"};

int main() {
    std::vector<int> graphSizes;
    const int numTestsPerSize = 100;
    const int maxWeight = 5; // Maximum edge weight
    std::default_random_engine generator; // Creates a generator
    std::uniform_int_distribution<int> weight_distribution;
    std::uniform_real_distribution<double> prob_distribution;
    
    try {
        graphSizes = {5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
        weight_distribution = std::uniform_int_distribution<int>(1, maxWeight); // Defines the range for your weights
        prob_distribution = std::uniform_real_distribution<double>(0.0, 1.0); // Defines the range for your probability
    } catch (const std::exception& e) {
        std::cerr << "Exception caught at the start of main: " << e.what() << '\n';
        return 1;
    }

    for (int graphSize : graphSizes) {
        for (int weight = 1; weight <= maxWeight; weight += 2) {
            for (int i = 0; i < numTestsPerSize; ++i) {
                int source = 0;
                Graph graph(graphSize);
                int numEdges = 0;

                for(int u = 0; u < graphSize; ++u) {
                    for(int v = u + 1; v < graphSize; ++v) {
                        if (prob_distribution(generator) < 0.1) {
                            int edgeWeight = weight_distribution(generator); // Generate a random weight
                            try {
                                addEdgeToList(graph, u, v, edgeWeight);
                                addEdgeToMatrix(graph, u, v, edgeWeight);
                                numEdges++;
                            } catch(const std::out_of_range& e) {
                                std::cerr << "Out of Range error in adding edge: " << e.what() << '\n';
                                continue;
                            } catch(const std::exception& e) {
                                std::cerr << "Exception caught in adding edge: " << e.what() << '\n';
                                continue;
                            }
                        }
                    }
                }

                float density = static_cast<float>(numEdges) / static_cast<float>(graphSize * (graphSize - 1) / 2);
                try {
                    allExecutionTimes[0].push_back(graphSize);
                    allExecutionTimes[1].push_back(density);
                } catch(const std::exception& e) {
                    std::cerr << "Exception caught in adding GWD: " << e.what() << '\n';
                    continue;
                    }
                try {
                    allExecutionTimes[2].push_back(measureExecutionTime(dijkstraBasicFormList, graph, source));
                    allExecutionTimes[3].push_back(measureExecutionTime(dijkstraBasicFormMatrix, graph, source));
                    allExecutionTimes[4].push_back(measureExecutionTime(dijkstraBinaryHeapList, graph, source));
                    allExecutionTimes[5].push_back(measureExecutionTime(dijkstraBinaryHeapMatrix, graph, source));
                    allExecutionTimes[6].push_back(measureExecutionTime(dijkstraBalancedBSTList, graph, source));
                    allExecutionTimes[7].push_back(measureExecutionTime(dijkstraBalancedBSTMatrix, graph, source));
                } catch(const std::exception& e) {
                    std::cerr << "Exception caught in measuring execution time: " << e.what() << '\n';
                    continue;
                }
            }
        }
    }
    // Write the execution times and column names to the CSV file
    try {
        std::ofstream outputFile("execution_times.csv");
        if (outputFile.is_open()) {
            // Write column names
            for (size_t i = 0; i < columnNames.size(); ++i) {
                outputFile << columnNames[i];
                if (i < columnNames.size() - 1) {
                    outputFile << ",";
                }
            }
            outputFile << std::endl;

            // Write execution times
            for (size_t i = 0; i < allExecutionTimes[0].size(); ++i) {
                for (size_t j = 0; j < allExecutionTimes.size(); ++j) {
                    outputFile << allExecutionTimes[j][i];
                    if (j < allExecutionTimes.size() - 1) {
                        outputFile << ",";
                    }
                }
                outputFile << std::endl;
            }

            outputFile.close();
            std::cout << "Execution times and graph density have been exported to 'execution_times.csv'." << std::endl;
        } else {
            std::cout << "Failed to open the output file." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught while writing to the CSV file: " << e.what() << std::endl;
    }

return 0;
}


