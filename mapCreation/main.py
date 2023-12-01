import os
import random
import math
import time

import numpy as np
import matplotlib.pyplot as plt
import subprocess


class Point:
    def __init__(self, x, y):
        """
        Initialize a Point object with coordinates (x, y) and an empty list of neighbors.

        :param x: X-coordinate of the point.
        :param y: Y-coordinate of the point.
        """
        self.x = x
        self.y = y
        self.neighbors = []

    def add_neighbors(self, *neighbor_data):
        """
        Add one or more neighboring points to the current point's list of neighbors.

        :param neighbor_data: One or more tuples (Point object, distance) to be added as neighbors.
        """
        for data in neighbor_data:
            if len(data) == 2:
                neighbor, distance = data
                if isinstance(neighbor, Point):
                    self.neighbors.append((neighbor, distance))
                    neighbor.neighbors.append((self, distance))  # Also add the current point to the neighbor's list
                else:
                    print("Error: Only Point objects can be added as neighbors")
            else:
                print("Error: Each neighbor data should be a tuple (Point object, distance)")


class Polygon:
    def __init__(self, map_size):
        """
        Initialize a Polygon object within a given map size. It generates random points within a circle and stores them.

        :param map_size: The size of the map.
        """
        self.points = []
        self.circle_radius = None
        self.circle_center = None
        self.number_of_points = 0

        self.random_circle(map_size)
        self.set_number_of_points()
        self.random_points_in_circle()

    def generate_random_radius(self, max_radius):
        """
        Generate a random circle radius between 0 and the maximum radius.

        :param max_radius: Maximum radius for the circle.
        :return: Random circle radius.
        """
        return random.uniform(0, max_radius)

    def random_circle(self, map_size):
        """
        Generate a random circle with a radius and center within the map size.

        :param map_size: The size of the map.
        """
        if map_size <= 0:
            return -1

        max_radius = map_size / 2

        # Generate a random circle radius between 0 and the maximum radius
        self.circle_radius = self.generate_random_radius(max_radius)

        # Generate a random circle center within the valid range for the radius
        self.circle_center = (
            random.uniform(self.circle_radius, map_size - self.circle_radius),
            random.uniform(self.circle_radius, map_size - self.circle_radius)
        )

    def set_number_of_points(self):
        """
        Set the number of points based on the circle radius.
        You can modify this logic based on your specific requirements.
        """
        # Example: set the number of points based on the circle radius
        self.number_of_points = int(2 * math.pi * self.circle_radius)

    def random_points_in_circle(self):
        """
        Generate random points within the circle and store them in the Polygon object.
        """
        for _ in range(self.number_of_points):
            # Generate a random angle
            theta = random.uniform(0, 2 * math.pi)
            # Generate a random distance from the center within the circle radius
            r = random.uniform(0, self.circle_radius)
            # Calculate the Cartesian coordinates
            x = self.circle_center[0] + r * math.cos(theta)
            y = self.circle_center[1] + r * math.sin(theta)
            # Create a Point object and add it to the list of points
            self.points.append(Point(x, y))

    def print_to_file(self, file):
        """
        Print the points of the polygon to a file.

        :param file: File object to write to.
        """
        # file.write("Points:\n")
        for point in self.points:
            file.write(f"{point.x} {point.y}\n")


