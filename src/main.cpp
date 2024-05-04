#include <iostream>
#include "RangeStore.hpp"

// Example program for using RangeStore
int main()
{
    range_store<int, char> storage{'a'};

    storage.assign(50, 100, 'b');
    storage.assign(150, 200, 'c');

    const auto &start = storage[0];    // We expect 'a'
    const auto &first = storage[75];   // We expect 'b'
    const auto &middle = storage[125]; // We expect 'a'
    const auto &second = storage[175]; // We expect 'c'

    std::cout
        << "The starting value is " << start << "\n"
        << "The value in the first range is " << first << "\n"
        << "The value between the two ranges is " << middle << "\n"
        << "The value in the last range is " << second << std::endl;
}