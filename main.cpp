#include "recognition.h"

int main()
{
    Recognition recognition;
    recognition.init( "./inputs/", "operations.txt", "questions.txt", "subjects.txt", "objects.txt" );

    std::string input = "";
    while( getline(std::cin, input) )
    {
        auto response = recognition.recognize( input );
        if( response.empty() )
            response = "Unrecognized intent!";
        std::cout << input << " => Prints (Intent: " << response << ")\n";
    }

    return EXIT_SUCCESS;
}