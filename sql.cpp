//
// Created by Andrew Lvovsky on 6/5/17.
//

#include "sql.h"

void SQL::parse_commands()
{
    if(_query[0] == "LOAD" || _query[0] == "load")
    {
        if(_query.size() > 1)   //checks if valid token comes after command
        {
            Table t(_query[1]);
            cout << "Importing..." << endl;
            bool file_found = t.import_records(_query[1] + ".csv");
            if (file_found)
            {
                _table.push_back(t);
                cout << "File successfully imported" << endl << endl;
            }
            else
                cout << "ERROR: Invalid filename / file not found." << endl << endl;
        }
        else
            cout << "ERROR: No filename entered." << endl << endl;
    }
    else if(_query[0] == "PRINT" || _query[0] == "print")
    {
        if(_query.size() > 1)
        {
            int i = 0;
            while(i < _table.size())
            {
                if(_query[1] == _table[i].get_table_name())
                {
                    _table[i].print_table(20);
                    break;
                }
                i++;

                if(i == _table.size())
                    cout << "ERROR: Table with name '" << _query[1] << "' not found." << endl << endl;
            }
        }
        else
            cout << "ERROR: No filename entered." << endl << endl;
    }
    else if(_query[0] == "SELECT" || _query[0] == "select")
    {
        if(_query.size() > 1)
            select_parse_tree();
        else
            cout << "ERROR: Select what?" << endl << endl;
    }
    else
        cout << "ERROR: Invalid command." << endl << endl;
}

void SQL::tokenize_query(string user_input)
{
    _query.clear(); //to prevent error from future prompts being pushed into vector
    char *temp, *parsed_data;

    temp = &user_input[0];
    parsed_data = strtok(temp, " ,");

    while(parsed_data != nullptr)
    {
        _query.push_back(string(parsed_data));
        parsed_data = strtok(nullptr, " ,");
    }
}

void SQL::select_parse_tree()
{
    if(_query[1] != "*")
    {
        int i = 1;
        while(_query[i] != "from" && _query[i] != "FROM")
        {
            _field.push_back(_query[i]);
            i++;
        }

        while((_query[i] != "where" && _query[i] != "WHERE") && (i < _query.size()))
        {
            _table_name.push_back(_query[i]);
            i++;
        }
    }
    else
    {
        int i = 2;

        if(i == _query.size())
        {
            for(int j = 0; j < _table.size(); j++)
            {
                cout << _table[j] << endl;
            }
        }
        else
        {
            while ((_query[i] != "where" && _query[i] != "WHERE") && (i < _query.size())) {
                _table_name.push_back(_query[i]);
                i++;
            }
        }
    }
}
