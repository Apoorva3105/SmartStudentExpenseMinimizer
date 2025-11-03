#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include <limits>

using namespace std;

struct Expense {
    int expenseId;
    string date;
    double amount;
    string category;
    string description;
};

struct CompareExpenseByDate {
    bool operator()(const Expense& a, const Expense& b) const {
        if (a.date != b.date) {
            return a.date < b.date;
        }
        return a.expenseId < b.expenseId;
    }
};

class ExpenseTracker {
private:
    map<int, Expense> allExpenses;
    // 
    map<string, vector<Expense>> expensesByCategory;
    set<Expense, CompareExpenseByDate> expensesByDate;
    map<string, map<string, double>> priceDatabase;

    int nextExpenseId = 1;

public:
    void addExpense(string date, double amount, string category, string description) {
        int newId = nextExpenseId++;
        Expense newExpense = {newId, date, amount, category, description};

        allExpenses[newId] = newExpense;
        expensesByCategory[category].push_back(newExpense);
        expensesByDate.insert(newExpense);

        cout << "\nSuccess: Added expense '" << description << "'." << endl;
    }

    void addPrice(string itemName, string storeName, double price) {
        priceDatabase[itemName][storeName] = price;
        cout << "\nSuccess: Added price for '" << itemName << "' at '" << storeName << "'." << endl;
    }

    void printExpensesByCategory(string category) {
        cout << "\n--- Expenses for Category: " << category << " ---" << endl;
        if (expensesByCategory.find(category) == expensesByCategory.end() || expensesByCategory[category].empty()) {
            cout << "No expenses found for this category." << endl;
            return;
        }

        vector<Expense>& expenses = expensesByCategory[category];
        for (const auto& expense : expenses) {
            cout << fixed << setprecision(2);
            cout << expense.date << " | " << "Rs. " << setw(7) << expense.amount
                 << " | " << expense.description << endl;
        }
    }

    void printAllExpensesByDate() {
        cout << "\n--- All Expenses (Sorted by Date) ---" << endl;
        if (expensesByDate.empty()) {
            cout << "No expenses to show." << endl;
            return;
        }
        for (const auto& expense : expensesByDate) {
            cout << fixed << setprecision(2);
            cout << expense.date << " | " << setw(10) << left << expense.category << " | "
                 << "Rs. " << setw(7) << right << expense.amount << " | "
                 << expense.description << endl;
        }
    }

    void findCheapestStore(string itemName) {
        cout << "\n--- Price Check for: " << itemName << " ---" << endl;
        if (priceDatabase.find(itemName) == priceDatabase.end()) {
            cout << "Sorry, no price data found for '" << itemName << "'." << endl;
            return;
        }

        map<string, double>& stores = priceDatabase[itemName];
        
        string cheapestStore = "";
        double minPrice = -1.0;

        for (const auto& pair : stores) {
            string storeName = pair.first;
            double price = pair.second;
            cout << "  - " << setw(15) << left << storeName << ": Rs. " << price << endl;

            if (minPrice == -1.0 || price < minPrice) {
                minPrice = price;
                cheapestStore = storeName;
            }
        }

        cout << "==> The cheapest store is '" << cheapestStore 
             << "' at Rs. " << minPrice << " <==" << endl;
    }

    void printCategorySummary() {
        cout << "\n--- Spending Summary by Category ---" << endl;
        if (expensesByCategory.empty()) {
            cout << "No expenses to summarize." << endl;
            return;
        }

        double totalSpent = 0.0;
        
        for (auto const& pair : expensesByCategory) {
            string category = pair.first;
            const vector<Expense>& expenses = pair.second;

            double categoryTotal = 0.0;
            for (const auto& expense : expenses) {
                categoryTotal += expense.amount;
            }
            cout << "  - " << setw(12) << left << category 
                 << ": Rs. " << fixed << setprecision(2) << categoryTotal << endl;
            totalSpent += categoryTotal;
        }
        cout << "--------------------------------------" << endl;
        cout << "  - " << setw(12) << left << "TOTAL" 
             << ": Rs. " << fixed << setprecision(2) << totalSpent << endl;
    }
};

void printMenu() {
    cout << "\n Smart Student Expense Minimizer " << endl;
    cout << "1. Add New Expense" << endl;
    cout << "2. Add Price Data (for Minimizer)" << endl;
    cout << "3. Find Cheapest Store for an Item" << endl;
    cout << "4. Show Expenses by Category" << endl;
    cout << "5. Show All Expenses (Sorted by Date)" << endl;
    cout << "6. Show Spending Summary" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
}


int main() {
    ExpenseTracker myTracker;
    
    cout << "Welcome to the Smart Student Expense Minimizer!" << endl;

    int choice;
    while (true) {
        printMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 7) {
            cout << "Goodbye!" << endl;
            break;
        }

        switch (choice) {
            case 1: {
                string date, category, description;
                double amount;

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                
                cout << "Enter amount: ";
                cin >> amount;
                while(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid amount. Please enter a number: ";
                    cin >> amount;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

                cout << "Enter category (e.g., Food, Transport, Books): ";
                getline(cin, category);
                
                cout << "Enter description: ";
                getline(cin, description);
                
                myTracker.addExpense(date, amount, category, description);
                break;
            }
            case 2: {
                string item, store;
                double price;

                cout << "Enter item name (e.g., Milk 1L): ";
                getline(cin, item);
                
                cout << "Enter store name: ";
                getline(cin, store);
                
                cout << "Enter price: ";
                cin >> price;
                while(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid price. Please enter a number: ";
                    cin >> price;
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                myTracker.addPrice(item, store, price);
                break;
            }
            case 3: {
                string item;
                cout << "Enter item name to check: ";
                getline(cin, item);
                myTracker.findCheapestStore(item);
                break;
            }
            case 4: {
                string category;
                cout << "Enter category to show: ";
                getline(cin, category);
                myTracker.printExpensesByCategory(category);
                break;
            }
            case 5: {
                myTracker.printAllExpensesByDate();
                break;
            }
            case 6: {
                myTracker.printCategorySummary();
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }

    return 0;
}