#include <wx/wx.h>
#include <vector>
#include <wx/textdlg.h>
#include <wx/numdlg.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;

class Account {
public:
    Account(const std::string& name, char gender, int pin, double balance, const std::string& accountType, const std::string& email, const std::string& aadhaar);

    std::string getAccNo() const { return accNo; }
    std::string getName() const { return name; }
    char getGender() const { return gender; }
    int getPin() const { return pin; }
    double getBalance() const { return balance; }
    std::string getAccountType() const { return accountType; }
    std::string getEmail() const { return email; }
    std::string getAadhaar() const { return aadhaar; }

    void withdraw(double amount);  // Function to withdraw money
    void deposit(double amount);   // Function to deposit money

    static int nextAccNo;

private:
    std::string accNo;
    std::string name;
    char gender;
    int pin;
    double balance;
    std::string accountType;
    std::string email;
    std::string aadhaar;
};

// Initialize static member
int Account::nextAccNo = 10001;

Account::Account(const string& name, char gender, int pin, double initialDeposit, const string& accountType, const string& email, const string& aadhaar)
    : name(name), gender(gender), pin(pin), balance(initialDeposit), accountType(accountType), email(email), aadhaar(aadhaar) {
    accNo = "SRM" + to_string(nextAccNo++);
}

void Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) balance -= amount;
}

void Account::deposit(double amount) {
    if (amount > 0) balance += amount;
}

// BankingApp class for main operations
class BankingApp : public wxFrame {
public:
    BankingApp(const wxString& title);
    void LoadAccounts(); // Function to load accounts from the file

private:
    vector<Account> accounts;
    bool adminLoggedIn = false; // To track if admin is logged in
    wxButton* createAccBtn;
    wxButton* depositBtn;
    wxButton* withdrawBtn;
    wxButton* showDetailsBtn;

    void OnCreateAccount(wxCommandEvent& event);
    void OnDeposit(wxCommandEvent& event);
    void OnWithdraw(wxCommandEvent& event);
    void OnShowDetails(wxCommandEvent& event);
    void OnAdminLogin(wxCommandEvent& event); // Admin login handler
    void ShowAdminOptions(); // Function to show options after successful login

    wxDECLARE_EVENT_TABLE();
};


wxBEGIN_EVENT_TABLE(BankingApp, wxFrame)
EVT_BUTTON(1001, BankingApp::OnCreateAccount)
EVT_BUTTON(1002, BankingApp::OnDeposit)
EVT_BUTTON(1003, BankingApp::OnWithdraw)
EVT_BUTTON(1004, BankingApp::OnShowDetails)
EVT_BUTTON(1005, BankingApp::OnAdminLogin)  // Admin login button event
wxEND_EVENT_TABLE()

