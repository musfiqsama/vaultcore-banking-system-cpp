#include "bank_system.h"
#include "file_manager.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

BankSystem::BankSystem() {
    loadAllData();
}

string BankSystem::currentDateTime() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

string BankSystem::generateAccountNumber() const {
    int nextId = 100001 + static_cast<int>(accounts.size());
    stringstream ss;
    ss << "VCB" << nextId;
    return ss.str();
}

string BankSystem::generateTransactionId() const {
    stringstream ss;
    ss << "TXN" << 10001 + transactions.size();
    return ss.str();
}

string BankSystem::generateLoanId() const {
    stringstream ss;
    ss << "LOAN" << 1001 + loans.size();
    return ss.str();
}

string BankSystem::generateFDId() const {
    stringstream ss;
    ss << "FD" << 501 + fds.size();
    return ss.str();
}

string BankSystem::generateRDId() const {
    stringstream ss;
    ss << "RD" << 701 + rds.size();
    return ss.str();
}

string BankSystem::generateBillId() const {
    stringstream ss;
    ss << "BILL" << 9001 + bills.size();
    return ss.str();
}

int BankSystem::findAccountIndex(const string& accountNumber) const {
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].accountNumber == accountNumber) return static_cast<int>(i);
    }
    return -1;
}

bool BankSystem::validateAmount(double amount) const {
    return amount > 0;
}

bool BankSystem::isFraudAlert(double amount) const {
    return amount >= 50000.0;
}

void BankSystem::addTransaction(const string& accountNumber, const string& type,
                                double amount, double previousBalance, double currentBalance,
                                const string& note) {
    Transaction t;
    t.transactionId = generateTransactionId();
    t.accountNumber = accountNumber;
    t.type = type;
    t.amount = amount;
    t.previousBalance = previousBalance;
    t.currentBalance = currentBalance;
    t.note = note;
    t.dateTime = currentDateTime();
    transactions.push_back(t);
}

void BankSystem::printReceipt(const string& title, const string& accNo,
                              double amount, const string& txId) const {
    cout << "\n===========================================\n";
    cout << "        VaultCore Banking System\n";
    cout << "===========================================\n";
    cout << "Receipt Type   : " << title << "\n";
    cout << "Account No     : " << accNo << "\n";
    cout << "Amount         : Tk. " << fixed << setprecision(2) << amount << "\n";
    cout << "Transaction ID : " << txId << "\n";
    cout << "Date Time      : " << currentDateTime() << "\n";
    cout << "===========================================\n";
}

void BankSystem::loadAllData() {
    accounts = FileManager::loadAccounts();
    transactions = FileManager::loadTransactions();
    loans = FileManager::loadLoans();
    fds = FileManager::loadFDs();
    rds = FileManager::loadRDs();
    bills = FileManager::loadBills();
}

void BankSystem::saveAllData() const {
    FileManager::saveAccounts(accounts);
    FileManager::saveTransactions(transactions);
    FileManager::saveLoans(loans);
    FileManager::saveFDs(fds);
    FileManager::saveRDs(rds);
    FileManager::saveBills(bills);
}

void BankSystem::welcomeScreen() const {
    cout << "==============================================\n";
    cout << "         VAULTCORE BANKING SYSTEM\n";
    cout << "==============================================\n";

    cout << "         Time: " << currentDateTime() << "\n";
    cout << "==============================================\n";
}

bool BankSystem::adminLogin() const {
    string username, password;
    cout << "\nAdmin Username: ";
    cin >> username;
    cout << "Admin Password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        FileManager::appendLog(currentDateTime() + " | ADMIN login success");
        return true;
    }

    FileManager::appendLog(currentDateTime() + " | ADMIN login failed");
    cout << "Invalid admin credentials.\n";
    return false;
}

