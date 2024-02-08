#ifndef EVAL3_PAGE_HPP
#define EVAL3_PAGE_HPP

#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <cctype>
#include <limits>
#include <regex>

// (varName, value)
typedef std::pair<std::string, long int> ChoiceRequirement;
// (destination page, description, requirement)
typedef std::tuple<size_t, std::string, ChoiceRequirement> Choice;
// (page, choice)
typedef std::vector<std::pair<size_t,size_t>> Path;

class Page {
    friend class Story;
private:
    enum Type {
        Unknown = -1,
        Win = 0,
        Lose = 1,
        Normal = 2
    };

    Type type;
    size_t pageNum;
    std::string content;
    std::vector<Choice> choices;
    // (varName, value), variables to set when the page starts
    std::map<std::string, long int> actionList;

    bool isReferenced;
public:
    // Default constructor
    Page() : type(Unknown), pageNum(-1), isReferenced(false) {}

    // Constructor
    Page(int pageNum, Type type, const std::vector<Choice>& choices, std::string content, bool isReferenced)
            : type(type), pageNum(pageNum), content(std::move(content)), choices(choices), isReferenced(isReferenced) {}

    // Copy constructor
    Page(const Page& other) = default;

    // Copy assignment operator

    Page& operator=(const Page& other) = default;
    // Destructor
    ~Page() = default;

    // Move constructor
    Page(Page&& other) = default;

    // Move assignment operator
    Page& operator=(Page&& other) = default;

    // print the content and choices
    void display() const;
};

void Page::display() const {
    std::cout << content << std::endl;
    switch (type) {
        case Page::Normal:
            std::cout << "What would you like to do?" << std::endl;
            std::cout << std::endl;
            break;
        case Page::Win:
            std::cout << "Congratulations! You have won. Hooray!" << std::endl;
            break;
        case Page::Lose:
            std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
            break;
        default:
            std::cerr << "Error: page type is unknown." << std::endl;
            exit(EXIT_FAILURE);
    }
}
#endif