void BankingApp::LoadAccounts() {
    std::ifstream inFile("user.txt");
    if (!inFile.is_open()) {
        wxMessageBox("Error loading accounts from file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    accounts.clear();
    std::string accNo, name, accountType, email, aadhaar;
    char gender;
    int pin;
    double balance;

    while (inFile >> accNo >> name >> gender >> pin >> balance >> accountType >> email >> aadhaar) {
        accounts.emplace_back(name, gender, pin, balance, accountType, email, aadhaar);
        int currentAccNo = std::stoi(accNo.substr(3));  // Assuming account number starts with 'SRM'
        if (currentAccNo >= Account::nextAccNo) {
            Account::nextAccNo = currentAccNo + 1;  // Update to the next available account number
        }
    }

    inFile.close();
}

BankingApp::BankingApp(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 450)) {
    LoadAccounts(); // Load existing accounts on startup

    wxPanel* panel = new wxPanel(this, -1);
    panel->SetBackgroundColour(wxColour(127, 161, 195));

    // Use wxBoxSizer with vertical direction to align the buttons vertically
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Create buttons
    createAccBtn = new wxButton(panel, 1001, "Create Account");
    depositBtn = new wxButton(panel, 1002, "Deposit Money");
    withdrawBtn = new wxButton(panel, 1003, "Withdraw Money");
    showDetailsBtn = new wxButton(panel, 1004, "Show Account Details");
    wxButton* adminLoginBtn = new wxButton(panel, 1005, "Admin Login");  // Admin login button

    // Set uniform button color (using the blue color you requested)
    wxColour buttonColor(226, 218, 214);  // Blue color for all buttons
    createAccBtn->SetBackgroundColour(buttonColor);
    depositBtn->SetBackgroundColour(buttonColor);
    withdrawBtn->SetBackgroundColour(buttonColor);
    showDetailsBtn->SetBackgroundColour(buttonColor);
    adminLoginBtn->SetBackgroundColour(buttonColor); // Set for admin button

    // Optional: Set white text color for contrast
    createAccBtn->SetForegroundColour(*wxBLACK);
    depositBtn->SetForegroundColour(*wxBLACK);
    withdrawBtn->SetForegroundColour(*wxBLACK);
    showDetailsBtn->SetForegroundColour(*wxBLACK);
    adminLoginBtn->SetForegroundColour(*wxBLACK);

    // Apply rounded corners (adjust curvature by increasing the size of the radius)
    createAccBtn->SetMinSize(wxSize(300, 50));  // Set the button size (width, height)
    depositBtn->SetMinSize(wxSize(300, 50));
    withdrawBtn->SetMinSize(wxSize(300, 50));
    showDetailsBtn->SetMinSize(wxSize(300, 50));
    adminLoginBtn->SetMinSize(wxSize(300, 50));  // Set admin login button size

    // Initially hide the user buttons
    createAccBtn->Show();
    depositBtn->Show();
    withdrawBtn->Show();
    showDetailsBtn->Show();

    // Add buttons with vertical alignment and spacing between them
    vbox->Add(adminLoginBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(createAccBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(depositBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(withdrawBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(showDetailsBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    // Admin button added

   // Set sizer for the panel to layout the components
    panel->SetSizerAndFit(vbox);
}
void BankingApp::OnAdminLogin(wxCommandEvent& event) {
    const std::string filename = "admin.txt"; // Adjust path if necessary
    std::ifstream inFile(filename);
    std::string storedUsername, storedPassword;
    std::string username, password;

    if (!inFile.is_open()) {
        wxMessageBox("Error opening admin file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    inFile >> storedUsername >> storedPassword;
    inFile.close();

    wxTextEntryDialog usernameDialog(this, "Enter admin username:", "Admin Login");
    if (usernameDialog.ShowModal() != wxID_OK) return;
    username = usernameDialog.GetValue().ToStdString();

    wxTextEntryDialog passwordDialog(this, "Enter admin password:", "Admin Login");

    if (passwordDialog.ShowModal() != wxID_OK) return;
    password = passwordDialog.GetValue().ToStdString();

    if (username == storedUsername && password == storedPassword) {
        wxMessageBox("Login Successful!", "Success", wxOK | wxICON_INFORMATION);
        // Admin functionalities can be implemented here
        adminLoggedIn = true;

    }
    else {
        wxMessageBox("Invalid credentials. Please try again.", "Error", wxOK | wxICON_ERROR);
    }
}

void BankingApp::ShowAdminOptions() {
    // Show the options after successful login
    createAccBtn->Show();
    depositBtn->Show();
    withdrawBtn->Show();
    showDetailsBtn->Show();

    // Force layout update to reflect changes
    Layout();  // Update the layout of the components

    // Refresh the entire window to reflect the UI changes without reloading the entire application
    Refresh();  // Ensure that the window reflects the changes immediately

    // Optionally call Fit() to adjust the window size to fit the buttons if needed
    Fit(); // Adjust the window size to accommodate the newly visible buttons
}

void BankingApp::OnCreateAccount(wxCommandEvent& event) {
    if (!adminLoggedIn) {
        wxMessageBox("Admin login required.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    // Prompt user for account details
    wxTextEntryDialog nameDialog(this, "Enter your name:", "Create Account");
    if (nameDialog.ShowModal() != wxID_OK) return;
    std::string name = nameDialog.GetValue().ToStdString();

    wxTextEntryDialog genderDialog(this, "Enter your gender (M/F):", "Create Account");
    if (genderDialog.ShowModal() != wxID_OK) return;
    std::string genderInput = genderDialog.GetValue().ToStdString();
    char gender = (genderInput.empty()) ? 'M' : genderInput[0]; // Default to 'M' if input is empty

    // Replace wxNumberEntryDialog with wxTextEntryDialog for PIN input
    wxTextEntryDialog pinDialog(this, "Enter a PIN (4 digits):", "Create Account", "");
    if (pinDialog.ShowModal() != wxID_OK) return;
    std::string pinInput = pinDialog.GetValue().ToStdString();

    // Validate PIN format (4 digits)
    if (pinInput.length() != 4 || !std::all_of(pinInput.begin(), pinInput.end(), ::isdigit)) {
        wxMessageBox("Invalid PIN. Please enter a 4-digit PIN.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Convert the PIN string to an integer
    int pin = std::stoi(pinInput);

    wxNumberEntryDialog balanceDialog(this, "Enter Initial Deposit Amount:", "Initial Deposit", "Amount", 0, 0, 1000000);
    if (balanceDialog.ShowModal() != wxID_OK) return;
    double balance = balanceDialog.GetValue();

    wxTextEntryDialog accountTypeDialog(this, "Enter account type (e.g., Savings, Current, Business, Loan, Fixed Deposit):", "Create Account");
    if (accountTypeDialog.ShowModal() != wxID_OK) return;
    std::string accountType = accountTypeDialog.GetValue().ToStdString();

    wxTextEntryDialog emailDialog(this, "Enter your email:", "Create Account");
    if (emailDialog.ShowModal() != wxID_OK) return;
    std::string email = emailDialog.GetValue().ToStdString();

    wxTextEntryDialog aadhaarDialog(this, "Enter your Aadhaar number:", "Create Account");
    if (aadhaarDialog.ShowModal() != wxID_OK) return;
    std::string aadhaar = aadhaarDialog.GetValue().ToStdString();

    // Validate Aadhaar format
    if (aadhaar.length() != 12 || !std::all_of(aadhaar.begin(), aadhaar.end(), ::isdigit)) {
        wxMessageBox("Invalid Aadhaar number. It must be a 12-digit numeric value.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Create a new account object
    Account newAccount(name, gender, pin, balance, accountType, email, aadhaar);
    accounts.push_back(newAccount);

    // Write the account details to user.txt
    std::ofstream outFile("user.txt", std::ios::app);  // Open file in append mode
    if (!outFile.is_open()) {
        wxMessageBox("Error saving account to file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    outFile << newAccount.getAccNo() << " "
        << newAccount.getName() << " "
        << newAccount.getGender() << " "
        << newAccount.getPin() << " "
        << newAccount.getBalance() << " "
        << newAccount.getAccountType() << " "
        << newAccount.getEmail() << " "
        << newAccount.getAadhaar() << "\n";
    outFile.close();

    // Notify the user of successful account creation
    std::string successMessage = "Account created successfully!\nAccount Number: " + newAccount.getAccNo();
    wxMessageBox(successMessage, "Success", wxOK | wxICON_INFORMATION);
}

void BankingApp::OnDeposit(wxCommandEvent& event) {

    if (!adminLoggedIn) {
        wxMessageBox("Admin login required to make a deposit.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    // Prompt user for account number
    wxTextEntryDialog accDialog(this, "Enter account number:", "Deposit Money");
    if (accDialog.ShowModal() != wxID_OK) return;

    std::string accNo = accDialog.GetValue().ToStdString();

    // Search for the account
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [&](const Account& acc) { return acc.getAccNo() == accNo; });

    if (it != accounts.end()) {
        // Prompt for deposit amount
        wxTextEntryDialog amountDialog(this, "Enter amount to deposit:", "Deposit Money");
        if (amountDialog.ShowModal() != wxID_OK) return;

        double amount;
        try {
            amount = std::stod(amountDialog.GetValue().ToStdString());
            if (amount <= 0) {
                wxMessageBox("Deposit amount must be positive!", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }
        catch (std::invalid_argument&) {
            wxMessageBox("Invalid amount entered!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Perform the deposit operation
        it->deposit(amount);

        // Update the user.txt file
        std::ofstream outFile("user.txt");
        if (!outFile.is_open()) {
            wxMessageBox("Error updating the file!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        for (const auto& acc : accounts) {
            outFile << acc.getAccNo() << " "
                << acc.getName() << " "
                << acc.getGender() << " "
                << acc.getPin() << " "
                << acc.getBalance() << " "
                << acc.getAccountType() << " "
                << acc.getEmail() << " "
                << acc.getAadhaar() << "\n";
        }

        outFile.close();

        // Notify user of success
        wxMessageBox("Deposit successful!", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Account not found!", "Error", wxOK | wxICON_ERROR);
    }
    // (Rest of the deposit functionality)
}

void BankingApp::OnWithdraw(wxCommandEvent& event) {
    if (!adminLoggedIn) {
        wxMessageBox("Admin login required to withdraw money.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    // (Rest of the withdrawal functionality)
    if (!adminLoggedIn) {
        wxMessageBox("Admin login required to withdraw money.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    // Prompt user for account number
    wxTextEntryDialog accDialog(this, "Enter account number:", "Withdraw Money");
    if (accDialog.ShowModal() != wxID_OK) return;

    std::string accNo = accDialog.GetValue().ToStdString();

    // Search for the account
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [&](const Account& acc) { return acc.getAccNo() == accNo; });

    if (it != accounts.end()) {
        // Prompt for withdrawal amount
        wxTextEntryDialog amountDialog(this, "Enter amount to withdraw:", "Withdraw Money");
        if (amountDialog.ShowModal() != wxID_OK) return;

        double amount;
        try {
            amount = std::stod(amountDialog.GetValue().ToStdString());
            if (amount <= 0) {
                wxMessageBox("Withdrawal amount must be positive!", "Error", wxOK | wxICON_ERROR);
                return;
            }
            if (amount > it->getBalance()) {
                wxMessageBox("Insufficient balance!", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }
        catch (std::invalid_argument&) {
            wxMessageBox("Invalid amount entered!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Perform the withdrawal operation
        it->withdraw(amount);

        // Update the user.txt file
        std::ofstream outFile("user.txt");
        if (!outFile.is_open()) {
            wxMessageBox("Error updating the file!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        for (const auto& acc : accounts) {
            outFile << acc.getAccNo() << " "
                << acc.getName() << " "
                << acc.getGender() << " "
                << acc.getPin() << " "
                << acc.getBalance() << " "
                << acc.getAccountType() << " "
                << acc.getEmail() << " "
                << acc.getAadhaar() << "\n";
        }

        outFile.close();

        // Notify user of success
        wxMessageBox("Withdrawal successful!", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Account not found!", "Error", wxOK | wxICON_ERROR);
    }
}

void BankingApp::OnShowDetails(wxCommandEvent& event) {
    if (!adminLoggedIn) {
        wxMessageBox("Admin login required to view details.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    // Prompt user for account number
    wxTextEntryDialog accDialog(this, "Enter account number:", "Account Details");
    if (accDialog.ShowModal() != wxID_OK) return;

    std::string accNo = accDialog.GetValue().ToStdString();

    // Search for the account
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [&](const Account& acc) { return acc.getAccNo() == accNo; });

    if (it != accounts.end()) {
        // Show account details
        std::ostringstream details;
        details << "Account Number: " << it->getAccNo() << "\n"
            << "Name: " << it->getName() << "\n"
            << "Gender: " << it->getGender() << "\n"
            << "PIN: " << it->getPin() << "\n"
            << "Balance: " << fixed << setprecision(2) << it->getBalance() << "\n"
            << "Account Type: " << it->getAccountType() << "\n"
            << "Email: " << it->getEmail() << "\n"
            << "Aadhaar: " << it->getAadhaar() << "\n";

        wxMessageBox(details.str(), "Account Details", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Account not found!", "Error", wxOK | wxICON_ERROR);
    }
    // (Rest of the show account details functionality)
}

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        BankingApp* appFrame = new BankingApp("Banking Application");
        appFrame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