string BankSystem::customerLogin() {
    string accountNumber, pin;
    cout << "\nEnter Account Number: ";
    cin >> accountNumber;
    cout << "Enter PIN: ";
    cin >> pin;

    int idx = findAccountIndex(accountNumber);
    if (idx == -1) {
        cout << "Account not found.\n";
        return "";
    }

    if (accounts[idx].status != "Active") {
        cout << "Account is not active.\n";
        return "";
    }

    if (accounts[idx].pin == pin) {
        accounts[idx].failedLoginAttempts = 0;
        saveAllData();
        FileManager::appendLog(currentDateTime() + " | CUSTOMER login success | " + accountNumber);
        return accountNumber;
    }

    accounts[idx].failedLoginAttempts++;
    if (accounts[idx].failedLoginAttempts >= 3) {
        accounts[idx].status = "Blocked";
        cout << "Too many wrong attempts. Account blocked.\n";
        FileManager::appendLog(currentDateTime() + " | FRAUD ALERT login lock | " + accountNumber);
    } else {
        cout << "Wrong PIN. Attempts left: " << 3 - accounts[idx].failedLoginAttempts << "\n";
    }

    saveAllData();
    return "";
}

void BankSystem::openAccount() {
    Account a;
    cout << "\n========== Open New Account ==========\n";
    cin.ignore();
    cout << "Full Name: ";
    getline(cin, a.fullName);
    cout << "Father Name: ";
    getline(cin, a.fatherName);
    cout << "Mother Name: ";
    getline(cin, a.motherName);
    cout << "NID: ";
    getline(cin, a.nid);
    cout << "Phone: ";
    getline(cin, a.phone);
    cout << "Email: ";
    getline(cin, a.email);
    cout << "Address: ";
    getline(cin, a.address);

    cout << "Account Type (Savings/Current/Student): ";
    getline(cin, a.accountType);

    cout << "Set 4-digit PIN: ";
    getline(cin, a.pin);

    cout << "Initial Deposit (Tk.): ";
    cin >> a.balance;

    if (!validateAmount(a.balance)) {
        cout << "Invalid deposit amount.\n";
        return;
    }

    a.accountNumber = generateAccountNumber();
    a.status = "Active";

    accounts.push_back(a);
    addTransaction(a.accountNumber, "Deposit", a.balance, 0.0, a.balance, "Initial deposit");
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | Account created | " + a.accountNumber);

    cout << "\nAccount created successfully.\n";
    cout << "Account Number: " << a.accountNumber << "\n";
    printReceipt("Account Opening", a.accountNumber, a.balance, transactions.back().transactionId);
}

void BankSystem::viewAllAccounts() const {
    cout << "\n================ All Accounts ================\n";
    if (accounts.empty()) {
        cout << "No accounts found.\n";
        return;
    }

    cout << left << setw(12) << "Acc No"
         << setw(20) << "Name"
         << setw(12) << "Type"
         << setw(12) << "Balance"
         << setw(10) << "Status" << "\n";

    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << left << setw(12) << accounts[i].accountNumber
             << setw(20) << accounts[i].fullName.substr(0, 18)
             << setw(12) << accounts[i].accountType
             << setw(12) << fixed << setprecision(2) << accounts[i].balance
             << setw(10) << accounts[i].status << "\n";
    }
}

void BankSystem::searchAccount() const {
    string key;
    cout << "\nEnter account number or exact name: ";
    cin.ignore();
    getline(cin, key);

    bool found = false;
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].accountNumber == key || accounts[i].fullName == key) {
            cout << "\nAccount Number : " << accounts[i].accountNumber << "\n";
            cout << "Name           : " << accounts[i].fullName << "\n";
            cout << "Phone          : " << accounts[i].phone << "\n";
            cout << "Email          : " << accounts[i].email << "\n";
            cout << "Type           : " << accounts[i].accountType << "\n";
            cout << "Balance        : Tk. " << fixed << setprecision(2) << accounts[i].balance << "\n";
            cout << "Status         : " << accounts[i].status << "\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Account not found.\n";
}

void BankSystem::depositMoney(const string& performer) {
    string accountNumber;
    double amount;
    cout << "\nEnter account number: ";
    cin >> accountNumber;
    cout << "Enter deposit amount (Tk.): ";
    cin >> amount;

    int idx = findAccountIndex(accountNumber);
    if (idx == -1 || accounts[idx].status != "Active") {
        cout << "Invalid or inactive account.\n";
        return;
    }
    if (!validateAmount(amount)) {
        cout << "Invalid amount.\n";
        return;
    }

    double prev = accounts[idx].balance;
    accounts[idx].balance += amount;
    addTransaction(accountNumber, "Deposit", amount, prev, accounts[idx].balance, performer + " deposit");
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | Deposit | " + accountNumber);
    printReceipt("Deposit", accountNumber, amount, transactions.back().transactionId);
}

