#ifndef EVAL3_STORY_HPP
#define EVAL3_STORY_HPP

#include "page.hpp"

// (type, seperated strings)
typedef std::pair<int, std::vector<std::string>> MatchResult;

class Story {
private:
    std::string directoryPath;
    std::vector<Page> pageList;
    // (page, choice)
    std::vector<Path> winningPaths;

    // add a page, for parseLine
    void addPage(const std::string& pagNum, const std::string& type, const std::string& content);

    // add a choice, for parseLine
    void addChoice(const std::string& srcPage, const std::string& dstPage, const std::string& description, const ChoiceRequirement& requirement);

    // add an action when page begins, for parseLine
    void addAction(const std::string& pagNum, const ChoiceRequirement& action);

    // identify the type of the string, for parseLine
    static MatchResult matchString(const std::string& input) ;

    // parse one line from story.txt
    void parseLine(const std::string& line);

    // dfs algorithm for step 3
    void dfs(size_t, Path);

    // return a Page pointer
    Page* findPage(size_t pageNum);

public:
    // Default constructor
    Story()= default;

    // Constructor
    explicit Story(std::string & _directoryPath);

    // Copy constructor
    Story(const Story& other) = default;

    // Copy assignment operator
    Story& operator=(const Story& other) = default;

    // Move constructor
    Story(Story&& other) noexcept = default;

    // Move assignment operator
    Story& operator=(Story&& other) noexcept = default;

    // Destructor
    ~Story() = default;

    // print pages for step 1
    void printPages() const;

    // check pages for step 2
    void checkPages();

    // story begins
    void begin();

    // print all the winning paths for step 3
    void winPath();
};

Story::Story(std::string & _directoryPath): directoryPath(_directoryPath){

    std::string storyFilePath = directoryPath + "/story.txt";
    std::ifstream storyFile(storyFilePath);

    if (!storyFile.is_open()) {
        std::cerr << "Error: could not open storyFile at " << storyFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string tempLine;

    while (!storyFile.eof()) {
        std::getline(storyFile, tempLine);
        if (tempLine.empty()) {
            continue;
        } else {
            parseLine(tempLine);
        }
    }

    storyFile.close();
}

void Story::addPage(const std::string& pagNum, const std::string& type, const std::string& content) {
    Page tempPage;

    // check the number
    try {
        tempPage.pageNum = std::stoul(pagNum);
    }catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "cannot convert string to size_t correctly.";
        throw std::runtime_error(oss.str());
    }

    // check the order
    if (!pageList.empty()) {
        if (tempPage.pageNum != pageList.back().pageNum + 1){
            std::ostringstream oss;
            oss << "page declarations must appear in order.";
            throw std::runtime_error(oss.str());
        }
    }

    // identify the type of the file
    if (type == "N"){
        tempPage.type = Page::Normal;
    } else if (type == "W"){
        tempPage.type = Page::Win;
    } else if (type == "L"){
        tempPage.type = Page::Lose;
    }

    // open the file
    std::ifstream file(directoryPath + "/" + content);
    if (!file.is_open()) {
        std::ostringstream oss;
        oss << "unable to open file " << directoryPath + "/" + content << std::endl;
        throw std::runtime_error(oss.str());
    }

    // read the file and save to content
    std::stringstream buffer;
    buffer << file.rdbuf();
    tempPage.content = buffer.str();
    file.close();

    // add to page list
    pageList.push_back(tempPage);
}

void Story::addChoice(const std::string& srcPage, const std::string& dstPage, const std::string& description, const ChoiceRequirement & requirement){
    size_t srcPageNum, dstPageNum;
    // check the page number
    try {
        srcPageNum = std::stoul(srcPage);
        dstPageNum = std::stoul(dstPage);
    }catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "cannot convert string to size_t correctly.";
        throw std::runtime_error(oss.str());
    }

    // check if the page is already declared
    Page* foundPage = findPage(srcPageNum);
    if (foundPage == nullptr){
        std::ostringstream oss;
        oss << "page " << srcPageNum << " must be declared before use." << std::endl;
        throw std::runtime_error(oss.str());
    }

    // check the page type
    if (foundPage->type == Page::Win or foundPage->type == Page::Lose){
        std::ostringstream oss;
        oss << "Win and Lose pages MUST NOT have any choices"<< std::endl;
        throw std::runtime_error(oss.str());
    }

    // check whether this choice already exists
    for (const auto& choice : foundPage->choices) {
        if (std::get<0>(choice) == dstPageNum) {
            std::ostringstream oss;
            oss << "choice " << srcPageNum << ":" << dstPageNum << " already exists." << std::endl;
            throw std::runtime_error(oss.str());
        }
    }

    // add the choice to the corresponding page in pageList
    foundPage->choices.emplace_back(dstPageNum, description, requirement);
}

