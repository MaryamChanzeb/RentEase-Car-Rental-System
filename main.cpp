#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>            // For date/time management
#include <cstdlib>           // For exit() functions
#include <limits>             // For stream limits 
#include <sstream>            // For building strings 
#include <algorithm>          // For remove_if
using namespace std;

//  function to clear console screen 
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// function to pause and wait for user to press Enter
void waitForEnter(const string& message = "Press Enter to continue...") {
    cout << "\n" << message;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// function to print a consistent header for menus or screens
void printHeader(const string& title) {
    clearScreen();
    cout << "\n==================================================\n";
    cout << "\t\t" << title << "\n";
    cout << "==================================================\n\n";
}

// Function declarations 
void adminMenu();
void customerMenu();
void MainMenu();
void adminPassCheck();
void customerControlMenu();
void checkOverdueCars();
void generateCarFile();
void showCustomerListSimple();


class Car {                 // class car
private:
    string carID, carModel, carBrand, carColor, availableUnits, rentalRate;
    fstream file;  // For file I/O operations

public:
    // Add a new car 
    void addCar() {
        printHeader("NEW CAR ENTRY");
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        cout << "Enter the Car ID: ";
        getline(cin, carID);
        cout << "Enter the Car Model: ";
        getline(cin, carModel);
        cout << "Enter the Car Brand: ";
        getline(cin, carBrand);
        cout << "Enter the Car Color: ";
        getline(cin, carColor);

        // Validate available units input
        while (true) {
            cout << "Enter available units: ";
            getline(cin, availableUnits);

            int units;
            try {
                units = stoi(availableUnits);
            }
            catch (...) {
                cout << "\n[Error] Invalid input. Please enter a numeric value.\n";
                continue;
            }

            if (units <= 0) {
                cout << "\n[Error] Available units must be greater than zero. Please enter again.\n";
            }
            else {
                break; // Valid units entered
            }
        }

        // Validate Rental Rate input
        while (true) {
            cout << "Enter Rental Rate per day (Rs.): ";
            getline(cin, rentalRate);

            int rate;
            try {
                rate = stoi(rentalRate);
            }
            catch (...) {
                cout << "\n[Error] Invalid input. Please enter a numeric value.\n";
                continue;
            }

            if (rate <= 0) {
                cout << "\n[Error] Rental rate must be greater than zero. Please enter again.\n";
            }
            else {
                break; // Valid rental rate entered
            }
        }

        file.open("carRecords.txt", ios::out | ios::app);
        if (!file) {
            cout << "\n[Error] Failed to open carRecords.txt for writing.\n";
            waitForEnter();
            return;
        }
        // Write data separated by '*' delimiter
        file << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;
        file.close();

        cout << "\n[Success] Car Added Successfully!\n";
        waitForEnter();
    }

    // Display all cars
    void showCars(bool isFromAdmin = true) {
        printHeader("CAR DETAILS");
        file.open("carRecords.txt", ios::in);
        if (!file) {
            cout << "No car records found!\n";
            waitForEnter(isFromAdmin ? "Press Enter to return to Admin Menu" : "Press Enter to continue...");
            if (isFromAdmin) adminMenu();
            return;
        }

        int count = 0;
        while (getline(file, carID, '*')) {
            getline(file, carModel, '*');
            getline(file, carBrand, '*');
            getline(file, carColor, '*');
            getline(file, availableUnits, '*');
            getline(file, rentalRate, '\n');

            cout << "----------------------------------------------\n";
            cout << "Car ID          : " << carID << "\n";
            cout << "Model           : " << carModel << "\n";
            cout << "Brand           : " << carBrand << "\n";
            cout << "Color           : " << carColor << "\n";
            cout << "Available Units : " << availableUnits << "\n";
            cout << "Rental Rate/day : Rs." << rentalRate << "\n";
            count++;
        }
        if (count == 0) cout << "No cars available.\n";
        cout << "----------------------------------------------\n";
        file.close();

        waitForEnter(isFromAdmin ? "Press Enter to return to Admin Menu" : "Press Enter to continue...");
        if (isFromAdmin) adminMenu();
    }

    // Search for a car by its ID and display details
    void searchCar() {
        printHeader("SEARCH CAR");
        string id;
        cout << "Enter Car ID to search: ";
        getline(cin, id);

        file.open("carRecords.txt", ios::in);
        bool found = false;
        while (getline(file, carID, '*')) {
            getline(file, carModel, '*');
            getline(file, carBrand, '*');
            getline(file, carColor, '*');
            getline(file, availableUnits, '*');
            getline(file, rentalRate, '\n');

            if (carID == id) {
                cout << "\n[Car Found]\n";
                cout << "Car ID          : " << carID << "\n";
                cout << "Model           : " << carModel << "\n";
                cout << "Brand           : " << carBrand << "\n";
                cout << "Color           : " << carColor << "\n";
                cout << "Available Units : " << availableUnits << "\n";
                cout << "Rental Rate/day : Rs." << rentalRate << "\n";
                found = true;
                break;
            }
        }
        file.close();

        if (!found) {
            cout << "\nNo car found with ID: " << id << "\n";
        }
        waitForEnter();
    }

    // Modify car data by rewriting the file except for the modified entry
    void modifyCar() {
        printHeader("MODIFY CAR");
        string id;
        cout << "Enter Car ID to modify: ";
        getline(cin, id);

        fstream tempFile("temp.txt", ios::out);
        file.open("carRecords.txt", ios::in);
        bool found = false;

        while (getline(file, carID, '*')) {
            getline(file, carModel, '*');
            getline(file, carBrand, '*');
            getline(file, carColor, '*');
            getline(file, availableUnits, '*');
            getline(file, rentalRate, '\n');

            if (carID == id) {
                found = true;
                cout << "Enter new Model: ";
                getline(cin, carModel);
                cout << "Enter new Brand: ";
                getline(cin, carBrand);
                cout << "Enter new Color: ";
                getline(cin, carColor);
                cout << "Enter new available units: ";
                getline(cin, availableUnits);
                cout << "Enter new rental rate per day: ";
                getline(cin, rentalRate);

                cout << "\n[Success] Car Modified Successfully!\n";
            }
            // Write either updated or original record to temp file
            tempFile << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;
        }
        file.close();
        tempFile.close();

        // Replace original file with temp file
        if (remove("carRecords.txt") != 0) {
            cout << "[Error] Error removing original file.\n";
        }
        else if (rename("temp.txt", "carRecords.txt") != 0) {
            cout << "[Error] Error renaming temp file.\n";
        }

        if (!found) cout << "[Info] Car with this ID does not exist.\n";

        waitForEnter();
    }

    // Delete a car record by rewriting all except the target to temp file
    void deleteCar() {
        printHeader("DELETE CAR");
        string id;
        cout << "Enter Car ID to delete: ";
        getline(cin, id);

        fstream tempFile("temp.txt", ios::out);
        file.open("carRecords.txt", ios::in);
        bool found = false;

        while (getline(file, carID, '*')) {
            getline(file, carModel, '*');
            getline(file, carBrand, '*');
            getline(file, carColor, '*');
            getline(file, availableUnits, '*');
            getline(file, rentalRate, '\n');

            if (carID == id) {
                found = true;
                cout << "\n[Deleted] Car with ID " << id << ".\n";
            }
            else {
                tempFile << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;
            }
        }
        file.close();
        tempFile.close();

        // Replace original file with temp file
        if (remove("carRecords.txt") != 0) {
            cout << "[Error] Error removing original file.\n";
        }
        else if (rename("temp.txt", "carRecords.txt") != 0) {
            cout << "[Error] Error renaming temp file.\n";
        }

        if (!found) cout << "[Info] Car with this ID does not exist.\n";

        waitForEnter();
    }

    // Rent a car for a customer by updating availability and recording rental info
    bool rentCar(string customerID) {
        printHeader("RENT A CAR");
        showCars(false); // Show all cars without returning to admin menu

        cout << "\nEnter Car ID to rent: ";
        string id;
        getline(cin, id);

        fstream tempFile("temp.txt", ios::out);
        file.open("carRecords.txt", ios::in);

        bool found = false;
        bool rented = false;

        while (getline(file, carID, '*')) {
            getline(file, carModel, '*');
            getline(file, carBrand, '*');
            getline(file, carColor, '*');
            getline(file, availableUnits, '*');
            getline(file, rentalRate, '\n');

            if (carID == id) {
                found = true;
                int units = stoi(availableUnits);
                if (units <= 0) {
                    cout << "\nSorry, this car is currently unavailable.\n";
                    tempFile << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;
                }
                else {
                    cout << "\nRental rate is Rs." << rentalRate << " per day.\n";

                    int days = 0;
                    cout << "Enter number of days to rent: ";
                    cin >> days;
                    while (days <= 0) {
                        cout << "Invalid number of days. Enter again: ";
                        cin >> days;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    int totalCost = stoi(rentalRate) * days;
                    cout << "\nTotal cost: Rs." << totalCost << "\n";

                    int payment = 0;
                    cout << "Enter payment amount: Rs.";
                    cin >> payment;
                    while (payment < totalCost) {
                        cout << "Insufficient payment. Enter amount >= " << totalCost << ": Rs.";
                        cin >> payment;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    int change = payment - totalCost;
                    cout << "Payment successful! Change: Rs." << change << "\n";

                    units--;
                    availableUnits = to_string(units);

                    tempFile << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;

                    // Save rental due date to rentalDueDates.txt
                    fstream rentalDates("rentalDueDates.txt", ios::out | ios::app);
                    time_t now = time(0);
                    tm ltm;
#ifdef _WIN32
                    localtime_s(&ltm, &now);
#else
                    localtime_r(&now, &ltm);
#endif
                    ltm.tm_mday += days; // Add rental days to current date
                    time_t due_time = mktime(&ltm);
                    tm* due_date = localtime(&due_time);

                    rentalDates << customerID << "*" << carID << "*"
                        << 1900 + due_date->tm_year << "-"
                        << 1 + due_date->tm_mon << "-"
                        << due_date->tm_mday << endl;
                    rentalDates.close();

                    addRentalHistory(customerID, carID, days, totalCost);
                    generateReceipt(customerID, carID, days, totalCost);

                    cout << "\n[Success] Car rented successfully!\n";
                    waitForEnter("Press Enter to return to Customer Menu...");
                    rented = true;
                }
            }
            else {
                tempFile << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;
            }
        }
        file.close();
        tempFile.close();

        // Replace original car record file with updated data
        if (remove("carRecords.txt") != 0) {
            cout << "[Error] Could not remove original file.\n";
        }
        else if (rename("temp.txt", "carRecords.txt") != 0) {
            cout << "[Error] Could not rename temp file.\n";
        }

        if (!found) {
            cout << "\nNo car found with this ID.\n";
            waitForEnter();
        }

        return rented;
    }

    // Return a rented car, update available units, and mark rental as returned
    void returnCar() {
        printHeader("RETURN A CAR");
        string customerID, id;
        cout << "Enter Your Customer ID: ";
        getline(cin, customerID);
        cout << "Enter Car ID to return: ";
        getline(cin, id);

        file.open("carRecords.txt", ios::in);
        fstream tempFile("temp.txt", ios::out);

        bool found = false;
        while (getline(file, carID, '*')) {
            getline(file, carModel, '*');
            getline(file, carBrand, '*');
            getline(file, carColor, '*');
            getline(file, availableUnits, '*');
            getline(file, rentalRate, '\n');

            if (carID == id) {
                int units = stoi(availableUnits);
                units++;
                availableUnits = to_string(units);
                cout << "\n[Success] Car returned successfully!\n";
                found = true;
            }
            tempFile << carID << "*" << carModel << "*" << carBrand << "*" << carColor << "*" << availableUnits << "*" << rentalRate << endl;
        }
        file.close();
        tempFile.close();

        remove("carRecords.txt");
        rename("temp.txt", "carRecords.txt");

        if (!found) {
            cout << "\nNo car found with this ID.\n";
        }

        // Update rental history returned status to true
        fstream history("rentalHistory.txt", ios::in);
        fstream tempHistory("tempHistory.txt", ios::out);

        string cID, hCarID, days, cost, returned;
        bool updated = false;

        while (getline(history, cID, '*')) {
            getline(history, hCarID, '*');
            getline(history, days, '*');
            getline(history, cost, '*');
            getline(history, returned, '\n');

            if (!updated && cID == customerID && hCarID == id && returned == "false") {
                tempHistory << cID << "*" << hCarID << "*" << days << "*" << cost << "*" << "true" << endl;
                updated = true;
            }
            else {
                tempHistory << cID << "*" << hCarID << "*" << days << "*" << cost << "*" << returned << endl;
            }
        }

        history.close();
        tempHistory.close();

        remove("rentalHistory.txt");
        rename("tempHistory.txt", "rentalHistory.txt");

        if (!updated) {
            cout << "\n[Warning] Matching rental record not found or already returned.\n";
        }
        waitForEnter();
    }

    // Generate a rental receipt and save to receipt.txt
    void generateReceipt(string customerID, string carID, int days, int totalCost) {
        ofstream receipt("receipt.txt", ios::out);

        time_t now = time(0);
        tm* ltm = localtime(&now);

        stringstream content;
        content << "\n=========== RENTAL RECEIPT ===========\n";
        content << "Customer ID: " << customerID << "\n";
        content << "Car ID: " << carID << "\n";
        content << "Rental Duration: " << days << " day(s)\n";
        content << "Total Cost: Rs." << totalCost << "\n";
        content << "Date: " << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << "\n";
        content << "======================================\n";

        receipt << content.str();
        receipt.close();

        cout << content.str();
        cout << "Rental receipt saved to 'receipt.txt'\n";
    }

    // Show rental history for a given customer
    void showRentalHistory(string customerID) {
        printHeader("RENTAL HISTORY");
        // Remove spaces from customerID for accurate matching
        customerID.erase(remove_if(customerID.begin(), customerID.end(), ::isspace), customerID.end());

        fstream history("rentalHistory.txt", ios::in);
        if (!history) {
            cout << "No rental history file found.\n";
            waitForEnter();
            return;
        }

        string cID, carID, days, cost, returned;
        bool found = false;

        cout << "Customer ID: " << customerID << "\n--------------------------------------\n";

        while (getline(history, cID, '*')) {
            getline(history, carID, '*');
            getline(history, days, '*');
            getline(history, cost, '*');
            getline(history, returned, '\n');

            cID.erase(remove_if(cID.begin(), cID.end(), ::isspace), cID.end());

            if (cID == customerID) {
                cout << "Car Rented   : " << carID << endl;
                cout << "Days Rented  : " << days << endl;
                cout << "Total Paid   : Rs." << cost << endl;
                cout << "Status       : " << (returned == "true" ? "Returned" : "Active") << endl;
                cout << "--------------------------------------\n";
                found = true;
            }
        }
        if (!found) cout << "No rental history found for this customer.\n";
        cout << "======================================\n";
        history.close();
        waitForEnter();
    }

    // Add a rental history record to file 
    void addRentalHistory(string customerID, string carID, int days, int totalCost) {
        customerID.erase(remove_if(customerID.begin(), customerID.end(), ::isspace), customerID.end());

        fstream history("rentalHistory.txt", ios::out | ios::app);
        if (!history) {
            cout << "\n[Error] Could not open rentalHistory.txt\n";
            return;
        }

        history << customerID << "*" << carID << "*" << days << "*" << totalCost << "*false\n";
        history.close();
    }
} carObj;

// Customer class 
class Customer {
    string customerID, customerName, customerPassword;
    fstream file;

public:
    // Add a new customer record
    void addCustomer() {
        printHeader("NEW CUSTOMER ENTRY");
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Customer ID: ";
        getline(cin, customerID);
        cout << "Enter Customer Name: ";
        getline(cin, customerName);
        cout << "Set Password: ";
        getline(cin, customerPassword);

        file.open("customerRecords.txt", ios::out | ios::app);
        if (!file) {
            cout << "\n[Error] Unable to write to customerRecords.txt\n";
            waitForEnter();
            return;
        }
        file << customerID << "*" << customerName << "*" << customerPassword << endl;
        file.close();

        cout << "\n[Success] Customer Added Successfully!\n";
        waitForEnter();
    }

    // Remove a customer record by ID
    void removeCustomer() {
        printHeader("REMOVE CUSTOMER");
        string id;
        cout << "Enter Customer ID to remove: ";
        getline(cin, id);

        fstream tempFile("temp.txt", ios::out);
        file.open("customerRecords.txt", ios::in);
        bool found = false;

        while (getline(file, customerID, '*')) {
            getline(file, customerName, '*');
            getline(file, customerPassword, '\n');

            if (id == customerID) {
                found = true;
                cout << "\n[Removed] Customer with ID " << id << ".\n";
            }
            else {
                tempFile << customerID << "*" << customerName << "*" << customerPassword << endl;
            }
        }
        file.close();
        tempFile.close();

        if (remove("customerRecords.txt") != 0) {
            cout << "[Error] Could not remove original file.\n";
        }
        else if (rename("temp.txt", "customerRecords.txt") != 0) {
            cout << "[Error] Could not rename temp file.\n";
        }

        if (!found) cout << "[Info] No customer found with this ID.\n";

        waitForEnter();
    }

    // Display all customers
    void showCustomers() {
        printHeader("CUSTOMER LIST");
        file.open("customerRecords.txt", ios::in);
        if (!file) {
            cout << "\nNo customer records found!\n";
            waitForEnter();
            return;
        }
        while (getline(file, customerID, '*')) {
            getline(file, customerName, '*');
            getline(file, customerPassword, '\n');
            cout << "----------------------------------------------\n";
            cout << "Customer Name : " << customerName << "\n";
            cout << "Customer ID   : " << customerID << "\n";
        }
        cout << "----------------------------------------------\n";
        file.close();
        waitForEnter();
    }

    // Search customer by ID
    void searchCustomer() {
        printHeader("SEARCH CUSTOMER");
        string id;
        cout << "Enter Customer ID to search: ";
        getline(cin, id);

        file.open("customerRecords.txt", ios::in);
        bool found = false;

        while (getline(file, customerID, '*')) {
            getline(file, customerName, '*');
            getline(file, customerPassword, '\n');
            if (customerID == id) {
                cout << "----------------------------------------------\n";
                cout << "Customer Name : " << customerName << "\n";
                cout << "Customer ID   : " << customerID << "\n";
                cout << "----------------------------------------------\n";
                found = true;
                break;
            }
        }
        file.close();

        if (!found) cout << "\nNo customer found with this ID.\n";

        waitForEnter();
    }

    // Modify customer details
    void modifyCustomer() {
        printHeader("MODIFY CUSTOMER");
        string id;
        cout << "Enter Customer ID to modify: ";
        getline(cin, id);

        fstream tempFile("temp.txt", ios::out);
        file.open("customerRecords.txt", ios::in);
        bool found = false;

        while (getline(file, customerID, '*')) {
            getline(file, customerName, '*');
            getline(file, customerPassword, '\n');

            if (customerID == id) {
                cout << "Enter new Customer Name: ";
                getline(cin, customerName);
                cout << "Enter new Password: ";
                getline(cin, customerPassword);
                found = true;
            }
            tempFile << customerID << "*" << customerName << "*" << customerPassword << endl;
        }
        file.close();
        tempFile.close();

        if (remove("customerRecords.txt") != 0) {
            cout << "[Error] Could not remove original file.\n";
        }
        else if (rename("temp.txt", "customerRecords.txt") != 0) {
            cout << "[Error] Could not rename temp file.\n";
        }

        if (!found) cout << "[Info] No customer found with this ID.\n";

        waitForEnter();
    }

    // Customer login with password attempts
    void customerLogin() {
        printHeader("CUSTOMER LOGIN");
        string id, pass;
        bool found = false;
        int tries = 0;

        cout << "Enter your Customer ID: ";
        getline(cin, id);

        file.open("customerRecords.txt", ios::in);
        while (getline(file, customerID, '*')) {
            getline(file, customerName, '*');
            getline(file, customerPassword, '\n');
            if (customerID == id) {
                found = true;
                while (tries < 3) {
                    cout << "Enter Password: ";
                    getline(cin, pass);
                    if (pass == customerPassword) {
                        cout << "\nWelcome " << customerName << "!\n";
                        file.close();
                        waitForEnter();
                        customerMenu(); // Go to customer menu
                        return;
                    }
                    else {
                        tries++;
                        cout << "\n[Error] Invalid Password. " << 3 - tries << " tries left.\n";
                    }
                }
                cout << "\nToo many failed attempts. Returning to main menu.\n";
                file.close();
                waitForEnter();
                return;
            }
        }
        file.close();

        if (!found) {
            cout << "\nNo customer found with this ID.\n";
            waitForEnter();
        }
    }
} customerObj;

// Customer control menu, admin only
void customerControlMenu() {
    int choice;
    while (true) {
        printHeader("CUSTOMER CONTROL MENU (ADMIN ONLY)");
        cout << "1. Add Customer\n";
        cout << "2. Remove Customer\n";
        cout << "3. Display Customers\n";
        cout << "4. Search Customer\n";
        cout << "5. Modify Customer\n";
        cout << "6. Back to Admin Menu\n";
        cout << "Enter your choice (1-6): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: customerObj.addCustomer(); break;
        case 2: customerObj.removeCustomer(); break;
        case 3: customerObj.showCustomers(); break;
        case 4: customerObj.searchCustomer(); break;
        case 5: customerObj.modifyCustomer(); break;
        case 6: return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            waitForEnter();
        }
    }
}

// Admin menu 
void adminMenu() {
    int choice;
    while (true) {
        printHeader("ADMIN MENU");
        cout << "1. Add New Car\n";
        cout << "2. Remove Car\n";
        cout << "3. Display All Cars\n";
        cout << "4. Search for a Car\n";
        cout << "5. Modify Car\n";
        cout << "6. Customer Control Menu\n";
        cout << "7. Check Rental History\n";
        cout << "8. Check Overdue Rentals\n";
        cout << "9. Generate Sample Car File\n";
        cout << "10. Back to Main Menu\n";
        cout << "Enter your choice (1-10): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: carObj.addCar(); break;
        case 2: carObj.deleteCar(); break;
        case 3: carObj.showCars(); break;
        case 4: carObj.searchCar(); break;
        case 5: carObj.modifyCar(); break;
        case 6: customerControlMenu(); break;
        case 7: {
            string id;
            cout << "Enter Customer ID: ";
            getline(cin, id);
            carObj.showRentalHistory(id);
            break;
        }
        case 8: checkOverdueCars(); break;
        case 9: generateCarFile(); break;
        case 10: return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            waitForEnter();
        }
    }
}

// Admin login password check with 3 attempts
void adminPassCheck() {
    const string adminPassword = "adminadmin";
    string inputPassword;
    int attempts = 0;

    printHeader("ADMIN LOGIN");

    while (attempts < 3) {
        cout << "Enter Admin Password: ";
        getline(cin, inputPassword);

        if (inputPassword == adminPassword) {
            cout << "\n[Success] Access Granted!\n";
            waitForEnter("Press Enter to proceed to Administrator Menu...");
            adminMenu();
            return;
        }
        else {
            attempts++;
            cout << "\n[Error] Incorrect password. Attempts left: " << (3 - attempts) << "\n";
        }
    }

    cout << "\nToo many incorrect attempts. Returning to Main Menu...\n";
    waitForEnter();
    MainMenu();
}

// Customer menu 
void customerMenu() {
    int choice;
    printHeader("CUSTOMER LOGIN");
    cout << "Enter Your Customer ID: ";
    string customerID;
    getline(cin, customerID);

    while (true) {
        printHeader("CUSTOMER MENU");
        cout << "1. Rent a Car\n";
        cout << "2. Return a Car\n";
        cout << "3. View Rental History\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice (1-4): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: carObj.rentCar(customerID); break;
        case 2: carObj.returnCar(); break;
        case 3: carObj.showRentalHistory(customerID); break;
        case 4: return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            waitForEnter();
        }
    }
}

