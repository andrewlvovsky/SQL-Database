//
// Created by Andrew Lvovsky on 5/3/17.
//

#ifndef B_TREE_MY_SET_H
#define B_TREE_MY_SET_H


#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "../includes/queue.h"
#include "array_funcs.h"

using namespace std;

template <typename T>
class Set
{
public:
    // CONSTRUCTORS AND DECONSTRUCTOR
    Set();
    Set(const Set& source);
    ~Set() { clear(); }

    // MODIFICATION MEMBER FUNCTIONS
    void operator =(const Set& source);
    void clear();
    bool insert(const T& entry);
    // Postcondition: If an equal entry was already in the set, the set is unchanged and the
    // return value is false. Otherwise, entry was added to the set and the return value is true.

    bool erase(const T& target);
    // Postcondition: If target was in the set, then it has been removed from the set and the
    // return value is 1. Otherwise the set is unchanged and the return value is zero.

    T* search(const T& target);    // determines whether an item (target) appears in the set

    // CONSTANT MEMBER FUNCTION
    bool empty() const {return (data_count == 0); }

    // PRINT FUNCTION
    void print(int indent) const;

    void traverse(int column_width);
private:
    // CONSTANTS
    static const size_t MIN = 1;
    static const size_t MAX = MIN * 2;
    static const size_t MAX_PTR = MAX + 2;

    // VARIABLES
    T data[MAX + 1];
    size_t data_count;
    Set<T>* child_ptr[MAX_PTR];
    size_t child_count;

    // HELPER FUNCTIONS
    bool is_leaf() const { return (child_count == 0); }

    bool loose_insert(const T& entry);
    // Precondition: The entire B-tree is valid.
    // Postcondition: If entry was already in the set, then the set is unchanged
    // and the return value is false. Otherwise, the entry has been added to the
    // set, the return value is true, and the entire B-tree is still
    // valid EXCEPT that the number of entries in the root of this set might be
    // one more than the allowed maximum.

    void fix_excess(size_t index);
    // Precondition: The entire B-tree is valid.
    // Postcondition: If entry was already in the set, then the set is unchanged
    // and the return value is false. Otherwise, the entry has been added to the
    // set, the return value is true, and the entire B-tree is still
    // valid EXCEPT that the number of entries in the root of this set might be
    // one more than the allowed maximum.

    bool loose_erase(const T& target);
    // Precondition: The entire B-tree is valid.
    // Postcondition: If target was not in the set, then the set is unchanged and
    // the return value is false. Otherwise, the target has been removed, the
    // return value is true, and the entire B-tree is still valid
    // EXCEPT that the number of entries in the root of this set might be one
    // less than the allowed minimum.

    void fix_shortage(size_t index);
    // Precondition: (i < child_count) and the entire B-tree is valid EXCEPT that
    // subset[i] has MINIMUM - 1 entries.
    // Postcondition: The tree has been rearranged so that the entire B-tree is
    // valid EXCEPT that the number of entries in the root of this set might be
    // one less than the allowed minimum.

    void remove_biggest(T& removed_entry);
    // Precondition: (data_count > 0) and the entire B-tree is valid.
    // Postcondition: The largest item in the set has been removed, and a copy
    // of this removed entry has been placed in removed_entry. The entire
    // B-tree is still valid, EXCEPT that the number of entries in the root of this
    // set might be one less than the allowed minimum.
};

template <typename T>
Set<T>::Set()
{
    data_count = 0;
    child_count = 0;

    for(int i = 0; i < MAX_PTR; i++)
        child_ptr[i] = nullptr;
}

template <typename T>
Set<T>::Set(const Set<T> &source)
{
    data_count = source.data_count;
    for(int i = 0; i < data_count; i++)
        data[i] = source.data[i];

    child_count = source.child_count;
    for(int i = 0; i < child_count; i++)
        child_ptr[i] = new Set<T>(*(source.child_ptr[i]));
}

template <typename T>
void Set<T>::operator =(const Set<T> &source)
{
    data_count = source.data_count;
    for(int i = 0; i < data_count; i++)
        data[i] = source.data[i];

    child_count = source.child_count;
    for(int i = 0; i < MAX_PTR; i++)
        child_ptr[i] = source.child_ptr[i];
}

template <typename T>
void Set<T>::clear()
{
    for(int i = 0; i < data_count; i++)
        data[i] = T();
    data_count = 0;

    for(int i = 0; i < child_count; i++)
        child_ptr[i] = nullptr;
    child_count = 0;
}

template <typename T>
bool Set<T>::insert(const T &entry)
{
    if(!loose_insert(entry))
        return false;
    if(data_count > MAX)
    {
        Set<T>* new_root = new Set<T>(*this);
        clear();
        child_ptr[0] = new_root;
        child_count++;
        fix_excess(0);
    }
    return true;
}

