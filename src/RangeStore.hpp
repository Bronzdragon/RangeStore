#ifndef _INTERVAL_MAP_H_
#define _INTERVAL_MAP_H_
#pragma once

#include <map>

template <typename K, typename V>
class range_store
{
    V m_valBegin;
    std::map<K, V> m_map;

public:
    range_store(V const &val)
        : m_valBegin(val){};

    void assign(K const &keyBegin, K const &keyEnd, V const &val)
    {
        if (!(keyBegin < keyEnd))
            return; // Empty interval. Ignore.

        auto boundStart = m_map.lower_bound(keyBegin);
        auto boundEnd = m_map.lower_bound(keyEnd);

        const bool isBounded = (boundEnd != m_map.end() && !(keyEnd < boundEnd->first));

        // We're starting by setting the range's end value (if necessary)
        if (isBounded)
        {
            if (boundEnd->second == val)
            {
                // We don't need this marker if the values are the same.
                m_map.erase(boundEnd);
            }
        }
        else if (boundStart != boundEnd)
        {
            // We must have markers already in this range.

            boundEnd--;
            // We can move one of the range markers to be our new bound.
            auto extractedValue = m_map.extract(boundEnd->first);
            extractedValue.key() = keyEnd;

            if (!(extractedValue.mapped() == val))
            {
                // We only need a marker if the ranges have different values
                // (Otherwise they can be 'merged')
                m_map.insert(std::move(extractedValue));
            }
        }
        else
        {
            // No marker in our range, and no marker on the bound.
            V const &prevVal = operator[](keyBegin);
            if (!(prevVal == val))
            {
                // We only need a marker if the ranges have different values
                // (Otherwise they can be 'merged', and we don't have to set any value.)

                m_map.insert({keyEnd, prevVal});
            }
        }

        // Delete all the inner markers.
        m_map.erase(m_map.lower_bound(keyBegin), m_map.lower_bound(keyEnd));

        // Since there's no markers in our range (we deleted them all earlier), we decide which value
        // to compare to by checking if there's a value before our range.
        boundStart = m_map.lower_bound(keyBegin);
        V const &valToCheck = (boundStart == m_map.begin()) ? m_valBegin : (--boundStart)->second;

        if (!(val == valToCheck))
        {
            // We need to insert a new key here.
            m_map.insert_or_assign(keyBegin, val);
        }
    }

    // look-up of the value associated with key
    inline V const &operator[](K const &key) const
    {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin())
        {
            return m_valBegin;
        }
        else
        {
            return (--it)->second;
        }
    }
};

#endif
