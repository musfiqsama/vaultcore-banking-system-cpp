#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <string>
#include "models.h"

class FileManager {
public:
    static void saveAccounts(const std::vector<Account>& accounts, const std::string& filename = "accounts.txt");
    static std::vector<Account> loadAccounts(const std::string& filename = "accounts.txt");

    static void saveTransactions(const std::vector<Transaction>& transactions, const std::string& filename = "transactions.txt");
    static std::vector<Transaction> loadTransactions(const std::string& filename = "transactions.txt");

    static void saveLoans(const std::vector<Loan>& loans, const std::string& filename = "loans.txt");
    static std::vector<Loan> loadLoans(const std::string& filename = "loans.txt");

    static void saveFDs(const std::vector<FixedDeposit>& fds, const std::string& filename = "fds.txt");
    static std::vector<FixedDeposit> loadFDs(const std::string& filename = "fds.txt");

    static void saveRDs(const std::vector<RecurringDeposit>& rds, const std::string& filename = "rds.txt");
    static std::vector<RecurringDeposit> loadRDs(const std::string& filename = "rds.txt");

    static void saveBills(const std::vector<BillPayment>& bills, const std::string& filename = "bills.txt");
    static std::vector<BillPayment> loadBills(const std::string& filename = "bills.txt");

    static void appendLog(const std::string& message, const std::string& filename = "logs.txt");

    static bool backupFile(const std::string& sourceFile, const std::string& backupFile);
    static bool restoreFile(const std::string& backupFile, const std::string& targetFile);

private:
    static std::vector<std::string> split(const std::string& line, char delimiter);
};

#endif
