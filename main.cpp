#include "FileReader.cpp"
using namespace std;
// The main, starts the entire program.
// Requires correct string input for the command file name.
int main(int argc, char *argv[]) {
    // Commands file name goes here.
    string fileName = argv[1];
    // Create the FileReader object, the object that reads the file and creates the commands needed from it.
    auto* reader = new FileReader();
    // Call the parser using the lexer from the FileReader object.
    // The parser creates the commands using the output of the lexer.
    reader->parser(reader->lexer(fileName));
}