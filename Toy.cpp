#include <iostream>
#include <string>

#include "Printable.h"
#include "Item.h"
#include "Toy.h"

using namespace std;

// Constructors
Toy::Toy(int numItem, double price, string brand, bool isPerishableItem, string toyType, int minAgeRestriction) : Item(numItem, price, brand, isPerishableItem), toyType(toyType), minAgeRestriction(minAgeRestriction)
{
}

// Getters
string Toy::get_toyType() const { return toyType; }
int Toy::get_minAgeRestriction() const { return minAgeRestriction; }

// Setters
void Toy::set_toyType(string toyType) { this->toyType = toyType; }
void Toy::set_minAgeRestriction(int minAgeRestriction) { this->minAgeRestriction = minAgeRestriction; }

// Methods
void Toy::print() {}
void Toy::change_numItem(int numItem) { this->numItem += numItem; }

// Destructor
Toy::~Toy() {}