//
// Created by Andrew Lvovsky on 6/1/17.
//

#include "table.h"

bool Table::import_records(string filename)
{
    ifstream file;
    string current_line;
    char *parsed_data, *temp;

    file.open(filename);

    if(file.fail())
        return false;

    getline(file, current_line, char('\r'));
    temp = &current_line[0];
    parsed_data = strtok(temp, ",");

    while(parsed_data != nullptr)
    {
        _field_name.push_back(string(parsed_data)); // imports field name
        parsed_data = strtok(nullptr, ",");
    }

    for(int i = 0; !file.eof(); i++)
    {
        getline(file, current_line, char('\r'));

        if(current_line == "")      // test in case extra whitespace at end of file
            break;

        temp = &current_line[0];    // convert from string to char array
        parsed_data = strtok(temp, ",\n\r");

        _record.push_back(Record());

        for(int j = 0; j < _field_name.size(); j++)
        {
            _record[i].add(string(parsed_data));
            parsed_data = strtok(nullptr, ",\n\r"); // nullptr to continue scanning for tokens
        }
    }

    index_trees();
    return true;
}

void Table::print_table(int column_width)
{

    for(int i = 0; i < _field_name.size(); i++)
    {
        cout << _field_name[i];
        cout << endl << endl;
        _index[i].print(column_width);
        cout << endl << endl;
    }
}

void Table::index_trees()
{
    for(int i = 0; i < _field_name.size(); i++)
    {
        _index.push_back(Map<string,long>());

        for(int j = 0; j < _record.size(); j++)
            _index[i].my_insert(_record[j].get_data(i), j);
    }
}

