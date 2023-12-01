#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "Map.h"
#include "Point.h"


class ControlManager {
private:
    int mapSize, numPolygons;
    std::vector<Polygon> polygons;
    Point startPoint;
    Point endPoint;
    std::vector<Point> path;

    // Functions for initializing line of sight information
    void allToAll();
    void startPointToAll();
    void allToEndPoint();
    void endPointToAll();
    void startPointToEndP();

    // Function to find line of sight between two points
    void findlineOfSight(Point* currentStart, Point* currentEnd);

    // Function to write line of sight information to a file
    void outlineOfSightArrayWithDistances(const std::vector<std::pair<Point*, double>>& lineOfSightArray, std::ofstream& outFile);

    // A* algorithm implementation for finding the shortest route
    std::vector<Point> aStar(Point& start, Point& goal);
public:
    // Constructor
    ControlManager();

    // Getter for the path
    const std::vector<Point>& getPath() const;

    // Setter for the path
    void setPath(const std::vector<Point>& newPath);

    // Read map data from a file
    void readData(const std::string& filename);

    // Write map data to a file
    void writeData();

    // Convex hull computation
    void performConvexHull();

    // Find line of sight for all points
    void findPointslineOfSight();

    // Write line of sight information to a file
    void writelineOfSight();

    // Find the shortest route using A* algorithm
    void findShortestRoute();
};