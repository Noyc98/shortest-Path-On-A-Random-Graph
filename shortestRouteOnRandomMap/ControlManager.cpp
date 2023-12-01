#include "ControlManager.h"
#include "Point.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <limits>
#include <queue>
#include <functional>


// Constructor implementation
ControlManager::ControlManager() : mapSize(0), numPolygons(0), startPoint(Point(0,0)), endPoint(Point(mapSize,mapSize)) {
}

/*
 * Getter for the path.
 *
 * @return A reference to the vector of points representing the path.
 */
const std::vector<Point>& ControlManager::getPath() const {
    return path;
}

/*
 * Setter for the path.
 *
 * @param newPath - The new path to be set.
 */
void ControlManager::setPath(const std::vector<Point>& newPath) {
    path = newPath;
}

 /*
 * Write data to a text file.
 */
void ControlManager::writeData()
{
    // Create and open a text file
    std::ofstream writeFile("writeFile.txt");

    // Iterate over polygons and write their data to the file
    for (int polygonNumber = 0; polygonNumber < numPolygons; polygonNumber++)
    {
        int pointsNumber = polygons[polygonNumber].getPolygonsPointsArray().size();
        writeFile << polygonNumber << "\n" << pointsNumber << "\n";

        // Write each point's coordinates to the file
        for (int j = 0; j < pointsNumber; j++)
        {
            writeFile << polygons[polygonNumber].getPolygonsPointsArray()[j].getX() << " ";
            writeFile << polygons[polygonNumber].getPolygonsPointsArray()[j].getY() << "\n";
        }

        writeFile << "\n";
    }

    writeFile.close();
}

/*
 * Read data from a text file.
 *
 * @param filename - The name of the file to read data from.
 */
void ControlManager:: readData(const std::string& filename)
{
    std::ifstream file(filename); // Open the text file for reading

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        exit(1);
    }

    file >> mapSize; // Read map size
    file >> numPolygons; // Read number of polygons

    // Iterate over polygons in the file and read their data
    for (int i = 0; i < numPolygons; ++i)
    {
        int polygonNumber, numPoints;
        file >> polygonNumber >> numPoints;

        std::vector<Point> polygonPoints;

        // Read each point's coordinates and create a polygon
        for (int j = 0; j < numPoints; ++j) 
        {
            double x, y;
            file >> x >> y;
            polygonPoints.emplace_back(x, y);
        }
        Polygon polygon(polygonPoints, polygonNumber);
        polygons.push_back(polygon);
    }
    this->startPoint = {0.0, 0.0};
    this->endPoint = {(double)mapSize, (double)mapSize};

    file.close();
}


// Convex Hull //

/*
 * Custom comparator function for sorting points based on polar angles and distances from a pivot point.
 *
 * @param pivot - The pivot point.
 * @param p1 - The first point for comparison.
 * @param p2 - The second point for comparison.
 *
 * @return True if p1 is less than p2 in terms of polar angle and distance from the pivot, false otherwise.
 */
// Convex Hull //
// Define a custom comparator function for sorting points based on polar angles and distances
bool customComparator(const Point& pivot, const Point& p1, const Point& p2) {
    double angle1 = std::atan2(p1.getY() - pivot.getY(), p1.getX() - pivot.getX());
    double angle2 = std::atan2(p2.getY() - pivot.getY(), p2.getX() - pivot.getX());

    if (angle1 == angle2) {
        return std::hypot(p1.getX() - pivot.getX(), p1.getY() - pivot.getY()) < std::hypot(p2.getX() - pivot.getX(), p2.getY() - pivot.getY());
    }

    return angle1 < angle2;
}

/*
 * Computes the convex hull of a set of points using the Graham's scan algorithm.
 *
 * @param points - The vector of points to compute the convex hull for.
 */