void BankSystem::withdrawMoney(const string& performer) {
    string accountNumber, pin;
    double amount;
    cout << "\nEnter account number: ";
    cin >> accountNumber;
    cout << "Enter amount (Tk.): ";
    cin >> amount;
    cout << "Enter PIN: ";
    cin >> pin;

    int idx = findAccountIndex(accountNumber);
    if (idx == -1 || accounts[idx].status != "Active") {
        cout << "Invalid or inactive account.\n";
        return;
    }
    if (accounts[idx].pin != pin) {
        accounts[idx].failedPinAttempts++;
        if (accounts[idx].failedPinAttempts >= 3) {
            accounts[idx].status = "Blocked";
            FileManager::appendLog(currentDateTime() + " | FRAUD ALERT PIN block | " + accountNumber);
        }
        saveAllData();
        cout << "Invalid PIN.\n";
        return;
    }

    accounts[idx].failedPinAttempts = 0;

    if (!validateAmount(amount)) {
        cout << "Invalid amount.\n";
        return;
    }
    if (accounts[idx].balance < amount) {
        cout << "Insufficient balance.\n";
        return;
    }
    if (accounts[idx].balance - amount < 500) {
        cout << "Minimum balance Tk. 500 must remain.\n";
        return;
    }

    double prev = accounts[idx].balance;
    accounts[idx].balance -= amount;
    addTransaction(accountNumber, "Withdraw", amount, prev, accounts[idx].balance, performer + " withdraw");
    saveAllData();

    if (isFraudAlert(amount)) {
        FileManager::appendLog(currentDateTime() + " | FRAUD ALERT large withdraw | " + accountNumber);
        cout << "[Alert] Large withdrawal detected.\n";
    }

    FileManager::appendLog(currentDateTime() + " | Withdraw | " + accountNumber);
    printReceipt("Withdraw", accountNumber, amount, transactions.back().transactionId);
}

void BankSystem::transferMoney(const string& performer) {
    string fromAcc, toAcc, pin;
    double amount;

    cout << "\nFrom Account: ";
    cin >> fromAcc;
    cout << "To Account: ";
    cin >> toAcc;
    cout << "Amount (Tk.): ";
    cin >> amount;
    cout << "PIN: ";
    cin >> pin;

    int fromIdx = findAccountIndex(fromAcc);
    int toIdx = findAccountIndex(toAcc);

    if (fromIdx == -1 || toIdx == -1 || fromAcc == toAcc) {
        cout << "Invalid account selection.\n";
        return;
    }
    if (accounts[fromIdx].status != "Active" || accounts[toIdx].status != "Active") {
        cout << "One of the accounts is inactive.\n";
        return;
    }
    if (accounts[fromIdx].pin != pin) {
        cout << "Invalid PIN.\n";
        return;
    }
    if (!validateAmount(amount) || accounts[fromIdx].balance < amount || accounts[fromIdx].balance - amount < 500) {
        cout << "Transfer failed due to invalid amount or insufficient balance.\n";
        return;
    }

    double prevFrom = accounts[fromIdx].balance;
    double prevTo = accounts[toIdx].balance;

    accounts[fromIdx].balance -= amount;
    accounts[toIdx].balance += amount;

    addTransaction(fromAcc, "Transfer Out", amount, prevFrom, accounts[fromIdx].balance, "To " + toAcc + " by " + performer);
    addTransaction(toAcc, "Transfer In", amount, prevTo, accounts[toIdx].balance, "From " + fromAcc + " by " + performer);
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | Transfer | " + fromAcc + " -> " + toAcc);
    printReceipt("Transfer", fromAcc, amount, transactions[transactions.size() - 2].transactionId);
}

void BankSystem::checkBalance(const string& accountNumber) const {
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) return;
    cout << "\nAccount Number : " << accounts[idx].accountNumber << "\n";
    cout << "Current Balance: Tk. " << fixed << setprecision(2) << accounts[idx].balance << "\n";
    cout << "Status         : " << accounts[idx].status << "\n";
}

