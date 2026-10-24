
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Friend {
    string name;
    double paid = 0;
};

struct Expense {
    string description;
    string category;
    string paidBy;
    double amount;
};

vector<Friend> friends;
vector<Expense> expenses;

int findFriend(string name) {
    for (int i = 0; i < friends.size(); i++) {
        if (friends[i].name == name)
            return i;
    }
    return -1;
}

void addFriends() {
    int n;
    cout << "How many friends are sharing expenses? ";
    cin >> n;
    cin.ignore();

    if (n < 2) {
        cout << "At least two friends are required.\n";
        return;
    }

    for (int i = 0; i < n; i++) {
        string name;
        cout << "Enter friend " << i + 1 << "'s name: ";
        getline(cin, name);

        if (findFriend(name) != -1) {
            cout << "This name already exists. Try again.\n";
            i--;
            continue;
        }

        friends.push_back({name, 0});
    }

    cout << "Friends added successfully!\n";
}

void addExpense() {
    if (friends.size() < 2) {
        cout << "Please add friends first.\n";
        return;
    }

    Expense e;

    cout << "Enter expense description: ";
    getline(cin >> ws, e.description);

    cout << "Enter category (Groceries/Utensils/Rent/etc.): ";
    getline(cin, e.category);

    cout << "Enter amount: ";
    cin >> e.amount;

    if (e.amount <= 0) {
        cout << "Invalid amount.\n";
        return;
    }

    cout << "Enter the name of the friend who paid: ";
    getline(cin >> ws, e.paidBy);

    int index = findFriend(e.paidBy);

    if (index == -1) {
        cout << "Friend not found. Expense not added.\n";
        return;
    }

    friends[index].paid += e.amount;
    expenses.push_back(e);

    cout << "Expense added successfully!\n";
}

void showExpenses() {
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    double total = 0;

    cout << "\n========== EXPENSE HISTORY ==========\n";
    cout << left << setw(20) << "Description"
         << setw(15) << "Category"
         << setw(15) << "Paid By"
         << right << setw(12) << "Amount\n";

    for (const auto &e : expenses) {
        cout << left << setw(20) << e.description
             << setw(15) << e.category
             << setw(15) << e.paidBy
             << right << setw(12) << fixed
             << setprecision(2) << e.amount << "\n";

        total += e.amount;
    }

    cout << "\nTotal expenses: Rs. "
         << fixed << setprecision(2) << total << "\n";
}

void monthlySummary() {
    if (friends.size() < 2 || expenses.empty()) {
        cout << "Add friends and expenses first.\n";
        return;
    }

    double total = 0;

    for (const auto &e : expenses)
        total += e.amount;

    double share = total / friends.size();

    cout << "\n========== MONTHLY SUMMARY ==========\n";
    cout << "Total expenses: Rs. " << fixed
         << setprecision(2) << total << "\n";

    cout << "Number of friends: " << friends.size() << "\n";
    cout << "Equal share per friend: Rs. " << share << "\n\n";

    cout << left << setw(20) << "Friend"
         << right << setw(15) << "Paid"
         << setw(15) << "Fair Share"
         << setw(15) << "Balance\n";

    vector<double> balance(friends.size());

    for (int i = 0; i < friends.size(); i++) {
        balance[i] = friends[i].paid - share;

        cout << left << setw(20) << friends[i].name
             << right << setw(15) << friends[i].paid
             << setw(15) << share
             << setw(15) << balance[i] << "\n";
    }

    cout << "\nPositive balance = should receive money\n";
    cout << "Negative balance = should pay money\n";

    // Settle payments between friends.
    cout << "\n========== FINAL SETTLEMENT ==========\n";

    vector<double> remaining = balance;
    double epsilon = 0.005;
    bool paymentFound = false;

    for (int i = 0; i < friends.size(); i++) {
        if (remaining[i] >= -epsilon)
            continue;

        for (int j = 0; j < friends.size(); j++) {
            if (remaining[j] <= epsilon)
                continue;

            double payment = min(-remaining[i], remaining[j]);

            if (payment > epsilon) {
                cout << friends[i].name << " pays "
                     << friends[j].name << " Rs. "
                     << fixed << setprecision(2)
                     << payment << "\n";

                remaining[i] += payment;
                remaining[j] -= payment;
                paymentFound = true;
            }

            if (remaining[i] >= -epsilon)
                break;
        }
    }

    if (!paymentFound)
        cout << "All expenses are already balanced.\n";
}

int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "    MONTHLY EXPENSE MANAGEMENT\n";
        cout << "====================================\n";
        cout << "1. Add Friends\n";
        cout << "2. Add Expense\n";
        cout << "3. View All Expenses\n";
        cout << "4. Monthly Summary\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting program.\n";
            break;
        }

        cin.ignore();

        switch (choice) {
            case 1:
                addFriends();
                break;

            case 2:
                addExpense();
                break;

            case 3:
                showExpenses();
                break;

            case 4:
                monthlySummary();
                break;

            case 5:
                cout << "Thank you for using the application!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}
