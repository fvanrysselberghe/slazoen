#include <iostream>
#include <ctime>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <random>

//! Splits a delimited string at its delimeters
std::vector<std::string> split(const std::string& toSplit, char separator) {
    auto result = std::vector<std::string>{};
    
    auto startIndex = size_t{0};
    auto endIndex   = toSplit.find_first_of(',');
    while (endIndex !=std::string::npos) {
        result.push_back(toSplit.substr(startIndex, endIndex-startIndex));
        startIndex = endIndex + 1;
        endIndex = toSplit.find_first_of(',', startIndex);
    }
    result.push_back(toSplit.substr(startIndex));

    return result;
}

//! Gets the current month number
short getMonthIdx(){
    auto epochNow = std::time(nullptr);
    auto &&now = std::tm{};

    localtime_s(&now,&epochNow);

    return (now.tm_mon + 1);
}

//! Selects a random vegetable
std::string selectVegetable() {
    auto numberOfMonth = getMonthIdx();

    auto calendarStream = std::ifstream{"calendar.lst"};
    if (!calendarStream.good())
        return ""; //optional would be a lot better, but we keep boost out of the picture for the moment 

    auto calendarLine   = std::string{""};
    auto lineNumber     = 0;

    while (std::getline(calendarStream, calendarLine) 
        && lineNumber != numberOfMonth-1) {
            ++lineNumber;
            }

    if (!calendarStream.good())
        return ""; //optional would be better, but we keep boost our of the picture for the moment

    auto vegetables = std::vector<std::string>{};
    vegetables = split(calendarLine, ',');

    if (vegetables.empty())
        return ""; //optional...

    auto numberOfVegetables = vegetables.size();
    auto &&randomDevice = std::random_device{};
    auto randomDistribution = std::uniform_int_distribution<size_t>{0,numberOfVegetables-1};

    return vegetables[randomDistribution(randomDevice)];
}

//! Selects a recipe with the given vegetable
std::string selectRecipe(const std::string &vegetable) {
    //Read the input stream into recipes, pick a recipe
    auto recipesWithVegetable = std::vector<std::string>{};

    auto recipeFile = std::ifstream{"recipes.txt"};
    auto recipeLine = std::string{};

    while (std::getline(recipeFile, recipeLine)) {
        if ( recipeLine.find(vegetable) != std::string::npos)
            recipesWithVegetable.push_back(recipeLine); //some more parsing will be needed
    }

    if (recipesWithVegetable.empty())
        return ""; //optional would be better
    
    if (recipesWithVegetable.size() == 1)
        return recipesWithVegetable.front();

    //pick random from list
    auto &&seed = std::random_device{};
    auto distribution = std::uniform_int_distribution<size_t>{0, recipesWithVegetable.size()-1};

    return recipesWithVegetable[distribution(seed)];
}

//! Main entry point of our code
int main(int argc, const char * argv[]) {
    //read the calendar file
    //pick vegetable in current month
    auto vegetable = selectVegetable();

    //get recipes with vegetable
    //select recipe
    auto recipe = selectRecipe(vegetable);

    std::cout << "Recipe selected: " << recipe.c_str() << std::endl; 
    return 0;
}