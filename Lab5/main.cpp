#include <iostream>
#include <string>
#include "TreeDB.h"
#include "DBentry.h"

using namespace std;

const string ERROR_ENTRY_NOT_FOUND = "Error: entry does not exist";
const string ERROR_ENTRY_EXISTS = "Error: entry already exists";
const string SUCCESS = "Success";

// BST database
TreeDB* treeDB;

// Command Functions:
void commandInsert();
void commandFind();
void commandRemove();
void commandPrintAll();
void commandPrintProbes();
void commandRemoveAll();
void commandCountActive();
void commandUpdateStatus();

// Convert status string to bool
bool statusToBool (const string& status);

int main() {
    // Initialize Database
    treeDB = new TreeDB;

    while (!cin.eof()) {
        cout << "> ";
        string command;
        cin >> command;

        if (command == "insert")
            commandInsert();
        else if (command == "find")
            commandFind();
        else if (command == "remove")
            commandRemove();
        else if (command == "printall")
            commandPrintAll();
        else if (command == "printprobes")
            commandPrintProbes();
        else if (command == "removeall")
            commandRemoveAll();
        else if (command == "countactive")
            commandCountActive();
        else if (command == "updatestatus")
            commandUpdateStatus();
    }

    delete treeDB;
    return EXIT_SUCCESS;
}

void commandInsert() {
    string name;
    unsigned int ip;
    string status;
    cin >> name >> ip >> status;
    DBentry* entry = new DBentry(name, ip, statusToBool(status));
    bool success = treeDB->insert(entry);
    if (success)
        cout << SUCCESS << endl;
    else {
        cout << ERROR_ENTRY_EXISTS << endl;
        delete entry; // Delete entry as it was not inserted
    }
}

void commandFind() {
    string name;
    cin >> name;
    DBentry* entry = treeDB->find(name);
    if (entry == nullptr)
        cout << ERROR_ENTRY_NOT_FOUND << endl;
    else
        cout << *entry << endl;
}

void commandRemove() {
    string name;
    cin >> name;
    bool success = treeDB->remove(name);
    if (success)
        cout << SUCCESS << endl;
    else
        cout << ERROR_ENTRY_NOT_FOUND << endl;
}

void commandPrintAll() {
    cout << *treeDB;
}

void commandPrintProbes() {
    string name;
    cin >> name;
    DBentry* entry = treeDB->find(name);
    if (entry == nullptr)
        cout << ERROR_ENTRY_NOT_FOUND << endl;
    else
        treeDB->printProbes();
}

void commandRemoveAll() {
    treeDB->clear();
    cout << SUCCESS << endl;
}

void commandCountActive() {
    treeDB->countActive();
}

void commandUpdateStatus() {
    string name;
    string status;
    cin >> name >> status;
    DBentry* entry = treeDB->find(name);
    if (entry == nullptr)
        cout << ERROR_ENTRY_NOT_FOUND << endl;
    else {
        entry->setActive(statusToBool(status));
        cout << SUCCESS << endl;
    }
}

bool statusToBool (const string& status) {
    return status == "active";
}