void convexHull(std::vector<Point>& points)
{
    // Check if there are less than three points
    if (points.size() < 3)
    {
        // Handle cases with less than three points
        // Convex hull is the input points themselves
        return;
    }

    // Function to compare two points for sorting
    auto comparePoints = [](const Point& p1, const Point& p2)
    {
        if (p1.getY() == p2.getY())
        {
            return p1.getX() < p2.getX();
        }
        return p1.getY() < p2.getY();
    };

    // Sort the points based on y-coordinates and then x-coordinates
    std::sort(points.begin(), points.end(), comparePoints);

    // Find the pivot point (the one with the lowest y-coordinate)
    Point pivot = points[0];

    // Sort the remaining points based on polar angle from the pivot using the custom comparator
    std::sort(points.begin() + 1, points.end(), [pivot](const Point& p1, const Point& p2)
        {
            return customComparator(pivot, p1, p2);
        });

    // Initialize the convex hull with the first two points
    std::stack<Point> hull;
    hull.push(points[0]);
    hull.push(points[1]);

    // Build the convex hull
    for (size_t i = 2; i < points.size(); i++)
    {
        Point top = hull.top();
        hull.pop();
        while (!hull.empty() && Point::crossProduct(hull.top(), top, points[i]) <= 0)
        {
            top = hull.top();
            hull.pop();
        }
        hull.push(top);
        hull.push(points[i]);
    }

    // Update the original points array with the computed convex hull
    std::vector<Point> convexHullPoints;
    while (!hull.empty())
    {
        convexHullPoints.push_back(hull.top());
        hull.pop();
    }

    std::reverse(convexHullPoints.begin(), convexHullPoints.end());
    points = convexHullPoints;
}

/*
 * Perform convex hull computation for each polygon in the ControlManager.
 */
void ControlManager::performConvexHull()
{
    for (int i = 0; i < this->numPolygons; i++)
    {
        convexHull((polygons[i].getPolygonsPointsArray()));
    }
}


// Line of Sight //

/*
 * Calculate the slope between two points.
 *
 * @param p1 - The first point.
 * @param p2 - The second point.
 *
 * @return The slope between the two points.
 */
