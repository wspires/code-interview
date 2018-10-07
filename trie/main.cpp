/**
    @file   main.cpp
    @author Wade Spires
    @date   2018/10/06
    @brief  Trie

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

// https://leetcode.com/explore/interview/card/google/62/recursion-4/462/discuss/178217/C++-solution-so-well-written-that-code-explains-itself
class Trie
{
public:
    static const int CharCount = 26;

    Trie()
		: m_root{std::make_unique<TrieNode>()}
	{}

    Trie(std::vector<std::string> const & words)
        : Trie{}
    {
        for (auto && word : words)
        {
            insert(word);
        }
    }

    void insert(std::string const & word)
    {
        TrieNode * p = m_root.get();
        for (char const c : word)
        {
            int idx = std::tolower(c) - 'a'; 
            if (not p->children[idx])
            {
                p->children[idx] = new TrieNode{};
            }
            p = p->children[idx];
        }
        p->is_word = true;
    }

    bool is_word(std::string const & word) const
    {
        TrieNode * p = find(word);
        return p and p->is_word;
    }

    bool is_prefix(std::string const & prefix) const
    {
        return find(prefix) != nullptr;
    }

private:
    struct TrieNode
    {
        bool is_word = false;
        std::vector<TrieNode *> children;

        TrieNode()
            : is_word{false}
            , children{CharCount, nullptr}
        {}

        ~TrieNode()
        {
            for (auto child : children)
            {
                delete child;
            }
        }
    };

    std::unique_ptr<TrieNode> m_root;

    TrieNode * find(std::string const & str) const
    {
        TrieNode * p = m_root.get();
        for (const char c : str)
        {
            int idx = std::tolower(c) - 'a'; 
            p = p->children[idx];
            if (not p)
            {
                break;
            }
        }
        return p;
    }
};

class Solution {
public:
    std::vector<std::string> findWords(std::vector<std::vector<char>> &board, std::vector<std::string> &words)
    {
        if (board.size() == 0)
        {
            return {};
        }

        std::unordered_set<std::string> ret{};

        int const num_rows = board.size();
        int const num_cols = board[0].size();

        Trie trie{words};
        for (int row = 0; row != num_rows; ++row)
        {
            for (int col = 0; col != num_cols; ++col)
            {
                std::vector<std::vector<bool>> visited(num_rows, std::vector<bool>(num_cols, false));
                std::string initial_word = std::string{1, board[row][col]};
                dfs(row, col, board, visited, trie, ret, initial_word);
            }
        }

        std::vector<std::string> ans(ret.begin(), ret.end());
        return ans;
    }

    bool is_valid(int row, int col, std::vector<std::vector<char>> const & board)
    {
        int const num_rows = board.size();
        int const num_cols = board[0].size();
        return row >= 0 and row < num_rows and col >= 0 and col < num_cols;
    }

    // Combination of row and column directions to try (down, up, right, left).
    std::vector<int> const row_directions = {1, -1, 0, 0};
    std::vector<int> const col_directions = {0, 0, 1, -1};

    void dfs(
          int row
        , int col
        , std::vector<std::vector<char>> const & board
        , std::vector<std::vector<bool>> & visited
        , Trie const & trie
        , std::unordered_set<std::string> & answer
        , std::string const & cur_word
        )
    {
        if (visited[row][col] or not trie.is_prefix(cur_word))
        {
            return;
        }
        visited[row][col] = true;

        if (trie.is_word(cur_word))
        {
            answer.insert(cur_word);
        }

        for (int i = 0; i != 4; ++i)
        {
            int next_row = row + row_directions[i];
            int next_col = col + col_directions[i];
            if (is_valid(next_row, next_col, board) and not visited[next_row][next_col])
            {
                std::string next_word = cur_word + std::string(1, board[next_row][next_col]);
                dfs(next_row, next_col, board, visited, trie, answer, next_word);
            }
        }
    }
};

int main(int argc, char * argv[])
{
    std::vector<std::string> words = {"Hello", "world"};
    Trie trie{words};

    for (auto && word : words)
    {
        std::cout << word << " is_word: " << trie.is_word(word) << std::endl;
    }

    for (auto && word : words)
    {
        auto sub_word = word.substr(0, word.size() - 1);
        std::cout << sub_word << " is_word: " << trie.is_word(sub_word) << std::endl;
        std::cout << sub_word << " is_prefix: " << trie.is_prefix(sub_word) << std::endl;
    }

    return EXIT_SUCCESS;
}
