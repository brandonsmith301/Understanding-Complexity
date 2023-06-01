#ifndef DIJKSTRA_FIBONACCI_H
#define DIJKSTRA_FIBONACCI_H

// https://arxiv.org/pdf/2303.10034.pdf implements Fibonacci Heap for Dijkstras algorithm

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <limits>

struct FibonacciNode {
    int degree; 
    FibonacciNode* parent; 
    FibonacciNode* child; 
    FibonacciNode* left; 
    FibonacciNode* right;
    bool mark; 
    int key; 
    int nodeIndex; 
};

class FibonacciHeap {
public:
    FibonacciNode* minNode;
    int numNodes;
    std::vector<FibonacciNode*> degTable;
    std::vector<FibonacciNode*> nodePtrs;
    public:
    FibonacciHeap(int n) {
        this->numNodes = 0;
        this->minNode = NULL;
        this->degTable = {};
        this->nodePtrs.resize(n);
    }
    ~FibonacciHeap() {
        this->numNodes = 0;
        this->minNode = NULL;
        this->degTable.clear();
        this->nodePtrs.clear();
    }
    int size() {
        return this->numNodes;
    }
    bool empty() {
        if (this->numNodes > 0) return false;
        else return true;
    }
	void insert(int u, int key) {
		FibonacciNode* node = new FibonacciNode;
		node->degree = 0;
		node->parent = NULL;
		node->child = NULL;
		node->left = node;
		node->right = node;
		node->mark = false;
		node->key = key;
		node->nodeIndex = u;
		this->nodePtrs[u] = node;
		if (this->minNode == NULL) {
			this->minNode = node;
		} else {
			addNode(node, this->minNode);
			if (node->key < this->minNode->key) {
				this->minNode = node;
			}
		}
		this->numNodes++;
	}
	void addNode(FibonacciNode* node, FibonacciNode* root) {
		node->left = root;
		node->right = root->right;
		root->right = node;
		node->right->left = node;
	}
	void decreaseKey(FibonacciNode* node, int key) {
		if (key > node->key) {
			return;
		}
		node->key = key;
		FibonacciNode* parent = node->parent;
		if (parent != NULL && node->key < parent->key) {
			cut(node, parent);
			cascadingCut(parent);
		}
		if (node->key < this->minNode->key) {
			this->minNode = node;
		}
	}
	void cut(FibonacciNode* node, FibonacciNode* parent) {
		if (node == node->right) {
			parent->child = NULL;
		}
		node->left->right = node->right;
		node->right->left = node->left;
		if (node == parent->child) {
			parent->child = node->right;
		}
		parent->degree--;
		addNode(node, this->minNode);
		node->parent = NULL;
		node->mark = false;
	}
	void cascadingCut(FibonacciNode* node) {
		FibonacciNode* parent = node->parent;
		if (parent != NULL) {
			if (node->mark) {
				cut(node, parent);
				cascadingCut(parent);
			} else {
				node->mark = true;
			}
		}
	}
	FibonacciNode* extractMin() {
		FibonacciNode* ret = this->minNode;
		if (ret != NULL) {
			int numKids = ret->degree;
			FibonacciNode* x = ret->child;
			FibonacciNode* tempRight;
			while (numKids > 0) {
				tempRight = x->right;
				x->left->right = x->right;
				x->right->left = x->left;
				addNode(x, this->minNode);
				x->parent = NULL;
				x = tempRight;
				numKids--;
			}
			ret->left->right = ret->right;
			ret->right->left = ret->left;
			if (ret == ret->right) {
				this->minNode = NULL;
			} else {
				this->minNode = ret->right;
				consolidate();
			}
			this->numNodes--;
		}
		return ret;
	}
void consolidate() {
    int arraySize = floor(log2(this->numNodes)) + 1;
    this->degTable.resize(arraySize);
    for (int i = 0; i < arraySize; i++) {
        this->degTable[i] = NULL;
    }
    int numRoots = 0;
    FibonacciNode* x = this->minNode;
    if (x != NULL) {
        numRoots++;
        x = x->right;
        while (x != this->minNode) {
            numRoots++;
            x = x->right;
        }
    }

    int idx;
    for (int i = 0; i < numRoots; i++) {
        x = this->minNode;
        int d = x->degree;
        FibonacciNode* next = x->right;
        while (true) {
            if (this->degTable[d] != NULL) {
                FibonacciNode* y = this->degTable[d];
                if (x->key > y->key) {
                    FibonacciNode* temp = y;
                    y = x;
                    x = temp;
                }
                if (y == this->minNode) {
                    this->minNode = x;
                }
                link(y, x);
                if (x->right == x) {
                    this->minNode = x;
                }
                this->degTable[d] = NULL;
                d++;
            } else {
                break;
            }
        }
        this->degTable[d] = x;
        if (x->key < this->minNode->key) {
            this->minNode = x;
        }
    }
    for (int i = 0; i < arraySize; i++) {
        if (this->degTable[i] != NULL) {
            FibonacciNode* y = this->degTable[i];
            if (y->key < this->minNode->key) {
                this->minNode = y;
            }
        }
    }
}

void link(FibonacciNode* y, FibonacciNode* x) {
    y->left->right = y->right;
    y->right->left = y->left;
    if (x->right == x) {
        this->minNode = x;
    }
    y->left = y;
    y->right = y;
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
    }
    y->right = x->child;
    y->left = x->child->left;
    x->child->left->right = y;
    x->child->left = y;
    if (y->key < x->child->key) {
        x->child = y;
    }
    x->degree++;
}
FibonacciNode* findMin() {
    return this->minNode;
}
int getDist(int u) {
    return this->nodePtrs[u]->key;
}
};