// Check for overdue rentals by comparing due dates with current date
void checkOverdueCars() {
    printHeader("CHECK OVERDUE RENTALS");
    ifstream file("rentalDueDates.txt");
    if (!file) {
        cout << "\nNo due dates found.\n";
        waitForEnter();
        return;
    }
    time_t now = time(0);
    tm* today = localtime(&now);

    string custID, carID, dueDate;
    bool anyOverdue = false;

    while (getline(file, custID, '*')) {
        getline(file, carID, '*');
        getline(file, dueDate);

        int y, m, d;
        sscanf(dueDate.c_str(), "%d-%d-%d", &y, &m, &d);

        // Compare due date with today's date
        if (y < (1900 + today->tm_year) ||
            (y == (1900 + today->tm_year) && m < (1 + today->tm_mon)) ||
            (y == (1900 + today->tm_year) && m == (1 + today->tm_mon) && d < today->tm_mday)) {
            cout << "\n[OVERDUE] Car ID: " << carID << " | Customer ID: " << custID << " | Due Date: " << dueDate;
            anyOverdue = true;
        }
    }

    if (!anyOverdue) cout << "\nNo overdue rentals.\n";
    file.close();
    waitForEnter();
}

// Show a simple list of customers 
void showCustomerListSimple() {
    printHeader("CUSTOMER LIST");
    fstream file("customerRecords.txt", ios::in);
    if (!file) {
        cout << "\nNo customers found.\n";
        waitForEnter();
        return;
    }
    cout << "Current Customers:\n";
    cout << "------------------------\n";
    string id, name, pass;
    while (getline(file, id, '*')) {
        getline(file, name, '*');
        getline(file, pass, '\n');
        cout << "ID: " << id << " | Name: " << name << "\n";
    }
    file.close();
    waitForEnter();
}

