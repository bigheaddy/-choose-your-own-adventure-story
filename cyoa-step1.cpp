#include "CYOA.hpp"

int main(int argc, char** argv){
    argumentCheck(argc, 2);

    Page current_page(argv[1]);
    current_page.printPage();

    return EXIT_SUCCESS;
}