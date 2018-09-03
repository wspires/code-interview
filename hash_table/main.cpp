/**
	@file   main.cpp
	@author Wade Spires
	@date   2018/04/30
	@brief  Chapter 6 - Hash Tables

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

// Deterministic and very efficient (O(1) time), its range limited to only 26 values.
// Hence, many strings will hash to the same value.
// Further, English words will not be uniformly distributed across these 26 values
// (12% of words start with 'a', but only .05% of words start with 'z').
// O(1) time.
std::size_t hash_first_char(std::string const & str)
{
    if (str.empty())
    {
        return 0;
    }
    return str[0] - 'a';
}

// This function has larger range, but is still small and rather sparse.
// No longer guarantees O(1) time.
// Hashing a long word takes longer than hashing a shorter word.
std::size_t hash_all_chars(std::string const & str)
{
    std::size_t hash_value = 0;
    for (auto c : str)
    {
        hash_value += c - 'a';
    }
    return hash_value;
}

// Hash function that takes each 4 character set from the word cast as an integer.
// This significantly increases the range.
// Sum these sets to distribute the words over the range.
// Guarantee O(1) time by bounding the number of characters used.
// This is better because it maintains O(1) time and we are increasing the range.
std::size_t hash_function(std::string const & str)
{
    int count = 16;
    std::size_t hash_value = 0;
    char const * c_str = str.c_str();
    while (c_str and *c_str and --count)
    {
        hash_value += (*c_str++ - 'a') << (count % 4);
    }
    return hash_value;
}

// Define item to be stored in a hash table.
// For simplicity, use a string as the key and int as the value.
struct Item
{
    std::string key{};
    int value = 0;
};

// Note: hash_function takes the key type only to allow for both insertion and removal
// (for removal, we would only have access to the key, not the item itself).
//std::size_t hash_function(std::string const &);
using Hashtable = std::vector<Item *>;

void insert(Hashtable & table, Item * entry)
{
    auto index = hash_function(entry->key) % table.size();
    table[index] = entry;
}

Item * find(Hashtable const & table, std::string const & key)
{
    auto index = hash_function(key) % table.size();
    if (table[index] != nullptr
        and key == table[index]->key)
    {
        return table[index];
    }
    return nullptr;
}

// Commonly, a hash table exposes a dynamic resizing method to increase the size of the underlying vector
// while maintaining the key to hash value mapping of the table.
// 1. Allocate a new buffer to store the elements.
// 2. Rehash the elements in the original table to the second.
// During dynamic resizing, a rehash is necessary (because modding with the new size to calculate the index).
// Because of this, it is sometimes beneficial to update the hash function at the same time.
// Such an update may be as simple as changing an offset for the hash value calculation.
void resize(Hashtable & table, std::size_t size)
{
    Hashtable new_table{size};
    for (auto entry : table)
    {
        if (entry != nullptr)
        {
            auto index = hash_function(entry->key) % new_table.size();
            new_table[index] = entry;
        }
    }
    table.swap(new_table);
}

// Hashtable definition with separate chaining.
using Hashtable_Chain = std::vector<std::list<Item *>>;

// Inserting into a hash table with separate chaining pushes onto the list stored at the bucket.
// This provides O(1) time insertion.
// This also guarantees that an item can be retrieved once it has been inserted as there is no eviction.
void insert(Hashtable_Chain & table, Item * entry)
{
    auto index = hash_function(entry->key) % table.size();
    table[index].push_back(entry);
}

Item * insert(Hashtable_Chain const & table, std::string const & key)
{
    int index = hash_function(key) % table.size();
    for (auto entry : table[index])
    {
        if (key == entry->key)
        {
            return entry;
        }
    }
    return nullptr;
}

// Insert with open addressing.
// Open addressing is a scheme by which collisions are resolved by inserting an element
// at the next available bucket in the hash table given some iteration scheme.
// Commonly, open addressing involves simply iterating through the table, called open addressing with linear probing.
// However, open addressing can be implemented by other probing schemes such as successively doubling the index value.
// If no available bucket is found, the table is dynamically resized and the insert resumes.
// Note that resizing guarantees an available bucket will be found.
// Dynamically resizing and a possible linear time enumeration are the cost the hash table pays for guaranteeing retrieval.
// TODO: How does removal work? If set entry's previous bucket_index to nullptr, then won't we lost entries that might come after it? Re-add/hash all items that come after it until reach nullptr?
void insert_lin_probe(Hashtable & table, Item * entry)
{
    while (true)
    {
        auto index = hash_function(entry->key) % table.size();
        for (std::size_t offset = 0; offset < table.size(); ++offset)
        {
            std::size_t bucket_index = (index + offset) % table.size();
            if (table[bucket_index] == nullptr)
            {
                table[bucket_index] = entry;
                return;
            }
        }
        resize(table, 2 * table.size() + 1);
    }
}
void insert_quad_probe(Hashtable & table, Item * entry)
{
    while (true)
    {
        // Same as linear probing, just offset *= 2.
        auto index = hash_function(entry->key) % table.size();
        for (std::size_t offset = 0; offset < table.size(); offset *= 2)
        {
            std::size_t bucket_index = (index + offset) % table.size();
            if (table[bucket_index] == nullptr)
            {
                table[bucket_index] = entry;
                return;
            }
        }
        resize(table, 2 * table.size() + 1);
    }
}

// To ensure that a false negative is not returned on lookup,
// an open addressing scheme must iterate through all successive buckets to ensure that an entry is not present at a successive location.
// Caution must be exercised to not loop continuously.
Item * find_lin_probe(Hashtable const & table, std::string const & key)
{
    std::size_t index = hash_function(key) % table.size();
    for (std::size_t offset = 0; offset < table.size(); ++offset)
    {
        auto bucket_index = (index + offset) % table.size();
        auto & item = table[bucket_index];
        if (item == nullptr)
        {
            return nullptr;
        }
        if (key == item->key)
        {
            return item;
        }

    }
    return nullptr;

    // TODO: Can the above loop be simplified to this?
    // for (std::size_t bucket_index = index; bucket_index < table.size(); ++bucket_index)
    // Seems like we do not need to have a separate offset variable nor mod each loop iteration.
}

// Cuckoo hashing is a modern approach to collision resolution that provides expected O(1) lookup and insertion
// when the load of the table is low.
// Uses a combination of 2 hash functions and 2 tables to find the next available bucket for a value.
struct CuckooHashtable
{
    using HashFunction = std::function<std::size_t (std::string const &)>;
    using HashFunctions = HashFunction[2];
    using Hashtables = Hashtable[2];

    HashFunctions hash_functions;
    Hashtables hash_tables;

    explicit CuckooHashtable(std::size_t);
    void insert(Item *);
    void resize(std::size_t);
    Item * find(std::string const &) const;
};

CuckooHashtable::
CuckooHashtable(std::size_t size)
    : hash_functions{hash_function, hash_function}
    , hash_tables{Hashtable(size), Hashtable(size)}
{
}

// Insertion begins by checking the first table.
// If the hash value bucket is occupied, its contents are replaced with the new entry and the old entry is promoted to the second table.
// This process repeates until all entries are added or else an infinite loop is detected.
// In the latter case, the table is resized and the process continues.
// table_index tracks whether the primary or secondary table is being used.
// The set is used to detect if a loop is encountered.
// The resize is necessary if an infinite loop is detected.
// With a poor choice of hash function, it is possible for the insert-resize process to also loop.
// To ensure against this, it is advisable to update the hash function during a resize.
void
CuckooHashtable::
insert(Item * entry)
{
    // TODO: If there are only ever 2 hash_tables/functions,
    // the set seems unnecessary as we could just use a simple flag to determine
    // if we've checked the first hash_table. Or, just use a for loop: for (table_index = 0; table_index != 2; ++table_index).
    std::set<std::size_t> visited[2];

    std::size_t table_index = 0;
    std::size_t index = 0;
    do
    {
        auto & table = hash_tables[table_index];
        auto hash_fun = hash_functions[table_index];
        index = hash_fun(entry->key) % table.size();
        if (table[index] == nullptr)
        {
            table[index] = entry;
            return;
        }

        std::swap(entry, table[index]);
        visited[table_index].insert(index);

        table_index = (table_index == 0) ? 1 : 0;
    }
    while (visited[table_index].find(index) == std::cend(visited[table_index]));

    resize(2 * hash_tables[0].size() + 1);
    insert(entry);
}

void
CuckooHashtable::
resize(std::size_t size)
{
    // Resize each hash table.
    ::resize(hash_tables[0], size);
    ::resize(hash_tables[1], size);
}

// Lookup in a hash table with cuckoo hasing must determine if the key is in the first or second table.
Item *
CuckooHashtable::
find(std::string const & key) const
{
    for (std::size_t table_index = 0; table_index < 2; ++table_index)
    {
        auto & table = hash_tables[table_index];
        auto hash_fun = hash_functions[table_index];
        auto index = hash_fun(key) % table.size();
        if (table[index] == nullptr)
        {
            return nullptr;
        }
        if (key == table[index]->key)
        {
            return table[index];
        }
    }
    return nullptr;
}

// Use unordered_map (hash table) to count frequency of each element in a list.
std::string find_most_common_element(
      std::istream & stream
    , std::unordered_map<std::string, std::size_t> & hash_table
    )
{
    std::string str{};
    while (not stream.eof() and stream >> str)
    {
        hash_table[str] += 1;
    }

    std::string max_str{};
    std::size_t max_count = 0;
    for (auto & entry : hash_table)
    {
        if (entry.second > max_count)
        {
            max_str = entry.first;
            max_count = entry.second;
        }
    }
    return max_str;
}

// A cache is a data structure used to reduce retrieval time of data from a remote store.
// A subset of elements is stored locally, and retrieval first looks locally before looking remotely.
// Caches are bounded and store only a subset of the data available from the remote store.
// For this reason, they must implement an eviction policy.
// A most recently used cache is one that evicts the least recently used element.
//
// The MRU cache must efficiently support both retrieval and priority updating.
// To implement this data type, we use a combination of 2 containers, unordered_map (i.e., hash table)
// and doubly linked list to maintain priority.
//
// The hash table is used to retrieve the address of the nodes of the linked list.
// The linked list implicitly stores the priority of each entry in the cache by the order of the nodes.
// The head of the linked list always contains the most recently accessed item, and the tail will be the least
// recently accessed item.
// In order to maintain correctness, the cache must be updated by calling find() whenever an element is used.
struct MruCache
{
    using key_type = std::string;
    using value_type = int;
    using Item = std::pair<key_type, value_type>;

    using List = std::list<Item const *>;
    List list{};

    using Hashtable = std::unordered_map<key_type, List::iterator>;
    Hashtable table{};
    std::size_t const bound;

    MruCache(std::size_t bound);
    Item const * find(key_type const &);
    void store(Item const *);
};

MruCache::
MruCache(std::size_t a_bound)
    : table{a_bound}
    , bound{a_bound}
{
}

// Use the hash table for quick retrieval of items from the cache.
// When find retrieves an item, that item is accessed and the priority must be updated.
// To update the priority without allocating new memory, move the location of item's node
// within the linked list to the head, which is done with std::list::splice().
// Time: O(1) because hash table lookup is O(1) and splice moving a node to the head is O(1).
MruCache::Item const *
MruCache::
find(key_type const & key)
{
    auto iter = table.find(key);
    if (iter != std::end(table))
    {
        // Move item to the front of the list since it is now the most recently used due to this access.
        list.splice(std::begin(list), list, iter->second);
        return *(iter->second);
    }
    return nullptr;
}

// When local retrieval fails, a method must call the remote store.
// The item retrieved is then saved to the cache by the store member function.
// It is within store that the eviction policy is implemented.
// The item stored is prepended to the front of the list.
// If the cache is above capacity after the item is added, the least recently used item is removed.
// This item is found at the tail of the list. When removed from the priority queue, the item must also be removed from the hash table.
// Time: O(1) because insertion into linked list and hash table and possibly list/table removal, which are all O(1).
// TODO: this assumes that find(item->key) already failed; otherwise, we could optimize it by checking before storing.
// TODO: a possible optimization is to reuse the back() node and copy item to it and then splice it to the front to avoid allocating a new node each call.
void
MruCache::
store(Item const * item)
{
    // Item is most recently used now.
    list.push_front(item);
    table[item->first] = std::begin(list);

    if (list.size() > bound)
    {
        // Evict least recently used from the cache.
        table.erase(list.back()->first);
        list.pop_back();
    }
}

int main(int argc, char * argv[])
{
    std::cout << "Hello" << std::endl;
	return EXIT_SUCCESS;
}
