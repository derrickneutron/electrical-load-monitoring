#include <iostream>
#include <fstream> // New: For file saving
using namespace std;

int main() {
    int choice;
    string applianceName[100];
    double power[100];
    double hours[100];
    int count = 0;

    // STEP A: LOAD DATA WHEN PROGRAM STARTS
    ifstream inFile("appliances.txt");
    if (inFile) {
        while (inFile >> applianceName[count] >> power[count] >> hours[count]) {
            count++;
        }
        inFile.close();
    }

    do {
        cout << "\nElectrical Load Monitoring System\n";
        cout << "1. Register Appliance\n";
        cout << "2. View Appliances\n";
        cout << "3. Calculate Bill\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if(choice == 1) {
            cout << "Enter appliance name: ";
            cin >> applianceName[count];

            // Safety Guard for Power
            while (true) {
                cout << "Enter power rating (Watts > 0): ";
                cin >> power[count];
                if (power[count] > 0) break; // Only exit loop if number is positive
                cout << "Invalid! Power must be greater than zero.\n";
            }

            // Safety Guard for Hours
            while (true) {
                cout << "Enter daily usage hours (0-24): ";
                cin >> hours[count];
                if (hours[count] >= 0 && hours[count] <= 24) break; // Only exit if 0-24
                cout << "Invalid! Hours must be between 0 and 24.\n";
            }

            // SAVE TO FILE
            ofstream outFile("appliances.txt", ios::app);
            outFile << applianceName[count] << " " << power[count] << " " << hours[count] << endl;
            outFile.close();

            count++;
            cout << "Appliance registered and saved to memory.\n";
        }
        else if(choice == 2) {
            if(count == 0) {
                cout << "No appliances registered yet.\n";
            } else {
                double totalEnergy = 0;
                for(int i = 0; i < count; i++) {
                    double energy = (power[i] * hours[i]) / 1000;
                    totalEnergy += energy;
                    cout << "\nAppliance " << i+1 << ": " << applianceName[i];
                    cout << "\nEnergy: " << energy << " kWh\n";
                }
                cout << "\nTotal System Energy: " << totalEnergy << " kWh\n";
            }
        }
       else if(choice == 3) {
            if(count == 0) {
                cout << "No appliances registered.\n";
            } else {
                double tariff;
                while (true) {
                    cout << "Enter tariff per kWh (e.g., 1.5): ";
                    cin >> tariff;
                    if (tariff > 0) break;
                    cout << "Invalid tariff! Must be a positive number.\n";
                }
                
                double totalEnergy = 0;
                for(int i = 0; i < count; i++) {
                    totalEnergy += (power[i] * hours[i]) / 1000;
                }
                cout << "---------------------------\n";
                cout << "Total Energy: " << totalEnergy << " kWh\n";
                cout << "Total Bill: GHS " << totalEnergy * tariff << "\n";
                cout << "---------------------------\n";
            }
        }
    } while(choice != 4);

    return 0;
}
