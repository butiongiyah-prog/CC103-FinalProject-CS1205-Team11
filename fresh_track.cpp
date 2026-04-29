// ============================================================
//  FRESH TRACK - Perishable Inventory Management System

//  Team Members:
//    25-09524  BUENDIA, BEVERLY ROSE T.
//    25-03580  BUTIONG, IYA ELAINE M.
//
//  Data Structures:
//    Stack         -> Undo last action (LIFO)
//    Queue         -> Delivery intake (FIFO)
//    Priority Queue-> Expiry alerts (Min-Heap, recursive heapify)
//
//  Algorithms:
//    Iterative -> menu loop, for-loop display, bubble sort, search
//    Recursive -> heapifyDown/Up (Priority Queue), display-all
// ============================================================

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <limits>
#include <algorithm>

using namespace std;

//  ANSI COLOR CODES
#define RESET     "\033[0m"
#define BOLD      "\033[1m"
#define DIM       "\033[2m"
#define RED       "\033[91m"
#define GREEN     "\033[92m"
#define YELLOW    "\033[93m"
#define BLUE      "\033[94m"
#define MAGENTA   "\033[95m"
#define CYAN      "\033[96m"
#define WHITE     "\033[97m"

//  UTILITIES
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnter() {
    cout << "\n" << DIM << "  Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

//  DATE
struct Date {
    int day, month, year;
    Date() : day(0), month(0), year(0) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    long toDays() const { return (long)year * 365 + month * 30 + day; }

    bool operator<(const Date& o) const { return toDays() < o.toDays(); }

    string toString() const {
        ostringstream ss;
        ss << setfill('0') << setw(2) << day << "/"
           << setw(2) << month << "/" << year;
        return ss.str();
    }

    bool isValid() const {
        return (year >= 2020 && year <= 2100 &&
                month >= 1 && month <= 12 &&
                day >= 1 && day <= 31);
    }
};

Date today() {
    time_t t = time(nullptr);
    tm* lt   = localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

int daysUntilExpiry(const Date& expiry) {
    return (int)(expiry.toDays() - today().toDays());
}

//  PRODUCT
class Product {
private:
    string name;
    int    quantity;
    Date   batchDate;
    Date   expiryDate;

public:
    Product() : name(""), quantity(0) {}
    Product(string n, int q, Date batch, Date expiry)
        : name(n), quantity(q), batchDate(batch), expiryDate(expiry) {}

    string getName()     const { return name; }
    int    getQuantity() const { return quantity; }
    Date   getBatch()    const { return batchDate; }
    Date   getExpiry()   const { return expiryDate; }
    void   setQuantity(int q) { quantity = q; }
    void   addQuantity(int q) { quantity += q; }

    void display(int index = -1) const {
        int days = daysUntilExpiry(expiryDate);
        string statusCol, statusTxt;
        if      (days < 0)  { statusCol = RED;    statusTxt = "EXPIRED      "; }
        else if (days == 0) { statusCol = RED;    statusTxt = "EXPIRES TODAY"; }
        else if (days <= 3) { statusCol = YELLOW; statusTxt = "NEAR EXPIRY  "; }
        else                { statusCol = GREEN;  statusTxt = "GOOD         "; }

        if (index >= 0) cout << "  " << CYAN << setw(3) << (index+1) << "." << RESET << " ";
        else            cout << "       ";

        cout << BOLD << left << setw(22) << name << RESET
             << setw(8)  << quantity
             << DIM << setw(14) << batchDate.toString()
             << setw(14) << expiryDate.toString() << RESET
             << statusCol << BOLD << statusTxt << RESET
             << DIM << " (" << days << " day" << (abs(days)==1?"":"s") << ")" << RESET << "\n";
    }
};

//  ACTION (for undo stack)
struct Action {
    string type;          // "ADD" or "SELL"
    string productName;
    int    quantity;
    Date   batchDate;
    Date   expiryDate;
};

//  STACK - LIFO - Undo mechanism
class InventoryStack {
private:
    static const int MAX = 100;
    Action data[MAX];
    int    top;
public:
    InventoryStack() : top(-1) {}
    bool isEmpty() const { return top == -1; }
    bool isFull()  const { return top == MAX - 1; }

    void push(const Action& a) {
        if (isFull()) {
            for (int i = 0; i < MAX - 1; i++) data[i] = data[i + 1];
            data[MAX - 1] = a;
        } else {
            data[++top] = a;
        }
    }

    Action pop() {
        if (isEmpty()) throw runtime_error("Undo stack is empty.");
        return data[top--];
    }

    int size() const { return top + 1; }

    void displayAll() const {
        if (isEmpty()) { cout << YELLOW << "  No actions recorded.\n" << RESET; return; }
        cout << "  " << DIM << "Top of stack (most recent) is shown first:\n\n" << RESET;
        for (int i = top; i >= 0; i--) {
            string col = (data[
                i].type == "ADD") ? GREEN : YELLOW;
            cout << "  " << DIM << "[" << (top - i + 1) << "] " << RESET
                 << col << BOLD << setw(6) << data[i].type << RESET
                 << "  " << left << setw(22) << data[i].productName
                 << "  qty: " << BOLD << data[i].quantity << RESET << "\n";
        }
    }
};

//  QUEUE - FIFO - Delivery intake
class DeliveryQueue {
private:
    static const int MAX = 50;
    Product data[MAX];
    int     front, rear, count;
public:
    DeliveryQueue() : front(0), rear(-1), count(0) {}
    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == MAX; }

    void enqueue(const Product& p) {
        if (isFull()) throw runtime_error("Queue full.");
        rear = (rear + 1) % MAX;
        data[rear] = p;
        count++;
    }

    Product dequeue() {
        if (isEmpty()) throw runtime_error("Queue empty.");
        Product p = data[front];
        front = (front + 1) % MAX;
        count--;
        return p;
    }

    int size() const { return count; }

    void displayAll() const {
        if (isEmpty()) { cout << YELLOW << "  Queue is empty.\n" << RESET; return; }
        cout << "  " << BOLD
             << left << setw(6) << "#"
             << setw(22) << "Product"
             << setw(8)  << "Qty"
             << setw(14) << "Batch Date"
             << "Expiry Date" << RESET << "\n";
        cout << "  " << DIM << string(64, '-') << RESET << "\n";
        int idx = front;
        for (int i = 0; i < count; i++) {
            cout << "  " << CYAN << BOLD << setw(6) << (i + 1) << RESET
                 << left << setw(22) << data[idx].getName()
                 << setw(8) << data[idx].getQuantity()
                 << DIM << setw(14) << data[idx].getBatch().toString()
                 << data[idx].getExpiry().toString() << RESET << "\n";
            idx = (idx + 1) % MAX;
        }
    }
};

//  PRIORITY QUEUE - Min-Heap - Expiry alerts
//  heapifyUp and heapifyDown are RECURSIVE
class ExpiryPriorityQueue {
private:
    static const int MAX = 200;
    Product heap[MAX];
    int     size_;

    bool higherPriority(int a, int b) const {
        return heap[a].getExpiry() < heap[b].getExpiry();
    }

    void heapifyUp(int idx) {               // RECURSIVE
        if (idx == 0) return;               // base case: reached root
        int parent = (idx - 1) / 2;
        if (higherPriority(idx, parent)) {
            swap(heap[idx], heap[parent]);
            heapifyUp(parent);              // recursive call
        }
    }

    void heapifyDown(int idx, int sz) {     // RECURSIVE
        int left = 2*idx+1, right = 2*idx+2, smallest = idx;
        if (left  < sz && higherPriority(left,  smallest)) smallest = left;
        if (right < sz && higherPriority(right, smallest)) smallest = right;
        if (smallest != idx) {
            swap(heap[idx], heap[smallest]);
            heapifyDown(smallest, sz);      // recursive call
        }                                   // base case: smallest == idx
    }

public:
    ExpiryPriorityQueue() : size_(0) {}
    bool isEmpty() const { return size_ == 0; }
    int  size()    const { return size_; }

    void insert(const Product& p) {
        if (size_ == MAX) throw runtime_error("Priority queue full.");
        heap[size_] = p;
        heapifyUp(size_);
        size_++;
    }

    Product extractMin() {
        if (isEmpty()) throw runtime_error("Priority queue empty.");
        Product top = heap[0];
        heap[0] = heap[--size_];
        heapifyDown(0, size_);
        return top;
    }
};

//  INVENTORY MANAGER
class InventoryManager {
private:
    static const int MAX_ITEMS = 200;
    Product        items[MAX_ITEMS];
    int            itemCount;
    InventoryStack undoStack;
    DeliveryQueue  deliveryQueue;

    // ITERATIVE linear search
    int findProduct(const string& name) const {
        string qLow = name;
        transform(qLow.begin(), qLow.end(), qLow.begin(), ::tolower);
        for (int i = 0; i < itemCount; i++) {
            string n = items[i].getName();
            transform(n.begin(), n.end(), n.begin(), ::tolower);
            if (n == qLow) return i;
        }
        return -1;
    }

    void printTableHeader() const {
        cout << "\n  " << BOLD
             << left << setw(7)  << ""
             << setw(22) << "Product"
             << setw(8)  << "Qty"
             << setw(14) << "Batch Date"
             << setw(14) << "Expiry Date"
             << "Status" << RESET << "\n";
        cout << "  " << DIM << string(74, '-') << RESET << "\n";
    }

    // RECURSIVE display helper
    void displayRecursiveHelper(int idx) const {
        if (idx == itemCount) return;       // base case
        items[idx].display(idx);
        displayRecursiveHelper(idx + 1);    // recursive call
    }

public:
    InventoryManager() : itemCount(0) {}

    // ---- ADD STOCK ----
    bool addStock(const string& name, int qty, const Date& batch, const Date& expiry) {
        int idx = findProduct(name);
        if (idx != -1) {
            items[idx].addQuantity(qty);
            cout << GREEN << "  [OK] '" << name << "' updated. New qty: "
                 << items[idx].getQuantity() << RESET << "\n";
        } else {
            if (itemCount >= MAX_ITEMS) {
                cout << RED << "  [ERROR] Inventory full.\n" << RESET;
                return false;
            }
            items[itemCount++] = Product(name, qty, batch, expiry);
            cout << GREEN << "  [OK] '" << name << "' added to inventory.\n" << RESET;
        }
        Action a;
        a.type = "ADD"; a.productName = name; a.quantity = qty;
        a.batchDate = batch; a.expiryDate = expiry;
        undoStack.push(a);
        return true;
    }

    // ---- SELL ITEM ----
    void sellItem(const string& name, int qty) {
        int idx = findProduct(name);
        if (idx == -1) {
            cout << RED << "  [ERROR] '" << name << "' not found.\n" << RESET;
            return;
        }
        if (items[idx].getQuantity() < qty) {
            cout << RED << "  [ERROR] Not enough stock. Available: "
                 << items[idx].getQuantity() << RESET << "\n";
            return;
        }
        Action a;
        a.type = "SELL"; a.productName = items[idx].getName(); a.quantity = qty;
        a.batchDate = items[idx].getBatch(); a.expiryDate = items[idx].getExpiry();
        undoStack.push(a);

        items[idx].setQuantity(items[idx].getQuantity() - qty);
        cout << GREEN << "  [OK] Sold " << qty << " of '" << name
             << "'. Remaining: " << items[idx].getQuantity() << RESET << "\n";

        if (items[idx].getQuantity() == 0) {
            cout << YELLOW << "  [INFO] '" << name << "' is out of stock and removed.\n" << RESET;
            for (int i = idx; i < itemCount - 1; i++) items[i] = items[i + 1];
            itemCount--;
        }
    }

    // ---- UNDO (Stack - LIFO) ----
    void undoLastAction() {
        if (undoStack.isEmpty()) {
            cout << YELLOW << "  [INFO] Nothing to undo.\n" << RESET;
            return;
        }
        Action a = undoStack.pop();
        if (a.type == "ADD") {
            int idx = findProduct(a.productName);
            if (idx != -1) {
                int nq = items[idx].getQuantity() - a.quantity;
                if (nq <= 0) {
                    for (int i = idx; i < itemCount - 1; i++) items[i] = items[i + 1];
                    itemCount--;
                } else {
                    items[idx].setQuantity(nq);
                }
            }
            cout << CYAN << "  [UNDO] Reversed ADD of " << a.quantity
                 << " x '" << a.productName << "'.\n" << RESET;
        } else {
            int idx = findProduct(a.productName);
            if (idx != -1) {
                items[idx].addQuantity(a.quantity);
            } else if (itemCount < MAX_ITEMS) {
                items[itemCount++] = Product(a.productName, a.quantity, a.batchDate, a.expiryDate);
            }
            cout << CYAN << "  [UNDO] Restored " << a.quantity
                 << " x '" << a.productName << "'.\n" << RESET;
        }
    }

    // ---- DISPLAY ITERATIVE (for-loop) ----
    void displayAllIterative() const {
        if (itemCount == 0) { cout << YELLOW << "  Inventory is empty.\n" << RESET; return; }
        printTableHeader();
        for (int i = 0; i < itemCount; i++) items[i].display(i);   // ITERATIVE
        cout << "  " << DIM << string(74, '-') << RESET << "\n";
        cout << "  " << BOLD << "Total: " << itemCount << " product(s)\n" << RESET;
    }

    // ---- DISPLAY RECURSIVE ----
    void displayAllRecursive() const {
        if (itemCount == 0) { cout << YELLOW << "  Inventory is empty.\n" << RESET; return; }
        printTableHeader();
        displayRecursiveHelper(0);   // RECURSIVE
        cout << "  " << DIM << string(74, '-') << RESET << "\n";
        cout << "  " << BOLD << "Total: " << itemCount << " product(s)\n" << RESET;
    }

    // ---- EXPIRY ALERTS (Priority Queue / Min-Heap) ----
    void viewExpiryAlerts() {
        if (itemCount == 0) { cout << YELLOW << "  Inventory is empty.\n" << RESET; return; }
        ExpiryPriorityQueue pq;
        for (int i = 0; i < itemCount; i++) pq.insert(items[i]);

        printTableHeader();
        int rank = 0;
        while (!pq.isEmpty()) {
            Product p = pq.extractMin();
            int days  = daysUntilExpiry(p.getExpiry());
            string col = (days < 0) ? RED : (days <= 3) ? YELLOW : GREEN;
            string statusTxt;
            if      (days < 0)  statusTxt = "EXPIRED      ";
            else if (days == 0) statusTxt = "EXPIRES TODAY";
            else if (days <= 3) statusTxt = "NEAR EXPIRY  ";
            else                statusTxt = "GOOD         ";

            cout << "  " << CYAN << BOLD << setw(4) << (++rank) << "." << RESET << "  ";
            cout << BOLD << left << setw(22) << p.getName() << RESET
                 << setw(8)  << p.getQuantity()
                 << DIM << setw(14) << p.getBatch().toString()
                 << setw(14) << p.getExpiry().toString() << RESET
                 << col << BOLD << statusTxt << RESET
                 << DIM << " (" << days << " day" << (abs(days)==1?"":"s") << ")" << RESET << "\n";
        }
        cout << "  " << DIM << string(74, '-') << RESET << "\n";
    }

    // ---- SEARCH (iterative) ----
    void searchProduct(const string& query) const {
        string qLow = query;
        transform(qLow.begin(), qLow.end(), qLow.begin(), ::tolower);
        bool found = false;
        printTableHeader();
        for (int i = 0; i < itemCount; i++) {
            string n = items[i].getName();
            transform(n.begin(), n.end(), n.begin(), ::tolower);
            if (n.find(qLow) != string::npos) { items[i].display(i); found = true; }
        }
        if (!found) cout << RED << "  No product matching '" << query << "' found.\n" << RESET;
        cout << "  " << DIM << string(74, '-') << RESET << "\n";
    }

    // ---- SORT (bubble sort, iterative) ----
    void sortInventory(int option) {
        if (itemCount == 0) { cout << YELLOW << "  Inventory is empty.\n" << RESET; return; }
        for (int i = 0; i < itemCount - 1; i++) {
            for (int j = 0; j < itemCount - i - 1; j++) {
                bool sw = (option == 1)
                    ? items[j+1].getExpiry() < items[j].getExpiry()
                    : items[j].getName() > items[j+1].getName();
                if (sw) swap(items[j], items[j+1]);
            }
        }
        cout << GREEN << "  [OK] Sorted by "
             << (option == 1 ? "expiry date" : "product name") << ".\n\n" << RESET;
        displayAllIterative();
    }

    // ---- QUEUE ----
    void enqueueDelivery(const Product& p) {
        deliveryQueue.enqueue(p);
        cout << BLUE << "  [QUEUE] '" << p.getName() << "' added. Queue size: "
             << deliveryQueue.size() << RESET << "\n";
    }

    void viewDeliveryQueue() const { deliveryQueue.displayAll(); }

    void processDeliveryQueue() {
        if (deliveryQueue.isEmpty()) { cout << YELLOW << "  Queue is empty.\n" << RESET; return; }
        int processed = 0;
        while (!deliveryQueue.isEmpty()) {
            Product p = deliveryQueue.dequeue();
            cout << "\n  " << CYAN << "Processing: " << BOLD << p.getName()
                 << RESET << CYAN << " x" << p.getQuantity() << "..." << RESET << "\n";
            addStock(p.getName(), p.getQuantity(), p.getBatch(), p.getExpiry());
            processed++;
        }
        cout << GREEN << "\n  [OK] " << processed << " delivery(ies) processed into inventory.\n" << RESET;
    }

    void viewUndoStack() const { undoStack.displayAll(); }
    int  getItemCount()  const { return itemCount; }
    int  getQueueSize()  const { return deliveryQueue.size(); }
    int  getUndoSize()   const { return undoStack.size(); }
};

//  INPUT HELPERS
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getInt(const string& prompt, int minVal = 1, int maxVal = 99999) {
    int val;
    while (true) {
        cout << CYAN << "  " << prompt << RESET << ": ";
        if (cin >> val && val >= minVal && val <= maxVal) { clearInput(); return val; }
        cout << RED << "  [ERROR] Enter a number (" << minVal << " - " << maxVal << ").\n" << RESET;
        clearInput();
    }
}

string getString(const string& prompt) {
    string s;
    cout << CYAN << "  " << prompt << RESET << ": ";
    getline(cin, s);
    while (!s.empty() && (s.front()==' '||s.front()=='\t')) s.erase(s.begin());
    while (!s.empty() && (s.back()==' ' ||s.back()=='\t'))  s.pop_back();
    return s;
}

Date getDate(const string& prompt) {
    while (true) {
        cout << CYAN << "  " << prompt << " (DD MM YYYY)" << RESET << ": ";
        Date d;
        if (cin >> d.day >> d.month >> d.year && d.isValid()) { clearInput(); return d; }
        cout << RED << "  [ERROR] Invalid date. Try again.\n" << RESET;
        clearInput();
    }
}

//  UI HELPERS
void sectionHeader(const string& title, const string& color = CYAN) {
    string bar(62, '=');
    int pad = max(0, (int)(60 - title.size()) / 2);
    cout << "\n" << color << BOLD
         << "  " << bar << "\n"
         << "  " << string(pad, ' ') << "[ " << title << " ]\n"
         << "  " << bar << RESET << "\n\n";
}

void printMenu(const InventoryManager& inv) {
    cout << GREEN << BOLD;
    cout << "  +------------------------------------------------------------+\n";
    cout << "  |         F R E S H   T R A C K   v1.0                      |\n";
    cout << "  |    Perishable Inventory Management System                  |\n";
    cout << "  +------------------------------------------------------------+\n";
    cout << RESET;
    cout << "  " << DIM
         << "Items: " << RESET << BOLD << inv.getItemCount() << RESET << DIM
         << "   Queue: " << RESET << BOLD << inv.getQueueSize() << RESET << DIM
         << "   Undo history: " << RESET << BOLD << inv.getUndoSize() << RESET << DIM
         << "   Today: " << RESET << BOLD << today().toString() << RESET << "\n";
    cout << "  " << DIM << string(64, '-') << RESET << "\n\n";

    cout << "  " << CYAN  << "[1]"  << RESET << "  Add New Stock\n";
    cout << "  " << CYAN  << "[2]"  << RESET << "  Sell / Process Item\n";
    cout << "  " << CYAN  << "[3]"  << RESET << "  Undo Last Action"
         << DIM << "              (Stack - LIFO)\n" << RESET;
    cout << "  " << CYAN  << "[4]"  << RESET << "  Display Inventory (Iterative)\n";
    cout << "  " << CYAN  << "[5]"  << RESET << "  Display Inventory (Recursive)\n";
    cout << "  " << CYAN  << "[6]"  << RESET << "  View Expiry Alerts"
         << DIM << "          (Priority Queue - Min-Heap)\n" << RESET;
    cout << "  " << CYAN  << "[7]"  << RESET << "  Search Product\n";
    cout << "  " << CYAN  << "[8]"  << RESET << "  Sort Inventory\n";
    cout << "  " << CYAN  << "[9]"  << RESET << "  Delivery Queue"
         << DIM << "               (Queue - FIFO)\n" << RESET;
    cout << "  " << CYAN  << "[10]" << RESET << " View Undo History\n";
    cout << "  " << RED   << "[0]"  << RESET << "  Exit\n";
    cout << "\n  " << DIM << string(64, '-') << RESET << "\n";
    cout << GREEN << BOLD << "  Choice" << RESET << ": ";
}

//  MAIN
int main() {
#ifdef _WIN32
    // Enable ANSI colors on Windows terminal / VS Code
    system("chcp 65001 > nul");
    system("");
#endif

    InventoryManager inv;
    clearScreen();

    // ---- SPLASH SCREEN ----
    cout << "\n\n";
    cout << GREEN << BOLD
         << "   ___             _    _  _____              _    \n"
         << "  | __| _ ___ ___ | |_ | ||_   _| _ _  __ _ | |__ \n"
         << "  | _| | '_/ -_)_-< ' \\| |  | |  | '_|/ _` || / / \n"
         << "  |_|  |_| \\___/__/_||_|_|  |_|  |_|  \\__,_||_\\_\\ \n"
         << RESET;
    cout << CYAN << BOLD
         << "         Perishable Inventory Management System\n" << RESET;
    cout << DIM
         << "         CC 103 - Data Structures and Algorithms\n"
         << "         Batangas State University  |  AY 2025-2026\n"
         << "\n"
         << "         Team Members:\n"
         << "           25-09524  BUENDIA, BEVERLY ROSE T.\n"
         << "           25-03580  BUTIONG, IYA ELAINE M.\n"
         << RESET << "\n";
    pressEnter();

    int choice;
    do {
        clearScreen();
        printMenu(inv);

        if (!(cin >> choice)) { clearInput(); choice = -1; }
        else clearInput();

        clearScreen();

        switch (choice) {

        case 1: {
            sectionHeader("ADD NEW STOCK", GREEN);
            string name = getString("Product name");
            if (name.empty()) { cout << RED << "  [ERROR] Name cannot be empty.\n" << RESET; pressEnter(); break; }
            int  qty    = getInt("Quantity");
            Date batch  = getDate("Batch date");
            Date expiry = getDate("Expiry date");
            if (expiry.toDays() < batch.toDays()) {
                cout << RED << "  [ERROR] Expiry cannot be before batch date.\n" << RESET;
                pressEnter(); break;
            }
            inv.addStock(name, qty, batch, expiry);
            pressEnter();
            break;
        }

        case 2: {
            sectionHeader("SELL / PROCESS ITEM", YELLOW);
            string name = getString("Product name");
            int    qty  = getInt("Quantity to sell");
            inv.sellItem(name, qty);
            pressEnter();
            break;
        }

        case 3: {
            sectionHeader("UNDO LAST ACTION", CYAN);
            cout << DIM << "  Data structure: Stack (LIFO)\n"
                 << "  The most recent action is always reversed first.\n\n" << RESET;
            inv.undoLastAction();
            pressEnter();
            break;
        }

        case 4: {
            sectionHeader("DISPLAY INVENTORY  (Iterative)", CYAN);
            cout << DIM << "  Algorithm: for-loop traversal — visits each item one by one.\n" << RESET;
            inv.displayAllIterative();
            pressEnter();
            break;
        }

        case 5: {
            sectionHeader("DISPLAY INVENTORY  (Recursive)", MAGENTA);
            cout << DIM << "  Algorithm: recursive function — calls itself with index+1\n"
                 << "  until the base case (index == total items) is reached.\n" << RESET;
            inv.displayAllRecursive();
            pressEnter();
            break;
        }

        case 6: {
            sectionHeader("EXPIRY ALERTS", RED);
            cout << DIM << "  Data structure: Priority Queue (Min-Heap)\n"
                 << "  heapifyUp/Down are implemented recursively.\n"
                 << "  Items with the nearest expiry date appear first.\n\n" << RESET;
            inv.viewExpiryAlerts();
            pressEnter();
            break;
        }

        case 7: {
            sectionHeader("SEARCH PRODUCT", BLUE);
            string query = getString("Product name to search");
            inv.searchProduct(query);
            pressEnter();
            break;
        }

        case 8: {
            sectionHeader("SORT INVENTORY", CYAN);
            cout << "  " << GREEN << "[1]" << RESET << " Sort by expiry date (nearest first)\n";
            cout << "  " << GREEN << "[2]" << RESET << " Sort by product name (A-Z)\n\n";
            int opt = getInt("Choice", 1, 2);
            inv.sortInventory(opt);
            pressEnter();
            break;
        }

        case 9: {
            sectionHeader("DELIVERY QUEUE", BLUE);
            cout << DIM << "  Data structure: Queue (FIFO)\n"
                 << "  The first delivery added will be the first one processed.\n\n" << RESET;
            cout << "  " << BLUE << "[1]" << RESET << " Add delivery to queue\n";
            cout << "  " << BLUE << "[2]" << RESET << " View current queue\n";
            cout << "  " << BLUE << "[3]" << RESET << " Process all deliveries (FIFO)\n\n";
            int sub = getInt("Choice", 1, 3);
            if (sub == 1) {
                string name = getString("Product name");
                if (name.empty()) { cout << RED << "  [ERROR] Name cannot be empty.\n" << RESET; pressEnter(); break; }
                int  qty    = getInt("Quantity");
                Date batch  = getDate("Batch date");
                Date expiry = getDate("Expiry date");
                if (expiry.toDays() < batch.toDays()) {
                    cout << RED << "  [ERROR] Expiry cannot be before batch date.\n" << RESET;
                    pressEnter(); break;
                }
                inv.enqueueDelivery(Product(name, qty, batch, expiry));
            } else if (sub == 2) {
                cout << "\n";
                inv.viewDeliveryQueue();
            } else {
                cout << "\n";
                inv.processDeliveryQueue();
            }
            pressEnter();
            break;
        }

        case 10: {
            sectionHeader("UNDO HISTORY  (Stack)", CYAN);
            inv.viewUndoStack();
            pressEnter();
            break;
        }
        
        case 0:
            clearScreen();
            cout << GREEN << BOLD << "\n\n"
                 << "  +--------------------------------------------+\n"
                 << "  |   Thank you for using Fresh Track!         |\n"
                 << "  |   Goodbye!                                 |\n"
                 << "  +--------------------------------------------+\n\n"
                 << RESET;
            break;

        default:
            cout << RED << "\n  [ERROR] Invalid choice. Enter 0-10.\n" << RESET;
            pressEnter();
        }

    } while (choice != 0);

    return 0;
}
