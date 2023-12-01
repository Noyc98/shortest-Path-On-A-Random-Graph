#pragma once
#include "Point.h"
#include <vector>

class Polygon {
private:
	// The vertices of the polygon
	std::vector<Point> points;
	// A unique identifier for the polygon
	int polygonNumber;

public:
	// Constructor for the Polygon class.
	Polygon(const std::vector<Point>& points, int number);

	// Gets the unique identifier of the polygon.
	int getPolygonNumber() const;

	// Calculates the area of the polygon.
	double calculateArea() const;

	// Gets the array of points representing the vertices of the polygon.
	std::vector<Point>& getPolygonsPointsArray();
};