class MapCreation:
    def __init__(self):
        """
        Initialize the MapCreation object by setting map size, number of polygons, and creating polygons.
        """
        self.map_size = 111
        self.number_of_polygons = 2
        self.polygons = []
        self.startPoint = Point(0, 0)
        self.shortestPath = []
        self.set_map_size()
        self.set_number_of_polygons()
        self.create_polygons()
        self.endPoint = Point(self.map_size, self.map_size)

    def set_map_size(self):
        """
        Set the map size to a random value between 100 and 300.
        """
        self.map_size = random.randrange(100, 300)

    def set_number_of_polygons(self):
        """
        Set the number of polygons to a random value between 1 and 10.
        """
        self.number_of_polygons = random.randrange(1, 10)

    def check_overlap(self, new_polygon):
        """
        Check if a newly generated polygon overlaps with any existing polygons.

        :param new_polygon: The new polygon to be checked for overlap.
        :return: True if there is an overlap, False if there is no overlap.
        """
        for existing_polygon in self.polygons:
            distance = math.sqrt((new_polygon.circle_center[0] - existing_polygon.circle_center[0]) ** 2 +
                                 (new_polygon.circle_center[1] - existing_polygon.circle_center[1]) ** 2)
            if distance < (new_polygon.circle_radius + existing_polygon.circle_radius):
                return True
        return False

    def create_polygons(self):
        """
        Create a specified number of non-overlapping polygons and add them to the list of polygons.
        """
        for i in range(self.number_of_polygons):
            polygon = Polygon(self.map_size)
            while self.check_overlap(polygon):
                polygon = Polygon(self.map_size)
            self.polygons.append(polygon)

    def plot_map(self):
        """
        Plot the map with polygons.
        """
        colors = ["red"]
        fig, ax = plt.subplots()

        for polygon in self.polygons:
            for point in polygon.points:
                plt.scatter(point.x, point.y, color=random.choice(colors), s=10)

            center = polygon.circle_center
            radius = polygon.circle_radius
            theta = np.linspace(0, 2 * np.pi, 100)
            x = center[0] + radius * np.cos(theta)
            y = center[1] + radius * np.sin(theta)
            ax.plot(x, y, label='Circle')

        ax.set_aspect('equal', adjustable='box')
        ax.set_xlabel('X-axis')
        ax.set_ylabel('Y-axis')
        ax.set_title('Map Plot')
        plt.show()

    def plot_line_of_sight(self):
        """
        Plot lines of sight from points to their neighbors, and from start/end points to their neighbors.
        """
        fig, ax = plt.subplots()
        colors = ["blue", "brown", "orange", "green", "pink", "black"]

        # Plot lines from each polygon point to its neighbors
        for polygon in self.polygons:
            chosen_color = random.choice(colors)
            for point in polygon.points:
                for neighbor_info in point.neighbors:
                    neighbor, _ = neighbor_info  # Assuming neighbor_info is a tuple (neighbor, distance)
                    plt.plot([point.x, neighbor.x], [point.y, neighbor.y], color=chosen_color, linestyle="--",
                             alpha=0.5)

        # Plot lines from start point to each neighbor
        for neighbor_info in self.startPoint.neighbors:
            neighbor, _ = neighbor_info
            plt.plot([self.startPoint.x, neighbor.x], [self.startPoint.y, neighbor.y], color="green",
                     linestyle="--", label="Start Point to Neighbor")

        # Plot lines from end point to each neighbor
        for neighbor_info in self.endPoint.neighbors:
            neighbor, _ = neighbor_info
            plt.plot([self.endPoint.x, neighbor.x], [self.endPoint.y, neighbor.y], color="blue",
                     linestyle="--", label="End Point to Neighbor")

        ax.set_aspect('equal')

    def plot_polygons(self):
        """
        Plot the polygons with start and end points.
        """
        for polygon in self.polygons:
            x = [point.x for point in polygon.points]
            y = [point.y for point in polygon.points]
            x.append(x[0])
            y.append(y[0])
            plt.plot(x, y)
            # Include start and end points in the plot
            plt.scatter([self.startPoint.x, self.endPoint.x], [self.startPoint.y, self.endPoint.y], color="red", s=30)

    def create_map_file(self, filename):
        """
        Create a map data file with a specified filename.

        :param filename: The name of the map data file to be created.
        """
        with open(filename, 'w') as file:
            file.write("Map Data:\n")

    def write_map_to_file(self, file_name):
        """
        Write map data to a file with a specified filename.

        :param file_name: The name of the file where map data will be written.
        """
        with open(file_name, 'w') as file:
            file.write(f"{self.map_size}\n")  # Map Size
            file.write(f"{self.number_of_polygons}\n")  # Number of Polygons

            for i, polygon in enumerate(self.polygons, start=1):
                file.write(f"\n{i}\n")     # Polygon's number
                file.write(f"{polygon.number_of_points}\n")  # Number of Points
                polygon.print_to_file(file)

    def read_data_from_file(self):
        """
        Read map data from a file and update the polygon data accordingly.
        """
        file_path = r"C:\Users\nulyc\projects\shortestPath\mapCreation/writeFile.txt"  # For Windows
        with open(file_path, 'r') as file:
            paragraphs = file.read().split('\n\n')  # Split the file into paragraphs

            for paragraph in paragraphs:
                lines = paragraph.strip().split('\n')

                if lines[0] != '':
                    # Extract the polygon number from the first line of the paragraph
                    polygon_number_line = lines[0]
                    if polygon_number_line:
                        polygon_number = int(polygon_number_line)

                    # Create a list to store points for the current polygon
                    new_points = []

                    # Extract points number
                    number_of_points = int(lines[1])

                    # Process the remaining lines as points
                    for line in lines[2:]:
                        # Split the line into coordinates (assuming they are space-separated)
                        x, y = map(float, line.split())
                        # Create a point tuple and add it to the points list
                        point = Point(x, y)
                        new_points.append(point)

                    self.polygons[polygon_number].points = new_points

    def read_neighbors_file(self):
        """
        Read information about neighboring points from a file and update the points with their neighbors.
        """
        important_point = 0
        file_path = r"C:\Users\nulyc\projects\shortestPath\mapCreation/neighborsFile.txt"  # For Windows

        with open(file_path, 'r') as file:
            lines = file.readlines()
            i = 0
            polygon_index = 0
            # Iterate through the lines and create Point objects
            while i < len(lines):
                if important_point == 0:
                    # Start point
                    neighbors_number = int(lines[i].strip())
                    i += 1
                    for n in range(neighbors_number):
                        neighbor_line = lines[i].strip()
                        x, y, distance = map(float, neighbor_line.split(","))
                        new_point = Point(x, y)
                        # Add the new_point to the neighbors list of the specified point within a polygon
                        self.startPoint.add_neighbors((new_point, distance))
                        i += 1

                    # End point
                    neighbors_number = int(lines[i].strip())
                    i += 1
                    for n in range(neighbors_number):
                        neighbor_line = lines[i].strip()
                        x, y, distance = map(float, neighbor_line.split(","))
                        new_point = Point(x, y)
                        # Add the new_point to the neighbors list of the specified point within a polygon
                        self.endPoint.add_neighbors((new_point, distance))
                        i += 1
                    important_point += 1

                polygon_number = int(lines[i].strip())
                i += 1
                points_number = int(lines[i].strip())
                i += 1
                for p in range(points_number):
                    neighbors_number = int(lines[i].strip())
                    i += 1
                    for n in range(neighbors_number):
                        neighbor_line = lines[i].strip()
                        x, y, distance = map(float, neighbor_line.split(","))
                        new_point = Point(x, y)
                        # Add the new_point to the neighbors list of the specified point within a polygon
                        points_array = self.polygons[polygon_index].points
                        points_array[p].add_neighbors((new_point, distance))

                        i += 1
                # Skip the empty line
                i += 1
                polygon_index += 1

    def read_shortest_path_from_file(self):
        """
        Read the shortest path from a text file and update the MapCreation object.
        """
        file_path = r"C:\Users\nulyc\projects\shortestPath\mapCreation\shortest_path.txt"  # For Windows
        with open(file_path, 'r') as file:
            # Read the space-separated values and create Point objects
            coordinates = [float(coord) for coord in file.read().strip().split()]

            # Create Point objects from the coordinates
            self.shortestPath = [Point(coordinates[i], coordinates[i + 1]) for i in range(0, len(coordinates), 2)]

    def plot_path(self):
        """
        Plot the shortest path along with start and end points.
        """
        if not self.shortestPath:
            print("No path to plot.")
            return

        # Extract x and y coordinates from the shortestPath
        x_coordinates = [point.x for point in self.shortestPath]
        y_coordinates = [point.y for point in self.shortestPath]

        # Plot the path
        plt.plot(x_coordinates, y_coordinates, marker='o', label='Shortest Path')
        plt.scatter(self.startPoint.x, self.startPoint.y, color='green', label='Start Point')
        plt.scatter(self.endPoint.x, self.endPoint.y, color='red', label='End Point')

        # Customize the plot
        plt.title('Shortest Path Visualization')
        plt.xlabel('X-coordinate')
        plt.ylabel('Y-coordinate')
        plt.legend()


