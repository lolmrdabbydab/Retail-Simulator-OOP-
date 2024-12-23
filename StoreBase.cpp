#include "StoreBase.h"

using namespace std;

// Constructors
StoreBase::StoreBase()
{
    this->numDifferentItem = 10;

    Item *item10 = new Soap(0, 13, "Dove", false, "Body Wash");
    this->inventory[itemNames[9]] = item10;

    Item *item9 = new Soap(0, 12, "Dove", false, "Shampoo");
    this->inventory[itemNames[8]] = item9;

    Item *item8 = new Toy(0, 7.7, "Lego", false, "Figure", 2);
    this->inventory[itemNames[7]] = item8;

    Item *item7 = new Toy(0, 7.7, "PlayDough", false, "Sand", 3);
    this->inventory[itemNames[6]] = item7;

    Item *item6 = new Meat(0, 13, "", 3, true, "Beef", "Sirloin");
    this->inventory[itemNames[5]] = item6;

    Item *item5 = new Meat(0, 8.6, "", 3, true, "Chicken", "Wing");
    this->inventory[itemNames[4]] = item5;

    Item *item4 = new Milk(0, 5, "", true, 5, "Almond", "Vegan");
    this->inventory[itemNames[3]] = item4;

    Item *item3 = new Milk(0, 5, "", true, 5, "Cow", "Non-vegan");
    this->inventory[itemNames[2]] = item3;

    Item *item2 = new Egg(0, 6, "", 7, true, true, "Duck");
    this->inventory[itemNames[1]] = item2;

    Item *item1 = new Egg(0, 5.5, "", 7, true, true, "Hens");
    this->inventory[itemNames[0]] = item1;
}

// Getters
int StoreBase::get_numDifferentItem() { return this->numDifferentItem; }
map<string, Item *> StoreBase::get_inventory() { return this->inventory; }
string *StoreBase::get_itemNames() { return itemNames; }

// Setters
void StoreBase::set_numDifferentItem(int n) { this->numDifferentItem = n; }
void StoreBase::set_inventory(map<string, Item *>) {}

// Methods
void StoreBase::change_numDifferentItem(int n) { this->numDifferentItem += n; }
void StoreBase::print() {}

// Destructor
StoreBase::~StoreBase()
{
    for (auto i = inventory.begin(); i != inventory.end(); i++)
    {
        delete i->second;
    }
}