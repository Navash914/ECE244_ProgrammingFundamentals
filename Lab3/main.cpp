//
//  main.cpp
//  lab3
//
//  Created by Naveed Ashfaq.
//


#include <iostream>
//#include <stdio.h> // Debug Only
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "shape.h"

// Define Error Messages:
#define INVALID_COMMAND         "invalid command"
#define INVALID_ARGUMENT        "invalid argument"
#define INVALID_NAME            "invalid shape name"
#define SHAPE_EXISTS(name)      "shape " + name + " exists"
#define SHAPE_NOT_FOUND(name)   "shape " + name + " not found"
#define INVALID_TYPE            "invalid shape type"
#define INVALID_VALUE           "invalid value"
#define TOO_MANY_ARGS           "too many arguments"
#define TOO_FEW_ARGS            "too few arguments"
#define ARRAY_FULL              "shape array is full"

// Define Error Priorities
#define INVALID_COMMAND_PRIORITY    1
#define INVALID_ARGUMENT_PRIORITY   2
#define INVALID_NAME_PRIORITY       3
#define SHAPE_EXISTS_PRIORITY       4
#define SHAPE_NOT_FOUND_PRIORITY    5
#define INVALID_TYPE_PRIORITY       6
#define INVALID_VALUE_PRIORITY      7
#define TOO_MANY_ARGS_PRIORITY      8
#define TOO_FEW_ARGS_PRIORITY       9
#define ARRAY_FULL_PRIORITY         10

// Shape database
shape** shapesArray;

// The number of shapes in the database, to be incremented
// everytime a shape is successfully created
int shapeCount = 0;

// The value of the argument to the maxShapes command
int maxShapes;

// Stores the error message and priority.
string errorMessage;
int errorPriority;

// Prototypes of functions.

// Function for each command.
void commandMaxShapes(stringstream& stream);                        // maxShapes Command
void commandCreate(stringstream& stream);                           // create Command
void commandMove(stringstream& stream);                             // move Command
void commandRotate(stringstream& stream);                           // rotate Command
void commandDraw(stringstream& stream);                             // draw Command
void commandDelete(stringstream& stream);                           // delete Command

void getInput(stringstream& stream, string& dest, bool isType);     // Read and error check string input
void getInput(stringstream& stream, int& dest);                     // Read and error check integer input

void resetError();                                                  // Resets error message and priority
void setError(const string& msg, int priority);                     // Set error message based on priority

bool inputExists(stringstream& stream);                             // Check if stream has more non-whitespace input
bool errorExists();                                                 // Check if any error was found

int findShapeIndexByName(const string& name);                       // Returns index in array for shape with name
void deleteShape(shape*& s, bool printOutput = true);               // Deletes shape

int main() {
    // Debug Line Only:
    //freopen(R"(E:\CLion Projects\ECE244 Labs\Lab3_CommandLineParser\cmake-build-debug\test.txt)", "r", stdin);

    string line;
    string command;

    cout << "> ";         // Prompt for input

    getline(cin, line);   // Get a line from standard input

    while (!cin.eof()) {
        // Debug Line Only:
        //cout << line << endl;

        // Put the line in a lineStream for parsing
        // Making a new sstream for each line so the flags are cleared

        stringstream lineStream (line);

        // Reset error check.
        resetError();

        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        if (command == "maxShapes")
            commandMaxShapes(lineStream);
        else if (command == "create")
            commandCreate(lineStream);
        else if (command == "move")
            commandMove(lineStream);
        else if (command == "rotate")
            commandRotate(lineStream);
        else if (command == "draw")
            commandDraw(lineStream);
        else if (command == "delete")
            commandDelete(lineStream);
        else // Invalid command.
            setError(INVALID_COMMAND, INVALID_COMMAND_PRIORITY);

        // Print error if error occurred.
        if (errorExists())
            cout << "Error: " << errorMessage << endl;

        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);   // Get the command line

    }  // End input loop until EOF.

    return EXIT_SUCCESS;
} // main

void commandMaxShapes(stringstream& stream) {
    stream >> maxShapes; // Input assumed to have no errors.

    // Create shapes array and initialize to NULL
    shapesArray = new shape*[maxShapes];
    for (int i = 0; i < maxShapes; i++)
        shapesArray[i] = nullptr;

    // Print Output
    cout << "New database: max shapes is " << maxShapes << endl;
} // commandMaxShapes

