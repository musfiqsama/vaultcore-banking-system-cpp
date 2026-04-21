#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <vector>
#include <string>
#include "models.h"

class BankSystem {
private:
    std::vector<Account> accounts;
    std::vector<Transaction> transactions;
    std::vector<Loan> loans;
    std::vector<FixedDeposit> fds;
    std::vector<RecurringDeposit> rds;
    std::vector<BillPayment> bills;

    std::string currentDateTime() const;
    std::string generateAccountNumber() const;
    std::string generateTransactionId() const;
    std::string generateLoanId() const;
    std::string generateFDId() const;
    std::string generateRDId() const;
    std::string generateBillId() const;

    int findAccountIndex(const std::string& accountNumber) const;
    bool validateAmount(double amount) const;
    void addTransaction(const std::string& accountNumber, const std::string& type,
                        double amount, double previousBalance, double currentBalance,
                        const std::string& note);
    void printReceipt(const std::string& title, const std::string& accNo,
                      double amount, const std::string& txId) const;
    bool isFraudAlert(double amount) const;

public:
    BankSystem();

    void loadAllData();
    void saveAllData() const;

    void welcomeScreen() const;
    void mainMenu();

    void adminMenu();
    void customerMenu(const std::string& accountNumber);

    // Core V1
    void openAccount();
    bool adminLogin() const;
    std::string customerLogin();

    void viewAllAccounts() const;
    void searchAccount() const;
    void depositMoney(const std::string& performer = "ADMIN");
    void withdrawMoney(const std::string& performer = "ADMIN");
    void transferMoney(const std::string& performer = "ADMIN");

    void checkBalance(const std::string& accountNumber) const;
    void viewProfile(const std::string& accountNumber) const;
    void updateProfile(const std::string& accountNumber);
    void changePin(const std::string& accountNumber);
    void transactionHistory(const std::string& accountNumber) const;
    void miniStatement(const std::string& accountNumber) const;
    void generateFullStatement(const std::string& accountNumber) const;

    void blockAccount();
    void unblockAccount();
    void closeAccount();
    void adminDashboard() const;
    void viewLogs() const;

    // Advanced
    void applyLoan();
    void processLoans();
    void viewLoansForAccount(const std::string& accountNumber) const;

    void calculateInterest() const;
    void createFixedDeposit();
    void createRecurringDeposit();
    void generateReports() const;
    void billPayment(const std::string& performer = "CUSTOMER");
    void atmSimulation();
    void backupData();
    void restoreData();
};

#endif