def main():
    # Get the current working directory
    current_directory = os.getcwd()

    # Create an instance of MapCreation to generate a map with obstacles
    map_of_obstacles = MapCreation()

    # Plot the generated map
    map_of_obstacles.plot_map()

    # Create a map data file and write map data to the file
    map_of_obstacles.create_map_file('mapData.txt')
    map_of_obstacles.write_map_to_file('mapData.txt')

    # Specify the path to the executable for finding the shortest route
    exe_path = current_directory + r"\..\shortestRouteOnRandomMap\Debug\Shortest route on a random map.exe"

    try:
        # Run the executable with the map data file as an argument
        subprocess.run([exe_path, os.path.join(current_directory, "mapData.txt")], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running {exe_path}: {e}")

    # Pause for a moment to allow the executable to finish processing
    time.sleep(3)

    # Read data from the file, including neighbor information and the shortest path
    map_of_obstacles.read_data_from_file()
    map_of_obstacles.read_neighbors_file()
    map_of_obstacles.read_shortest_path_from_file()

    # Plot line of sight and polygons
    map_of_obstacles.plot_line_of_sight()
    map_of_obstacles.plot_polygons()
    plt.show()

    # Plot the shortest path and polygons
    map_of_obstacles.plot_path()
    map_of_obstacles.plot_polygons()
    plt.show()


if __name__ == '__main__':
    main()
