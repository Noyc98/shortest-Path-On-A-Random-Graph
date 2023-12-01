#include "Point.h"
#include <cmath>

// Constructor implementation
Point::Point(double x, double y): x(x), y(y){}

// Getter implementation for x-coordinate
double Point::getX() const 
{
    return this->x;
}

// Getter implementation for y-coordinate
double Point::getY() const 
{
    return this->y;
}

// Implementation of the equality (==) operator
bool Point::operator==(const Point& other)const
{
    return this->x == other.x && this->y == other.y;
}

/*
 * Getter for line of sight neighbors and their distances.
 *
 * @return A reference to the vector of neighbor points and distances.
 */
const std::vector<std::pair<Point*, double>>& Point::getlineOfSight() const {
    return lineOfSight;
}

/*
 * Sets a new neighbor in the line of sight with the given distance.
 *
 * @param newNeighbor - The new neighbor point.
 * @param distance - The distance to the new neighbor.
 */
void Point::setlineOfSight(Point* newNeighbor, double distance) {
    lineOfSight.push_back(std::make_pair(newNeighbor, distance));
}

/*
 * Utility function to calculate the cross product of three points.
 *
 * @param a, b, c - The three points for which the cross product is calculated.
 *
 * @return The cross product value.
 */
double Point::crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (c.getX() - a.getX()) * (b.getY() - a.getY());
}

/*
 * Calculate the Euclidean distance between two points.
 *
 * @param other - The other point to calculate the distance to.
 *
 * @return The Euclidean distance between the points.
 */
double Point::calculateDistance(const Point& other) const {
    return sqrt(pow(other.getX() - x, 2) + pow(other.getY() - y, 2));
}