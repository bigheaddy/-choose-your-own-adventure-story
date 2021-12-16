#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <stack>


// single page
class Page{
public:
    // default constructor
    Page();
    // constructors
    Page(const size_t pn);
    Page(const char * file_name);
    // destructor
    ~Page(){}

    // If the string is positive, the positive number is returned, otherwise 0 is returned.
    int isPositiveNum(std::string content);

    // Gets the page number from the file name.
    void setPageNum(std::string file_name);

    // Check whether it is a choice quesition.
    bool isOption(const std::string str);

    // Extract the number of pages referenced by each choice question.
    void addChoice(std::string str);

    // set the page's type: CHOICE, WIN, LOSE
    void setPageType(const std::string str);

    // set the page's story-text (after'#').
    void setText(const std::string str);

    // the handler to store the info in a page.
    void setPage(std::istream &page);

    // get the optional page numbers
    std::vector<size_t> getChoices();

    // get the page number.
    size_t getPageNum();

    // get the page type.
    std::string getType();
    
    // print the page's info.
    void printPage();

private:
    size_t pagination; // page number
    std::vector<std::pair<std::string, size_t> > choices; // optional page numbers
    std::vector<std::string> text; // story behind the '#'
    std::string page_type; // "CHOICE"/"WIN"/"LOSE"
};

// print the error and exit the program.
void findError(const std::string str);

// check the argument number from command line.
void argumentCheck(int argc, int want_argc);

// open the file.
void openFile(const char * name, std::ifstream &f);
