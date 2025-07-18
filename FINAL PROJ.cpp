#include <iostream> // For input/output operations
#include <fstream>  // For file handling
#include <queue>    // For using the queue data structure
#include <cstring>  // For C-style string functions like strcpy, strcmp, etc.
#include <limits>   // For numeric limits used in input handling

using namespace std; // Use the standard namespace

// Define a struct to hold the rental information
struct Rental {
    char renterName[100];   // Character array for renter's name
    char phoneModel[50];    // Character array for phone model
    char modelVariant[50];  // Character array for phone variant
    char startDate[11];     // Character array for rental start date in MM/DD/YYYY
    char endDate[11];       // Character array for rental end date in MM/DD/YYYY
    int days;               // Number of rental days
    int totalAmount;        // Total rental cost
};

class RentalServiceSystem {
private:
    queue<Rental> rentals; // Queue to store all rental records

    void getName(char name[]); // Function to get renter name
    void getPhoneModel(char model[], char variant[]); // Function to select phone model and variant
    void getDate(char date[], const string &prompt); // Function to input date with format validation
    int calculateDays(const char start[], const char end[]); // Function to calculate number of days between two dates
    void saveToFile(); // Save queue of rentals to a file
    void loadFromFile(); // Load rental records from a file
    void addRental(); // Add a new rental record
    void displayAll(); // Display all rentals
    void displaySpecificRental(); // Display a specific rental by name or phone model
    void deleteRental(); // Delete a rental record by renter's name
    void displayGroupInfo(); // Display information about the project group
    void showMenu(); // Main menu interface

public:
    void run(); // Public function to start the program
};

// Function to get renter name
void RentalServiceSystem::getName(char name[]) {
    string tempName;
    while (true) {
        cout << "Enter Renter Name (letters and spaces only): ";
        getline(cin, tempName);
        bool valid = !tempName.empty();
        for (char c : tempName) {
            if (!isalpha(c) && c != ' ') {
                valid = false;
                break;
            }
        }
        if (valid) {
            strncpy(name, tempName.c_str(), sizeof(Rental::renterName) - 1);
            name[sizeof(Rental::renterName) - 1] = '\0';
            break;
        }
        cout << "Invalid input! Name must contain only letters and spaces.\n";
    }
}

// Function to select phone model and variant
void RentalServiceSystem::getPhoneModel(char model[], char variant[]) {
    string tempModel, tempVariant;
    while (true) {
        cout << "Select Phone Brand:\n1. iPhone 16\n2. Samsung Galaxy S25\nEnter choice (1 or 2): ";
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            tempModel = "iPhone 16";
            while (true) {
                cout << "Select Variant (base/pro/pro max): ";
                getline(cin, tempVariant);
                if (tempVariant == "base" || tempVariant == "pro" || tempVariant == "pro max") {
                    break;
                }
                cout << "Invalid variant!\n";
            }
            break;
        } else if (choice == "2") {
            tempModel = "Samsung Galaxy S25";
            while (true) {
                cout << "Select Variant (base/plus/ultra): ";
                getline(cin, tempVariant);
                if (tempVariant == "base" || tempVariant == "plus" || tempVariant == "ultra") {
                    break;
                }
                cout << "Invalid variant!\n";
            }
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }
    strncpy(model, tempModel.c_str(), sizeof(Rental::phoneModel) - 1);
    model[sizeof(Rental::phoneModel) - 1] = '\0';
    strncpy(variant, tempVariant.c_str(), sizeof(Rental::modelVariant) - 1);
    variant[sizeof(Rental::modelVariant) - 1] = '\0';
}

// Function to input and validate date
void RentalServiceSystem::getDate(char date[], const string &prompt) {
    string tempDate;
    while (true) {
        cout << prompt;
        getline(cin, tempDate);
        if (tempDate.length() == 10 && tempDate[2] == '/' && tempDate[5] == '/') {
            strncpy(date, tempDate.c_str(), sizeof(Rental::startDate) - 1);
            date[sizeof(Rental::startDate) - 1] = '\0';
            break;
        }
        cout << "Invalid date format! Please use MM/DD/YYYY.\n";
    }
}

// Function to calculate rental days
int RentalServiceSystem::calculateDays(const char start[], const char end[]) {
    int sm, sd, sy, em, ed, ey;
    sscanf(start, "%d/%d/%d", &sm, &sd, &sy);
    sscanf(end, "%d/%d/%d", &em, &ed, &ey);
    int days = (em - sm) * 30 + (ed - sd) + (ey - sy) * 365 + 1;
    return days < 1 ? 1 : days;
}

// Save rentals to file
void RentalServiceSystem::saveToFile() {
    ofstream file("rentals.txt");
    queue<Rental> temp = rentals;
    while (!temp.empty()) {
        Rental r = temp.front();
        temp.pop();
        file << r.renterName << "|" << r.phoneModel << "|" << r.modelVariant << "|"
             << r.startDate << "|" << r.endDate << "|" << r.days << "|" << r.totalAmount << "\n";
    }
    cout << "Records saved successfully!\n";
}

