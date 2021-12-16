#include "CYOA.hpp"

// ===================================================

//                  CYOA Class

// ===================================================
/**
 * @brief Construct a new CYOA::CYOA object
 * 
 */
CYOA::CYOA(): story_name(NULL), page_num(0), pages(), current_page(NULL), current_choices(), referenced() {}
/**
 * @brief Construct a new CYOA::CYOA object
 * 
 * @param directory_name 
 */
CYOA::CYOA(const std::string directory_name): story_name(directory_name), page_num(0), pages(), current_page(NULL), current_choices(), referenced() {
    savePages(directory_name);
    checkPages();
    setCurrent(1);
}
/**
 * @brief Destroy the CYOA::CYOA object
 * 
 */
CYOA::~CYOA(){}

/**
 * @brief get each page file name.
 * 
 * @param dir directory name
 * @param num page number
 * @return std::string the path of file.
 */
std::string CYOA::getFileName(const std::string dir, int num){
    std::string name = dir + "/page" + std::to_string(num) + ".txt";
    return name;
}

/**
 * @brief check whether the named file exist.
 * 
 * @param filename the file name to be checked.
 * @return true the file exist.
 * @return false the file doesn't exist.
 */
bool CYOA::hasFile(const std::string & filename){
    std::ifstream f(filename.c_str());
    return f.good();
}

/**
 * @brief add the referenced relationship.
 * 
 * @param page the new relationship in new page
 */
void CYOA::addReferenced(Page & page){
    std::vector<size_t> choice = page.getChoices();
    for(size_t i = 0; i < choice.size(); i++){
        size_t pn = choice[i];
        if(pn > page_num){
            findError("There is a page number out of bound!");
        }
        referenced[pn - 1].push_back(page.getPageNum()); // when page A has reference to pageB, referenced[B].push_back(A).
    }
}

/**
 * @brief handler to store all valid pages in story.
 * 
 * @param dir directory name
 */
void CYOA::savePages(const std::string & dir){
    std::string path = dir;
    if(!hasFile(getFileName(dir, 1))){
        findError("page 1.txt dose not exist!");
    }
    pages.push_back(Page(getFileName(dir, 1).c_str()));
    ++page_num;

    while(hasFile(getFileName(dir, page_num + 1))){
        pages.push_back(Page(getFileName(dir, ++page_num).c_str()));
    }
    referenced.resize(page_num);
    for(size_t i = 0; i < pages.size(); ++i){
        addReferenced(pages[i]);
    }
}

/**
 * @brief check whether the user's input is valid.
 * 
 * @param choice choice description
 * @return int if valid, return positive number.
 */
int CYOA::isValidChoice(const std::string choice){
    size_t choice_num = current_page->isPositiveNum(choice); // number > 0 ?
    if(choice_num > 0 && choice_num <= current_choices.size()){ // The number is greater than 0 and it's in the options
        size_t next_page = current_choices[choice_num - 1];
        return next_page <= page_num? next_page : 0; //If the option is within the page range, return next page number.
    }
    return 0; //  Otherwise, it is invalid.
}

/**
 * @brief check whether the story format.
 * 
 */
void CYOA::checkPages(){//check referenced relationship, WIN number, LOSE number
    size_t Win_num = 0, Lose_num = 0;
    for(size_t i = 0; i < page_num; ++i){
        if(pages[i].getType() == "WIN"){
            ++Win_num;
        }
        else if(pages[i].getType() == "LOSE"){
            ++Lose_num;
        }
    }
    if(Win_num == 0 || Lose_num == 0){
        findError("At least one page must be a WIN page and at least one page must be a LOSE page.");
    }
    for(size_t j = 1; j < page_num; ++j){
        if(!referenced[j].size()){
            findError("Every page is referenced by at least one *other* page's choices.");
        }
    }
} 

/**
 * @brief update the current page.
 * 
 * @param pn page number.
 */
void CYOA::setCurrent(size_t pn){
    current_page = &pages[pn - 1];
    current_choices = pages[pn - 1].getChoices();
}

/**
 * @brief start the CYOA story.
 * 
 */
void CYOA::readCYOA(){
    current_page->printPage();
    std::string in;
    int is_over = 0;
    while(getline(std::cin, in) && is_over == 0){
        int choice_num = isValidChoice(in);
        if(!choice_num){
            std::cout << "That is not a valid choice, please try again" << std::endl;
            continue;
        }
        setCurrent(choice_num);
        current_page->printPage();
        if(current_page->getType() == "WIN" || current_page->getType() == "LOSE"){
            is_over = 1;
            break;
        }
    }
}

/**
 * @brief calculate the reachable page's depth.
 * 
 * @return std::map<size_t, size_t> page depth result.
 */
