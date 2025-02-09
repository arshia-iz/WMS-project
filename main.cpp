#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_USERS = 5;

struct Wallet {
    double balance;
};

struct User {
    string username;
    string password;
    string role;
    Wallet wallet;
};

struct Product {
    string name;
    double price;
    int quantity;
};

Product products[MAX_PRODUCTS];
User users[MAX_USERS];
int productCount = 0;
int userCount = 0;

void displayHelp(const string &command = "") {
    if (command == "") {  
        cout << "Available commands:\n";
        cout << "# balance - Show wallet balance\n";
        cout << "# show <product name> - Show product details\n";
        cout << "# buy <product name> - Buy a product\n";
        cout << "# help [command] - Show help for a specific command\n";
        cout << "# exit - Exit the program\n";
        cout << "$ add <product name> <price> - Add a new product\n";
        cout << "$ remove <product name> - Remove a product\n";
        cout << "$ rename <old product name> <new product name> - Rename a product\n";
        cout << "$ price <product name> <new price> - Change product price\n";
        cout << "$ credit <username> <amount> - Credit a user's wallet\n";
    } else {  
        if (command == "balance") {
            cout << "balance - Displays the current wallet balance.\n";
        } else if (command == "show") {
            cout << "show <product name> - Displays details about the specified product.\n";
        } else if (command == "buy") {
            cout << "buy <product name> - Attempts to buy the specified product if it is in stock and the user has enough balance.\n";
        } else if (command == "help") {
            cout << "help [command] - Shows help for a specific command.\n";
        } else if (command == "exit") {
            cout << "exit - Exits the program.\n";
        } else if (command == "add") {
            cout << "add <product name> <price> - Adds a new product to the inventory with the given price.\n";
        } else if (command == "remove") {
            cout << "remove <product name> - Removes the specified product from the inventory.\n";
        } else if (command == "rename") {
            cout << "rename <old product name> <new product name> - Renames a product from the old name to the new name.\n";
        } else if (command == "price") {
            cout << "price <product name> <new price> - Updates the price of the specified product.\n";
        } else if (command == "credit") {
            cout << "credit <username> <amount> - Adds the specified amount to the user's wallet balance.\n";
        } else {
            cout << "Unknown command: " << command << ". Type 'help' for a list of available commands.\n";
        }
    }
}


void LoadUsers(const string &filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line) && userCount < MAX_USERS) {
        stringstream ss(line);
        string username, password, role;
        double balance;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, role, ',');
        ss >> balance;
        users[userCount++] = {username, password, role, {balance}};
    }
    file.close();
}

void saveUsersToFile(const string &filename) {
    ofstream file(filename);
    for (int i = 0; i < userCount; ++i) {
        file << users[i].username << "," << users[i].password << "," << users[i].role << "," << users[i].wallet.balance << endl;
    }
    file.close();
}

bool login(const string &username, const string &password, string &role, Wallet &wallet) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username && users[i].password == password) {
            role = users[i].role;
            wallet = users[i].wallet;
            return true;
        }
    }
    cout << "Login failed." << endl;
    return false;
}

void loadProductsFromFile(const string &filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line) && productCount < MAX_PRODUCTS) {
        stringstream ss(line);
        string name;
        double price;
        int quantity;
        getline(ss, name, ',');
        ss >> price;
        ss.ignore();
        ss >> quantity;
        products[productCount++] = {name, price, quantity};
    }
    file.close();
}

void saveProductsToFile(const string &filename) {
    ofstream file(filename);
    for (int i = 0; i < productCount; ++i) {
        file << products[i].name << "," << products[i].price << "," << products[i].quantity << endl;
    }
    file.close();
}

void addProduct(const string &input) {
    stringstream ss(input);
    string name;
    double price;

    while (ss >> name >> price) {
        bool productFound = false;
        for (int i = 0; i < productCount; ++i) {
            if (products[i].name == name) {
                products[i].quantity++;  
                cout << "Added one more " << name << " to the stock. New quantity: " << products[i].quantity << endl;
                productFound = true;
                break;
            }
        }

        if (!productFound && productCount < MAX_PRODUCTS) {
            products[productCount++] = {name, price, 1};  
            cout << "Product added: " << name << " with price: " << price << endl;
        }
    }

    saveProductsToFile("products.txt");
}

void removeProduct(const string &productName) {
    for (int i = 0; i < productCount; ++i) {
        if (products[i].name == productName) {
            for (int j = i; j < productCount - 1; ++j) {
                products[j] = products[j + 1];
            }
            --productCount;
            cout << "Product removed: " << productName << endl;
            saveProductsToFile("products.txt");
            return;
        }
    }
    cout << "Product not found." << endl;
}

