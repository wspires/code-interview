/**
	@file   main.cpp
	@author Wade Spires
	@date   2018/04/30
	@brief  Chapter 7 - Binary Search

	Copyright (c) 2018 Wade Spires. All rights reserved.
 */

#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::size_t find(std::vector<int> const & array, int key)
{
    auto lower = std::cbegin(array);
    auto upper = std::cend(array) - 1;
    while (lower < upper)
    {
        auto mid = lower + (upper - lower) / 2;
        if (key == *mid)
        {
            return mid - std::cbegin(array);
        }

        if (key < *mid)
        {
            upper = mid - 1;
        }
        else // key > *mid
        {
            lower = mid + 1;
        }
    }
    return lower - std::cbegin(array);
}

void print(std::vector<int> const & array)
{
    std::cout << "[";
    for (auto i : array)
    {
        std::cout << i << " ";
    }
    std::cout << "]";
}

void print(std::vector<int> const & array, int key)
{
    auto idx = find(array, key);
    print(array);
    std::cout << ": key=" << key << ", idx=" << idx << ", value=" << array[idx] << ", found=" << (array[idx] == key) << std::endl;
}

void test_binary_search()
{
    print({1, 2, 3}, 1);
    print({1, 2, 3}, 2);
    print({1, 2, 3}, 3);
    print({1, 2, 3}, 0);
    print({1, 2, 3}, 4);
}

int main(int argc, char * argv[])
{
    test_binary_search();
	return EXIT_SUCCESS;
}