template <typename T>
bool Set<T>::erase(const T &target)
{
    if(!loose_erase(target))
        return false;
    if((data_count == 0) && (child_count == 1))
    {
        Set<T>* temp = child_ptr[0];
        clear();
        for(int i = 0; i < temp->data_count; i++)
            data[i] = temp->data[i];
        for(int i = 0; i < child_count; i++)
            child_ptr[i] = temp->child_ptr[i];
        data_count = temp->data_count;
        child_count = temp->child_count;

        delete temp;
    }
    return true;
}

template <typename T>
T* Set<T>::search(const T& target)
{
    size_t index = _where_this_goes(data, data_count, target);

    if((index < data_count) && !(target < data[index]))
        return &data[index];    // might need to fix
    else if(is_leaf())
        return nullptr;
    else
        return child_ptr[index]->search(target);
}

template <typename T>
void Set<T>::print(int indent) const
{
    const int EXTRA_INDENTATION = 4;
    size_t i;

    cout << setw(indent) << "";

    for (i = 0; i < data_count; i++)
        cout << data[i] << " ";
    cout << endl;

    for (i = 0; i < child_count; i++)
        child_ptr[i]->print(indent + EXTRA_INDENTATION);
}

template <typename T>
bool Set<T>::loose_insert(const T &entry)
{
    size_t index = _where_this_goes(data, data_count, entry);

    if((index < data_count) && !(entry < data[index]))
        return false;   // new entry is already in set
    else if(is_leaf())
    {
        _shift_right(data, data_count, index); // shifts element to the right of index
        data[index] = entry;
        data_count++;
        return true;    // entry is added
    }
    else
    {
        child_ptr[index]->loose_insert(entry);
        if(child_ptr[index]->data_count > MAX)
            fix_excess(index);
        return true;
    }
}

template <typename T>
void Set<T>::fix_excess(size_t index)
{
    Set<T>* sibling = new Set<T>();

    _split(child_ptr[index]->data, child_ptr[index]->data_count, sibling->data, sibling->data_count);

    if(data_count == 0)
        data[0] = child_ptr[index]->data[child_ptr[index]->data_count - 1];
    else
    {
        size_t root_index = _where_this_goes(data, data_count, child_ptr[index]->data[data_count - 1]);
        _shift_right(data, data_count, root_index);
        _shift_right(child_ptr, child_count, root_index);
        data[root_index] = child_ptr[index]->data[child_ptr[index]->data_count-1];
    }

    _split(child_ptr[index]->child_ptr, child_ptr[index]->child_count, sibling->child_ptr, sibling->child_count);

    child_ptr[index+1] = sibling;
    child_count++;
    child_ptr[index]->data_count--;
    data_count++;
}

template <typename T>
bool Set<T>::loose_erase(const T &target)
{
    size_t index = _where_this_goes(data, data_count, target);

    if((is_leaf()) && (data[index] != target))
        return false;
    else if((is_leaf()) && (data[index] == target))
    {
        for(size_t i = index; i < data_count; i++)
            data[i] = data[i + 1];
        data_count--;

        return true;
    }
    else if((!is_leaf()) && (data[index] != target))
    {
        bool saved_answer = child_ptr[index]->loose_erase(target);

        if(child_ptr[index]->data_count < MIN)
            fix_shortage(index);

        return saved_answer;
    }
    else if((!is_leaf()) && (data[index] == target))
    {
        child_ptr[index]->remove_biggest(data[index]);

        if(child_ptr[index]->data_count < MIN)
            fix_shortage(index);

        return true;
    }
    return false;
}

