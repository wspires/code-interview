/**
	@file   main.cpp
	@author Wade Spires
	@date   2018/04/30
	@brief  Chapter 5 - Heaps

	Copyright (c) 2018 Wade Spires. All rights reserved.

    A heap is an abstract data structure similar to a binary search tree.
    Like a BST, a heap is a collection of nodes that have a data member value and at most 2 child nodes.
    Unlike a BST, nodes in a heap satisfy the heap property.
    The heap property is held when p.value >= n.value for each child node n of a parent p.
    A heap satisfyin this property is called a max-heap.
    A min-heap satisfies the property that a parents data value never exceeds any child's data: p.value < n.value.
    The root of a min-heap has the smallest value in the data structure, and the root of a max-heap has the largest value in the data structure.

    The heap ops are insert, find_max, remove_max, and increase_key.
    By maintaining the heap property as an invariant, the find_max op runs in O(1) (constant) time.

    A simpler variant is the binary heap. In the binary heap, there is a single binary tree structure.
    The tree structure is always balanced, and grows by insertion in level order. While the binary heap is simple to implement and understand,
    it does not achieve the asymptotic efficiency of the other variants.
    Namely, while find_max taks O(1) time and remove_max achieves O(log n) time, all other ops have at least O(log n) lower bounds.
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


// Functions for calculating the left and right child of a node at the given index. 
// The binary tree structure comes from index of entries in a vector to level order position of nodes in a complete binary tree.
// Index 0 corresponds to the root of the binary tree.
size_t lchild_index(size_t const index)
{
    return index * 2 + 1;
}
size_t rchild_index(size_t const index)
{
    return index * 2 + 2; // == lchild_index(index) + 1
}
size_t parent_index(size_t const index)
{
    // Note: relies on integer division to round down for calculating parent of right child.
    return (index != 0) ? ((index - 1) / 2) : 0;
}

// heapify starts with the single element of the vector at index 0.
// This single element sub-array trivially satisfies the heap property.
// Next, iteratively add elements from the vector in such a way that the heap property is maintained.
// New elements are first added as leaves.
// Afterward, parent and child pairs are repeatedly swapped if in violation of the heap property.
// When finished, the max value of the heap is at the first index and the heap property holds.
void heapify(std::vector<int> & array)
{
    for (std::size_t i = 0; i != array.size(); ++i)
    {
        auto index = i;
        auto parent = parent_index(index);
        while (index != parent and array[parent] < array[index])
        {
            std::swap(array[parent], array[index]);
            index = parent;
            parent = parent_index(index);
        }
    }
}

// Insertion of a new value begins with just adding the value as the next available leaf node in the binary tree
// (which is simply at the very end as we insert in level order).
// Next, the parent-child relationships are updated to satisfy the heap property as was done in heapify.
// Insertion requires the vector to be dynamically resized, and hence is not an in-place operation.
void insert(std::vector<int> & heap, int value)
{
    heap.push_back(value);

    auto index = heap.size() - 1;
    auto parent = parent_index(index);
    while (index != 0
        and heap[index] > heap[parent])
    {
        std::swap(heap[index], heap[parent]);
        index = parent;
        parent = parent_index(index);
    }
}

// Requires only dereferencing the first index of the vector.
int find_max(std::vector<int> const & heap)
{
    return heap[0];
}

// Removal of the max heap value must accomplish 2 objectives aside from replacing the value at the initial position.
// First, it must promote the second minimum value to the first position.
// Doing so causes the tree to pivot and may have a cascading effect on the structure of the heap.
// Secondly, the tree structure and parent-child relationships must be maintained.
// Both requirements are satisfied by the following sequence:
// 1. Replace the root value of the heap with the value of the last leaf node (and resize the array to the smaller size).
// 2. Fixup the heap property by iteratively swapping parent-child pairs if in violation of the heap property.
void remove_max(std::vector<int> & heap)
{
    std::swap(*heap.begin(), *(heap.end() - 1));
    heap.resize(heap.size() - 1);

    std::size_t index = 0;
    while (index < heap.size())
    {
        auto child = lchild_index(index);
        if (child >= heap.size())
        {
            break;
        }

        auto right = rchild_index(index);
        if (right < heap.size()
            and heap[right] > heap[child])
        {
            child = right;
        }

        if (heap[index] >= heap[child])
        {
            break;
        }

        std::swap(heap[index], heap[child]);
        index = child;
    }
}

// The heap property may no longer hold for a node whose key is increased.
// Hence, we may need to decrease its index repeatedly until the array again has the heap property
// (by iterative swapping).
void increase_key(std::vector<int> & heap, std::size_t index)
{
    ++heap[index];

    auto parent = parent_index(index);
    while (index != 0
        and heap[index] > heap[parent])
    {
        std::swap(heap[index], heap[parent]);
        index = parent_index(index);
        parent = parent_index(index);
    }
}

// Enumeration of Powers
// Given a set of integers the question is to list the powers of the integers in increasing order without duplicates.
// For example, given the set {2, 3, 4}, the first 6 powers would be:
// 1 = 2^0
// 2 = 2^1
// 3 = 3^1
// 4 = 4^1
// 8 = 2^3
// 9 = 3^2
void enumerate_powers(
      std::set<unsigned> const & set
    , std::size_t num_powers
    , std::vector<int> & out
    )
{
    using ValueTerm = std::pair<unsigned, unsigned>;
    std::vector<ValueTerm> heap{};

    // Initialize heap with {1, value^0}.
    // Use std::greater to make min-heap.
    for (auto value : set)
    {
        heap.push_back({1, value});
    }
    std::make_heap(std::begin(heap), std::end(heap), std::greater<ValueTerm>{});

    int value = 0;
    while (num_powers != 0 and heap.size() != 0)
    {
        // Top of the heap holds the smallest power.
        auto entry = heap.front();
        std::pop_heap(std::begin(heap), std::end(heap), std::greater<ValueTerm>{});

        // Output next power (but only if it is unique).
        if (value != entry.first)
        {
            value = entry.first;
            out.push_back(value);
            --num_powers;
        }

        // Calculate next power for this value and push onto the heap
        // to be considered for the next iteration.
        heap.back() = {entry.first * entry.second, entry.second};
        std::push_heap(std::begin(heap), std::end(heap), std::greater<ValueTerm>{});
    }
}

// The runtime is an efficient O(n log k) as each removal of the min element and insertion takes time logarithmic in the size of the heap.
void find_topk(
      std::istream & in
    , std::size_t k
    , std::vector<int> & heap
    )
{
    // Initialize heap with first k elements.
    heap.reserve(k);
    int val = 0;
    while (heap.size() < k and in >> val)
    {
        heap.push_back(val);
    }
    std::make_heap(std::begin(heap), std::end(heap), std::greater<int>{});

    // With each additional input element,
    // maintain only k elements in the heap
    // skipping the element if it is not in the top k
    // and if it is in the top k, push it onto the heap and pop the current min from the heap.
    while (not in.eof() and in >> val)
    {
        // Skip if not in the top k.
        if (val <= heap.front())
        {
            continue;
        }

        // val is in the top k, so pop the min and push val.
        std::pop_heap(std::begin(heap), std::end(heap), std::greater<int>{});
        heap.back() = val;
        std::push_heap(std::begin(heap), std::end(heap), std::greater<int>{});
    }
}

int main(int argc, char * argv[])
{
    std::cout << "Hello" << std::endl;
	return EXIT_SUCCESS;
}
