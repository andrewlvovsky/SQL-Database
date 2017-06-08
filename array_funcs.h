//
// Created by Andrew Lvovsky on 5/11/17.
//

#ifndef ARRAY_FUNCS_H
#define ARRAY_FUNCS_H


#include <cstdlib>

template <typename T>
size_t _where_this_goes(T data[], size_t data_count, const T &item);

template <typename T>
void _shift_right(T data[], size_t data_count, size_t index);

template <typename T>
void _split(T source[], size_t &source_size, T dest[], size_t &dest_size);

// ==================================================================

template <typename T>
size_t _where_this_goes(T data[], size_t data_count, const T &item)
{
    size_t index = 0;

    while(index < data_count)
    {
        if(data[index] < item)
            index++;
        else
            break;
    }

    return index;
}

template <typename T>
void _shift_right(T data[], size_t data_count, size_t index)
{
    if(data_count != 0)
    {
        for (size_t j = data_count; j > index; j--)
            data[j] = data[j - 1];
    }
}

template <typename T>
void _split(T source[], size_t &source_size, T dest[], size_t &dest_size)
{
    dest_size = source_size / 2;
    source_size -= dest_size;

    for(int i = 0; i < dest_size; i++)
        dest[i] = source[source_size + i];
}


#endif //ARRAY_FUNCS_H
