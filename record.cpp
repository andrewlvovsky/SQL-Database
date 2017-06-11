//
// Created by Andrew Lvovsky on 5/31/17.
//

#include "record.h"

void Record::add(string entry)
{
    _field_data.push_back(entry);
}

void Record::print(int column_width)
{
    for(int i = 0; i < _field_data.size(); i++)
        cout << _field_data[i] << setw(column_width);
    cout << setw(0) << endl;
}

string Record::get_data(int index)
{
    return _field_data[index];
}
