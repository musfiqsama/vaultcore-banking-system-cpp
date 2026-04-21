# 💳 VaultCore Banking System

A complete **Console-Based Banking Management System** built in **C++**.  
This project simulates real-world banking operations including account management, transactions, loans, deposits, and security features.

---

## 🚀 Features

### 🔐 Authentication
- Admin Login system
- Customer Login using Account Number & PIN
- Account auto-block after 3 failed login attempts

### 👤 Account Management
- Open New Account
- View All Accounts (Admin)
- Search Account by number or name
- Update Customer Profile
- Block / Unblock / Close Account

### 💰 Transactions
- Deposit Money
- Withdraw Money (with PIN verification)
- Money Transfer between accounts
- Transaction history
- Mini statement (last 5 transactions)
- Receipt generation after each transaction

### 📊 Banking Operations
- Balance check
- Full statement generation (file output)
- Interest calculation

### 🏦 Advanced Features
- Loan system (Apply & Admin Approval)
- Fixed Deposit (FD)
- Recurring Deposit (RD)
- Bill payment (Electricity, Water, Internet, Mobile)
- ATM Simulation (Fast Cash, Balance, Mini Statement)

### 🛡️ Security & Logs
- Fraud detection for large transactions
- Login attempt tracking
- PIN attempt tracking
- Audit log system

### 💾 Data Management
- File-based data storage
- Backup & Restore system
- Persistent data across runs

---

## 🧠 System Architecture

- **BankSystem Class** → Core system logic
- **FileManager Class** → File operations (load, save, backup, restore)
- **Models (Structs)** → Data structures for:
  - Account
  - Transaction
  - Loan
  - Fixed Deposit
  - Recurring Deposit
  - Bill Payment

---

## 📂 Project Structure

VaultCoreBankingSystem/
│
├── main.cpp
├── bank_system.cpp
├── bank_system.h
├── file_manager.cpp
├── file_manager.h
├── models.h
├── accounts.txt
├── transactions.txt
├── loans.txt
├── logs.txt



---

## ▶️ How to Run

### 🔧 Compile
```bash
g++ main.cpp bank_system.cpp file_manager.cpp -o bank

./bank

Username: admin
Password: admin123



---

If you want next level polish, I can also:
- 🔥 Add badges (GitHub style)
- 🎨 Create banner image
- 📸 Add demo screenshots section

Just tell me 👍



