#pragma once
#include <vector>

class Point {
private:
    double x, y;
    std::vector<std::pair<Point*, double>> lineOfSight; // Store neighbors and their distances

public:
    // Constructor
    Point(double x, double y);

    // Getter for x-coordinate
    double getX() const;

    // Getter for y-coordinate
    double getY() const;

    // Getter for line of sight neighbors and their distances
    const std::vector<std::pair<Point*, double>>& getlineOfSight() const;

    // Set a new neighbor in the line of sight with the given distance
    void setlineOfSight(Point* newNeighbor, double distance);

    // Overload the equality (==) operator
    bool operator==(const Point& other) const;

    // Utility function to calculate the cross product of three points
    static double crossProduct(const Point& a, const Point& b, const Point& c);

    // Calculate the Euclidean distance between two points
    double calculateDistance(const Point& other) const;
};