// Load rentals from file
void RentalServiceSystem::loadFromFile() {
    ifstream file("rentals.txt");
    if (!file) return;
    string line;
    while (getline(file, line)) {
        Rental r;
        size_t pos = 0;
        for (int i = 0; i < 6; ++i) {
            size_t next = line.find('|', pos);
            string val = line.substr(pos, next - pos);
            switch (i) {
                case 0: strncpy(r.renterName, val.c_str(), sizeof(r.renterName) - 1); r.renterName[sizeof(r.renterName) - 1] = '\0'; break;
                case 1: strncpy(r.phoneModel, val.c_str(), sizeof(r.phoneModel) - 1); r.phoneModel[sizeof(r.phoneModel) - 1] = '\0'; break;
                case 2: strncpy(r.modelVariant, val.c_str(), sizeof(r.modelVariant) - 1); r.modelVariant[sizeof(r.modelVariant) - 1] = '\0'; break;
                case 3: strncpy(r.startDate, val.c_str(), sizeof(r.startDate) - 1); r.startDate[sizeof(r.startDate) - 1] = '\0'; break;
                case 4: strncpy(r.endDate, val.c_str(), sizeof(r.endDate) - 1); r.endDate[sizeof(r.endDate) - 1] = '\0'; break;
                case 5: r.days = stoi(val); break;
            }
            pos = next + 1;
        }
        r.totalAmount = stoi(line.substr(pos));
        rentals.push(r);
    }
}

// Add a new rental record
void RentalServiceSystem::addRental() {
    Rental r;
    getName(r.renterName);
    getPhoneModel(r.phoneModel, r.modelVariant);
    getDate(r.startDate, "Enter Start Date (MM/DD/YYYY): ");
    getDate(r.endDate, "Enter End Date (MM/DD/YYYY): ");
    r.days = calculateDays(r.startDate, r.endDate);
    r.totalAmount = r.days * 2000;
    cout << "Amount to be paid: " << r.totalAmount << " pesos for " << r.days << " day(s).\n";

    string confirm;
    while (true) {
        cout << "Confirm rental? (yes/no): ";
        getline(cin, confirm);
        if (confirm == "yes") {
            rentals.push(r);
            saveToFile();
            cout << "Rental record added successfully!\n";
            break;
        } else if (confirm == "no") {
            cout << "Rental cancelled. Returning to menu.\n";
            break;
        } else {
            cout << "Please enter 'yes' or 'no'.\n";
        }
    }
}

// Display all rental records
void RentalServiceSystem::displayAll() {
    if (rentals.empty()) {
        cout << "No records found.\n";
        return;
    }
    queue<Rental> temp = rentals;
    while (!temp.empty()) {
        Rental &r = temp.front();
        cout << "Renter: " << r.renterName << " | Phone: " << r.phoneModel << " (" << r.modelVariant << ")"
             << " | Start: " << r.startDate << " | End: " << r.endDate
             << " | Days: " << r.days << " | Amount: " << r.totalAmount << " pesos\n";
        temp.pop();
    }
}

// Display a specific rental by renter name or phone model
void RentalServiceSystem::displaySpecificRental() {
    string searchTermStr;
    cout << "Enter Renter Name or Phone Model to search: ";
    getline(cin, searchTermStr);
    queue<Rental> temp = rentals;
    bool found = false;
    while (!temp.empty()) {
        Rental &r = temp.front();
        if (strcmp(r.renterName, searchTermStr.c_str()) == 0 || strcmp(r.phoneModel, searchTermStr.c_str()) == 0) {
            cout << "Record found:\nRenter: " << r.renterName << "\nPhone: " << r.phoneModel << " (" << r.modelVariant << ")"
                 << "\nStart: " << r.startDate << "\nEnd: " << r.endDate
                 << "\nDays: " << r.days << "\nAmount: " << r.totalAmount << " pesos\n";
            found = true;
        }
        temp.pop();
    }
    if (!found) {
        cout << "No record found with the given information.\n";
    }
}

// Delete a rental by name
void RentalServiceSystem::deleteRental() {
    string nameToDelete;
    cout << "Enter Renter Name to delete: ";
    getline(cin, nameToDelete);
    queue<Rental> updated;
    bool deleted = false;
    while (!rentals.empty()) {
        Rental r = rentals.front();
        rentals.pop();
        if (!deleted && strcmp(r.renterName, nameToDelete.c_str()) == 0) {
            deleted = true;
            continue;
        }
        updated.push(r);
    }
    rentals = updated;
    if (deleted) {
        cout << "Record deleted successfully.\n";
        saveToFile();
    } else {
        cout << "No record found with the given Renter Name.\n";
    }
}

// Display group info
void RentalServiceSystem::displayGroupInfo() {
    cout << "\n===============\n";
    cout << "Project by: Group NAME\n";
    cout << "Members: John Campaner, Raizen Hirato, Cholo Torres, Mac Cayabyab, Ramiel Manlabao, Jeram Matthew\n";
    cout << "===============\n";
}

// Show menu and handle user input
void RentalServiceSystem::showMenu() {
    int choice;
    do {
        cout << "\n===============\nMobile Phone Rental Service\n";
        cout << "1. Add New Rental\n2. Search Rental\n3. Display All Rentals\n4. Display Specific Rental\n5. Delete Rental\n6. Exit\n";
        cout << "===============\nEnter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: addRental(); break;
            case 2: displaySpecificRental(); break;
            case 3: displayAll(); break;
            case 4: displaySpecificRental(); break;
            case 5: deleteRental(); break;
            case 6: cout << "Exiting...\n"; displayGroupInfo(); break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);
}

// Run the system
void RentalServiceSystem::run() {
    loadFromFile(); // Load rentals from file when program starts
    showMenu();     // Show the menu to user for further operations
}

// Main function
int main() {
    RentalServiceSystem rentalSystem; // Create an instance of the rental system class
    rentalSystem.run();               // Run the rental system interface
    return 0;                         // End of program
}
