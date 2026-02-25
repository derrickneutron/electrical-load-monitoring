#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

// ================= SAVE TO FILE =================
void saveToFile(vector<string>& name, vector<double>& power, vector<double>& hours) {
    ofstream outFile("appliances.txt");
    for (size_t i = 0; i < name.size(); i++) {
        outFile << name[i] << "," << power[i] << "," << hours[i] << endl;
    }
    outFile.close();
}

// ================= LOAD FROM FILE =================
void loadFromFile(vector<string>& name, vector<double>& power, vector<double>& hours) {
    ifstream inFile("appliances.txt");
    string line;
    if (!inFile) return; // Carry on if file doesn't exist yet

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        size_t firstComma = line.find(",");
        size_t secondComma = line.find(",", firstComma + 1);

        if (firstComma != string::npos && secondComma != string::npos) {
            name.push_back(line.substr(0, firstComma));
            power.push_back(atof(line.substr(firstComma + 1, secondComma - firstComma - 1).c_str()));
            hours.push_back(atof(line.substr(secondComma + 1).c_str()));
        }
    }
    inFile.close();
}

// ================= SEARCH (NEW WEEK 4 FEATURE) =================
void searchAppliance(const vector<string>& name, const vector<double>& power, const vector<double>& hours) {
    if (name.empty()) {
        cout << "\nNo data available to search.\n";
        return;
    }
    string query;
    cout << "\nEnter appliance name to search: ";
    cin.ignore();
    getline(cin, query);

    bool found = false;
    for (size_t i = 0; i < name.size(); i++) {
        if (name[i].find(query) != string::npos) {
            cout << "Found: " << name[i] << " [" << power[i] << "W, " << hours[i] << " hrs/day]\n";
            found = true;
        }
    }
    if (!found) cout << "No matches found for '" << query << "'.\n";
}

// ================= REGISTER =================
void registerAppliance(vector<string>& name, vector<double>& power, vector<double>& hours) {
    string applianceName;
    double appliancePower, applianceHours;

    cin.ignore();
    cout << "Enter appliance name: ";
    getline(cin, applianceName);

    cout << "Enter power rating (Watts): ";
    cin >> appliancePower;
    while (appliancePower <= 0) {
        cout << "Invalid. Enter positive Watts: ";
        cin >> appliancePower;
    }

    cout << "Enter usage hours (0-24): ";
    cin >> applianceHours;
    while (applianceHours < 0 || applianceHours > 24) {
        cout << "Invalid. Enter 0 to 24: ";
        cin >> applianceHours;
    }

    name.push_back(applianceName);
    power.push_back(appliancePower);
    hours.push_back(applianceHours);
    saveToFile(name, power, hours);
    cout << "Success: Registered and Saved to File.\n";
}

// ================= VIEW =================
void viewAppliances(const vector<string>& name, const vector<double>& power, const vector<double>& hours) {
    if (name.empty()) {
        cout << "\nInventory is empty.\n";
        return;
    }

    double totalEnergy = 0;
    cout << "\n" << setfill('=') << setw(60) << "" << setfill(' ') << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Watts" << setw(10) << "Hours" << setw(10) << "kWh" << endl;
    cout << setw(60) << setfill('-') << "" << setfill(' ') << endl;

    for (size_t i = 0; i < name.size(); i++) {
        double energy = (power[i] * hours[i]) / 1000;
        totalEnergy += energy;
        cout << left << setw(5) << i + 1 << setw(20) << name[i] << setw(10) << power[i] << setw(10) << hours[i] << setw(10) << fixed << setprecision(2) << energy << endl;
    }
    cout << setw(60) << setfill('-') << "" << setfill(' ') << endl;
    cout << "Total Daily Consumption: " << totalEnergy << " kWh" << endl;
    cout << "Total Monthly (30 days): " << totalEnergy * 30 << " kWh" << endl;
}

// ================= DELETE =================
void deleteAppliance(vector<string>& name, vector<double>& power, vector<double>& hours) {
    if (name.empty()) {
        cout << "Nothing to delete.\n";
        return;
    }
    viewAppliances(name, power, hours);
    int index;
    cout << "Enter ID to delete: ";
    cin >> index;

    if (index < 1 || index > (int)name.size()) {
        cout << "Invalid ID.\n";
        return;
    }

    name.erase(name.begin() + (index - 1));
    power.erase(power.begin() + (index - 1));
    hours.erase(hours.begin() + (index - 1));
    saveToFile(name, power, hours);
    cout << "Appliance removed.\n";
}

// ================= BILL =================
void calculateBill(const vector<string>& name, const vector<double>& power, const vector<double>& hours) {
    if (name.empty()) return;
    double tariff;
    cout << "Enter tariff (GHS per kWh): ";
    cin >> tariff;

    double totalEnergy = 0;
    for (size_t i = 0; i < name.size(); i++) {
        totalEnergy += (power[i] * hours[i]) / 1000;
    }
    cout << "\nEstimated Monthly Bill: GHS " << fixed << setprecision(2) << (totalEnergy * 30 * tariff) << endl;
}

// ================= MAIN =================
int main() {
    vector<string> applianceName;
    vector<double> power, hours;
    loadFromFile(applianceName, power, hours);

    int choice;
    do {
        cout << "\n" << setfill('*') << setw(45) << "" << endl;
        cout << "* ELECTRICAL LOAD MONITOR (CAPSTONE 2026) *" << endl;
        cout << setfill('*') << setw(45) << "" << setfill(' ') << endl;
        cout << "1. Register New Appliance\n2. View All & Energy Stats\n3. Calculate Monthly Bill\n4. Search Appliance\n5. Delete Appliance\n6. Exit\n";
        cout << "Action: ";
        cin >> choice;

        switch (choice) {
            case 1: registerAppliance(applianceName, power, hours); break;
            case 2: viewAppliances(applianceName, power, hours); break;
            case 3: calculateBill(applianceName, power, hours); break;
            case 4: searchAppliance(applianceName, power, hours); break;
            case 5: deleteAppliance(applianceName, power, hours); break;
            case 6: cout << "System shutting down...\n"; break;
            default: cout << "Error: Selection not recognized.\n";
        }
    } while (choice != 6);

    return 0;
}
