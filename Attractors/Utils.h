#pragma once
#include <vector>
#include <map>

template<typename T, typename U>
std::vector<T> keys(std::map<T, U> const& m)
{
    std::vector<T> k;
    for(auto const& element : m)
    {
        k.push_back(element.first);
    }
    return k;
}

template<typename T, typename U>
std::vector<U> values(std::map<T, U> const& m)
{
    std::vector<U> v;
    for(auto const& element : m)
    {
        v.push_back(element.second);
    }
    return v;
}