template <typename T>
void Set<T>::fix_shortage(size_t index)
{
    if((child_ptr[index - 1] != nullptr) && (child_ptr[index - 1]->data_count > MIN))
    {
        _shift_right(child_ptr[index]->data, child_ptr[index]->data_count, 0);
        child_ptr[index]->data[0] = data[index - 1];
        child_ptr[index]->data_count++;

        data[index - 1] = child_ptr[index - 1]->data[child_ptr[index - 1]->data_count - 1];
        child_ptr[index - 1]->data_count--;

        if(!(child_ptr[index - 1]->is_leaf()))
        {
            _shift_right(child_ptr[index]->child_ptr, child_ptr[index]->child_count, 0);
            child_ptr[index]->child_count++;
            child_ptr[index]->child_ptr[0] = child_ptr[child_count - 1];
            child_ptr[index - 1]->child_count--;
        }
    }
    else if((child_ptr[index + 1] != nullptr) && (child_ptr[index + 1]->data_count > MIN)) // be careful with this, might have to retest
    {
        _shift_right(child_ptr[index]->data, child_ptr[index]->data_count, 0);
        child_ptr[index]->data[0] = data[index + 1];
        child_ptr[index]->data_count++;

        data[index + 1] = child_ptr[index - 1]->data[child_ptr[index + 1]->data_count - 1];
        child_ptr[index + 1]->data_count--;

        if(!(child_ptr[index + 1]->is_leaf()))
        {
            _shift_right(child_ptr[index]->child_ptr, child_ptr[index]->child_count, 0);
            child_ptr[index]->child_count++;
            child_ptr[index]->child_ptr[0] = child_ptr[child_count - 1];
            child_ptr[index + 1]->child_count--;
        }
    }
    else if((child_ptr[index - 1] != nullptr) && (child_ptr[index - 1]->data_count == MIN))    //might need to add i > 0 case
    {
        child_ptr[index - 1]->data[child_ptr[index - 1]->data_count] = data[index - 1];
        child_ptr[index - 1]->child_count++;

        for(size_t i = index; i < data_count; i++)  //might need to fix
            data[i - 1] = data[i];
        data_count--;

        size_t j = 0;
        for(size_t i = child_ptr[index - 1]->data_count; j < child_ptr[index]->data_count; i++) // might need to fix
        {
            child_ptr[index - 1]->data[i] = child_ptr[index]->data[j];
            child_ptr[index - 1]->data_count++;
            j++;
        }
        j = 0;
        for(size_t i = child_ptr[index - 1]->child_count; j < child_ptr[index]->child_count; i++) // might need to fix
        {
            child_ptr[index - 1]->child_ptr[i] = child_ptr[index]->child_ptr[j];
            child_ptr[index - 1]->child_count++;
            j++;
        }

        child_ptr[index]->data_count = 0;
        child_ptr[index]->child_count = 0;

        delete child_ptr[index];
        child_count--;

        for(size_t i = index; i < child_count; i++) //might need to fix
            child_ptr[i] = child_ptr[i + 1];
    }
    else if((child_ptr[index + 1] != nullptr) && (child_ptr[index + 1]->data_count == MIN))
    {
        //shift older sibling (child_ptr[index + 1]) to right to make room for data[index]
        _shift_right(child_ptr[index + 1]->data, child_ptr[index + 1]->data_count, 0);

        //bring in data[index + 1] to the front of that child
        child_ptr[index + 1]->data[0] = data[index + 1];
        child_ptr[index + 1]->data_count++;

        //shift all of root data's elements to the left to fill the gap
        for(size_t i = index; i < data_count; i++)  //might need to fix
            data[i] = data[i + 1];
        data_count--;

        //shift older sibling to the right once more to make room for its younger sibling
        for(int i = 0; i < MIN - 1; i++)
        {
            _shift_right(child_ptr[index + 1]->data, child_ptr[index + 1]->data_count, 0);
            child_ptr[index + 1]->data_count++;
        }
        //and do the same shifting for pointers
        for(int i = 0; i < (MIN - 1) * 2; i++)
        {
            _shift_right(child_ptr[index + 1]->child_ptr, child_ptr[index + 1]->child_count, 0);
            child_ptr[index + 1]->child_count++;
        }

        //next, transfer all data/children from child_ptr[index] to the front of older sibling
        for(size_t i = 0; i < child_ptr[index]->data_count; i++)
            child_ptr[index + 1]->data[i] = child_ptr[index]->data[i];
        for(size_t i = 0; i < child_ptr[index]->child_count; i++)
            child_ptr[index + 1]->child_ptr[i] = child_ptr[index]->child_ptr[i];

        //set problem childs' counts to 0 in preparation for removal
        child_ptr[index]->data_count = 0;
        child_ptr[index]->child_count = 0;

        //remove the now-empty child from memory
        delete child_ptr[index];
        child_count--;

        //shift all children to left to fill gap
        for(size_t i = index; i < child_count; i++)
            child_ptr[i] = child_ptr[index + 1];
    }
}

template <typename T>
void Set<T>::remove_biggest(T &removed_entry)
{
    if(is_leaf())
    {
        removed_entry = data[data_count - 1];
        data_count--;
    }
    else
    {
        child_ptr[child_count - 1]->remove_biggest(removed_entry);

        if(child_ptr[child_count - 1]->data_count < MIN)
            fix_shortage(child_count - 1);
    }
}

template <typename T>
void Set<T>::traverse(int column_width)
{
    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < data_count; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (!is_leaf())
            child_ptr[i]->traverse(column_width);
        cout << data[i];
    }

    // Print the subtree rooted with last child
    if (!is_leaf())
        child_ptr[i]->traverse(column_width);
}


#endif //B_TREE_MY_SET_H