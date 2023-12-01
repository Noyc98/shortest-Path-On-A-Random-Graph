#include "Map.h"

/*
 * Adds a polygon to the map.
 *
 * @param polygon - The polygon to be added to the map.
 */
void Map::addPolygon(const Polygon& polygon)
{
    polygons.push_back(polygon);
}

/*
 * Calculates the total area covered by all polygons on the map.
 *
 * @return The total area of the map.
 */
double Map::calculateTotalArea() const
{
    double totalArea = 0.0;

    // Iterate through each polygon and accumulate its area
    for (const Polygon& polygon : polygons)
    {
        totalArea += polygon.calculateArea();
    }

    return totalArea;
}

/*
 * Gets the size of the map.
 *
 * @return The size of the map.
 */
double Map::getMapSize()
{
    return mapSize;
}
