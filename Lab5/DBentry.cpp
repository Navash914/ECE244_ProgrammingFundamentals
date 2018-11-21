#include <iostream>

//
// DBentry.cpp
//
// Created by Naveed Ashfaq on 2018-11-19.
//

#include "DBentry.h"

DBentry::DBentry() {
    name.clear();
    IPaddress = 0;
    active = false;
}

DBentry::DBentry(string _name, unsigned int _IPaddress, bool _active) {
    name = _name;
    IPaddress = _IPaddress;
    active = _active;
}

DBentry::~DBentry() = default;

void DBentry::setName(string _name) {
    name = _name;
}

void DBentry::setIPaddress(unsigned int _IPaddress) {
    IPaddress = _IPaddress;
}

void DBentry::setActive(bool _active) {
    active = _active;
}

string DBentry::getName() const {
    return name;
}

unsigned int DBentry::getIPaddress() const {
    return IPaddress;
}

bool DBentry::getActive() const {
    return active;
}

ostream &operator<<(ostream &out, const DBentry &rhs) {
    string active = rhs.getActive() ? "active" : "inactive";
    out << rhs.getName() << " : "
        << rhs.getIPaddress() << " : "
        << active << std::endl;
    return out;
}