void BankSystem::viewProfile(const string& accountNumber) const {
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) return;
    const Account& a = accounts[idx];
    cout << "\n=========== Profile ===========\n";
    cout << "Account No : " << a.accountNumber << "\n";
    cout << "Name       : " << a.fullName << "\n";
    cout << "Father     : " << a.fatherName << "\n";
    cout << "Mother     : " << a.motherName << "\n";
    cout << "Phone      : " << a.phone << "\n";
    cout << "Email      : " << a.email << "\n";
    cout << "Address    : " << a.address << "\n";
    cout << "Type       : " << a.accountType << "\n";
    cout << "Balance    : Tk. " << fixed << setprecision(2) << a.balance << "\n";
    cout << "Status     : " << a.status << "\n";
}

void BankSystem::updateProfile(const string& accountNumber) {
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) return;

    cin.ignore();
    cout << "\nNew Phone: ";
    getline(cin, accounts[idx].phone);
    cout << "New Email: ";
    getline(cin, accounts[idx].email);
    cout << "New Address: ";
    getline(cin, accounts[idx].address);

    saveAllData();
    FileManager::appendLog(currentDateTime() + " | Profile updated | " + accountNumber);
    cout << "Profile updated successfully.\n";
}

void BankSystem::changePin(const string& accountNumber) {
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) return;

    string oldPin, newPin;
    cout << "\nEnter old PIN: ";
    cin >> oldPin;
    if (accounts[idx].pin != oldPin) {
        cout << "Old PIN incorrect.\n";
        return;
    }
    cout << "Enter new PIN: ";
    cin >> newPin;
    accounts[idx].pin = newPin;
    saveAllData();
    FileManager::appendLog(currentDateTime() + " | PIN changed | " + accountNumber);
    cout << "PIN changed successfully.\n";
}

void BankSystem::transactionHistory(const string& accountNumber) const {
    cout << "\n=========== Transaction History ===========\n";
    bool found = false;
    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].accountNumber == accountNumber) {
            cout << transactions[i].transactionId << " | "
                 << transactions[i].type << " | Tk. "
                 << fixed << setprecision(2) << transactions[i].amount
                 << " | " << transactions[i].dateTime
                 << " | " << transactions[i].note << "\n";
            found = true;
        }
    }
    if (!found) cout << "No transactions found.\n";
}

void BankSystem::miniStatement(const string& accountNumber) const {
    cout << "\n=========== Mini Statement ===========\n";
    int count = 0;
    for (int i = static_cast<int>(transactions.size()) - 1; i >= 0 && count < 5; --i) {
        if (transactions[i].accountNumber == accountNumber) {
            cout << transactions[i].transactionId << " | "
                 << transactions[i].type << " | Tk. "
                 << fixed << setprecision(2) << transactions[i].amount
                 << " | " << transactions[i].dateTime << "\n";
            count++;
        }
    }
    if (count == 0) cout << "No transactions found.\n";
}

void BankSystem::generateFullStatement(const string& accountNumber) const {
    string filename = "statement_" + accountNumber + ".txt";
    ofstream fout(filename.c_str());

    fout << "VaultCore Banking System - Full Statement\n";
    fout << "Account: " << accountNumber << "\n\n";

    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].accountNumber == accountNumber) {
            fout << transactions[i].transactionId << " | "
                 << transactions[i].type << " | Tk. "
                 << fixed << setprecision(2) << transactions[i].amount
                 << " | Prev: " << transactions[i].previousBalance
                 << " | Curr: " << transactions[i].currentBalance
                 << " | " << transactions[i].dateTime
                 << " | " << transactions[i].note << "\n";
        }
    }

    fout.close();
    cout << "Statement generated: " << filename << "\n";
}

void BankSystem::blockAccount() {
    string accountNumber;
    cout << "Enter account number to block: ";
    cin >> accountNumber;
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) {
        cout << "Account not found.\n";
        return;
    }
    accounts[idx].status = "Blocked";
    saveAllData();
    FileManager::appendLog(currentDateTime() + " | Account blocked | " + accountNumber);
    cout << "Account blocked.\n";
}

