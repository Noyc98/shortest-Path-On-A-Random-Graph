#pragma once
#include "Polygon.h"
#include <vector>

class Map {
private:
    // Collection of polygons representing the map
    std::vector<Polygon> polygons;

    // Size of the map (assuming it's a square map)
    double mapSize;

public:
    // Add a polygon to the map
    void addPolygon(const Polygon& polygon);

    // Get the size of the map
    double getMapSize();

    // Calculate the total area covered by all polygons on the map
    double calculateTotalArea() const;
};
