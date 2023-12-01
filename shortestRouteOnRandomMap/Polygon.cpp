#include "Polygon.h"
#include <cmath>

/*
 * Constructor for the Polygon class.
 *
 * @param points - The vertices of the polygon.
 * @param number - A unique identifier for the polygon.
 */
Polygon::Polygon(const std::vector<Point>& points, int number) : points(points), polygonNumber(number){}

/*
 * Gets the unique identifier of the polygon.
 *
 * @return The unique identifier of the polygon.
 */
int Polygon::getPolygonNumber() const {
    return polygonNumber;
}

/*
 * Calculates the area of the polygon.
 *
 * @return The area of the polygon.
 */
double Polygon::calculateArea() const {

    int n = points.size();
    double area = 0.0;

    // Apply the shoelace formula to calculate the area of the polygon
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += (points[i].getX() * points[j].getY() - points[j].getX() * points[i].getY());
    }

    // Take the absolute value and divide by 2 to get the area
    return std::abs(area) / 2.0;
}

/*
 * Gets the array of points representing the vertices of the polygon.
 *
 * @return A reference to the array of points.
 */
std::vector<Point>& Polygon::getPolygonsPointsArray()
{
    return points;
}