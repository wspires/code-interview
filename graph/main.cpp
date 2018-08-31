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
    std::set<node *> visited{};
    std::queue<node *> queue{};

    queue.push(origin);
    while (not queue.empty())
    {
        // Visit next node in the queue.
        node * current = queue.front();
        queue.pop();
        visit(current);
        visited.insert(current);

        for (auto neighbor : current->neighbors)
        {
            if (visited.find(neighbor) == std::cend(visited))
            {
                queue.push(neighbor);
            }
        }
    }
}

int main(int argc, char * argv[])
{
    std::cout << "Hello" << std::endl;
	return EXIT_SUCCESS;
}
