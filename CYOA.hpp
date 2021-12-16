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
#include "Page.hpp"

class CYOA{
public:
    // default constructor
    CYOA();
    // constructor
    CYOA(const std::string directory_name);
    // destructor
    ~CYOA();
    
    // get each page file name.
    std::string getFileName(const std::string dir, int num);

    // check whether the named file exist.
    bool hasFile(const std::string & filename);

    // add the referenced relationship.
    void addReferenced(Page & page);

    // handler to store all valid pages in story.
    void savePages(const std::string & dir);

    // check whether the user's input is valid.
    int isValidChoice(const std::string choice);

    // check whether the story format.
    void checkPages(); //referenced relationship, WIN, LOSE

    // update the current page.
    void setCurrent(size_t pn);

    // start the CYOA story.
    void readCYOA();

    // calculate the reachable page's depth.
    std::map<size_t, size_t> getPageDepth();

    // print the page depth.
    void printDepth();

    // check whether has at least one reachable win result.
    bool hasWin();

    // check whether the page in the reading path.
    bool hasPage(std::vector<std::pair<size_t, size_t> > current_path, size_t page);

    // get the WIN way.
    std::vector<std::vector<std::pair<size_t, size_t> > > getWinRoute();

    // print all the WIN way.
    void printStrategy();

private:
    std::string story_name; // story name
    size_t page_num; // total valid pages number in the story
    std::vector<Page> pages; // all valid pages in the stroy
    Page *current_page; // current page
    std::vector<size_t> current_choices; // current optional page numbers
    std::vector<std::vector<size_t> > referenced; // pages referenced
};
