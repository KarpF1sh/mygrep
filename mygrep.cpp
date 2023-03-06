#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

// Flags
struct Flags {
    bool printLines = false;
    bool printCount = false;
    bool ignoreCase = false;
    bool reverse = false;
};

void setFlags(int argc, char* argv[], Flags &flags) {

    const int NPOS = std::string::npos;

    // Parse optional arguments
    for (int i = 1; i < argc; i++) {

        // Store arg in string
        std::string arg = argv[i];

        // Store the length of the characters before the actual options
        int flagLen;

        if (arg.rfind("-o", 0) == 0) {
            flagLen = 2;
        }
        else {
            break;
        }


        // Test if "-o" string was found in the arguments /
        // Then skip the first argument to find the flags after that
        if (arg.substr(flagLen).find("o") != NPOS)
            flags.printCount = true;

        if (arg.substr(flagLen).find("l") != NPOS)
            flags.printLines = true;

        if (arg.substr(flagLen).find("i") != NPOS)
            flags.ignoreCase = true;

        if (arg.substr(flagLen).find("r") != NPOS)
            flags.reverse = true;

        /*
        else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }*/
    }
}

bool findSubstring(std::string string, std::string substr, bool ignoreCase = false) {
    
    // If we want to ignore capital case, convert strings to lower
    if (ignoreCase) {
        for (auto& c : string) c = tolower(c);
        for (auto& c : substr) c = tolower(c);
    }

    // Convert srings to char[] and get the pointer to the substring if it's found
    const char* str_ptr = string.c_str();
    const char* substr_ptr = substr.c_str();
    const char* foundPtr = strstr(str_ptr, substr_ptr);

    // Return true if string was found, false if otherwise
    return foundPtr != NULL ? true : false;
}

// For simple string finding
void noArgsMain() {

    // Variables for the string and search string
    std::string string = "", search = "";

    // Ask and get input
    std::cout << "Give a string from which to search for: ";
    std::getline(std::cin, string);

    std::cout << "Give a search string: ";
    std::getline(std::cin, search);

    // Test if string was found
    bool found = findSubstring(string, search);

    // If string found
    if (found) {
        // Print
        std::cout << std::endl << "\"" << search << "\" found in \"" << string << "\" in position " << found << std::endl;
    }
    else {
        // --||--.
        std::cout << std::endl << "\"" << search << "\" NOT found in \"" << string << "\"" << std::endl;
    }
}

// Main
int main(int argc, char* argv[])
{
    if (argc > 1) {

        //int last = sizeof(argv) / sizeof(argv[0]);

        if (argc < 3) {
            std::cerr << "Too few arguments " << std::endl;
            return 1;
        }

        // filename should always be the last argument and the search string the second last
        std::string fileName = argv[argc - 1];
        std::string toFind = argv[argc - 2];

        Flags flags;
        setFlags(argc, argv, flags);

        try 
        {
            // Create new filestream
            std::ifstream myfile;

            // Open the file
            myfile.open(fileName);

            // Generate exceptions since iostream does not have them
            myfile.exceptions(std::ifstream::badbit);
            
            // Test for errors
            if (!myfile.is_open()) {
                std::cerr << "Error reading file: " << fileName << std::endl;
                return 1;
            }

            int lines = 0, count = 0;

            for (std::string line; getline(myfile, line);) {

                // Test if string was found
                bool found = findSubstring(line, toFind, flags.ignoreCase);

                // If string was in line /
                // Depending if reverse is set, do the inverse
                if ((found && !flags.reverse) || (!found && flags.reverse)) {

                    // Print
                    std::cout << std::endl;

                    // If we want line numbers
                    if (flags.printLines)
                        std::cout << std::to_string(lines) + ":     ";

                    std::cout << line;

                    // Add to counter
                    count++;
                }

                // Add to line counter
                lines++;
            }

            // Check if there was an error reading the file
            if (myfile.bad()) {
                std::cerr << "Error reading file: " << fileName << std::endl;
                return 1;
            }

            // Print the amount if asked
            if (flags.printCount)
                std::cout << std::endl << std::endl << "Occurrences of lines" << (flags.reverse ? " NOT" : "") << " containing \"" << toFind << "\": " << count;

            // Close the file
            myfile.close();

        }
        catch (std::exception const& e) {
            // Print out the exception type
            std::cout << std::endl << std::endl << "There was an error: " << e.what() << std::endl;

            return 1;   
        }

        // Exit
        return 0;
    }
    
    // Following code will run if no arguments are given //

    noArgsMain();

    // Exit
    return 0;
}
