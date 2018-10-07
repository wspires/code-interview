/**
	@file   main.cpp
	@author Wade Spires
	@date   2018/04/30
	@brief  Chapter 4 - Queues and Stacks

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

class queue
{
public:
    void push(int data)
    {
        list.push_back(data);
    }

    int front() const
    {
        return list.front();
    }

    void pop()
    {
        list.pop_front();
    }

private:
    std::list<int> list;
};
void test_queue()
{
    queue q{};
    q.push(5);
    q.push(10);
    std::cout << q.front() << std::endl;
}

class stack
{
public:
    void push(int data)
    {
        list.push_front(data);
    }

    int top() const
    {
        return list.front();
    }

    void pop()
    {
        list.pop_front();
    }

private:
    std::list<int> list;
};
void test_stack()
{
    stack s{};
    s.push(5);
    s.push(10);
    std::cout << s.top() << std::endl;
}

void test()
{
    test_queue();
    test_stack();
}

int main(int argc, char * argv[])
{
    test();
	return EXIT_SUCCESS;
}
