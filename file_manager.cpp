#include "file_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::string> FileManager::split(const std::string& line, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        parts.push_back(item);
    }
    return parts;
}

void FileManager::saveAccounts(const std::vector<Account>& accounts, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    for (size_t i = 0; i < accounts.size(); ++i) {
        const Account& a = accounts[i];
        fout << a.accountNumber << '|'
             << a.fullName << '|'
             << a.fatherName << '|'
             << a.motherName << '|'
             << a.nid << '|'
             << a.phone << '|'
             << a.email << '|'
             << a.address << '|'
             << a.accountType << '|'
             << a.pin << '|'
             << a.balance << '|'
             << a.status << '|'
             << a.failedLoginAttempts << '|'
             << a.failedPinAttempts << '\n';
    }
}

std::vector<Account> FileManager::loadAccounts(const std::string& filename) {
    std::vector<Account> accounts;
    std::ifstream fin(filename.c_str());
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> p = split(line, '|');
        if (p.size() >= 14) {
            Account a;
            a.accountNumber = p[0];
            a.fullName = p[1];
            a.fatherName = p[2];
            a.motherName = p[3];
            a.nid = p[4];
            a.phone = p[5];
            a.email = p[6];
            a.address = p[7];
            a.accountType = p[8];
            a.pin = p[9];
            a.balance = atof(p[10].c_str());
            a.status = p[11];
            a.failedLoginAttempts = atoi(p[12].c_str());
            a.failedPinAttempts = atoi(p[13].c_str());
            accounts.push_back(a);
        }
    }
    return accounts;
}

void FileManager::saveTransactions(const std::vector<Transaction>& transactions, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    for (size_t i = 0; i < transactions.size(); ++i) {
        const Transaction& t = transactions[i];
        fout << t.transactionId << '|'
             << t.accountNumber << '|'
             << t.type << '|'
             << t.amount << '|'
             << t.previousBalance << '|'
             << t.currentBalance << '|'
             << t.note << '|'
             << t.dateTime << '\n';
    }
}

std::vector<Transaction> FileManager::loadTransactions(const std::string& filename) {
    std::vector<Transaction> transactions;
    std::ifstream fin(filename.c_str());
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> p = split(line, '|');
        if (p.size() >= 8) {
            Transaction t;
            t.transactionId = p[0];
            t.accountNumber = p[1];
            t.type = p[2];
            t.amount = atof(p[3].c_str());
            t.previousBalance = atof(p[4].c_str());
            t.currentBalance = atof(p[5].c_str());
            t.note = p[6];
            t.dateTime = p[7];
            transactions.push_back(t);
        }
    }
    return transactions;
}

void FileManager::saveLoans(const std::vector<Loan>& loans, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    for (size_t i = 0; i < loans.size(); ++i) {
        const Loan& l = loans[i];
        fout << l.loanId << '|'
             << l.accountNumber << '|'
             << l.loanType << '|'
             << l.amount << '|'
             << l.interestRate << '|'
             << l.months << '|'
             << l.emi << '|'
             << l.status << '|'
             << l.dateTime << '\n';
    }
}

std::vector<Loan> FileManager::loadLoans(const std::string& filename) {
    std::vector<Loan> loans;
    std::ifstream fin(filename.c_str());
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> p = split(line, '|');
        if (p.size() >= 9) {
            Loan l;
            l.loanId = p[0];
            l.accountNumber = p[1];
            l.loanType = p[2];
            l.amount = atof(p[3].c_str());
            l.interestRate = atof(p[4].c_str());
            l.months = atoi(p[5].c_str());
            l.emi = atof(p[6].c_str());
            l.status = p[7];
            l.dateTime = p[8];
            loans.push_back(l);
        }
    }
    return loans;
}

void FileManager::saveFDs(const std::vector<FixedDeposit>& fds, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    for (size_t i = 0; i < fds.size(); ++i) {
        const FixedDeposit& f = fds[i];
        fout << f.fdId << '|'
             << f.accountNumber << '|'
             << f.principal << '|'
             << f.interestRate << '|'
             << f.months << '|'
             << f.maturityAmount << '|'
             << f.startDate << '|'
             << f.maturityDate << '\n';
    }
}

std::vector<FixedDeposit> FileManager::loadFDs(const std::string& filename) {
    std::vector<FixedDeposit> fds;
    std::ifstream fin(filename.c_str());
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> p = split(line, '|');
        if (p.size() >= 8) {
            FixedDeposit f;
            f.fdId = p[0];
            f.accountNumber = p[1];
            f.principal = atof(p[2].c_str());
            f.interestRate = atof(p[3].c_str());
            f.months = atoi(p[4].c_str());
            f.maturityAmount = atof(p[5].c_str());
            f.startDate = p[6];
            f.maturityDate = p[7];
            fds.push_back(f);
        }
    }
    return fds;
}

void FileManager::saveRDs(const std::vector<RecurringDeposit>& rds, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    for (size_t i = 0; i < rds.size(); ++i) {
        const RecurringDeposit& r = rds[i];
        fout << r.rdId << '|'
             << r.accountNumber << '|'
             << r.monthlyInstallment << '|'
             << r.interestRate << '|'
             << r.months << '|'
             << r.maturityAmount << '|'
             << r.startDate << '\n';
    }
}

std::vector<RecurringDeposit> FileManager::loadRDs(const std::string& filename) {
    std::vector<RecurringDeposit> rds;
    std::ifstream fin(filename.c_str());
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> p = split(line, '|');
        if (p.size() >= 7) {
            RecurringDeposit r;
            r.rdId = p[0];
            r.accountNumber = p[1];
            r.monthlyInstallment = atof(p[2].c_str());
            r.interestRate = atof(p[3].c_str());
            r.months = atoi(p[4].c_str());
            r.maturityAmount = atof(p[5].c_str());
            r.startDate = p[6];
            rds.push_back(r);
        }
    }
    return rds;
}

void FileManager::saveBills(const std::vector<BillPayment>& bills, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    for (size_t i = 0; i < bills.size(); ++i) {
        const BillPayment& b = bills[i];
        fout << b.paymentId << '|'
             << b.accountNumber << '|'
             << b.billType << '|'
             << b.referenceNumber << '|'
             << b.amount << '|'
             << b.dateTime << '\n';
    }
}

std::vector<BillPayment> FileManager::loadBills(const std::string& filename) {
    std::vector<BillPayment> bills;
    std::ifstream fin(filename.c_str());
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> p = split(line, '|');
        if (p.size() >= 6) {
            BillPayment b;
            b.paymentId = p[0];
            b.accountNumber = p[1];
            b.billType = p[2];
            b.referenceNumber = p[3];
            b.amount = atof(p[4].c_str());
            b.dateTime = p[5];
            bills.push_back(b);
        }
    }
    return bills;
}

void FileManager::appendLog(const std::string& message, const std::string& filename) {
    std::ofstream fout(filename.c_str(), std::ios::app);
    fout << message << '\n';
}

bool FileManager::backupFile(const std::string& sourceFile, const std::string& backupFile) {
    std::ifstream src(sourceFile.c_str(), std::ios::binary);
    std::ofstream dst(backupFile.c_str(), std::ios::binary);
    if (!src || !dst) return false;
    dst << src.rdbuf();
    return true;
}

bool FileManager::restoreFile(const std::string& backupFile, const std::string& targetFile) {
    std::ifstream src(backupFile.c_str(), std::ios::binary);
    std::ofstream dst(targetFile.c_str(), std::ios::binary);
    if (!src || !dst) return false;
    dst << src.rdbuf();
    return true;
}
