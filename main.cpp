#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//#include "table.h"
#include "sql.h"
//#include "record.h"
//#include "map.h"

void map_test();
//void table_test();
void load_table();

int main()
{
    string user_query;
    cout << endl << "Welcome to Andrew's SQL Database!" << endl << endl;

    SQL sql;
    while(user_query != "EXIT" && user_query != "exit")
    {
        cout << ">> ";
        getline(cin, user_query);
        sql.tokenize_query(user_query);
        sql.parse_commands();
        sql.current_tables();
    }
    return 0;
}

// template<K,V>
// class Map{
//      Map()
//      vector<V> &operator[](K Key);

//Table t("CS8 Roster");
//
//    t.import_records("roster.csv");

//void map_test()
//{
//    Map<string, long> m;
//
//    ifstream file;
//    string text;
//
//    file.open("firstNames.txt");
//
//    for(int i = 0; !file.eof(); i++)
//    {
//        getline(file, text);
//        //m.my_insert(string(ok), i);
//    }
//
//
//    m.print();
//}
//
//void table_test()
//{
//    Table t("Test Table");
//
//    t.import_records("roster.csv");
//
//    t.print_table(20);
//}

//    if(pt.parse_commands() != "")
//    {
//        Table t(pt.parse_commands());
//        bool file_found = t.import_records(pt.parse_commands() + ".csv");
//
//        if(file_found)
//            cout << "File successfully imported" << endl << endl;
//        else if(pt.parse_commands() == "1337")
//            cout << "ERROR: Invalid command." << endl << endl;
//        else
//            cout << "ERROR: Invalid filename / file not found." << endl << endl;
//    }
//    else if(pt.parse_commands() == "1337")
//        cout << "ERROR: Invalid command." << endl << endl;
//    else
//        cout << "ERROR: No filename entered." << endl << endl;