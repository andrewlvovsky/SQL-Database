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
            cout << "ERROR: Select what now?" << endl << endl;
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

        while(_query[i] != "from" && _query[i] != "FROM")   // get field tokens
        {
            _field.push_back(_query[i]);
            i++;
        }

        i++;

        while((_query[i] != "where" && _query[i] != "WHERE") && (i < _query.size()))    // get table tokens
        {
            _table_name.push_back(_query[i]);
            i++;
        }

        if((_query[i] == "where" && _query[i] == "WHERE") || (i < _query.size()))   //get condition tokens
        {
            i++;
            while(i < _query.size())
            {
                _condition.push_back(_query[i]);
                i++;
            }
        }

        if(_condition.empty())
        {
            for (i = 0; i < _field.size(); i++) {
                for (int j = 0; j < _table.size(); j++) {
                    if (!(_table[j].print_field(_field[i]))) // magic
                        cout << "ERROR: No '" << _field[i] << "' column found in table '" << _table_name[j] << "'."
                             << endl;
                }
            }
        }
        else
        {
            for(i = 0; i < _field.size(); i++)
            {
                if(_condition[0] == _field[i])
                {
                    if(_condition[1] == "==" || _condition[1] == "=")
                    {
                        for(int j = 0; j < _table.size(); j++)
                        {
                            for(int k = 0; k < _table_name.size(); k++)
                            {
                                if (_table_name[k] == _table[j].get_table_name())
                                {
                                    vector<long> rec = _table[j].search_index_trees(_field[i], _condition[2]);

                                    if(rec.size() > 0)
                                        _table[j].print_from_records(rec);
                                    else
                                        cout << "ERROR: '" << _condition[2] << "' was not found in '"
                                             << _field[i] << "'." << endl;
                                }
                            }
                        }
                    }
                    else
                        cout << "ERROR: Invalid conditionals." << endl << endl;
                }
                else
                    cout << "ERROR: Invalid conditionals." << endl << endl;
            }
        }
    }
    else
    {
        int i = 2;

        if(i == _query.size())
        {
            for(int j = 0; j < _table.size(); j++)
                cout << _table[j] << endl;
        }
        else if((_query[i] == "from" || _query[i] == "FROM") && (i < _query.size()))    //select * from "table" case
        {
            i++;

            string current_table_name = _query[i];    //used for remembering where the current table is

            if(i == _query.size())
                cout << "ERROR: Select all from what now?" << endl;

            while(i < _query.size())
            {
                _table_name.push_back(_query[i]);
                i++;
            }

            for(int j = 0; j < _table.size(); j++)
            {
                if(_table[j].get_table_name() == current_table_name)
                {
                    current_table_name = _query[i + j];

                    for (int k = 0; k < _table[j].get_num_of_fields(); k++)
                    {
                        if (!_table[j].print_field(_table[j].get_field_name(k))) // magic
                            cout << "ERROR: No '" << _field[i] << "' column found in table '" << _table_name[j] << "'."
                                 << endl;
                    }
                }
            }
        }
        else
        {
            while ((_query[i] != "where" && _query[i] != "WHERE") && (i < _query.size()))
            {
                _table_name.push_back(_query[i]);
                i++;
            }

            if((_query[i] == "where" && _query[i] == "WHERE") && (i < _query.size()))   //get condition tokens
            {
                while(i < _query.size())
                {
                    _condition.push_back(_query[i]);
                    i++;
                }
            }
        }
    }

    // Clear the field and table name entry
    _field.clear();
    _table_name.clear();
    _condition.clear();
}

void SQL::current_tables()
{
    cout << "Current tables: ";

    for(int i = 0; i < _table.size(); i++)
        cout << _table[i].get_table_name() << " ";

    cout << endl << endl;
}