std::map<size_t, size_t> CYOA::getPageDepth(){
    std::queue<size_t> waiting_do;
    std::map<size_t, size_t> page_depth; // (page num, page depth)

    size_t current_index = 1; // page 1
    size_t current_depth = 0;
    waiting_do.push(current_index); // push page 1 into container
    page_depth[current_index] = current_depth;

    while(!waiting_do.empty()){
        current_index = waiting_do.front();
        waiting_do.pop();
        
        std::vector<size_t> options = pages[current_index - 1].getChoices();
        current_depth = page_depth[current_index] + 1;
        for(size_t i = 0; i < options.size(); ++i){
            if(!page_depth.count(options[i])){ // if not yet visited
                page_depth[options[i]] = current_depth;
                waiting_do.push(options[i]);
            }
        }
    }
    return page_depth;
}

/**
 * @brief print the page depth.
 * 
 */
void CYOA::printDepth(){
    std::map<size_t, size_t> page_depth = getPageDepth();
    std::map<size_t, size_t>::iterator it;

    for(size_t i = 0; i < page_num; ++i){
        std::cout << "Page " << i+1;
        it = page_depth.find(i+1);
        if(it != page_depth.end()){
           std::cout << ":" << it->second << std::endl;             
        }
        else{
            std::cout<< " is not reachable" << std::endl;
        }
    }
}

/**
 * @brief check whether has at least one reachable win result.
 * 
 * @return true yes, it has.
 * @return false no way to win.
 */
bool CYOA::hasWin(){
    std::map<size_t, size_t> page_depth = getPageDepth();
    std::map<size_t, size_t>::iterator it;
    size_t win_reachable = 0;

    for(size_t i = 0; i < page_num; ++i){
        it = page_depth.find(i+1);
        if(it != page_depth.end()){ // this page is reachable
            if(pages[i].getType() == "WIN"){ // a reachable WIN page
                ++win_reachable;
            }
        }
    }

    if(win_reachable == 0){
        return false;
    }
    return true;
}

/**
 * @brief check whether the page in the reading path.
 * 
 * @param current_path the pages have been read
 * @param page page is to be checked
 * @return true the page is in the path
 * @return false the page isn't in the path
 */
bool CYOA::hasPage(std::vector<std::pair<size_t, size_t> > current_path, size_t page){
    std::vector<std::pair<size_t, size_t> >::iterator it;
    for(it = current_path.begin(); it != current_path.end(); ++it){
        if(page == it->first){
            return true;
        }
    }
    return false;
}

/**
 * @brief get the WIN way.
 * 
 * @return std::vector<std::vector<std::pair<size_t, size_t> > > the WIN way result(s).
 */
std::vector<std::vector<std::pair<size_t, size_t> > > CYOA::getWinRoute(){
    if(hasWin() == false){ // this story has no reachable WIN page
        std::cout << "This story is unwinnable!" <<std::endl;
        exit(EXIT_SUCCESS);
    }
    std::vector<std::pair<size_t, size_t> > current_path; //(page num, choice num)
    std::vector<std::vector<std::pair<size_t, size_t> > > paths; // all win path
    std::stack<std::pair<size_t, size_t> > waiting_do;
    std::map<size_t, size_t> subroute_num;

    size_t current_index = 1;
    waiting_do.push(std::pair<size_t, size_t>(current_index,0));
    while(!waiting_do.empty()){
        std::pair<size_t, size_t> current_p = waiting_do.top();
        current_index = current_p.first;
        waiting_do.pop();
        if(!current_path.empty()){ //B assigns the second value of the preceding node A of the path.
            current_path[current_path.size() - 1].second = current_p.second;
        }

        if(hasPage(current_path, current_index) == false){ // The node to be queried is not in the current path
            current_path.push_back(current_p); // Add to current path
            std::vector<size_t> options = pages[current_index - 1].getChoices(); // Get options for the current node
            size_t options_num = options.size();
            subroute_num[current_index] = options_num; // Records how many children of the current node need to be traversed

            if(options_num == 0){ // If the end of the current path is a WIN page
                if(pages[current_index - 1].getType() == "WIN"){
                    paths.push_back(current_path); //add the current path to the result container
                }
            }
            else{ // Adds a child page at the end of the current path
                for(size_t i = 0; i < options.size(); ++i){
                    waiting_do.push(std::pair<size_t, size_t>(options[i], i + 1)); //(page num, choice num)
                }
            }

        }
        if(current_path.empty()){
            break;
        }
        size_t last_page = current_path.back().first;
        while(subroute_num[last_page] == 0 && current_path.size() > 1){
            current_path.pop_back(); // The "leaf" page is deleted from the current path
            if(!current_path.empty()){
                last_page = current_path.back().first;
                subroute_num[last_page] --;
            }
        }
    }
    return paths;
}

/**
 * @brief print all the WIN way.
 * 
 */
void CYOA::printStrategy(){
    std::vector<std::vector<std::pair<size_t, size_t> > > paths = getWinRoute();
    size_t i = 0, j = 0;
    for(i = 0; i < paths.size(); ++i){
        for(j = 0; j < paths[i].size() - 1; ++j){
            std::cout<< paths[i][j].first << "(" <<paths[i][j].second << "),";
        }
        std::cout << paths[i][j].first << "(win)" << std::endl;
    }
}
