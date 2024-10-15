#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <limits>
#include <ctime>
#include <vector>
#include <map>

#include "Printable.h"
#include "StoreBase.h"
#include "Store.h"
#include "Suppliant.h"
#include "Item.h"
#include "PerishableItem.h"
#include "Egg.h"
#include "Meat.h"
#include "Milk.h"
#include "Soap.h"
#include "Toy.h"

using namespace std;

int main()
{
    bool restart = false;
    do
    {
        system("clear");

        /* -------------------------
            -= Set-up Suppliant =-
        ------------------------- */
        Suppliant suppliant = Suppliant();
        string *itemsName = new string[10];
        for (int j = 0; j < 10; j++)
        {
            itemsName[j] = suppliant.get_itemNames()[j];
        }

        /* -------------------------
            -= Set-up Store =-
        ------------------------- */
        Store store = Store();
        map<string, Item *> inventory;
        inventory = store.get_inventory();
        // for (auto i = inventory.begin();i!= inventory.end();i++){
        //     cout<<i->first<<endl;
        //     cout<<i->second->get_shelfLifeInDay()<<endl;
        // }

        /* -------------------------------
            -= Introduction Prompt =-
        ------------------------------- */
        string choice;

        cout << "-= Welcome to The Retail Simulator Game =-" << endl;
        cout << "Would you like to see the game introduction? (y/n): ";

        // Error handling for user input
        while (!(cin >> choice) || (choice != "y" && choice != "n"))
        {
            cout << "Invalid input. Please enter (y/n): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == "y")
        {
            cout << "\nWelcome to the Retail Simulator Game!\n"
                 << "In this game, you will manage a store, purchase stock, and keep customers happy.\n"
                 << "Your goal is to maintain profitability and avoid bankruptcy!\n";
        }

        /* -------------------------
          -= Choosing Progression =-
        ------------------------- */
        if (!restart) // If user has previously chosen to start new game
        {
            int gameProgessChoice;

            cout << "Would you like to:\n";
            cout << "\t1. Start a new game\n";
            cout << "\t2. Continue from where you left off\n";
            cout << "Enter your choice (1 or 2): ";

            // Error handling for user input
            while (!(cin >> gameProgessChoice) || (gameProgessChoice != 1 && gameProgessChoice != 2))
            {
                cout << "Invalid input. Please enter '1' to start a new game or '2' to continue: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (gameProgessChoice == 1)
            {
                ofstream MyFile("data.txt"); // Ensure data.txt is available
                MyFile << 0 << endl; // Ensure data.txt has default starting data
                MyFile.close();
            }
            else if (gameProgessChoice == 2)
            {
                /* -------------------------
                -= Read & Use Data.txt File =-
                ------------------------- */
                string haveAccount;
                ifstream ReadFile("data.txt"); // Assign found "data.txt" to variable: ReadFile
                getline(ReadFile, haveAccount);

                if (stoi(haveAccount) == 1)
                {
                    suppliant.updateCost(); // Update costList (Saved -> Made past Day 1 -> Default costList must be change)

                    /* -------------------------
                    -= Set-up Store Status =-
                    ------------------------- */
                    string currentDay;
                    getline(ReadFile, currentDay);
                    store.set_currentDay(stoi(currentDay));

                    string balance;
                    getline(ReadFile, balance);
                    store.set_balance(stod(balance));

                    string rating;
                    getline(ReadFile, rating);
                    store.set_rating(stod(rating));

                    string target;
                    getline(ReadFile, target);
                    store.set_target(stod(target));

                    string numCustomer;
                    getline(ReadFile, numCustomer);
                    store.set_numCustomer(stoi(numCustomer));

                    /* -------------------------
                    -= Set-up Store's Inventory =-
                    ------------------------- */
                    for (int i = 0; i < 10; i++)
                    {
                        string line;
                        string word;
                        vector<string> information;

                        getline(ReadFile, line);
                        stringstream wordsInLine(line);

                        while (wordsInLine >> word)
                        {
                            information.push_back(word);
                        }
                        if (store.get_inventory()[itemsName[stoi(information.at(0))]]->get_isPerishableItem() == false) // For Non-Perishable Item
                        {
                            store.get_inventory()[itemsName[stoi(information.at(0))]]->set_numItem(stoi(information.at(1)));
                        }
                        else // For Perishable Item
                        {

                            /* ----------------------------------------------------
                            -= Set-up Perishable Item's numItem & expirationList =-
                            ---------------------------------------------------- */
                            store.get_inventory()[itemsName[stoi(information.at(0))]]->set_numItem(stoi(information.at(1)));

                            int shelfLifeInDay = store.get_inventory()[itemsName[stoi(information.at(0))]]->get_shelfLifeInDay();
                            int *expirationList = new int[shelfLifeInDay];
                            int index = 0;

                            for (int j = 2; j < information.size(); j++)
                            {
                                expirationList[index] = stoi(information[j]);
                                index++;
                            }
                            store.get_inventory()[itemsName[stoi(information.at(0))]]->set_expirationList(expirationList);

                            delete[] expirationList;
                        }
                    }
                }
                ReadFile.close();
            }
        }

        /* -----------------------------
        -= Set-Up Store Data Variable =-
        ----------------------------- */
        double balance = store.get_balance();
        double rating = store.get_rating();
        double target = store.get_target();
        int currentDay = store.get_currentDay();
        int numCustomer = store.get_numCustomer();

        /* -------------------------
        -= User Store Purchasing =-
        ------------------------- */
        bool running = true;
        while (running)
        {
            srand(time(nullptr)); // Randomize Time
            cout << "\n-------------------------------" << endl;
            cout << "\tWelcome to day " << currentDay << endl;
            cout << "-------------------------------\n"
                 << endl;
            cout << "* Current Balance: $" << balance << endl;
            cout << "* Balance Target: $" << target << endl;
            cout << "-------------------------------\n"
                 << endl;

            double *costList = new double[10];
            for (int j = 0; j < 10; j++)
            {
                costList[j] = suppliant.get_costList()[j];
            }
            cout << "-----> Buying goods for day " << currentDay << " <-----" << endl;
            suppliant.print();

            /* -------------------------
            -= Purchase Input Handling =-
            ------------------------- */
            while (true)
            {
                /* -------------------------
                    -= Choosing Item =-
                ------------------------- */
                int itemNum;
                int itemIndex;
                cout << "\nPlease choose items to buy (1-10) (Enter any other numbers to stop purchase): ";

                while (!(cin >> itemNum)) // Loop until an int input
                {
                    cout << "Invalid input type. Please enter an integer from (1-10): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                if (itemNum < 1 || itemNum > 10) // Stop input request if input number not in range
                {
                    break;
                }
                itemIndex = itemNum - 1;

                /* -------------------------
                -= Choosing Item Amount =-
                ------------------------- */
                int amount;
                cout << "Amount: ";

                while (!(cin >> amount)) // Loop until an int input
                {
                    cout << "Invalid input type. Please enter an integer: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                if (amount * costList[itemIndex] > balance) // Insufficient fund for purchase
                {
                    cout << "Not enough money!!!" << endl;
                }
                else // Add purchase to Store's inventory
                {
                    string itemName = itemsName[itemIndex];
                    map<string, Item *> inventory = store.get_inventory();

                    auto itr = inventory.find(itemName);
                    if (itr != inventory.end()) // Check if itemName is found
                    {
                        itr->second->change_numItem(amount);
                        if (itr->second->get_isPerishableItem())
                        {
                            int expLength = itr->second->get_shelfLifeInDay();
                            int *tmp = new int[expLength];
                            for (int j = 0; j < expLength; j++)
                            {
                                tmp[j] = itr->second->get_expirationList()[j];
                            }
                            tmp[expLength - 1] += amount;
                            itr->second->set_expirationList(tmp);
                            delete[] tmp;
                        }
                        balance -= costList[itemIndex] * amount;
                        cout << "\n* Remaining Balance: $" << balance << " *" << endl;
                    }
                }
            }

            /* ---------------------------------
            -= Show Inventory Post-Purchase =-
            --------------------------------- */
            cout << "\n" << endl;
            store.print();

            /* -------------------------
                    -= Run Day =-
            ------------------------- */

            sleep(1);

            cout << "\n========--------- " << "Simulating Day " << store.get_currentDay() << " ---------========" << endl;

            sleep(2);

            /* -------------------------
            -= Randomise Purchase =-
            ------------------------- */
            for (int i = 0; i < numCustomer; i++)
            {
                int good = rand() % 10;                   // Randomise Good (Index)
                int numGoodBuy = rand() % currentDay + 1; // Randomise Amount of Good to Buy

                cout << "\n----> Customer (" << i + 1 << ") want to buy \"" << numGoodBuy << " " << suppliant.get_itemNames()[good] << "\"" << endl;

                /* ------------------------------------------
                -= Update Inventory & Rating after Purchase =-
                ------------------------------------------ */
                map<string, Item *> inventory = store.get_inventory();
                auto itr = inventory.find(itemsName[good]);

                if (itr != inventory.end()) // Check if good is found in inventory
                {
                    Item *item = itr->second;
                    int availableItems = item->get_numItem();

                    if (availableItems >= numGoodBuy) // If there's enough goods in stock
                    {
                        balance += numGoodBuy * item->get_price();
                        item->change_numItem(-numGoodBuy);
                        item->sellItem(numGoodBuy);
                        rating = min(rating + 0.3, 5.0); // Ensure rating doesn't exceed 5
                    }
                    else // If there's not enough goods in stock
                    {
                        rating -= 0.5 * (numGoodBuy - availableItems); // Rating drop -> 0.5 * amount of unavaliable goods
                        rating = max(rating, 0.0);                     // Ensure rating doesn't drop below 0
                        balance += availableItems * item->get_price();
                        item->sellItem(item->get_numItem()); // Sell all avaliable stock
                        item->set_numItem(0);
                    }
                }

                /* -------------------------
                     -= Day Summary =-
                ------------------------- */
                cout << "\n=======-------- Store Status (End of Day " << store.get_currentDay() << ") --------=======\n"
                     << endl;
                cout << "* Balance: $" << balance << endl;
                cout << "* Rating (Stars): " << rating << endl;
                sleep(2);
            }

            cout << "\n=======-------- Final Verdict --------=======\n"
                 << endl;
            if (balance < target) // Fail due having less money than target
            {
                ofstream MyFile("data.txt");
                MyFile << 0;
                MyFile.close();

                cout << "----> You failed to meet Day " << store.get_currentDay() << " target balance of $" << store.get_target() << "\n----> Your store faces bankruptcy!!!" << endl;
                running = false;
            }
            else if (rating <= 0) // Fail due having bad rating (<=0)
            {
                ofstream MyFile("data.txt");
                MyFile << 0;
                MyFile.close();

                cout << "----> Your store rating reaches 0. Nobody wants to buy from you and you closed down!" << endl;
                running = false;
            }
            else // Survived the day
            {
                cout << "----> You made it through Day " << store.get_currentDay() << endl;
                cout << "\nChose one of the following option to proceed: " << endl;
                cout << "\t1. Play the next day" << endl;
                cout << "\t2. Save and exit" << endl;
                cout << "\t3. Start new game" << endl;
                cout << "\nHow would you like to proceed (Enter 1, 2 or 3): ";

                int choice;
                while (true) // Progression option
                {
                    if (cin >> choice && ((choice == 1 || choice == 2) || (choice == 3)))
                    {
                        if (choice == 3)
                        {
                            ofstream MyFile("data.txt");
                            MyFile << 0 << endl; // Clear data.txt
                            MyFile.close();

                            running = false;
                            restart = true;
                        }

                        break;
                    }

                    cout << "Invalid input type. Please enter an integer (1, 2 or 3): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                /* ----------------------------
                -= Update Suppliant & Store =-
                ---------------------------- */
                suppliant.updateCost();
                store.updateStore();
                store.set_balance(balance);
                store.set_rating(rating);
                delete[] costList;
                currentDay += 1;
                target += currentDay + 5 * (currentDay / 5);

                /* ----------------------------
                -= Save Progress to Data.txt =-
                ---------------------------- */

                if (choice == 2)
                {
                    ofstream MyFile("data.txt");
                    MyFile << 1 << endl;
                    MyFile << store.get_currentDay() << endl;
                    MyFile << store.get_balance() << endl;
                    MyFile << store.get_rating() << endl;
                    MyFile << store.get_target() << endl;
                    MyFile << store.get_numCustomer() << endl;

                    map<string, Item *> inventory = store.get_inventory();

                    /* -----------------------------------
                    -= Save Store Inventory to Data.txt =-
                    ----------------------------------- */
                    for (auto i = inventory.begin(); i != inventory.end(); i++)
                    {
                        if (i->second->get_isPerishableItem()) // Save perishable Item data
                        {
                            int shelfLifeInDay = i->second->get_shelfLifeInDay();
                            int *expirationList = new int[10];
                            for (int j = 0; j < shelfLifeInDay; j++)
                            {
                                expirationList[j] = i->second->get_expirationList()[j];
                            }
                            for (int j = 0; j < 10; j++)
                            {
                                if (itemsName[j] == i->first)
                                {
                                    MyFile << j << " ";
                                    break;
                                }
                            }
                            MyFile << i->second->get_numItem() << " ";
                            for (int j = 0; j < shelfLifeInDay; j++)
                            {
                                MyFile << expirationList[j] << " ";
                            }
                            delete[] expirationList;
                        }
                        else // Save non-perishable Item data
                        {
                            for (int j = 0; j < 10; j++)
                            {
                                if (itemsName[j] == i->first)
                                {
                                    MyFile << j << " ";
                                    break;
                                }
                            }
                            MyFile << i->second->get_numItem();
                        }
                        MyFile << "\n";
                    }
                    MyFile.close();

                    cout << "Game Status Saved successfully!!!" << endl;
                    restart = false;
                    running = false;
                }
            }
        }
        delete[] itemsName;
    } while (restart);

    return 0;
}