void BankSystem::unblockAccount() {
    string accountNumber;
    cout << "Enter account number to unblock: ";
    cin >> accountNumber;
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) {
        cout << "Account not found.\n";
        return;
    }
    accounts[idx].status = "Active";
    accounts[idx].failedLoginAttempts = 0;
    accounts[idx].failedPinAttempts = 0;
    saveAllData();
    FileManager::appendLog(currentDateTime() + " | Account unblocked | " + accountNumber);
    cout << "Account unblocked.\n";
}

void BankSystem::closeAccount() {
    string accountNumber;
    cout << "Enter account number to close: ";
    cin >> accountNumber;
    int idx = findAccountIndex(accountNumber);
    if (idx == -1) {
        cout << "Account not found.\n";
        return;
    }
    accounts[idx].status = "Closed";
    saveAllData();
    FileManager::appendLog(currentDateTime() + " | Account closed | " + accountNumber);
    cout << "Account closed.\n";
}

void BankSystem::adminDashboard() const {
    int totalCustomers = static_cast<int>(accounts.size());
    int activeCount = 0, blockedCount = 0, closedCount = 0;
    double totalBalance = 0, totalDeposit = 0, totalWithdraw = 0;

    for (size_t i = 0; i < accounts.size(); ++i) {
        totalBalance += accounts[i].balance;
        if (accounts[i].status == "Active") activeCount++;
        else if (accounts[i].status == "Blocked") blockedCount++;
        else if (accounts[i].status == "Closed") closedCount++;
    }

    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].type == "Deposit") totalDeposit += transactions[i].amount;
        if (transactions[i].type == "Withdraw") totalWithdraw += transactions[i].amount;
    }

    cout << "\n=========== Admin Dashboard ===========\n";
    cout << "Total Customers    : " << totalCustomers << "\n";
    cout << "Active Accounts    : " << activeCount << "\n";
    cout << "Blocked Accounts   : " << blockedCount << "\n";
    cout << "Closed Accounts    : " << closedCount << "\n";
    cout << "Total Bank Balance : Tk. " << fixed << setprecision(2) << totalBalance << "\n";
    cout << "Total Deposits     : Tk. " << totalDeposit << "\n";
    cout << "Total Withdrawals  : Tk. " << totalWithdraw << "\n";
    cout << "Pending Loans      : ";
    int pending = 0;
    for (size_t i = 0; i < loans.size(); ++i) if (loans[i].status == "Pending") pending++;
    cout << pending << "\n";
}

void BankSystem::viewLogs() const {
    ifstream fin("logs.txt");
    string line;
    cout << "\n=============== Audit Logs ===============\n";
    while (getline(fin, line)) {
        cout << line << "\n";
    }
}

void BankSystem::applyLoan() {
    string accNo;
    cout << "Enter account number: ";
    cin >> accNo;

    int idx = findAccountIndex(accNo);
    if (idx == -1 || accounts[idx].status != "Active") {
        cout << "Invalid or inactive account.\n";
        return;
    }

    Loan l;
    l.loanId = generateLoanId();
    l.accountNumber = accNo;
    cout << "Loan Type (Personal/Student/Business): ";
    cin >> l.loanType;
    cout << "Loan Amount: ";
    cin >> l.amount;
    cout << "Interest Rate (% yearly): ";
    cin >> l.interestRate;
    cout << "Repayment Months: ";
    cin >> l.months;

    double totalPayable = l.amount + (l.amount * l.interestRate * l.months / 1200.0);
    l.emi = totalPayable / l.months;
    l.status = "Pending";
    l.dateTime = currentDateTime();

    loans.push_back(l);
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | Loan applied | " + accNo + " | " + l.loanId);
    cout << "Loan applied successfully. EMI: Tk. " << fixed << setprecision(2) << l.emi << "\n";
}

