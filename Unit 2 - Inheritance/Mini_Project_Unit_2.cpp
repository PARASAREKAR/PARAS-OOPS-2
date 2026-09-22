#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>

using namespace std;

// Base Class: Account
class Account {
protected:
    string accountNumber;
    string accountHolder;
    double balance;

public:
    Account(string accNum, string holderName, double initialBalance)
        : accountNumber(accNum), accountHolder(holderName), balance(initialBalance) {}

    virtual ~Account() = default;

    // Standard deposit available to all accounts
    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited $" << fixed << setprecision(2) << amount 
                 << ". New Balance: $" << balance << "\n";
        } else {
            cout << "Deposit amount must be positive.\n";
        }
    }

    // Virtual withdrawal (overridden by specific rules)
    virtual bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient funds for withdrawal.\n";
            return false;
        }
        balance -= amount;
        cout << "Withdrew $" << fixed << setprecision(2) << amount 
             << ". New Balance: $" << balance << "\n";
        return true;
    }

    // Pure virtual method for interest calculation
    virtual void calculateInterest() = 0;

    // Display basic account profile
    virtual void displayDetails() const {
        cout << "Acc #: " << accountNumber 
             << " | Holder: " << accountHolder 
             << " | Balance: $" << fixed << setprecision(2) << balance;
    }

    string getAccountNumber() const { return accountNumber; }
};

// Derived Class 1: Savings Account (Earns standard interest)
class SavingsAccount : public Account {
private:
    double interestRate; // Annual interest rate percentage (e.g., 4.0 for 4%)

public:
    SavingsAccount(string accNum, string holderName, double initialBalance, double rate = 4.0)
        : Account(accNum, holderName, initialBalance), interestRate(rate) {}

    void calculateInterest() override {
        double interest = balance * (interestRate / 100.0);
        balance += interest;
        cout << "Applied " << interestRate << "% interest: +$" 
             << fixed << setprecision(2) << interest 
             << ". New Balance: $" << balance << "\n";
    }

    void displayDetails() const override {
        cout << "[Savings Account] ";
        Account::displayDetails();
        cout << " | Interest Rate: " << interestRate << "%\n";
    }
};

// Derived Class 2: Current Account (Includes Overdraft Facility)
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(string accNum, string holderName, double initialBalance, double limit = 500.0)
        : Account(accNum, holderName, initialBalance), overdraftLimit(limit) {}

    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "Withdrawal amount must be positive.\n";
            return false;
        }
        // Allows withdrawal down to -overdraftLimit
        if (balance - amount < -overdraftLimit) {
            cout << "Withdrawal declined. Overdraft limit of $" 
                 << fixed << setprecision(2) << overdraftLimit << " exceeded.\n";
            return false;
        }
        balance -= amount;
        cout << "Withdrew $" << fixed << setprecision(2) << amount 
             << ". New Balance: $" << balance << "\n";
        return true;
    }

    // Current accounts typically yield no interest
    void calculateInterest() override {
        cout << "Current Accounts do not earn interest.\n";
    }

    void displayDetails() const override {
        cout << "[Current Account] ";
        Account::displayDetails();
        cout << " | Overdraft Limit: $" << overdraftLimit << "\n";
    }
};

// Derived Class 3: Fixed Deposit Account (Higher interest, restricted premature withdrawals)
class FixedDepositAccount : public Account {
private:
    int tenureMonths;
    double interestRate;
    bool isMatured;

public:
    FixedDepositAccount(string accNum, string holderName, double depositAmount, int months, double rate = 7.5)
        : Account(accNum, holderName, depositAmount), tenureMonths(months), interestRate(rate), isMatured(false) {}

    void calculateInterest() override {
        if (!isMatured) {
            // Simple term interest formula
            double interest = balance * (interestRate / 100.0) * (tenureMonths / 12.0);
            balance += interest;
            isMatured = true;
            cout << "Fixed Deposit matured (" << tenureMonths << " months @ " << interestRate 
                 << "%). Interest added: +$" << fixed << setprecision(2) << interest 
                 << ". Total Payout: $" << balance << "\n";
        } else {
            cout << "Interest has already been calculated and applied for this term.\n";
        }
    }

    bool withdraw(double amount) override {
        if (!isMatured) {
            cout << "Withdrawal blocked: Fixed Deposit has not reached maturity.\n";
            return false;
        }
        return Account::withdraw(amount);
    }

    void displayDetails() const override {
        cout << "[Fixed Deposit]  ";
        Account::displayDetails();
        cout << " | Term: " << tenureMonths << " mos | Matured: " 
             << (isMatured ? "Yes" : "No") << "\n";
    }
};

int main() {
    // Polymorphic collection using dynamic smart pointers
    vector<unique_ptr<Account>> bank;

    bank.push_back(make_unique<SavingsAccount>("SAV-101", "Alice Smith", 1200.0, 3.5));
    bank.push_back(make_unique<CurrentAccount>("CUR-202", "Bob Johnson", 300.0, 500.0));
    bank.push_back(make_unique<FixedDepositAccount>("FD-303", "Charlie Brown", 5000.0, 12, 6.5));

    cout << "--- Initial Account Status ---\n";
    for (const auto& acc : bank) {
        acc->displayDetails();
    }

    cout << "\n--- Performing Operations ---\n";
    
    // 1. Savings Operations
    cout << "\n[Alice - Savings]:\n";
    bank[0]->deposit(300.0);
    bank[0]->calculateInterest();

    // 2. Current Account Operations (Testing Overdraft)
    cout << "\n[Bob - Current]:\n";
    bank[1]->withdraw(600.0); // Uses balance + overdraft
    bank[1]->calculateInterest();

    // 3. Fixed Deposit Operations (Testing Early vs Matured Withdrawal)
    cout << "\n[Charlie - Fixed Deposit]:\n";
    bank[2]->withdraw(1000.0);      // Fails due to maturity rule
    bank[2]->calculateInterest();   // Matures the deposit
    bank[2]->withdraw(1000.0);      // Works after maturity

    cout << "\n--- Final Account Status ---\n";
    for (const auto& acc : bank) {
        acc->displayDetails();
    }

    return 0;
}