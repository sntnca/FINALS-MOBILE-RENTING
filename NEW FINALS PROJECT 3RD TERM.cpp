#include <iostream>
#include <fstream>
#include <queue>
#include <cstring> // Required for C-style string functions like strcpy, strncpy, strcmp
#include <limits>  // Required for numeric_limits

// Using the standard namespace to avoid prefixing std::
using namespace std;

// Structure to hold rental information
// All string variables are now character arrays (C-style strings)
struct Rental {
    char renterName[100];   // Character array for renter's name, max 99 chars + null terminator
    char phoneModel[50];    // Character array for phone model, max 49 chars + null terminator
    char modelVariant[50];  // Character array for model variant, max 49 chars + null terminator
    char startDate[11];     // MM/DD/YYYY (10 chars) + null terminator = 11 chars
    char endDate[11];       // MM/DD/YYYY (10 chars) + null terminator = 11 chars
    int days;               // Number of rental days
    int totalAmount;        // Total amount for the rental
};

// Queue to store all rentals
queue<Rental> rentals;

// Function to get and validate renter's name
// Takes a char array as an argument to store the name
void getName(char name[]) {
    string tempName; // Use a temporary std::string for input to leverage getline
    while (true) {
        cout << "Enter Renter Name (letters and spaces only): ";
        getline(cin, tempName); // Read the full line into std::string

        bool valid = !tempName.empty(); // Check if input is not empty
        for (char c : tempName) {
            if (!isalpha(c) && c != ' ') { // Check if characters are letters or spaces
                valid = false;
                break;
            }
        }

        if (valid) {
            // Copy the content from std::string to char array
            // strncpy is used to prevent buffer overflow, copying at most size-1 characters
            strncpy(name, tempName.c_str(), sizeof(Rental::renterName) - 1);
            // Ensure null termination, as strncpy might not null-terminate if source is too long
            name[sizeof(Rental::renterName) - 1] = '\0';
            break;
        }
        cout << "Invalid input! Name must contain only letters and spaces.\n";
    }
}