void BankSystem::processLoans() {
    string loanId, decision;
    cout << "Enter Loan ID: ";
    cin >> loanId;
    cout << "Decision (Approved/Rejected): ";
    cin >> decision;

    for (size_t i = 0; i < loans.size(); ++i) {
        if (loans[i].loanId == loanId) {
            loans[i].status = decision;
            if (decision == "Approved") {
                int idx = findAccountIndex(loans[i].accountNumber);
                if (idx != -1) {
                    double prev = accounts[idx].balance;
                    accounts[idx].balance += loans[i].amount;
                    addTransaction(accounts[idx].accountNumber, "Loan Credit", loans[i].amount, prev, accounts[idx].balance, "Approved loan");
                }
            }
            saveAllData();
            FileManager::appendLog(currentDateTime() + " | Loan processed | " + loanId + " | " + decision);
            cout << "Loan updated.\n";
            return;
        }
    }
    cout << "Loan not found.\n";
}

void BankSystem::viewLoansForAccount(const string& accountNumber) const {
    cout << "\n=========== Loan History ===========\n";
    bool found = false;
    for (size_t i = 0; i < loans.size(); ++i) {
        if (loans[i].accountNumber == accountNumber) {
            cout << loans[i].loanId << " | "
                 << loans[i].loanType << " | Tk. "
                 << loans[i].amount << " | EMI: "
                 << loans[i].emi << " | " << loans[i].status << "\n";
            found = true;
        }
    }
    if (!found) cout << "No loan records found.\n";
}

void BankSystem::calculateInterest() const {
    double principal, rate;
    int years;
    cout << "Enter principal amount: ";
    cin >> principal;
    cout << "Enter yearly interest rate (%): ";
    cin >> rate;
    cout << "Enter years: ";
    cin >> years;

    double simpleInterest = principal * rate * years / 100.0;
    double futureAmount = principal + simpleInterest;

    cout << "\nSimple Interest : Tk. " << fixed << setprecision(2) << simpleInterest << "\n";
    cout << "Future Balance  : Tk. " << futureAmount << "\n";
}

void BankSystem::createFixedDeposit() {
    string accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1 || accounts[idx].status != "Active") {
        cout << "Invalid account.\n";
        return;
    }

    FixedDeposit fd;
    fd.fdId = generateFDId();
    fd.accountNumber = accNo;
    cout << "FD Principal Amount: ";
    cin >> fd.principal;
    cout << "Interest Rate (% yearly): ";
    cin >> fd.interestRate;
    cout << "Duration (months): ";
    cin >> fd.months;

    if (accounts[idx].balance < fd.principal) {
        cout << "Insufficient balance.\n";
        return;
    }

    double prev = accounts[idx].balance;
    accounts[idx].balance -= fd.principal;
    fd.maturityAmount = fd.principal + (fd.principal * fd.interestRate * fd.months / 1200.0);
    fd.startDate = currentDateTime();
    fd.maturityDate = "After " + static_cast<ostringstream*>(&(ostringstream() << fd.months))->str() + " months";

    fds.push_back(fd);
    addTransaction(accNo, "FD Open", fd.principal, prev, accounts[idx].balance, fd.fdId);
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | FD created | " + fd.fdId);
    cout << "FD created. Maturity Amount: Tk. " << fixed << setprecision(2) << fd.maturityAmount << "\n";
}

void BankSystem::createRecurringDeposit() {
    string accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1 || accounts[idx].status != "Active") {
        cout << "Invalid account.\n";
        return;
    }

    RecurringDeposit rd;
    rd.rdId = generateRDId();
    rd.accountNumber = accNo;
    cout << "Monthly Installment: ";
    cin >> rd.monthlyInstallment;
    cout << "Interest Rate (% yearly): ";
    cin >> rd.interestRate;
    cout << "Duration (months): ";
    cin >> rd.months;

    double totalPrincipal = rd.monthlyInstallment * rd.months;
    rd.maturityAmount = totalPrincipal + (totalPrincipal * rd.interestRate * rd.months / 2400.0);
    rd.startDate = currentDateTime();

    rds.push_back(rd);
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | RD created | " + rd.rdId);
    cout << "RD created. Estimated maturity amount: Tk. " << fixed << setprecision(2) << rd.maturityAmount << "\n";
}

