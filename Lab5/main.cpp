#include <iostream>
#include <string>
#include "TreeDB.h"
#include "DBentry.h"

using namespace std;

// Command Functions:
void commandInsert(TreeDB* treeDB);
void commandFind(TreeDB* treeDB);
void commandRemove(TreeDB* treeDB);
void commandPrintAll(TreeDB* treeDB);
void commandPrintProbes(TreeDB* treeDB);
void commandRemoveAll(TreeDB* treeDB);
void commandCountActive(TreeDB* treeDB);
void commandUpdateStatus(TreeDB* treeDB);

// Convert status string to bool
bool statusToBool (const string& status);

int main() {
    // Initialize BST Database
    TreeDB* treeDB = new TreeDB;

    while (!cin.eof()) {
        cout << "> ";
        string command;
        cin >> command;

        if (command == "insert")
            commandInsert(treeDB);
        else if (command == "find")
            commandFind(treeDB);
        else if (command == "remove")
            commandRemove(treeDB);
        else if (command == "printall")
            commandPrintAll(treeDB);
        else if (command == "printprobes")
            commandPrintProbes(treeDB);
        else if (command == "removeall")
            commandRemoveAll(treeDB);
        else if (command == "countactive")
            commandCountActive(treeDB);
        else if (command == "updatestatus")
            commandUpdateStatus(treeDB);
    }

    // Delete BST database before exit
    delete treeDB;

    return EXIT_SUCCESS;
}

void commandInsert(TreeDB* treeDB) {
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
        delete entry; // Delete entry as it was not inserted
    }
}

void commandFind(TreeDB* treeDB) {
    string name;
    cin >> name;
    DBentry* entry = treeDB->find(name);
    if (entry == nullptr)
        cout << "Error: entry does not exist" << endl;
    else
        cout << *entry << endl;
}

void commandRemove(TreeDB* treeDB) {
    string name;
    cin >> name;
    bool success = treeDB->remove(name);
    if (success)
        cout << "Success" << endl;
    else
        cout << "Error: entry does not exist" << endl;
}

void commandPrintAll(TreeDB* treeDB) {
    cout << *treeDB;
}

void commandPrintProbes(TreeDB* treeDB) {
    string name;
    cin >> name;
    DBentry* entry = treeDB->find(name);
    if (entry == nullptr)
        cout << "Error: entry does not exist" << endl;
    else
        treeDB->printProbes();
}

void commandRemoveAll(TreeDB* treeDB) {
    treeDB->clear();
    cout << "Success" << endl;
}

void commandCountActive(TreeDB* treeDB) {
    treeDB->countActive();
}

void commandUpdateStatus(TreeDB* treeDB) {
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
}

bool statusToBool (const string& status) {
    return status == "active";
}