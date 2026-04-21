# 💳 VaultCore Banking System

A premium **console-based banking management system** built using **C++**, designed to simulate real-world banking operations with advanced features like transactions, loans, security, and data management.

---

## 🚀 Features

### 🔐 Authentication System
- Admin & Customer login
- Secure PIN verification
- Auto account lock after multiple failed attempts

### 👤 Account Management
- Create new accounts
- View & search accounts
- Update customer profile
- Block / Unblock / Close accounts

### 💰 Transaction System
- Deposit & Withdraw money
- Fund transfer between accounts
- Transaction history & mini statement
- Auto receipt generation

### 🏦 Banking Services
- Loan system (Apply & Admin Approval)
- Fixed Deposit (FD)
- Recurring Deposit (RD)
- Bill payment system
- ATM simulation (Fast Cash, Balance Check)

### 🛡️ Security & Data Management
- Fraud detection (large transactions alert)
- Login & activity logs (audit system)
- File-based persistent storage
- Backup & restore functionality

---

## 🛠️ Technologies Used
- C++
- Object-Oriented Programming (OOP)
- File Handling
- Code::Blocks

---

## 🔑 Admin Login
- **Username:** admin  
- **Password:** admin123  

---

## ▶️ How to Run

### Using Code::Blocks
1. Open the project in **Code::Blocks**
2. Add all `.cpp` and `.h` files
3. Build & Run

### Using Terminal
```bash
g++ main.cpp bank_system.cpp file_manager.cpp -o bank
./bank