void BankSystem::generateReports() const {
    ofstream fout("report_summary.txt");
    fout << "VaultCore Banking Report Summary\n";
    fout << "Generated At: " << currentDateTime() << "\n\n";
    fout << "Total Accounts: " << accounts.size() << "\n";
    fout << "Total Transactions: " << transactions.size() << "\n";
    fout << "Total Loans: " << loans.size() << "\n";
    fout << "Total FDs: " << fds.size() << "\n";
    fout << "Total RDs: " << rds.size() << "\n";
    fout << "Total Bills: " << bills.size() << "\n";

    double highestBalance = -1.0;
    string holder = "N/A";
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].balance > highestBalance) {
            highestBalance = accounts[i].balance;
            holder = accounts[i].accountNumber + " - " + accounts[i].fullName;
        }
    }
    fout << "Highest Balance Holder: " << holder << "\n";
    fout << "Highest Balance Amount: Tk. " << highestBalance << "\n";
    fout.close();

    cout << "Report generated: report_summary.txt\n";
}

void BankSystem::billPayment(const string& performer) {
    string accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1 || accounts[idx].status != "Active") {
        cout << "Invalid account.\n";
        return;
    }

    BillPayment b;
    b.paymentId = generateBillId();
    b.accountNumber = accNo;
    cout << "Bill Type (Electricity/Water/Internet/Mobile): ";
    cin >> b.billType;
    cout << "Reference Number: ";
    cin >> b.referenceNumber;
    cout << "Amount: ";
    cin >> b.amount;

    if (accounts[idx].balance < b.amount || !validateAmount(b.amount)) {
        cout << "Payment failed.\n";
        return;
    }

    double prev = accounts[idx].balance;
    accounts[idx].balance -= b.amount;
    b.dateTime = currentDateTime();
    bills.push_back(b);

    addTransaction(accNo, "Bill Payment", b.amount, prev, accounts[idx].balance, performer + " " + b.billType);
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | Bill paid | " + accNo + " | " + b.billType);
    printReceipt("Bill Payment", accNo, b.amount, transactions.back().transactionId);
}

void BankSystem::atmSimulation() {
    string accNo, pin;
    cout << "\n======= ATM Simulation =======\n";
    cout << "Enter Account Number: ";
    cin >> accNo;
    cout << "Enter PIN: ";
    cin >> pin;

    int idx = findAccountIndex(accNo);
    if (idx == -1 || accounts[idx].status != "Active" || accounts[idx].pin != pin) {
        cout << "ATM access denied.\n";
        return;
    }

    int choice;
    cout << "1. Fast Cash 500\n";
    cout << "2. Fast Cash 1000\n";
    cout << "3. Fast Cash 2000\n";
    cout << "4. Balance Check\n";
    cout << "5. Mini Statement\n";
    cout << "Choose: ";
    cin >> choice;

    if (choice == 4) {
        checkBalance(accNo);
        return;
    }
    if (choice == 5) {
        miniStatement(accNo);
        return;
    }

    double amount = 0;
    if (choice == 1) amount = 500;
    else if (choice == 2) amount = 1000;
    else if (choice == 3) amount = 2000;
    else {
        cout << "Invalid option.\n";
        return;
    }

    if (accounts[idx].balance < amount || accounts[idx].balance - amount < 500) {
        cout << "Insufficient balance.\n";
        return;
    }

    double prev = accounts[idx].balance;
    accounts[idx].balance -= amount;
    addTransaction(accNo, "ATM Withdraw", amount, prev, accounts[idx].balance, "ATM fast cash");
    saveAllData();

    FileManager::appendLog(currentDateTime() + " | ATM withdraw | " + accNo);
    printReceipt("ATM Withdraw", accNo, amount, transactions.back().transactionId);
}

void BankSystem::backupData() {
    bool ok1 = FileManager::backupFile("accounts.txt", "backup_accounts.txt");
    bool ok2 = FileManager::backupFile("transactions.txt", "backup_transactions.txt");
    bool ok3 = FileManager::backupFile("loans.txt", "backup_loans.txt");
    bool ok4 = FileManager::backupFile("fds.txt", "backup_fds.txt");
    bool ok5 = FileManager::backupFile("rds.txt", "backup_rds.txt");
    bool ok6 = FileManager::backupFile("bills.txt", "backup_bills.txt");

    if (ok1 && ok2 && ok3 && ok4 && ok5 && ok6) {
        cout << "Backup completed successfully.\n";
        FileManager::appendLog(currentDateTime() + " | Backup completed");
    } else {
        cout << "Backup incomplete.\n";
    }
}