double calculateSlope(const Point& p1, const Point& p2) {
    return (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
}

/*
 * Check if two line segments defined by points (p1, p2) and (p3, p4) are crossing.
 *
 * @param p1, p2 - Points defining the first line segment.
 * @param p3, p4 - Points defining the second line segment.
 *
 * @return True if the line segments intersect, false otherwise.
 */
bool areLinesCrossing(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
{
    double m1 = calculateSlope(p1, p2);
    double m2 = calculateSlope(p3, p4);

    // Check if the slope are equal
    if (m1 == m2)
    {
        return false;
    }

    // Check if the 2 lines intersect in the area
    double x_intersection = (m1 * p1.getX() - p1.getY() - m2 * p3.getX() + p3.getY()) / (m1 - m2);
    double y_intersection = m1 * (x_intersection - p1.getX()) + p1.getY();

    // Check if the intersection point lies within the area defined by the four points.
    return (x_intersection >= std::min(p1.getX(), p2.getX()) &&
        x_intersection <= std::max(p1.getX(), p2.getX()) &&
        x_intersection >= std::min(p3.getX(), p4.getX()) &&
        x_intersection <= std::max(p3.getX(), p4.getX()) &&
        y_intersection >= std::min(p1.getY(), p2.getY()) &&
        y_intersection <= std::max(p1.getY(), p2.getY()) &&
        y_intersection >= std::min(p3.getY(), p4.getY()) &&
        y_intersection <= std::max(p3.getY(), p4.getY()));
}

/*
 * Find line of sight between two points by checking for obstacles (polygons).
 *
 * @param currentStart - The starting point of the line of sight.
 * @param currentEnd - The ending point of the line of sight.
 */
void ControlManager::findlineOfSight(Point* currentStart, Point* currentEnd) 
{
    if (currentStart == currentEnd) {
        return;
    }
    int counterCrossing = 0;

    // Iterate over polygons and check for line crossings
    for (int checkPolygon = 0; checkPolygon < numPolygons; checkPolygon++) 
    {
        auto& pointsArray = polygons[checkPolygon].getPolygonsPointsArray();
        int size = pointsArray.size();

        for (int crossPoint = 0; crossPoint < size; crossPoint++) 
        {
            Point* point1 = &pointsArray[crossPoint];
            Point* point2 = &pointsArray[(crossPoint + 1) % size];

            // Skip the current points
            if (point1 == currentEnd || point2 == currentStart || point2 == currentEnd || point1 == currentStart)
            {
                continue;
            }

            // check if 2 lines are not crossing
            if ((areLinesCrossing(*currentStart, *currentEnd, *point1, *point2)))
            {
                counterCrossing++;
            }
        }
    }

    // If no line crossings occurred, calculate and store the distance between currentStart and currentEnd
    if (counterCrossing == 0)
    {
        double distance = currentStart->calculateDistance(*currentEnd);
        currentStart->setlineOfSight(currentEnd, distance);
    }
}

/*
 * Find line of sight for all pairs of points.
 */
void ControlManager::findPointslineOfSight()
{
    allToAll();
    startPointToAll();
    allToEndPoint();
    endPointToAll();
    startPointToEndP();
}

/*
 * Find line of sight between all pairs of points in different polygons.
 */
void ControlManager::allToAll()
{
    for (int mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        auto& mainPoints = polygons[mainPolygon].getPolygonsPointsArray();
        int mainSize = mainPoints.size();

        for (int point = 0; point < mainSize; point++)
        {
            Point* currentStart = &mainPoints[point];

            for (int secPolygon = 0; secPolygon < numPolygons; secPolygon++)
            {
                auto& secPoints = polygons[secPolygon].getPolygonsPointsArray();
                int secSize = secPoints.size();

                for (int point2 = 0; point2 < secSize; point2++)
                {
                    Point* currentEnd = &secPoints[point2];

                    if (mainPolygon != secPolygon)
                    {
                        // Use the setlineOfSight function to add/update neighbors
                        findlineOfSight(currentStart, currentEnd);
                    }
                    else if (point != point2)
                    {
                        // add neighbors from right and left, if they are not the same point
                        currentStart->setlineOfSight(&mainPoints[(point + 1) % mainSize],
                            currentStart->calculateDistance(mainPoints[(point + 1) % mainSize]));

                        currentStart->setlineOfSight(&mainPoints[(point - 1 + mainSize) % mainSize],
                            currentStart->calculateDistance(mainPoints[(point - 1 + mainSize) % mainSize]));
                    }
                }
            }
        }
    }
}


/*
 * Find line of sight from the start point to all points in each polygon.
 */
void ControlManager::startPointToAll()
{
    Point* currentStart = &startPoint;

    for (size_t mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        for (size_t point = 0; point < (this->polygons[mainPolygon].getPolygonsPointsArray()).size(); point++)
        {
            Point* currentEnd = &(polygons[mainPolygon].getPolygonsPointsArray()[point]);

            findlineOfSight(currentStart, currentEnd);
        }
    }
}

/*
 * Find line of sight from all points in each polygon to the end point.
 */
void ControlManager::allToEndPoint()
{
    for (int mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        auto& mainPoints = polygons[mainPolygon].getPolygonsPointsArray();
        int mainSize = mainPoints.size();

        for (int point = 0; point < mainSize; point++)
        {
            Point* currentStart = &mainPoints[point];
            Point* currentEnd = &endPoint;

            findlineOfSight(currentStart, currentEnd);
                
            
        }
    }
}

/*
 * Find line of sight from the end point to all points in each polygon.
 */
void ControlManager::endPointToAll()
{
    Point* currentStart = &endPoint;

    for (size_t mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        for (size_t point = 0; point < (this->polygons[mainPolygon].getPolygonsPointsArray()).size(); point++)
        {
            Point* currentEnd = &(polygons[mainPolygon].getPolygonsPointsArray()[point]);

            findlineOfSight(currentStart, currentEnd);


        }
    }
}

/*
 * Find line of sight from the start point to the end point.
 */
void ControlManager::startPointToEndP()
{
    Point* currentStart = &startPoint;

    Point* currentEnd = &endPoint;

    findlineOfSight(currentStart, currentEnd);
}

/*
 * Write line of sight information to a text file, including start and end points and each polygon's points.
 *
 * @param filename - The name of the file to write the line of sight information.
 */
void ControlManager::writelineOfSight()
{
    // Create and open a text file
    std::ofstream outFile("neighborsFile.txt");

    // Write line of sight information for the start point
    const std::vector<std::pair<Point*, double>>& startlineOfSight = this->startPoint.getlineOfSight();
    outlineOfSightArrayWithDistances(startlineOfSight, outFile);

    // Write line of sight information for the end point
    const std::vector<std::pair<Point*, double>>& endlineOfSight = this->endPoint.getlineOfSight();
    outlineOfSightArrayWithDistances(endlineOfSight, outFile);

    // Write line of sight information for each polygon's points
    for (int polygonNumber = 0; polygonNumber < numPolygons; polygonNumber++)
    {
        const auto& polygonPoints = polygons[polygonNumber].getPolygonsPointsArray();
        int pointsNumber = polygonPoints.size();
        outFile << polygonNumber << "\n" << pointsNumber << "\n";

        // Write line of sight information for each point in the polygon
        for (int j = 0; j < pointsNumber; j++)
        {
            const std::vector<std::pair<Point*, double>>& pointlineOfSight = polygonPoints[j].getlineOfSight();
            outlineOfSightArrayWithDistances(pointlineOfSight, outFile);
        }

        outFile << "\n";
    }

    outFile.close();
}

/*
 * Write neighbors array with distances to the file.
 *
 * @param lineOfSightArray - The array containing neighbors and their distances.
 * @param outFile - The output file stream to write the information.
 */
void ControlManager::outlineOfSightArrayWithDistances(const std::vector<std::pair<Point*, double>>& lineOfSightArray, std::ofstream& outFile)
{
    outFile << lineOfSightArray.size() << "\n";
    // Iterate over the lineOfSightArray and write each Point and distance to the file
    for (const auto& neighbor : lineOfSightArray)
    {
        outFile << neighbor.first->getX() << ", " << neighbor.first->getY() << ", " << neighbor.second << "\n";
    }
}

/*
 * Reconstruct the path from the A* algorithm result.
 *
 * @param cameFrom - A map containing the relationships between points in the path.
 * @param current - The current point being processed.
 *
 * @return A vector of points representing the reconstructed path.
 */
std::vector<Point> reconstructPath(const std::unordered_map<Point*, Point*>& cameFrom, Point* current) {
    std::vector<Point> totalPath;
    totalPath.push_back(*current);

    while (cameFrom.find(current) != cameFrom.end()) {
        current = cameFrom.at(current);
        totalPath.insert(totalPath.begin(), *current);
    }

    return totalPath;
}

/*
 * Implementation of the A* algorithm to find the shortest route between start and goal points.
 *
 * @param start - The starting point.
 * @param goal - The goal point.
 *
 * @return A vector of points representing the shortest path.
 */
std::vector<Point> ControlManager::aStar(Point& start, Point& goal)
{
    std::priority_queue<std::pair<double, Point*>, std::vector<std::pair<double, Point*>>, std::greater<>> openSet;
    std::unordered_map<Point*, double> gScore;
    std::unordered_map<Point*, double> fScore;
    std::unordered_map<Point*, Point*> cameFrom;
    std::unordered_set<Point*> processedPoints;

    // Initialize gScore with infinity for all nodes
    for (const auto& neighborInfo : start.getlineOfSight()) {
        Point* neighbor = neighborInfo.first;
        gScore[neighbor] = std::numeric_limits<double>::infinity();
    }

    gScore[&start] = 0;

    // Initialize fScore with infinity for all nodes
    for (const auto& neighborInfo : start.getlineOfSight()) {
        Point* neighbor = neighborInfo.first;
        fScore[neighbor] = std::numeric_limits<double>::infinity();
    }

    fScore[&start] = start.calculateDistance(goal);

    openSet.push({ 0, &start });

    while (!openSet.empty()) {
        Point* current = openSet.top().second;
        openSet.pop();

        // Skip if the point is already processed
        if (processedPoints.find(current) != processedPoints.end()) {
            continue;
        }

        processedPoints.insert(current);

        if (*current == goal) {
            return reconstructPath(cameFrom, current);
        }

        for (const auto& neighborInfo : current->getlineOfSight()) {
            Point* neighbor = neighborInfo.first;
            if (gScore.find(neighbor) == gScore.end()) {
                gScore[neighbor] = INFINITY;
            }
            double distanceToNeighbor = neighborInfo.second;
            double tentativeGScore = gScore[current] + distanceToNeighbor;

            if (tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = tentativeGScore + neighbor->calculateDistance(goal);

                openSet.push({ fScore[neighbor], neighbor });
            }
        }
    }

    std::vector<Point> result;
    return result;
}


/*
 * Find the shortest route using the A* algorithm and save it to a text file.
 */
void ControlManager::findShortestRoute()
{
    this->setPath(aStar(startPoint, endPoint));

    if (path.empty()) 
    {
        // Handle failure
        std::cout << "Path not found." << std::endl;
    }
    else {
        // Save the found path to a text file
        std::ofstream outFile("shortest_path.txt");

        if (outFile.is_open()) {
            for (const auto& point : path) {
                outFile << point.getX() << " " << point.getY() << " ";
            }
            outFile.close();
            std::cout << "Path saved to shortest_path.txt." << std::endl;
        }
        else {
            std::cerr << "Unable to open file for writing." << std::endl;
        }
    }
}