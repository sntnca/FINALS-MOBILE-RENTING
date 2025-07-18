#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "Rental.h"
using namespace std;

queue<Rental> rentals;

void Rental::input() {
    cout << "\nEnter Renter Name: ";
    getline(cin, renterName);
    while (renterName.empty() || !isAlphabetic(renterName)) {
        cout << "Invalid input. Name must contain only letters. Enter Renter Name: ";
        getline(cin, renterName);
    }

    cout << "Enter Phone Model: ";
    getline(cin, phoneModel);
    while (phoneModel.empty() || !isAlphabetic(phoneModel)) {
        cout << "Phone Model must contain only letters. Enter Phone Model: ";
        getline(cin, phoneModel);
    }

    cout << "Enter Phone Variant: ";
    getline(cin, phoneVariant);
    while (phoneVariant.empty() || !isAlphabetic(phoneVariant)) {
        cout << "Phone Variant must contain only letters. Enter Phone Variant: ";
        getline(cin, phoneVariant);
    }

    cout << "Enter Start Date (YYYY-MM-DD): ";
    getline(cin, startDate);
    while (startDate.empty()) {
        cout << "Start Date cannot be empty. Enter Start Date: ";
        getline(cin, startDate);
    }

    cout << "Enter End Date (YYYY-MM-DD): ";
    getline(cin, endDate);
    while (endDate.empty()) {
        cout << "End Date cannot be empty. Enter End Date: ";
        getline(cin, endDate);
    }

    string amountInput;
    while (true) {
        cout << "Enter Total Amount: ";
        getline(cin, amountInput);
        if (isNumeric(amountInput)) {
            totalAmount = atof(amountInput.c_str());
            break;
        } else {
            cout << "Invalid amount. Please enter a valid number.\n";
        }
    }
}

void Rental::display() const {
    cout << "\n--------------------------\n";
    cout << "Renter Name   : " << renterName << endl;
    cout << "Phone Model   : " << phoneModel << endl;
    cout << "Phone Variant : " << phoneVariant << endl;
    cout << "Start Date    : " << startDate << endl;
    cout << "End Date      : " << endDate << endl;
    cout << "Total Amount  : PHP " << totalAmount << endl;
}

void Rental::writeToFile(ofstream &out) const {
    out << renterName << endl;
    out << phoneModel << endl;
    out << phoneVariant << endl;
    out << startDate << endl;
    out << endDate << endl;
    out << totalAmount << endl;
}

bool Rental::readFromFile(ifstream &in) {
    if (!getline(in, renterName)) return false;
    if (!getline(in, phoneModel)) return false;
    if (!getline(in, phoneVariant)) return false;
    if (!getline(in, startDate)) return false;
    if (!getline(in, endDate)) return false;
    string amountStr;
    if (!getline(in, amountStr)) return false;
    totalAmount = atof(amountStr.c_str());
    return true;
}

void saveToFile() {
    ofstream out("rentals.txt");
    queue<Rental> temp = rentals;
    while (!temp.empty()) {
        temp.front().writeToFile(out);
        temp.pop();
    }
    out.close();
}

void loadFromFile() {
    ifstream in("rentals.txt");
    Rental r;
    while (r.readFromFile(in)) {
        rentals.push(r);
    }
    in.close();
}

void addRental() {
    Rental r;
    r.input();
    rentals.push(r);
    saveToFile();
    cout << "Rental added successfully!\n";
}

void displayRentals() {
    if (rentals.empty()) {
        cout << "\nNo rentals to display.\n";
        return;
    }
    queue<Rental> temp = rentals;
    while (!temp.empty()) {
        temp.front().display();
        temp.pop();
    }
}

void deleteRental() {
    string name;
    cout << "\nEnter Renter Name to delete: ";
    getline(cin, name);
    queue<Rental> temp;
    bool found = false;

    while (!rentals.empty()) {
        if (rentals.front().renterName != name) {
            temp.push(rentals.front());
        } else {
            found = true;
        }
        rentals.pop();
    }
    rentals = temp;
    saveToFile();

    if (found)
        cout << "Rental deleted successfully.\n";
    else
        cout << "Rental not found.\n";
}

void bubbleSortAndDisplay() {
    int size = rentals.size();
    if (size <= 1) {
        displayRentals();
        return;
    }

    Rental* arr = new Rental[size];
    queue<Rental> temp = rentals;
    for (int i = 0; i < size; ++i) {
        arr[i] = temp.front();
        temp.pop();
    }

    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j].renterName > arr[j + 1].renterName) {
                Rental temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        arr[i].display();
    }
    delete[] arr;
}

void displayGroupInfo() {
    cout << "\n========================================\n";
    cout << "         GROUP INFORMATION\n";
    cout << "========================================\n";
    cout << " Group Name : RENT NOW CREW\n";
    cout << " Course     : BSIT 2-B\n";
    cout << " Members    :\n";
    cout << "  - Ramiel Manlabao\n";
    cout << "  - Jeram Matthew De Jesus\n";
    cout << "  - Mac Glenvere Cayabyab\n";
    cout << "  - Raizen Hirato\n";
    cout << "  - Cholo Torres\n";
    cout << "  - Clarence Joaquin Evasco\n";
    cout << "  - John Gabriel Campaner\n";
    cout << "========================================\n\n";
}

void showMenu() {
    int choice;
    string input;
    do {
        cout << "\n==== PHONE RENTAL SYSTEM ====\n";
        cout << "1. Add Rental\n";
        cout << "2. Display All Rentals\n";
        cout << "3. Display Sorted Rentals (By Renter Name)\n";
        cout << "4. Delete Rental\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        getline(cin, input);

        if (input.length() == 1 && isdigit(input[0]))
            choice = input[0] - '0';
        else
            choice = -1;

        switch (choice) {
            case 1: addRental(); break;
            case 2: displayRentals(); break;
            case 3: bubbleSortAndDisplay(); break;
            case 4: deleteRental(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

int main() {
    displayGroupInfo();
    loadFromFile();
    showMenu();
    return 0;
}
