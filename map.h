//
// Created by Andrew Lvovsky on 6/1/17.
//

#ifndef SQL_PARSING_MAP_H
#define SQL_PARSING_MAP_H

#include <vector>
#include "my_set.h"
#include <iostream>
using namespace std;

template <typename K, typename V>
struct Pair
{
    // CONSTRUCTORS

    Pair() {}

    Pair(K key) {_key = key;}

    Pair(K key, V value) {_key = key; _value.push_back(value);}

    // OVERLOADED PAIR OPERATORS

    vector<V> &operator [](int i) {return _value[i];}   // might need to add error check if greater than vector size

    bool operator <(const Pair<K,V> &right_side) const {return _key < right_side._key;}

    friend ostream& operator <<(ostream& out, const Pair<K,V>& print_this)
    {
        out << "Key: " << print_this._key << " " << "Value: [";
        for(int i = 0; i < print_this._value.size(); i++) {

            if (i == print_this._value.size() - 1)
                out << print_this._value[i];
            else
                out << print_this._value[i] << " ";
        }
        out << "]" << endl;

        return out;
    }

    // MEMBER VARIABLES
    K _key;
    vector<V> _value;   //might have to change to vector<V>
};

// =====================================================================

template <typename K, typename V>
class Map
{
public:
    Map() {}

    void my_insert(K key, V value);
    vector<V> get_values(K key);

    void print(int column_width) {_data.traverse(column_width);}
private:
    Set<Pair<K,V> > _data;
};

template <typename K, typename V>
void Map<K,V>::my_insert(K key, V value)
{
    if(_data.search(Pair<K,V>(key,value)) == nullptr)  //if the key isn't found
        _data.insert(Pair<K,V>(key,value));
    else
    {
        Pair<K,V> *insert_here = _data.search(Pair<K,V>(key,value));
        insert_here->_value.push_back(value);  // might need to fix
    }
}

template <typename K, typename V>
vector<V> Map<K,V>::get_values(K key)
{
    Pair<K,V>* data = _data.search(Pair<K,V>(key));
    return data->_value;
}


#endif //SQL_PARSING_MAP_H