//
// Created by Andrew Lvovsky on 5/31/17.
//

#ifndef SQL_PARSING_RECORD_H
#define SQL_PARSING_RECORD_H


#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

#include "constants.h"

using namespace std;

class Record
{
public:
    Record() {}

    void add(string entry);

    void print(int column_width);

    string get_data(int index);

    friend ostream& operator <<(ostream& out, Record& print_this)
    {
        print_this.print(COLUMN_WIDTH);
        return out;
    }
private:
    vector<string> _field_data;
    //bool _remove;
};


#endif //SQL_PARSING_RECORD_H