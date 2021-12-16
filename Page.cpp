#include "Page.hpp"

/**
 * @brief print the error and exit the program.
 * 
 * @param str error information.
 */
void findError(const std::string str){
    std::cerr << str << std::endl;
    exit(EXIT_FAILURE);
}

/**
 * @brief check the argument number from command line.
 * 
 * @param argc  argument number you want to check.
 * @param want_argc wanna number.
 */
void argumentCheck(int argc, int want_argc){
    if(argc != want_argc){
        findError("Wrong number of input arguments!");
    }
}

/**
 * @brief open the file.
 * 
 * @param name filename
 * @param f the stream
 */
void openFile(const char * name, std::ifstream &f){
    f.open(name);
}

// ===================================================

//                  Page Class

// ===================================================
/**
 * @brief Construct a new Page:: Page object
 * 
 */
Page::Page() : pagination(0), choices(), text(), page_type("NOTYPE"){}

/**
 * @brief Construct a new Page:: Page object
 * 
 * @param pn page number
 */
Page::Page(const size_t pn) : pagination(pn), choices(), text(), page_type("NOTYPE"){}

/**
 * @brief Construct a new Page:: Page object
 * 
 * @param file_name file name
 */
Page::Page(const char* file_name) : pagination(0), choices(), text(), page_type("NOTYPE"){
    std::ifstream page_file;
    openFile(file_name, page_file);
    if(!page_file.is_open()){
        findError("file open unsuccessfully!");
    }
    setPageNum(file_name);
    setPage(page_file);
    page_file.close();
}

/**
 * @brief check whether the string is positive number.
 * 
 * @param content the string to be checked.
 * @return int if so, return positive int. otherwise, return 0.
 */
int Page::isPositiveNum(std::string content){
    if(content.length() == 0){
        return 0;
    }
    size_t content_len = content.length();
    for(size_t i = 0; i < content_len; i++){
        if(!isdigit(content[i])){
            return 0;
        }
    }
    size_t number = atoi(content.c_str());
    return (number >=1)? number : 0;
}

/**
 * @brief Gets the page number from the file name
 * 
 * @param file_name the file name.
 */
void Page::setPageNum(std::string file_name){
    size_t period = file_name.find('.');
    size_t before_num = file_name.find("page") + 4;
    std::string pn_str = file_name.substr(before_num, period - before_num);
    int pn = isPositiveNum(pn_str);
    if(pn){
        pagination = pn;
    }
}

/**
 * @brief Check whether it is a choice quesition.
 * 
 * @param str the string to be checked.
 * @return true is option(has':' and the number is > 0).
 * @return false is not a option.
 */
bool Page::isOption(const std::string str){
    size_t find_colon = str.find(':');
    if(find_colon == str.npos){
        findError("This choice has no colon, illegal format!");
    }

    std::string pagination = str.substr(0, find_colon);
    if(isPositiveNum(pagination) == 0){
        findError("This choice has illegal page number!");
    }

    return true;
}

/**
 * @brief Extract the number of pages referenced by each choice question.
 * 
 * @param str the choice
 */
void Page::addChoice(std::string str){
    if(isOption(str)){
        size_t colon = str.find(':');
        size_t pn = atoi(str.substr(0, colon).c_str());
        std::string choice_text = str.substr(colon + 1);
        choices.push_back(std::pair<std::string, size_t>(choice_text, pn));
    }
}

/**
 * @brief set the page's type: CHOICE, WIN, LOSE
 * 
 * @param str the type
 */
void Page::setPageType(const std::string str){
    if(page_type == "NOTYPE"){
        page_type = str;
    }
    else if(page_type != str){
        findError("It's a mix type page, illegal!");
    }
}

/**
 * @brief set the page's story-text (after'#').
 * 
 * @param str line in the story text.
 */
void Page::setText(const std::string str){
    text.push_back(str);
}

/**
 * @brief the handler to store the info in a page.
 * 
 * @param page the page information
 */
void Page::setPage(std::istream &page){
    int pound_sign = 0; // judge the stream is before or behind the '#'
    int empty_file = 1; // check whether it is an empty file

    std::string line;
    while(getline(page, line)){
        empty_file = 0;

        if(pound_sign == 0){ // stream still before '#'
            if(line == "WIN"){
                if(page_type == "WIN"){
                    findError("The navigation section of the WIN page has redundant content!");
                }
                setPageType(line); // set type as "WIN"
            }
            else if(line == "LOSE"){
                if(page_type == "LOSE"){
                    findError("The navigation section of the LOSE page has redundant content!");
                }
                setPageType(line); // set type as "LOSE"
            }
            else{
                if(line[0] == '#'){
                    pound_sign = 1;
                    continue;
                }
                addChoice(line);
                if(choices.size() == 1){
                    setPageType("CHOICE"); // set type as "CHOICE"
                }
            }
        }
        else{ // content behind the '#'
            setText(line);
        }
    }
    if(!page.eof()){
        findError("Not reach the end of file!");
    }
    if(empty_file == 1){ // if it's an empty file.
        findError("It's an empty page, illegal input!");
    }
}

/**
 * @brief get the optional page numbers
 * 
 * @return std::vector<size_t> optional page number in container.
 */
std::vector<size_t> Page::getChoices(){
    std:: vector<size_t> res;
    std:: vector<std::pair<std::string, size_t> > :: iterator it;
    for(it = choices.begin(); it != choices.end(); ++it){
        res.push_back(it->second);
    }
    return res;
}

/**
 * @brief get the page number.
 * 
 * @return size_t  pagination
 */
size_t Page::getPageNum(){
    return pagination;
}

/**
 * @brief get the page type.
 * 
 * @return std::string "CHOICE","WIN","LOSE"
 */
std::string Page::getType(){
    return page_type;
}

/**
 * @brief print the page's info.
 * 
 */
void Page::printPage(){
    // 1. print the text of the page
    for(size_t i = 0; i < text.size(); i++){
        std::cout << text[i] << std::endl;
    }

    // 2. Next, print a blank line.
    std::cout << std::endl;
    if(page_type == "CHOICE"){
        // 3. Then print What would you like to do?
        std::cout << "What would you like to do?" << std::endl;

        // 4. Then print another blank line.
        std::cout << std::endl;

        // 5. Then print each possible choice, one per line.
        std:: vector<std::pair<std::string, size_t> >:: iterator it;
        int choice_num = 1;
        for(it = choices.begin(); it != choices.end(); ++it){
            std::cout << " " << choice_num++ << ". " << it->first << std::endl;
        }
    }
    else if(page_type == "WIN"){
        std::cout<< "Congratulations! You have won. Hooray!" << std::endl;
    }
    else if(page_type == "LOSE"){
        std::cout<< "Sorry, you have lost. Better luck next time!" << std::endl;
    }
}
