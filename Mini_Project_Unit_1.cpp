#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include <ctime>
#include <limits>

using namespace std;

// Helper function to fetch current timestamp
string getCurrentTime() {
    time_t now = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// Helper to clear input buffer safely
void clearBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Base Class: SmartDevice
class SmartDevice {
protected:
    string id;
    string location;
    bool isOn;
    string lastUpdated;

public:
    SmartDevice(string deviceId, string deviceLocation)
        : id(deviceId), location(deviceLocation), isOn(false) {
        touch();
    }

    virtual ~SmartDevice() = default;

    void turnOn() {
        isOn = true;
        touch();
    }

    void turnOff() {
        isOn = false;
        touch();
    }

    void touch() {
        lastUpdated = getCurrentTime();
    }

    string getId() const { return id; }
    string getLocation() const { return location; }
    bool getIsOn() const { return isOn; }
    string getLastUpdated() const { return lastUpdated; }

    virtual void displayDetails() const = 0;
    virtual string getType() const = 0;
    virtual void modifyCustomSettings() = 0;
};

// Derived Class: Light
class Light : public SmartDevice {
private:
    int brightness; // 0 - 100%

public:
    Light(string deviceId, string deviceLocation, int initialBrightness = 80)
        : SmartDevice(deviceId, deviceLocation), brightness(initialBrightness) {}

    void setBrightness(int level) {
        if (level < 0) brightness = 0;
        else if (level > 100) brightness = 100;
        else brightness = level;
        touch();
    }

    string getType() const override { return "Light"; }

    void modifyCustomSettings() override {
        cout << "Enter new brightness level (0-100): ";
        int b;
        if (cin >> b) {
            setBrightness(b);
            cout << "Brightness updated successfully.\n";
        } else {
            cout << "Invalid input.\n";
            clearBuffer();
        }
    }

    void displayDetails() const override {
        cout << "| " << setw(10) << left << id
             << "| " << setw(12) << left << getType()
             << "| " << setw(16) << left << location
             << "| " << setw(8)  << left << (isOn ? "ON" : "OFF")
             << "| " << setw(20) << left << ("Brightness: " + to_string(brightness) + "%")
             << "| " << lastUpdated << " |\n";
    }
};

// Derived Class: Thermostat
class Thermostat : public SmartDevice {
private:
    float temperature;

public:
    Thermostat(string deviceId, string deviceLocation, float initialTemp = 22.0f)
        : SmartDevice(deviceId, deviceLocation), temperature(initialTemp) {}

    void setTemperature(float temp) {
        temperature = temp;
        touch();
    }

    string getType() const override { return "Thermostat"; }

    void modifyCustomSettings() override {
        cout << "Enter target temperature (°C): ";
        float t;
        if (cin >> t) {
            setTemperature(t);
            cout << "Temperature updated successfully.\n";
        } else {
            cout << "Invalid input.\n";
            clearBuffer();
        }
    }

    void displayDetails() const override {
        string tempStr = "Temp: ";
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1f C", temperature);
        tempStr += buf;

        cout << "| " << setw(10) << left << id
             << "| " << setw(12) << left << getType()
             << "| " << setw(16) << left << location
             << "| " << setw(8)  << left << (isOn ? "ON" : "OFF")
             << "| " << setw(20) << left << tempStr
             << "| " << lastUpdated << " |\n";
    }
};

// Derived Class: Camera
class Camera : public SmartDevice {
private:
    bool isRecording;

public:
    Camera(string deviceId, string deviceLocation)
        : SmartDevice(deviceId, deviceLocation), isRecording(false) {}

    void toggleRecording() {
        isRecording = !isRecording;
        touch();
    }

    string getType() const override { return "Camera"; }

    void modifyCustomSettings() override {
        cout << "1. Enable Recording\n2. Disable Recording\nChoice: ";
        int opt;
        if (cin >> opt) {
            if (opt == 1) isRecording = true;
            else if (opt == 2) isRecording = false;
            touch();
            cout << "Camera recording state updated.\n";
        } else {
            cout << "Invalid input.\n";
            clearBuffer();
        }
    }

    void displayDetails() const override {
        cout << "| " << setw(10) << left << id
             << "| " << setw(12) << left << getType()
             << "| " << setw(16) << left << location
             << "| " << setw(8)  << left << (isOn ? "ON" : "OFF")
             << "| " << setw(20) << left << (isRecording ? "Rec: ACTIVE" : "Rec: IDLE")
             << "| " << lastUpdated << " |\n";
    }
};

// Derived Class: DoorLock
class DoorLock : public SmartDevice {
private:
    bool isLocked;

public:
    DoorLock(string deviceId, string deviceLocation)
        : SmartDevice(deviceId, deviceLocation), isLocked(true) {
        isOn = true;
    }

    void setLocked(bool state) {
        isLocked = state;
        touch();
    }

    string getType() const override { return "Door Lock"; }

    void modifyCustomSettings() override {
        cout << "1. Lock Door\n2. Unlock Door\nChoice: ";
        int opt;
        if (cin >> opt) {
            if (opt == 1) setLocked(true);
            else if (opt == 2) setLocked(false);
            cout << "Door lock status updated.\n";
        } else {
            cout << "Invalid input.\n";
            clearBuffer();
        }
    }

    void displayDetails() const override {
        cout << "| " << setw(10) << left << id
             << "| " << setw(12) << left << getType()
             << "| " << setw(16) << left << location
             << "| " << setw(8)  << left << (isOn ? "ON" : "OFF")
             << "| " << setw(20) << left << (isLocked ? "Status: LOCKED" : "Status: UNLOCKED")
             << "| " << lastUpdated << " |\n";
    }
};

// System Manager Class
class SmartHomeManager {
private:
    vector<unique_ptr<SmartDevice>> devices;

public:
    void addDevice(unique_ptr<SmartDevice> device) {
        devices.push_back(move(device));
        cout << "\n[Success] Device added successfully!\n";
    }

    SmartDevice* findDevice(const string& id) {
        for (auto& dev : devices) {
            if (dev->getId() == id) {
                return dev.get();
            }
        }
        return nullptr;
    }

    void togglePower(const string& id, bool powerOn) {
        SmartDevice* dev = findDevice(id);
        if (dev) {
            if (powerOn) dev->turnOn();
            else dev->turnOff();
            cout << "\n[Success] Device " << id << " set to " << (powerOn ? "ON" : "OFF") << ".\n";
        } else {
            cout << "\n[Error] Device with ID '" << id << "' not found.\n";
        }
    }

    void configureDevice(const string& id) {
        SmartDevice* dev = findDevice(id);
        if (dev) {
            dev->modifyCustomSettings();
        } else {
            cout << "\n[Error] Device with ID '" << id << "' not found.\n";
        }
    }

    void displayDashboard() const {
        cout << "\n========================================================================================\n";
        cout << "                                 SMART HOME DASHBOARD                                   \n";
        cout << "========================================================================================\n";
        cout << "| " << setw(10) << left << "Device ID"
             << "| " << setw(12) << left << "Type"
             << "| " << setw(16) << left << "Location"
             << "| " << setw(8)  << left << "Power"
             << "| " << setw(20) << left << "Custom Settings"
             << "| " << setw(19) << left << "Last Updated" << " |\n";
        cout << "----------------------------------------------------------------------------------------\n";

        if (devices.empty()) {
            cout << "|                      No smart devices added to the system yet.                       |\n";
        } else {
            for (const auto& dev : devices) {
                dev->displayDetails();
            }
        }
        cout << "========================================================================================\n\n";
    }
};

int main() {
    SmartHomeManager home;
    int choice = 0;

    while (true) {
        cout << "========== SMART HOME MANAGEMENT SYSTEM ==========\n";
        cout << "1. Display Home Dashboard\n";
        cout << "2. Add New Smart Device\n";
        cout << "3. Turn Device ON\n";
        cout << "4. Turn Device OFF\n";
        cout << "5. Configure Device Settings\n";
        cout << "6. Exit\n";
        cout << "Enter choice (1-6): ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input. Please enter a number.\n\n";
            clearBuffer();
            continue;
        }

        if (choice == 6) {
            cout << "Exiting Smart Home Manager. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1:
                home.displayDashboard();
                break;

            case 2: {
                cout << "\nSelect Device Type to Add:\n";
                cout << "1. Light\n2. Thermostat\n3. Camera\n4. Door Lock\nChoice: ";
                int typeChoice;
                if (!(cin >> typeChoice) || typeChoice < 1 || typeChoice > 4) {
                    cout << "Invalid device type selection.\n";
                    clearBuffer();
                    break;
                }

                string id, location;
                cout << "Enter unique Device ID (e.g., LGT-01): ";
                cin >> id;
                clearBuffer();
                cout << "Enter Location (e.g., Living Room): ";
                getline(cin, location);

                if (typeChoice == 1) {
                    home.addDevice(make_unique<Light>(id, location));
                } else if (typeChoice == 2) {
                    home.addDevice(make_unique<Thermostat>(id, location));
                } else if (typeChoice == 3) {
                    home.addDevice(make_unique<Camera>(id, location));
                } else if (typeChoice == 4) {
                    home.addDevice(make_unique<DoorLock>(id, location));
                }
                break;
            }

            case 3: {
                string id;
                cout << "Enter Device ID to turn ON: ";
                cin >> id;
                home.togglePower(id, true);
                break;
            }

            case 4: {
                string id;
                cout << "Enter Device ID to turn OFF: ";
                cin >> id;
                home.togglePower(id, false);
                break;
            }

            case 5: {
                string id;
                cout << "Enter Device ID to configure: ";
                cin >> id;
                home.configureDevice(id);
                break;
            }

            default:
                cout << "\nInvalid selection. Choose between 1 and 6.\n\n";
                break;
        }
    }

    return 0;
}