// Function to select phone model and variant
// Takes char arrays for model and variant
void getPhoneModel(char model[], char variant[]) {
    string tempModel, tempVariant; // Temporary std::strings for input
    while (true) {
        cout << "Select Phone Brand:\n1. iPhone 16\n2. Samsung Galaxy S25\nEnter choice (1 or 2): ";
        string choice;
        getline(cin, choice); // Read choice into std::string

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
    // Copy selected model and variant from temporary std::strings to char arrays
    strncpy(model, tempModel.c_str(), sizeof(Rental::phoneModel) - 1);
    model[sizeof(Rental::phoneModel) - 1] = '\0';
    strncpy(variant, tempVariant.c_str(), sizeof(Rental::modelVariant) - 1);
    variant[sizeof(Rental::modelVariant) - 1] = '\0';
}

// Function to get and validate date input
// Takes a char array for date
void getDate(char date[], const string &prompt) {
    string tempDate; // Temporary std::string for input
    while (true) {
        cout << prompt;
        getline(cin, tempDate); // Read date into std::string
        // Check for MM/DD/YYYY format: length 10, slashes at index 2 and 5
        if (tempDate.length() == 10 && tempDate[2] == '/' && tempDate[5] == '/') {
            // Copy from std::string to char array
            strncpy(date, tempDate.c_str(), sizeof(Rental::startDate) - 1); // Using startDate size as both are 11
            date[sizeof(Rental::startDate) - 1] = '\0';
            break;
        }
        cout << "Invalid date format! Please use MM/DD/YYYY.\n";
    }
}

// Function to calculate number of days between two dates (approximate)
// Now takes const char arrays as arguments
int calculateDays(const char start[], const char end[]) {
    int sm, sd, sy, em, ed, ey;
    // sscanf works directly with C-style strings (char arrays)
    sscanf(start, "%d/%d/%d", &sm, &sd, &sy);
    sscanf(end, "%d/%d/%d", &em, &ed, &ey);
    // Simple approximation for days calculation
    int days = (em - sm) * 30 + (ed - sd) + (ey - sy) * 365 + 1;
    return days < 1 ? 1 : days; // Ensure at least 1 day
}

// Function to save all rentals to a file
void saveToFile() {
    ofstream file("rentals.txt");
    queue<Rental> temp = rentals; // Create a temporary copy of the queue
    while (!temp.empty()) {
        Rental r = temp.front();
        temp.pop();
        // Output char arrays directly to the file stream
        file << r.renterName << "|" << r.phoneModel << "|" << r.modelVariant << "|"
             << r.startDate << "|" << r.endDate << "|" << r.days << "|" << r.totalAmount << "\n";
    }
    cout << "Records saved successfully!\n";
}

// Function to load rentals from a file
void loadFromFile() {
    ifstream file("rentals.txt");
    if (!file) {
        // If file doesn't exist, simply return. No records to load.
        return;
    }
    string line;
    while (getline(file, line)) { // Read each line into a std::string
        Rental r;
        size_t pos = 0;
        // Iterate through the fields separated by '|'
        for (int i = 0; i < 6; ++i) {
            size_t next = line.find('|', pos); // Find the next delimiter
            string val = line.substr(pos, next - pos); // Extract the substring as std::string
            switch (i) {
            case 0: // Renter Name
                strncpy(r.renterName, val.c_str(), sizeof(r.renterName) - 1);
                r.renterName[sizeof(r.renterName) - 1] = '\0';
                break;
            case 1: // Phone Model
                strncpy(r.phoneModel, val.c_str(), sizeof(r.phoneModel) - 1);
                r.phoneModel[sizeof(r.phoneModel) - 1] = '\0';
                break;
            case 2: // Model Variant
                strncpy(r.modelVariant, val.c_str(), sizeof(r.modelVariant) - 1);
                r.modelVariant[sizeof(r.modelVariant) - 1] = '\0';
                break;
            case 3: // Start Date
                strncpy(r.startDate, val.c_str(), sizeof(r.startDate) - 1);
                r.startDate[sizeof(r.startDate) - 1] = '\0';
                break;
            case 4: // End Date
                strncpy(r.endDate, val.c_str(), sizeof(r.endDate) - 1);
                r.endDate[sizeof(r.endDate) - 1] = '\0';
                break;
            case 5: // Days
                r.days = stoi(val); // Convert std::string to int
                break;
            }
            pos = next + 1; // Move past the delimiter
        }
        r.totalAmount = stoi(line.substr(pos)); // Get the last field (totalAmount)
        rentals.push(r); // Add the loaded rental to the queue
    }
}

// Function to add a new rental record
void addRental() {
    Rental r;
    // Call functions to get rental details, passing char arrays
    getName(r.renterName);
    getPhoneModel(r.phoneModel, r.modelVariant);
    getDate(r.startDate, "Enter Start Date (MM/DD/YYYY): ");
    getDate(r.endDate, "Enter End Date (MM/DD/YYYY): ");

    r.days = calculateDays(r.startDate, r.endDate); // Calculate days
    r.totalAmount = r.days * 2000; // Calculate total amount
    cout << "Amount to be paid: " << r.totalAmount << " pesos for " << r.days << " day(s).\n";

    string confirm; // Use std::string for confirmation input
    while (true) {
        cout << "Confirm rental? (yes/no): ";
        getline(cin, confirm);
        if (confirm == "yes") {
            rentals.push(r); // Add to queue
            saveToFile();    // Save all rentals to file
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

// Function to display all rental records
void displayAll() {
    if (rentals.empty()) {
        cout << "No records found.\n";
        return;
    }
    queue<Rental> temp = rentals; // Create a temporary copy to iterate
    while (!temp.empty()) {
        Rental &r = temp.front(); // Get reference to the front element
        // Print details, char arrays are directly printable
        cout << "Renter: " << r.renterName << " | Phone: " << r.phoneModel << " (" << r.modelVariant << ")"
             << " | Start: " << r.startDate << " | End: " << r.endDate
             << " | Days: " << r.days << " | Amount: " << r.totalAmount << " pesos\n";
        temp.pop(); // Remove from temporary queue
    }
}

// Function to search and display a specific rental by renter name or phone model
void displaySpecificRental() {
    string searchTermStr; // Use std::string for search term input
    cout << "Enter Renter Name or Phone Model to search: ";
    getline(cin, searchTermStr);

    queue<Rental> temp = rentals; // Create a temporary copy
    bool found = false;
    while (!temp.empty()) {
        Rental &r = temp.front();
        // Use strcmp for comparing C-style strings with the std::string search term's c_str()
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

// Function to delete a rental record by renter name
void deleteRental() {
    string nameToDelete; // Use std::string for name to delete input
    cout << "Enter Renter Name to delete: ";
    getline(cin, nameToDelete);

    queue<Rental> updated; // A new queue to store records that are not deleted
    bool deleted = false;
    while (!rentals.empty()) {
        Rental r = rentals.front();
        rentals.pop();
        // If not yet deleted and current record's renter name matches, mark as deleted and skip
        if (!deleted && strcmp(r.renterName, nameToDelete.c_str()) == 0) {
            deleted = true;
            continue;
        }
        updated.push(r); // Push non-deleted records to the updated queue
    }
    rentals = updated; // Replace the original queue with the updated one

    if (deleted) {
        cout << "Record deleted successfully.\n";
        saveToFile(); // Save changes to file
    } else {
        cout << "No record found with the given Renter Name.\n";
    }
}

// Function to display group/project information
void displayGroupInfo() {
    cout << "\n===============\n";
    cout << "Project by: Group NAME\n";
    cout << "Members: John Campaner, Raizen Hirato, Cholo Torres, Mac Cayabyab, Ramiel Manlabao, Jeram Matthew\n";
    cout << "===============\n";
}

// Function to show the main menu and handle user choices
void showMenu() {
    int choice;
    do {
        cout << "\n===============\nMobile Phone Rental Service\n";
        cout << "1. Add New Rental\n2. Search Rental\n3. Display All Rentals\n4. Display Specific Rental\n5. Delete Rental\n6. Exit\n";
        cout << "===============\nEnter choice: ";
        cin >> choice;
        // Ignore the rest of the line after reading the integer choice to prevent issues with subsequent getline()
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            addRental();
            break;
        case 2:
            displaySpecificRental();
            break;
        case 3:
            displayAll();
            break;
        case 4:
            displaySpecificRental(); // This case is redundant with case 2, but kept as per original code
            break;
        case 5:
            deleteRental();
            break;
        case 6:
            cout << "Exiting...\n";
            displayGroupInfo();
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6); // Loop until user chooses to exit
}

// Main function: loads data and starts the menu
int main() {
    loadFromFile(); // Load existing rental data from file
    showMenu();     // Display the main menu and start interaction
    return 0;       // Indicate successful execution
}