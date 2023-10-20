#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <iomanip>
#include <fstream>

class Node {
public:
    int key;
    Node* parent;
    std::vector<Node*> children;

    Node() {
        parent = nullptr;
    }

    void setParent(Node* theParent) {
        parent = theParent;
        parent->children.push_back(this);
    }
};


bool readInput(const std::string& inputFileName, int &n, std::vector<Node> &nodes) {
    std::ifstream inFile(inputFileName);

    if (!inFile.is_open()) {
        std::cerr << "Unable to open the input file '" << inputFileName << "'." << std::endl;
        return false;
    }

    // Initialize n to 0.
    n = 0;

    // Read all integer values from the input file and consider each value as a node.
    int value;
    while (inFile >> value) {
        nodes.push_back(Node()); // Create a new node for each value.
        n++;
    }
    n=n-1;

    inFile.close();

    // Ensure that at least one node was read.
    if (n == 0) {
        std::cerr << "No data found in the input file." << std::endl;
        return false;
    }

    return true;
}


void buildTree(std::vector<Node> &nodes, const std::string& inputFileName) {
    std::ifstream inFile(inputFileName);

    // Read the total number of nodes.
    int n;
    inFile >> n;

    // Initialize the nodes vector with n nodes.
    nodes.resize(n);

    for (int child_index = 0; child_index < n; ++child_index) {
        int parent_index;
        inFile >> parent_index;

        if (parent_index != -1) {
            // It's not the root node; set it as a child of its parent.
            nodes[child_index].setParent(&nodes[parent_index]);
        }

        // Set the key for the current node.
        nodes[child_index].key = child_index;

        // Debug output
        std::cout << "Node " << child_index << " is a child of Node " << parent_index << std::endl;
    }

    inFile.close();
}



int calculateTreeHeight(const std::vector<Node>& nodes) {
    int maxHeight = 0;
    
    bool measureTime = true; // Define measureTime here.

    // Find the root node (a node with a parent of -1).
    int rootIndex = -1;
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].parent == nullptr) {
            rootIndex = i;
            break; // Found the root, exit the loop.
        }
    }

    if (rootIndex != -1) {
        const Node* rootNode = &nodes[rootIndex]; // Use a pointer to the root node.
        std::queue<const Node*> nodeQueue;
        nodeQueue.push(rootNode);

        std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

        while (!nodeQueue.empty()) {
            int levelSize = nodeQueue.size();

            for (int i = 0; i < levelSize; ++i) {
                const Node* current = nodeQueue.front();
                nodeQueue.pop();

                for (const Node* child : current->children) {
                    nodeQueue.push(child);
                }
            }

            if (measureTime) {
                std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
                std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
                std::cout << "Elapsed time for height calculation: " << std::fixed << std::setprecision(6)
                    << duration.count() / 1000000.0 << " milliseconds" << std::endl;
                measureTime = false; // Disable further time measurement.
            }

            // Increment the height for each level.
            maxHeight++;
        }
    }

    return maxHeight;
}




int main() {
    const std::string inputFileName = "input_data.txt";
    int n = 0;
    std::vector<Node> nodes;

    if (!readInput(inputFileName, n, nodes)) {
        return 1;
    }

    if (n == 0) {
        std::cout << 0 << std::endl;
    } else {
        // Output the number of nodes read from the input.
        std::cout << "Number of nodes: " << n << std::endl;

        buildTree(nodes, inputFileName);  // Call the buildTree function.

        // Output the parent-child relationships.
        for (int i = 0; i < nodes.size(); ++i) {
            std::cout << "Node " << i << " has children: ";
            for (Node* child : nodes[i].children) {
                std::cout << child->key << " ";
            }
            std::cout << std::endl;
        }

        bool measureTime = true;
        int maxHeight = calculateTreeHeight(nodes);
        std::cout << "Height is: " << maxHeight << std::endl;

    }

    return 0;
}


