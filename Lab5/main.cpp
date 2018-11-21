#include <iostream>
#include <string>
#include "TreeDB.h"
#include "DBentry.h"

using namespace std;

bool statusToBool (const string& status) {
    return status == "active";
}

int main() {
    TreeDB* treeDB = new TreeDB;

    while (!cin.eof()) {
        cout << "> ";
        string command;
        cin >> command;

        if (command == "insert") {
            string name;
            unsigned int ip;
            string status;
            cin >> name >> ip >> status;
            DBentry* entry = new DBentry(name, ip, statusToBool(status));
            bool success = treeDB->insert(entry);
            if (success)
                cout << "Success" << endl;
            else {
                cout << "Error: entry already exists" << endl;
                delete entry;
            }
        } else if (command == "find") {
            string name;
            cin >> name;
            DBentry* entry = treeDB->find(name);
            if (entry == nullptr)
                cout << "Error: entry does not exist" << endl;
            else
                cout << *entry << endl;
        } else if (command == "remove") {
            string name;
            cin >> name;
            bool success = treeDB->remove(name);
            if (success)
                cout << "Success" << endl;
            else
                cout << "Error: entry does not exist" << endl;
        } else if (command == "printall") {
            cout << *treeDB;
        } else if (command == "printprobes") {
            string name;
            cin >> name;
            DBentry* entry = treeDB->find(name);
            if (entry == nullptr)
                cout << "Error: entry does not exist" << endl;
            else
                treeDB->printProbes();
        } else if (command == "removeall") {
            treeDB->clear();
            cout << "Success" << endl;
        } else if (command == "countactive") {
            treeDB->countActive();
        } else if (command == "updatestatus") {
            string name;
            string status;
            cin >> name >> status;
            DBentry* entry = treeDB->find(name);
            if (entry == nullptr)
                cout << "Error: entry does not exist" << endl;
            else {
                entry->setActive(statusToBool(status));
                cout << "Success" << endl;
            }
        } else {
            // Should never reach this point.
            cout << "Invalid command" << endl;
        }
    }

    delete treeDB;

    return EXIT_SUCCESS;
}