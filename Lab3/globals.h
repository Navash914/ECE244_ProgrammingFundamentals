//
//  globals.h
//  lab3
//
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018 Tarek Abdelrahman. All rights reserved.
//

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********

#ifndef globals_h
#define globals_h
#include<string>

// These arrays should make it easier to check that a name is not a reserved word
// and that an entered shape type is valid
#define NUM_KEYWORDS 7
string keyWordsList[7]={"all", "maxShapes", "create", "move", "rotate", "draw", "delete"};

#define NUM_TYPES 3
string shapeTypesList[3]={"ellipse", "rectangle","triangle"};

#endif /* globals_h */
