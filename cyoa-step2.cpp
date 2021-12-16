#include "CYOA.hpp"

int main(int argc, char** argv){
    argumentCheck(argc, 2);

    CYOA story(argv[1]);
    story.readCYOA();

    return EXIT_SUCCESS;
}