#endif // DIJKSTRA_FIBONACCI_H

// // Function to perform Dijkstra's algorithm using Fibonacci Heap and adjacency list
// void dijkstraFibonacciHeapList(vector<vector<pair<int, int>>>& graph, int source) {
//     int V = graph.size(); // Number of vertices

//     // Create a Fibonacci heap to store vertices that are being processed
//     FibonacciHeap fibHeap(V);

//     // Create a vector to store distances from the source vertex
//     vector<int> dist(V, numeric_limits<int>::max());

//     // Initialise the distance of the source vertex to 0
//     dist[source] = 0;

//     // Insert all vertices into the heap
//     for (int i = 0; i < V; i++) {
//         fibHeap.insert(i, i == source ? 0 : numeric_limits<int>::max());
//     }

//     while (!fibHeap.empty()) {
//         // Extract the vertex with the minimum distance
//         int u = fibHeap.extractMin()->nodeIndex;

//         // Process each adjacent vertex of u
//         for (auto& edge : graph[u]) {
//             int v = edge.first;
//             int weight = edge.second;

//             // If a shorter path to v is found, update the distance and update the heap
//             if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
//                 // Decrease the key of v in the heap
//                 fibHeap.decreaseKey(fibHeap.nodePtrs[v], dist[u] + weight);

//                 // Update the distance of v
//                 dist[v] = dist[u] + weight;
//             }
//         }
//     }
// }

// void dijkstraFibonacciHeapMatrix(vector<vector<int>>& graph, int source) {
//     int V = graph.size(); // Number of vertices

//     // Create a Fibonacci heap to store vertices that are being processed
//     FibonacciHeap fibHeap(V);

//     // Create a vector to store distances from the source vertex
//     vector<int> dist(V, numeric_limits<int>::max());

//     // Initialise the distance of the source vertex to 0
//     dist[source] = 0;

//     // Insert all vertices into the heap
//     for(int i = 0; i < V; i++) {
//         fibHeap.insert(i, i == source ? 0 : numeric_limits<int>::max());
//     }

//     while (!fibHeap.empty()) {
//         // Extract the vertex with the minimum distance
//         int u = fibHeap.extractMin()->nodeIndex;

//         // Process each adjacent vertex of u
//         for(int v = 0; v < V; v++) {
//             // If there is no edge from u to v, skip
//             if(graph[u][v] == numeric_limits<int>::max()) {
//                 continue;
//             }

//             // If a shorter path to v is found, update the distance and update the heap
//             if(dist[u] != numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
//                 // Decrease the key of v in the heap
//                 fibHeap.decreaseKey(fibHeap.nodePtrs[v], dist[u] + graph[u][v]);

//                 // Update the distance of v
//                 dist[v] = dist[u] + graph[u][v];
//             }
//         }
//     }
// }