// Create an empty carRecords.txt file
void generateCarFile() {
    printHeader("GENERATE CAR FILE");
    ofstream carFile("carRecords.txt");
    if (!carFile) {
        cout << "[Error] Error opening file for writing!\n";
        waitForEnter();
        return;
    }
    carFile.close();
    cout << "[Success] File created successfully!\n";
    waitForEnter();
}

// Welcome screen with title and credit
void start() {
    clearScreen();
    cout << "\n\n\n";
    cout << "##################################################\n";
    cout << "#                                                #\n";
    cout << "#                  RENT  EASE                    #\n";
    cout << "#                                                #\n";
    cout << "##################################################\n\n";

    cout << "                Developed by Zahra and Maryam\n\n";

    waitForEnter("Press Enter to continue...");
}

// Main menu loop with options for customer, admin login, or exit
void MainMenu() {
    int choice;
    while (true) {
        printHeader("MAIN MENU");
        cout << "1. Customer Portal\n";
        cout << "2. Admin Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            customerMenu();
            break;
        case 2:
            adminPassCheck();
            break;
        case 3:
            cout << "\nThank you for using the Car Rental System. Goodbye!\n";
            exit(0);
        default:
            cout << "\nInvalid choice. Please try again.\n";
            waitForEnter();
        }
    }
}

int main() {
    start();
    MainMenu();
    return 0;
}