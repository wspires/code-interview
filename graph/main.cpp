/**
	@file   main.cpp
	@author Wade Spires
	@date   2018/04/30
	@brief  Chapter 8 - Graphical Search

	Copyright (c) 2018 Wade Spires. All rights reserved.
 */

#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Each node has a value stored as the member variable.
// The edges originating at a node are in the vector variable neighbors.
struct node
{
    int value = 0;
    std::vector<node *> neighbors = {};
};

// General function applied by each graph search algorithm when a node is visited.
using Visitor = std::function<void (node *)>;

// Breadth-first search.
void
bfs(node * origin, Visitor visit)
{
    std::queue<node *> queue{};
    std::set<node *> visited{};

    queue.push(origin);
    while (not queue.empty())
    {
        // Visit next node in the queue.
        node * current = queue.front();
        queue.pop();
        visit(current);
        visited.insert(current);

        // Queue adjacent nodes if not visited.
        for (auto neighbor : current->neighbors)
        {
            if (visited.find(neighbor) == std::cend(visited))
            {
                queue.push(neighbor);
            }
        }
    }
}

// Find the distance between nodes in a graph.
// bfs visits each node in order of its distance from the origin,
// so use it to find the minimal distance from an origin to all other nodes in a graph.
// Upon return, distances will contain the shortest path distance from origin to all reachable nodes.
void find_distances(
      node * origin
    , std::map<node *, std::size_t> & distances
    )
{
    // Initialize or fetch current node distance.
    auto distance = [&distances](node * vertex)
        {
            auto distance_iter = distances.find(vertex);
            if (distance_iter == std::cend(distances))
            {
                // Initialize an unvisited node's distance to max.
                return std::numeric_limits<std::size_t>::max();
            }
            return distance_iter->second;
        };

    auto visit = [&distances, &distance](node * vertex)
        {
            // Special case for origin node, which has distance 0 to itself.
            if (distances.empty())
            {
                distances[vertex] = 0;
            }

            auto vertexDistance = distance(vertex);

            // Update visited node's neighbors' distances.
            // When a node is first seen, all its neighbors are initialized with a distance equal to the distance of the parent plus one.
            // Since bfs visits each node in order of its distance, a parent's distance will always be set before a child is encountered.
            for (node * neighbor : vertex->neighbors)
            {
                auto neighborDistance = distance(neighbor);
                distances[neighbor] = std::min(neighborDistance, vertexDistance + 1);
            }
        };

    // Bread-first search with visitor lambda defined above.
    bfs(origin, visit);
}

int main(int argc, char * argv[])
{
    std::cout << "Hello" << std::endl;
	return EXIT_SUCCESS;
}
