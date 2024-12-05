
# Banking Application with wxWidgets GUI

This project is a GUI-based banking application developed using C++ and wxWidgets. The application provides functionalities for managing user accounts, performing transactions (deposit and withdrawal), and administrative access.

---

## Features


🌟 Create Account
Allows users to create a new bank account.

💰 Deposit Funds
Enables users to deposit money into their accounts.

💳 Withdraw Funds
Provides the option to withdraw money.

📄 Check Balance
Users can view their account balance.

🔄 Update Account Information
Update personal or account details.

❌ Delete Account
Option to delete an existing account.

🔍 Search Accounts
Search for an account using account number or name.

---

## Requirements

- **Development Environment**:
  - C++17 or above
  - wxWidgets library installed
- **Files Required**:
  - `user.txt`: Stores user account information.
  - `admin.txt`: Contains admin credentials (username and password).

---


## Description of the Libraries Used in the Project

1. **`#include <iostream>`**  
   Used for input and output operations. Provides the standard input (`std::cin`), output (`std::cout`), and error (`std::cerr`) streams.

2. **`#include <fstream>`**  
   Enables file handling, such as reading from and writing to files. Used to manage account and admin data persistence.

3. **`#include <string>`**  
   Supports string manipulation, allowing operations like concatenation, comparison, and transformations.

4. **`#include <iomanip>`**  
   Provides control over the formatting of input and output, such as setting precision, aligning text, and controlling decimal points in numeric data.

5. **`#include <vector>`**  
   Offers a dynamic array-like structure to store and manage user-defined data, such as account information, efficiently.

6. **`#include <sstream>`**  
   Used for string stream operations, enabling conversion between strings and other data types or extracting formatted input from strings.

7. **`#include <wx/wx.h>`**  
   Part of the wxWidgets library, which is used for creating the graphical user interface (GUI). It supports multi-platform development, making the application visually appealing and user-friendly.

These libraries collectively empower the project to handle diverse functionalities, including GUI development, data management, and file handling.

--- 

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/banking-app.git
   cd banking-app
   ```

2. Compile the code:
   - On Linux/Windows (with wxWidgets setup):
     ```bash
     g++ main.cpp -o BankingApp `wx-config --cxxflags --libs`
     ```
   - On Windows (Visual Studio): Use the wxWidgets project setup.

3. Run the application:
   ```bash
   ./BankingApp
   ```

4. Ensure the `user.txt` and `admin.txt` files are present in the same directory as the executable:
   - **user.txt** (Example Format):
     ```
     SRM10001 John M 1234 5000 Savings john@example.com 123456789012
     ```
   - **admin.txt** (Example Format):
     ```
     admin admin123
     ```

---

## Usage Instructions

1. **Admin Login**:
   - Launch the application.
   - Click "Admin Login" and enter valid credentials to access admin options.

2. **Create Account**:
   - After admin login, click "Create Account."
   - Fill in the required details (e.g., name, gender, PIN, deposit, etc.).

3. **Deposit Money**:
   - Click "Deposit Money" and enter the account number and amount.

4. **Withdraw Money**:
   - Click "Withdraw Money" and specify the account number and withdrawal amount.

5. **View Account Details**:
   - Click "Show Account Details" and enter the account number to view details.

---

## File Details

- `main.cpp`: Main application code with classes for account management and GUI logic.
- `user.txt`: Persistent storage for user account details.
- `admin.txt`: Contains admin credentials for login.

---

## Screenshots
![image](https://github.com/user-attachments/assets/2d1331dc-6540-4d8a-951e-4946246f8c1f)
![image](https://github.com/user-attachments/assets/ec7e4793-2780-48a6-8309-567e7c449892)



---

## Future Enhancements

1. User login functionality for individual account access.
2. Enhanced encryption for PIN and sensitive information.
3. Detailed transaction history for each account.

---

## Contributing

Contributions are welcome! Please fork the repository, create a feature branch, and submit a pull request.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
