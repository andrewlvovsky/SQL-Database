//
// Created by Andrew Lvovsky on 6/5/17.
//

#ifndef SQL_PARSING_SQL_H
#define SQL_PARSING_SQL_H


#include <iostream>
#include <vector>
#include "table.h"

using namespace std;

class SQL
{
public:
public:
    SQL() {}
    SQL(string user_input) {tokenize_query(user_input);}

    void tokenize_query(string user_input);
    void parse_commands();
    void current_tables();

private:
    vector<string> _query;
    vector<Table> _table;

    vector<string> _field;
    vector<string> _table_name;
    vector<string> _condition;
    vector<string> _temp_fields;

    void select_parse_tree();
};


#endif //SQL_PARSING_SQL_H