void Story::addAction(const std::string& pagNum, const ChoiceRequirement& action){
    size_t pageNum;

    // check the number
    try {
        pageNum = std::stoul(pagNum);
    }catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "cannot convert string to size_t correctly.";
        throw std::runtime_error(oss.str());
    }

    // check if the page is already declared
    Page* foundPage = findPage(pageNum);
    if (foundPage == nullptr){
        std::ostringstream oss;
        oss << "page " << pageNum << " must be declared before use." << std::endl;
        throw std::runtime_error(oss.str());
    }

    // check if the action is already in the list
    if (foundPage->actionList.find(action.first) != foundPage->actionList.end()) {
        std::ostringstream oss;
        oss << "variable " << action.first << " already exists in the action list of " << pageNum << std::endl;
        throw std::runtime_error(oss.str());
    }

    foundPage->actionList[action.first] = action.second;
}


MatchResult Story::matchString(const std::string& input){
    // 4 types:
    // 1: 0@N:page0.txt
    // 2: 1:4:Leave the store
    // 3: 8[sword=1]:11:Charge the dragon with your sword.
    // 4: 4$pastry=1
    MatchResult result;
    size_t index = 0;

    while (index < input.length() and std::isdigit(input[index])) {
        index++;
    }

    // it is empty or starts with a non-digit or all the char are digits
    if (index >= input.length() - 1 or index == 0) {
        return result;
    }

    // digit part
    result.second.push_back(input.substr(0, index));

    std::string restPart = input.substr(index+1);
    switch (input[index]) {
        // 1: 0@N:page0.txt
        case '@':{
            size_t colonPos = restPart.find(':');
            if (colonPos == 1) {
                result.first = 1;
                // N/W/L
                result.second.push_back(restPart.substr(0, 1));
                // string
                result.second.push_back(restPart.substr(2));
            }
            break;
        }
        // 2: 1:4:Leave the store
        case ':':{
            size_t colonPos = restPart.find(':');
            if (colonPos != std::string::npos){
                result.first = 2;
                // num
                result.second.push_back(restPart.substr(0, colonPos));
                // string
                result.second.push_back(restPart.substr(colonPos + 1));
            }
            break;
        }
        // 3: 8[sword=1]:11:Charge the dragon with your sword.
        case '[':{
            size_t firstColonPos = restPart.find(':');
            size_t secondColonPos = restPart.find(':', firstColonPos + 1);
            size_t rightBracketPos = restPart.find(']');
            size_t equalsPos = restPart.find('=');
            if (firstColonPos != std::string::npos and secondColonPos != std::string::npos and rightBracketPos == firstColonPos - 1 and equalsPos < rightBracketPos ){
                result.first = 3;
                // string
                result.second.push_back(restPart.substr(0, equalsPos));
                // num
                result.second.push_back(restPart.substr(equalsPos + 1, rightBracketPos - equalsPos - 1));
                // num
                result.second.push_back(restPart.substr(firstColonPos + 1, secondColonPos - firstColonPos - 1));
                // string
                result.second.push_back(restPart.substr(secondColonPos + 1));
            }
            break;
        }

        // 4: 4$pastry=1
        case '$':{
            size_t equalsPos = restPart.find('=');
            if (equalsPos != std::string::npos and equalsPos != 0) {
                result.first = 4;
                // string
                result.second.push_back(restPart.substr(0, equalsPos));
                // num
                result.second.push_back(restPart.substr(equalsPos + 1));
            }
            break;
        }

        default:{}
    }

    return result;
}

void Story::parseLine(const std::string& line) {
    MatchResult matchResult = matchString(line);

    switch (matchResult.first) {
        case 1:{
            // 0:3:Purchase a muffin.
            addPage(matchResult.second[0],matchResult.second[1],matchResult.second[2]);
            break;
        }

        case 2:{
            // 1:4:Leave the store
            ChoiceRequirement noRequirement; // string == "", int = 0
            addChoice(matchResult.second[0],matchResult.second[1],matchResult.second[2], noRequirement);
            break;
        }

        case 3:{
            // 8[sword=1]:11:Charge the dragon with your sword.
            long int varValue;
            try {
                varValue = std::stol(matchResult.second[2]);
            }catch (const std::exception& e) {
                std::ostringstream oss;
                oss << "cannot convert string to long int correctly.";
                throw std::runtime_error(oss.str());
            }
            addChoice(matchResult.second[0],matchResult.second[3],matchResult.second[4], ChoiceRequirement(matchResult.second[1], varValue));
            break;
        }

        case 4:{
            // 4$pastry=1
            long int varValue;
            try {
                varValue = std::stol(matchResult.second[2]);
            }catch (const std::exception& e) {
                std::ostringstream oss;
                oss << "cannot convert string to long int correctly.";
                throw std::runtime_error(oss.str());
            }

            // add an action
            addAction(matchResult.second[0], ChoiceRequirement(matchResult.second[1], varValue));
            break;
        }

        default:{
            std::ostringstream oss;
            oss << "unable to parse line \"" << line << "\"" << std::endl;
            throw std::runtime_error(oss.str());
        }
    }
}

