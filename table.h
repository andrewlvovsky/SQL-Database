//
// Created by Andrew Lvovsky on 6/1/17.
//

#ifndef SQL_PARSING_TABLE_H
#define SQL_PARSING_TABLE_H


#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "record.h"
#include "map.h"
#include "constants.h"

using namespace std;



class Table
{
public:
    Table() {_table_name = "UNKNOWN TABLE";}

    Table(string name) {_table_name = name;}

    string get_table_name() {return _table_name;}

    bool import_records(string filename);

    void print_table(int column_width);

    bool print_field(string field_name_key);

    int get_num_of_fields() {return _field_name.size();}

    string get_field_name(int index) {return _field_name[index];}

    void print_from_records(vector<long> index_for_recs);

    vector<long> search_index_trees(string field_name, string entry_to_search);

    friend ostream& operator <<(ostream& out, Table& print_this)
    {
        print_this.print_table(COLUMN_WIDTH);
        return out;
    }
private:
    string _table_name;
    vector<string> _field_name;
    vector<Record> _record;
    vector<Map<string,long> > _index;

    void index_trees(); // stores all data respective to each field into b-trees
};


#endif //SQL_PARSING_TABLE_H