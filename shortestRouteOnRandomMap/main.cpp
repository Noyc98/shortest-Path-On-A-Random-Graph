#include "ControlManager.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, const char* argv[])
{
    // Check if a file path is provided as a command-line argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1; // Return an error code
    }

    // Get the file path from the command-line argument
    std::string filePath = argv[1];

    // Create a ControlManager instance
    ControlManager manager;

    // Read data from the input file
    manager.readData(filePath);

    // Perform convex hull computation
    manager.performConvexHull();

    // Find points in the line of sight
    manager.findPointslineOfSight();

    // Write the results to output files
    manager.writeData();
    manager.writelineOfSight();

    // Find the shortest route
    manager.findShortestRoute();

    return 0; // Return success
}