void commandCreate(stringstream& stream) {

    // Variables to store arguments
    string name, type;
    int x_loc, y_loc, x_sz, y_sz;

    // Get argument inputs.
    getInput(stream, name, false);

    // Check if shape exists.
    int shapeIndex = findShapeIndexByName(name);
    if (shapeIndex >= 0)
        setError(SHAPE_EXISTS(name), SHAPE_EXISTS_PRIORITY); // Shape already exists

    // Continue taking arguments
    getInput(stream, type, true);
    getInput(stream, x_loc);
    getInput(stream, y_loc);
    getInput(stream, x_sz);
    getInput(stream, y_sz);

    // Check for too many args or full array.
    if (!errorExists() && inputExists(stream))
        setError(TOO_MANY_ARGS, TOO_MANY_ARGS_PRIORITY);
    if (!errorExists() && shapeCount >= maxShapes)
        setError(ARRAY_FULL, ARRAY_FULL_PRIORITY);

    // If no errors at this points, safe to create object.
    if (!errorExists()) {
        // Create new shape and increment shapeCount
        shapesArray[shapeCount++] = new shape(name, type, x_loc, y_loc, x_sz, y_sz);
        // Print Output
        cout << "Created ";
        shapesArray[shapeCount-1]->draw();
    }
} // commandCreate

void commandMove(stringstream& stream) {
    // Variables to store arguments
    string name;
    int x_loc, y_loc;

    // Get argument inputs.
    getInput(stream, name, false);

    // Check if shape name exists
    int shapeIndex = findShapeIndexByName(name);
    if (!name.empty() && shapeIndex < 0)
        setError(SHAPE_NOT_FOUND(name), SHAPE_NOT_FOUND_PRIORITY); // Shape does not exist

    // Continue taking arguments
    getInput(stream, x_loc);
    getInput(stream, y_loc);

    // Check for too many args.
    if (!errorExists() && inputExists(stream))
        setError(TOO_MANY_ARGS, TOO_MANY_ARGS_PRIORITY);

    // If no errors at this point, safe to move object.
    if (!errorExists()) {
        // Move object
        shapesArray[shapeIndex]->setXlocation(x_loc);
        shapesArray[shapeIndex]->setYlocation(y_loc);
        // Print Output.
        cout << "Moved " << name << " to "
             << x_loc << " " << y_loc << endl;
    }
} // commandMove

void commandRotate(stringstream& stream) {
    // Variables to hold arguments
    string name;
    int angle;

    // Get argument inputs.
    getInput(stream, name, false);

    // Check if shape name exists
    int shapeIndex = findShapeIndexByName(name);
    if (!name.empty() && shapeIndex < 0)
        setError(SHAPE_NOT_FOUND(name), SHAPE_NOT_FOUND_PRIORITY); // Shape does not exist.

    // Continue taking arguments
    getInput(stream, angle);

    // Check if angle greater than 360 error
    if (angle > 360)
        setError(INVALID_VALUE, INVALID_VALUE_PRIORITY);

    // Check for too many args.
    if (!errorExists() && inputExists(stream))
        setError(TOO_MANY_ARGS, TOO_MANY_ARGS_PRIORITY);

    // If no errors at this point, safe to rotate object.
    if (!errorExists()) {
        // Rotate object
        shapesArray[shapeIndex]->setRotate(angle);
        // Print Output
        cout << "Rotated " << name << " by " << angle << endl;
    }
} // commandRotate

void commandDraw(stringstream& stream) {
    // Variables to store arguments.
    string name;

    // Get argument inputs.
    getInput(stream, name, false);

    // If 'name' arg is 'all', will normally have an INVALID_NAME error.
    // Here 'all' is a valid argument, so check for that.
    if (errorMessage == INVALID_NAME && name == "all")
        resetError(); // Remove error

    // Check if shape name exists
    int shapeIndex = findShapeIndexByName(name);
    if (!name.empty() && name != "all" && shapeIndex < 0)
        setError(SHAPE_NOT_FOUND(name), SHAPE_NOT_FOUND_PRIORITY); // Shape does not exist

    // Check for too many args.
    if (!errorExists() && inputExists(stream))
        setError(TOO_MANY_ARGS, TOO_MANY_ARGS_PRIORITY);

    // If no errors at this point, safe to draw object.
    if (!errorExists()) {
        if (name == "all") {
            // Draw all shapes
            cout << "Drew all shapes" << endl;
            for (int i = 0; i < shapeCount; i++) {
                if (shapesArray[i] == nullptr) continue; // Skip deleted objects
                shapesArray[i]->draw();
            }
        } else {
            cout << "Drew " << name << endl;
            shapesArray[shapeIndex]->draw();
        }
    }
} // commandDraw