void BankSystem::restoreData() {
    bool ok1 = FileManager::restoreFile("backup_accounts.txt", "accounts.txt");
    bool ok2 = FileManager::restoreFile("backup_transactions.txt", "transactions.txt");
    bool ok3 = FileManager::restoreFile("backup_loans.txt", "loans.txt");
    bool ok4 = FileManager::restoreFile("backup_fds.txt", "fds.txt");
    bool ok5 = FileManager::restoreFile("backup_rds.txt", "rds.txt");
    bool ok6 = FileManager::restoreFile("backup_bills.txt", "bills.txt");

    if (ok1 && ok2 && ok3 && ok4 && ok5 && ok6) {
        loadAllData();
        cout << "Restore completed successfully.\n";
        FileManager::appendLog(currentDateTime() + " | Restore completed");
    } else {
        cout << "Restore incomplete.\n";
    }
}

void BankSystem::customerMenu(const string& accountNumber) {
    int choice;
    do {
        cout << "\n=========== Customer Menu ===========\n";
        cout << "1. View Profile\n";
        cout << "2. Balance Check\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. Transaction History\n";
        cout << "7. Mini Statement\n";
        cout << "8. Change PIN\n";
        cout << "9. Update Profile\n";
        cout << "10. Generate Full Statement\n";
        cout << "11. Apply Loan\n";
        cout << "12. View Loan History\n";
        cout << "13. Interest Calculator\n";
        cout << "14. Create Fixed Deposit\n";
        cout << "15. Create Recurring Deposit\n";
        cout << "16. Bill Payment\n";
        cout << "17. ATM Simulation\n";
        cout << "0. Logout\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: viewProfile(accountNumber); break;
            case 2: checkBalance(accountNumber); break;
            case 3: depositMoney("CUSTOMER"); break;
            case 4: withdrawMoney("CUSTOMER"); break;
            case 5: transferMoney("CUSTOMER"); break;
            case 6: transactionHistory(accountNumber); break;
            case 7: miniStatement(accountNumber); break;
            case 8: changePin(accountNumber); break;
            case 9: updateProfile(accountNumber); break;
            case 10: generateFullStatement(accountNumber); break;
            case 11: applyLoan(); break;
            case 12: viewLoansForAccount(accountNumber); break;
            case 13: calculateInterest(); break;
            case 14: createFixedDeposit(); break;
            case 15: createRecurringDeposit(); break;
            case 16: billPayment("CUSTOMER"); break;
            case 17: atmSimulation(); break;
            case 0: cout << "Logged out.\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 0);
}

void BankSystem::adminMenu() {
    int choice;
    do {
        cout << "\n=========== Admin Menu ===========\n";
        cout << "1. View All Accounts\n";
        cout << "2. Search Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. Block Account\n";
        cout << "7. Unblock Account\n";
        cout << "8. Close Account\n";
        cout << "9. Dashboard\n";
        cout << "10. View Logs\n";
        cout << "11. Process Loans\n";
        cout << "12. Generate Reports\n";
        cout << "13. Backup Data\n";
        cout << "14. Restore Data\n";
        cout << "0. Logout\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: viewAllAccounts(); break;
            case 2: searchAccount(); break;
            case 3: depositMoney("ADMIN"); break;
            case 4: withdrawMoney("ADMIN"); break;
            case 5: transferMoney("ADMIN"); break;
            case 6: blockAccount(); break;
            case 7: unblockAccount(); break;
            case 8: closeAccount(); break;
            case 9: adminDashboard(); break;
            case 10: viewLogs(); break;
            case 11: processLoans(); break;
            case 12: generateReports(); break;
            case 13: backupData(); break;
            case 14: restoreData(); break;
            case 0: cout << "Logged out.\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 0);
}

void BankSystem::mainMenu() {
    int choice;
    do {
        welcomeScreen();
        cout << "\n1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Open New Account\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (adminLogin()) adminMenu();
                break;
            case 2: {
                string acc = customerLogin();
                if (!acc.empty()) customerMenu(acc);
                break;
            }
            case 3:
                openAccount();
                break;
            case 0:
                cout << "Thank you for using VaultCore.\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 0);
}