void Story::printPages() const {
    for (auto & page : pageList) {
        std::cout << "Page " << page.pageNum << std::endl;
        std::cout << "==========" << std::endl;
        // display content
        page.display();

        // display choices
        size_t choiceCount = 1;
        for (auto & choice : page.choices) {
            std::cout << " " << choiceCount << ". " << std::get<1>(choice) << std::endl;
            choiceCount++;
        }
    }
}

Page* Story::findPage(size_t pageNum) {
    if (pageNum > pageList.size() - 1){
        return nullptr;
    }
    return &pageList[pageNum];
}

void Story::checkPages() {
    // check the beginning page
    if (pageList.empty() or pageList.front().pageNum != 0){
        std::ostringstream oss;
        oss << "the beginning page does not exist.";
        throw std::runtime_error(oss.str());
    }

    // check whether every referenced page exists
    for (const auto & page : pageList) {
        for(const auto & choice : page.choices){
            Page* foundPage = findPage(std::get<0>(choice));
            if (foundPage == nullptr){
                std::ostringstream oss;
                oss << "can not find referenced page " << std::get<0>(choice) << ".";
                throw std::runtime_error(oss.str());
            } else{
                foundPage->isReferenced = true;
            }
        }
    }

    // check whether all pages are referenced
    bool winExists = false;
    bool loseExists = false;
    for (const auto & page : pageList) {
        if (page.type == Page::Win){
            winExists = true;
        }
        if (page.type == Page::Lose){
            loseExists = true;
        }
        if (!page.isReferenced and page.pageNum != 0){
            std::ostringstream oss;
            oss << "page " << page.pageNum << " is not referenced by others.";
            throw std::runtime_error(oss.str());
        }
    }

    // check whether win and lose page exists
    if (!winExists or !loseExists){
        std::ostringstream oss;
        oss << "win or lose page does not exist.";
        throw std::runtime_error(oss.str());
    }
}

void Story::begin() {
    // (varName, value), hold the variables when story begins for step 4
    std::map<std::string, long int> varList;
    varList[""] = 0; // for those who has no requirement

    // begin the story
    Page* currentPage = &pageList.front();
    while(currentPage->type != Page::Win and currentPage->type != Page::Lose){
        // display content
        currentPage->display();

        // set variables
        for (const auto& var : currentPage->actionList) {
            varList[var.first] = var.second;
        }

        // unavailable choice list
        std::vector<size_t> unavailableChoices;

        // display choices
        size_t choiceCount = 1;
        for (auto & choice : currentPage->choices) {

            ChoiceRequirement requirement = std::get<2>(choice);
            // check the var list, if the var does not exist, [] method will automatically add it and initialize the value to be 0
            if (varList[requirement.first] == requirement.second) {
                // if the var fulfill the requirement or there is no requirement
                std::cout << " " << choiceCount << ". " << std::get<1>(choice) << std::endl;
            } else{
                // if the var does not fulfill the requirement
                unavailableChoices.push_back(choiceCount);
                std::cout << " " << choiceCount << ". <UNAVAILABLE>" << std::endl;
            }

            choiceCount++;
        }

        // handle input
        size_t inputChoice;
        // if input <= 0  or > size, or it is in the unavailableChoices
        while (true) {
            if (!(std::cin >> inputChoice) or inputChoice <= 0 or inputChoice > currentPage->choices.size()) {
                std::cout << "That is not a valid choice, please try again" << std::endl;
            } else if (std::find(unavailableChoices.begin(), unavailableChoices.end(), inputChoice) != unavailableChoices.end()) {
                std::cout << "That choice is not available at this time, please try again" << std::endl;
            } else{
                // correct input
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        currentPage = findPage(std::get<0>(currentPage->choices[inputChoice - 1]));
    }
    currentPage->display();
}

void Story::dfs(size_t currentPageNum, Path visitedPath) {
    Page& current = *findPage(currentPageNum);
    visitedPath.emplace_back(currentPageNum, 0);

    // losing end
    if (current.type == Page::Lose){
        return;
    }

    // winning end
    if (current.type == Page::Win){
        winningPaths.push_back(visitedPath);
        return;
    }

    // try all the choices
    for (size_t i = 0; i < current.choices.size(); ++i) {
        size_t nextNum = std::get<0>(current.choices[i]);

        bool didVisit = false;
        for (const auto& visited : visitedPath){
            if (nextNum == visited.first){
                didVisit = true;
                break;
            }
        }

        // if didn't visit the destination of the choice
        if (!didVisit){
            visitedPath.back().second = i+1;
            dfs(nextNum, visitedPath);
        }
    }
}

void Story::winPath(){
    if (winningPaths.empty()){
        Path currentPath;
        dfs(pageList.front().pageNum, currentPath);
        if(winningPaths.empty()){
            std::cout << "This story is unwinnable!" << std::endl;
        }
    }

    // print all paths
    for (const auto& path : winningPaths) {
        for (const auto& node : path) {
            if(node != path.back())
                std::cout << node.first << "(" << node.second << "),";
            else
                std::cout << node.first << "(win)" << std::endl;
        }
    }
}

#endif //EVAL3_STORY_HPP