void renameProduct(const string &oldName, const string &newName) {
    for (int i = 0; i < productCount; ++i) {
        if (products[i].name == oldName) {
            products[i].name = newName;
            cout << "Product renamed from " << oldName << " to " << newName << endl;
            saveProductsToFile("products.txt");
            return;
        }
    }
    cout << "Product not found." << endl;
}

void changeProductPrice(const string &productName, double newPrice) {
    for (int i = 0; i < productCount; ++i) {
        if (products[i].name == productName) {
            products[i].price = newPrice;
            cout << "Price of " << productName << " changed to " << newPrice << endl;
            saveProductsToFile("products.txt");
            return;
        }
    }
    cout << "Product not found." << endl;
}

void creditUser(const string &username, double amount) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username) {
            users[i].wallet.balance += amount;
            cout << "Credited " << amount << " units to " << username << "'s wallet." << endl;
            saveUsersToFile("users.txt");
            return;
        }
    }
    cout << "User not found." << endl;
}

void displayBalance(const Wallet &wallet) {
    cout << "Current balance: " << wallet.balance << " units" << endl;
}

bool deductBalance(Wallet &wallet, double amount) {
    if (wallet.balance >= amount) {
        wallet.balance -= amount;
        return true;
    } else {
        cout << "Insufficient balance." << endl;
        return false;
    }
}

void showProduct(const string &productName) {
    for (int i = 0; i < productCount; ++i) {
        if (products[i].name == productName) {
            cout << "Product: " << products[i].name
                 << ", Price: " << products[i].price
                 << ", Quantity: " << products[i].quantity << endl;
            return;
        }
    }
    cout << "Product not found." << endl;
}

bool buyProduct(const string &productName, Wallet &wallet) {
    for (int i = 0; i < productCount; ++i) {
        if (products[i].name == productName) {
            if (products[i].quantity == 0) {
                cout << "Sorry, " << productName << " is out of stock." << endl;
                return false;
            } else if (wallet.balance >= products[i].price) {
                wallet.balance -= products[i].price;
                products[i].quantity--;
                saveProductsToFile("products.txt");
                cout << "Purchase successful! New balance: " << wallet.balance << " units." << endl;
                return true;
            } else {
                cout << "Insufficient balance." << endl;
                return false;
            }
        }
    }
    cout << "Product not found." << endl;
    return false;
}

int main() {
    LoadUsers("users.txt");
    loadProductsFromFile("products.txt");

    string username, password, role;
    Wallet wallet;

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (login(username, password, role, wallet)) {
        cout << "Login successful! Welcome, " << role << "!" << endl;

        string command;
        while (true) {
            cout << (role == "admin" ? "$ " : "# ");
            cin >> command;

            if (command == "balance") {
                displayBalance(wallet);
            }
            else if (command == "show") {
                string productName;
                if (!(cin >> productName)) {
                    cout << "Usage: show <product_name>" << endl;
                } else {
                    showProduct(productName);
                }
            }
            else if (command == "buy") {
                string productName;
                if (!(cin >> productName)) {
                    cout << "Usage: buy <product_name>" << endl;
                } else {
                    buyProduct(productName, wallet);
                }
            }
            else if (command == "help") {
                string helpCommand;
                cin.ignore();
                getline(cin, helpCommand);
                displayHelp(helpCommand);
            }
            else if (command == "exit") {
                break;
            }
            else if (role == "admin") {
                if (command == "add") {
                    string input;
                    cin.ignore();
                    getline(cin, input);
                    addProduct(input);
                }
                else if (command == "remove") {
                    string productName;
                    if (!(cin >> productName)) {
                        cout << "Usage: remove <product_name>" << endl;
                    } else {
                        removeProduct(productName);
                    }
                }
                else if (command == "rename") {
                    string oldName, newName;
                    if (!(cin >> oldName >> newName)) {
                        cout << "Usage: rename <old_product_name> <new_product_name>" << endl;
                    } else {
                        renameProduct(oldName, newName);
                    }
                }
                else if (command == "price") {
                    string productName;
                    double newPrice;
                    if (!(cin >> productName >> newPrice)) {
                        cout << "Usage: price <product_name> <new_price>" << endl;
                    } else {
                        changeProductPrice(productName, newPrice);
                    }
                }
                else if (command == "credit") {
                    string username;
                    double amount;
                    if (!(cin >> username >> amount)) {
                        cout << "Usage: credit <username> <amount>" << endl;
                    } else {
                        creditUser(username, amount);
                    }
                }
                else {
                    cout << "Unknown command. Type 'help' for a list of commands." << endl;
                }
            }
            else {
                cout << "Unknown command. Type 'help' for a list of commands." << endl;
            }
        }
    }
    return 0;
}
