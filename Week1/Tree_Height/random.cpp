#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

int main() {
    const int numNumbers = 500;
    const int minValue = 0;
    const int maxValue = 499;
    
    std::vector<int> numbers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(minValue, maxValue);

    // Generate 499 random numbers
    for (int i = 0; i < numNumbers - 1; ++i) {
        int randomNum = dis(gen);
        numbers.push_back(randomNum);
    }

    // Add the number '-1' once
    numbers.push_back(-1);

    // Shuffle the numbers randomly
    std::shuffle(numbers.begin(), numbers.end(), gen);

    // Write the numbers to a file
    std::ofstream outFile("input_data.txt");
    if (outFile.is_open()) {
        for (int num : numbers) {
            outFile << num << '\n';
        }
        outFile.close();
        std::cout << "File 'input_data.txt' created successfully." << std::endl;
    } else {
        std::cerr << "Unable to create the file." << std::endl;
    }

    return 0;
}
