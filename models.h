#ifndef MODELS_H
#define MODELS_H

#include <string>

struct Account {
    std::string accountNumber;
    std::string fullName;
    std::string fatherName;
    std::string motherName;
    std::string nid;
    std::string phone;
    std::string email;
    std::string address;
    std::string accountType;
    std::string pin;
    double balance;
    std::string status; // Active / Blocked / Closed
    int failedLoginAttempts;
    int failedPinAttempts;

    Account()
        : balance(0.0), status("Active"), failedLoginAttempts(0), failedPinAttempts(0) {}
};

struct Transaction {
    std::string transactionId;
    std::string accountNumber;
    std::string type; // Deposit / Withdraw / Transfer In / Transfer Out / Bill / Loan etc.
    double amount;
    double previousBalance;
    double currentBalance;
    std::string note;
    std::string dateTime;

    Transaction()
        : amount(0.0), previousBalance(0.0), currentBalance(0.0) {}
};

struct Loan {
    std::string loanId;
    std::string accountNumber;
    std::string loanType; // Personal / Student / Business
    double amount;
    double interestRate;
    int months;
    double emi;
    std::string status; // Pending / Approved / Rejected
    std::string dateTime;

    Loan()
        : amount(0.0), interestRate(0.0), months(0), emi(0.0), status("Pending") {}
};

struct FixedDeposit {
    std::string fdId;
    std::string accountNumber;
    double principal;
    double interestRate;
    int months;
    double maturityAmount;
    std::string startDate;
    std::string maturityDate;

    FixedDeposit()
        : principal(0.0), interestRate(0.0), months(0), maturityAmount(0.0) {}
};

struct RecurringDeposit {
    std::string rdId;
    std::string accountNumber;
    double monthlyInstallment;
    double interestRate;
    int months;
    double maturityAmount;
    std::string startDate;

    RecurringDeposit()
        : monthlyInstallment(0.0), interestRate(0.0), months(0), maturityAmount(0.0) {}
};

struct BillPayment {
    std::string paymentId;
    std::string accountNumber;
    std::string billType; // Electricity / Water / Internet / Mobile
    std::string referenceNumber;
    double amount;
    std::string dateTime;

    BillPayment() : amount(0.0) {}
};

#endif