void commandDelete(stringstream& stream) {
    // Variables to store arguments
    string name;

    // Get argument inputs.
    getInput(stream, name, false);

    // If 'name' arg is 'all', will normally have an INVALID_NAME error.
    // Here 'all' is a valid argument, so check for that.
    if (errorMessage == INVALID_NAME && name == "all")
        resetError(); // Remove error

    // Check if shape name exists
    int shapeIndex = findShapeIndexByName(name);
    if (!name.empty() && name != "all" && shapeIndex < 0)
        setError(SHAPE_NOT_FOUND(name), SHAPE_NOT_FOUND_PRIORITY); // Shape does not exist

    // Check for too many args.
    if (!errorExists() && inputExists(stream))
        setError(TOO_MANY_ARGS, TOO_MANY_ARGS_PRIORITY);

    // If no errors at this point, safe to delete object.
    if (!errorExists()) {
        if (name == "all") {
            // Delete all shapes
            cout << "Deleted: all shapes" << endl;
            for (int i = 0; i < shapeCount; i++) {
                if (shapesArray[i] == nullptr) continue; // Skip already deleted shapes
                deleteShape(shapesArray[i], false);
            }
        } else deleteShape(shapesArray[shapeIndex]);
    }

}

// Gets string input and does error checking
void getInput(stringstream& stream, string& dest, bool isType) {

    if (errorExists()) return;
    // Check if arg exists
    if (!inputExists(stream)) {
        setError(TOO_FEW_ARGS, TOO_FEW_ARGS_PRIORITY);
        return;
    }

    // Get input from stream;
    stream >> dest;

    // Since arg is string, it will be of correct type.

    if (isType) {
        // Check for errors related to shape type

        // Check if shape type is valid
        bool isValidType = false;
        for (int i = 0; i < NUM_TYPES; i++) {
            if (dest == shapeTypesList[i]) {
                isValidType = true;
                break;
            }
        }
        if (!isValidType) {
            setError(INVALID_TYPE, INVALID_TYPE_PRIORITY);
            return;
        }

    } else {
        // Check for errors related to shape name.

        // Check for invalid shape name
        int max = NUM_KEYWORDS > NUM_TYPES ? NUM_KEYWORDS : NUM_TYPES; // Larger value
        for (int i = 0; i < max; i++) {
            if ((i < NUM_KEYWORDS && dest == keyWordsList[i])
                || (i < NUM_TYPES && dest == shapeTypesList[i])
                    ) {
                // Shape name is a keyword or a type name
                setError(INVALID_NAME, INVALID_NAME_PRIORITY);
                return;
            }
        }
    }
} // getInput(string)

// Gets integer input and does error checking
void getInput(stringstream& stream, int& dest) {
    if (errorExists()) return;
    // Check if arg exists.
    if (!inputExists(stream)) {
        setError(TOO_FEW_ARGS, TOO_FEW_ARGS_PRIORITY);
        return;
    }

    // Get input from stream;
    stream >> dest;

    // Check for valid integer input.
    char c = (char) stream.peek();
    bool isWhiteSpace = c == ' ' || c == '\n' || c == '\r' ||
                        c == '\t' || c == '\f' || c == '\v';

    if (!stream.eof() && (stream.fail() || !isWhiteSpace)) {
        // Input is invalid if stream fails or is delimited by a non-whitespace character
        setError(INVALID_ARGUMENT, INVALID_ARGUMENT_PRIORITY);
        stream.clear();
        stream.ignore(1000, ' ');
    }

    // Check for appropriate value as value cannot be negative
    if (dest < 0)
        setError(INVALID_VALUE, INVALID_VALUE_PRIORITY);
} // getInput (integer)

bool inputExists(stringstream& stream) {
    stream >> std::ws; // Remove leading whitespaces
    return stream.peek() != EOF;
}

void setError(const string& msg, int priority) {
    if (errorPriority <= 0 || priority < errorPriority) {
        errorMessage = msg;
        if (priority > 0) errorPriority = priority;
    }
}

void resetError() {
    errorMessage = "";
    errorPriority = 0;
}

bool errorExists() {
    return !errorMessage.empty();
}

// Returns index in shapesArray for shape with matching name (-1 if shape does not exist)
int findShapeIndexByName(const string& name) {
    if (name == "all") return -1; // Return -1 if checking for all.

    for (int i = 0; i < shapeCount; i++) {
        if (shapesArray[i] != nullptr && shapesArray[i]->getName() == name) {
            return i; // Shape exists.
        }
    }

    return -1; // Shape does not exist.
}

void deleteShape(shape*& s, bool printOutput) {
    if (printOutput) cout << "Deleted shape " << s->getName() << endl;
    delete s;
    s = nullptr;
}