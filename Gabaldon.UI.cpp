// ============================================================
//  FRESH TRACK - Perishable Inventory Management System
//  CC 103 - Data Structures and Algorithms
//  Batangas State University | AY 2025-2026
//
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

// ============================================================
//  ANSI COLOR CODES
// ============================================================
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


//        UTILITY FUNCTIONS                    
//       clearScreen() and pressEnter()                         

// 
//  clearScreen() — wipes the terminal so the menu looks clean
//  Uses different commands depending on the operating system:
//  - Windows → "cls"
//  - Linux/Mac → "clear"
//  #ifdef _WIN32 checks if the program is running on Windows
// 
void clearScreen() {
#ifdef _WIN32
    system("cls");    // Windows command to clear terminal
#else
    system("clear");  // Linux/Mac command to clear terminal
#endif
}


//  pressEnter() — pauses the program and waits for user input
//  Shows "Press Enter to continue..." then waits
//  This gives the user time to READ the output before the
//  screen gets cleared by the next clearScreen() call
//
//  HOW IT WORKS:
//  cin.ignore() — throws away any leftover characters in input
//  cin.get()    — waits until user presses Enter
void pressEnter() {
    cout << "\n" << DIM << "  Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

─────────────────────────────────────────────────────────────
//  getDate() — safely gets a date from the user
//  Format: DD MM YYYY (space separated, e.g. 25 04 2025)
//  Keeps asking until user enters a valid date
//
//  HOW IT WORKS:
//  1. Show prompt
//  2. Try to read 3 integers into d.day, d.month, d.year
//  3. Call d.isValid() to check the values make sense
//  4. If invalid → show error, clear input, try again
// ─────────────────────────────────────────────────────────────

// NOTE: Date struct is defined in the main program.
// Shown here as a reminder of what getDate() works with:
struct Date {
    int day, month, year;
    Date() : day(0), month(0), year(0) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    long toDays() const { return (long)year * 365 + month * 30 + day; }
    string toString() const {
        ostringstream ss;
        ss << setfill('0') << setw(2) << day << "/"
           << setw(2) << month << "/" << year;
        return ss.str();
    }
    bool isValid() const {
        return (year >= 2020 && year <= 2100 &&
                month >= 1  && month <= 12   &&
                day   >= 1  && day   <= 31);
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

// ============================================================
//  PRODUCT
// ============================================================
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

// ============================================================
//  ACTION (for undo stack)
// ============================================================
struct Action {
    string type;          // "ADD" or "SELL"
    string productName;
    int    quantity;
    Date   batchDate;
    Date   expiryDate;
};

// ============================================================
//  STACK - LIFO - Undo mechanism
// ============================================================
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
            string col = (data[i].type == "ADD") ? GREEN : YELLOW;
            cout << "  " << DIM << "[" << (top - i + 1) << "] " << RESET
                 << col << BOLD << setw(6) << data[i].type << RESET
                 << "  " << left << setw(22) << data[i].productName
                 << "  qty: " << BOLD << data[i].quantity << RESET << "\n";
        }
    }
};

// ============================================================
//  QUEUE - FIFO - Delivery intake
// ============================================================
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

// BACKGROUND: WHAT IS A HEAP?      
//
//  A HEAP is a special tree stored inside a regular array.
//  In a MIN-HEAP, the SMALLEST value is always at the ROOT (top).
//
//  In FreshTrack, "smallest" means SOONEST EXPIRY DATE.
//  So the product expiring FIRST is always at position 0.
//
//  HOW A HEAP LOOKS AS A TREE:
//
//              [Kangkong Apr25]         ← ROOT (soonest expiry)
//             /                \
//      [Kamatis Apr27]    [Sayote Apr30]
//       /          \
//  [Sibuyas May10] [Bawang Jun1]
//
//  HOW THAT SAME HEAP LOOKS AS AN ARRAY:
//
//  Index:    0           1           2           3           4
//         [Kangkong] [Kamatis]  [Sayote]   [Sibuyas]   [Bawang]
//
//  THE PARENT-CHILD FORMULA (memorize this!):
//  - Parent of node at index i  →  (i - 1) / 2
//  - Left child of node at i   →  2*i + 1
//  - Right child of node at i  →  2*i + 2
//
//  Example: node at index 1 (Kamatis)
//  - Parent  = (1-1)/2 = 0  → Kangkong ✓
//  - Left    = 2*1+1   = 3  → Sibuyas ✓
//  - Right   = 2*1+2   = 4  → Bawang ✓


//
//YOUR MAIN TOPIC: ExpiryPriorityQueue CLASS           
// DATA STRUCTURE: PRIORITY QUEUE (MIN-HEAP)              
class ExpiryPriorityQueue {
private:
    static const int MAX = 200;
    Product heap[MAX];  // The heap stored as a flat array
    int     size_;      // How many items are currently in the heap

    // ─────────────────────────────────────────────────────────
    //  higherPriority() — helper to compare two heap items
    //  Returns true if item at index a expires SOONER than b
    //  "Higher priority" = expires sooner = smaller date
    // ─────────────────────────────────────────────────────────
    bool higherPriority(int a, int b) const {
        return heap[a].getExpiry() < heap[b].getExpiry();
    }

    // ─────────────────────────────────────────────────────────
    //  heapifyUp() — RECURSIVE
    //  Called AFTER inserting a new item at the END
    //  Bubbles the new item UP until it is in the right place
    //
    //  VISUALIZE: imagine the new item is too urgent (expires soon)
    //  It needs to "bubble up" past its parent toward the root
    //
    //  EXAMPLE:
    //  Before insert of [Kangkong Apr25]:
    //              [Kamatis Apr27]
    //             /
    //      [Sayote Apr30]
    //
    //  After insert at end (index 2):
    //              [Kamatis Apr27]
    //             /               \
    //      [Sayote Apr30]    [Kangkong Apr25]  ← new item, too urgent!
    //
    //  heapifyUp(2) runs:
    //  - parent = (2-1)/2 = 0 → Kamatis Apr27
    //  - Kangkong Apr25 < Kamatis Apr27? YES → SWAP
    //
    //  After swap:
    //              [Kangkong Apr25]   ← now at root, correct!
    //             /               \
    //      [Sayote Apr30]    [Kamatis Apr27]
    //
    //  heapifyUp(0) runs:
    //  - idx == 0 → BASE CASE: stop (reached root)
    // ─────────────────────────────────────────────────────────
    void heapifyUp(int idx) {           // RECURSIVE FUNCTION
        if (idx == 0) return;           // BASE CASE: at root, nothing above, stop

        int parent = (idx - 1) / 2;    // Find parent index using formula

        if (higherPriority(idx, parent)) {      // If current is MORE urgent than parent
            swap(heap[idx], heap[parent]);       // Swap them
            heapifyUp(parent);                  // RECURSIVE CALL: check parent's position
        }
        // If current is NOT more urgent than parent → already in right place, stop
    }

    //  heapifyDown() — RECURSIVE
    //  Called AFTER removing the root (most urgent item)
    //  Pushes the replacement item DOWN until it's in right place
    //
    //  VISUALIZE: after extractMin(), the last item replaces root
    //  It might be less urgent than its children, so it needs
    //  to "sink down" to its correct position
    //
    //  EXAMPLE:
    //  Before extractMin():
    //              [Kangkong Apr25]   ← ROOT removed
    //             /               \
    //      [Kamatis Apr27]    [Sayote Apr30]
    //             \
    //          [Bawang Jun1]
    //
    //  Last item (Bawang Jun1) moves to root:
    //              [Bawang Jun1]      ← wrong! too late, needs to sink
    //             /               \
    //      [Kamatis Apr27]    [Sayote Apr30]
    //
    //  heapifyDown(0) runs:
    //  - left=1 (Kamatis Apr27), right=2 (Sayote Apr30)
    //  - smallest = 1 (Kamatis is more urgent)
    //  - Bawang != smallest → SWAP Bawang and Kamatis
    //
    //  After swap:
    //              [Kamatis Apr27]    ← correct!
    //             /               \
    //      [Bawang Jun1]      [Sayote Apr30]
    //
    //  heapifyDown(1) runs on Bawang's new position:
    //  - no children more urgent → BASE CASE: stop
    void heapifyDown(int idx, int sz) {     // RECURSIVE FUNCTION
        int left    = 2*idx + 1;            // Left child index
        int right   = 2*idx + 2;            // Right child index
        int smallest = idx;                 // Assume current is smallest

        // Check if left child exists AND is more urgent than current smallest
        if (left < sz && higherPriority(left, smallest))
            smallest = left;

        // Check if right child exists AND is more urgent than current smallest
        if (right < sz && higherPriority(right, smallest))
            smallest = right;

        if (smallest != idx) {              // A child is more urgent than current
            swap(heap[idx], heap[smallest]);        // Swap with the more urgent child
            heapifyDown(smallest, sz);      // RECURSIVE CALL: continue sinking down
        }
        // BASE CASE: smallest == idx → current is already more urgent than both
        // children, heap property is satisfied, stop
    }

public:
    ExpiryPriorityQueue() : size_(0) {}
    bool isEmpty() const { return size_ == 0; }
    int  size()    const { return size_; }

    // ─────────────────────────────────────────────────────────
    //  insert() — ADD a product into the heap
    //  Step 1: Place at the END of the array
    //  Step 2: Call heapifyUp() to bubble it to correct position
    // ─────────────────────────────────────────────────────────
    void insert(const Product& p) {
        if (size_ == MAX) throw runtime_error("Priority queue full.");

        heap[size_] = p;        // Step 1: Place new item at the END
        heapifyUp(size_);       // Step 2: Bubble UP to correct position
        size_++;                // Increase count
    }

    // ─────────────────────────────────────────────────────────
    //  extractMin() — REMOVE and RETURN the most urgent item
    //  (the product with the soonest expiry date)
    //  Step 1: Save the root (most urgent)
    //  Step 2: Move the LAST item to the root
    //  Step 3: Call heapifyDown() to sink it to correct position
    // ─────────────────────────────────────────────────────────
    Product extractMin() {
        if (isEmpty()) throw runtime_error("Priority queue empty.");

        Product top = heap[0];          // Step 1: Save root (most urgent)
        heap[0] = heap[--size_];        // Step 2: Move last item to root
                                        //         --size_ decrements FIRST
        heapifyDown(0, size_);          // Step 3: Sink DOWN to correct position

        return top;                     // Return the most urgent product
    }
};


// ============================================================
//  INVENTORY MANAGER
// ============================================================
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

//     YOUR SECOND TOPIC: INPUT HELPER FUNCTIONS              
//     clearInput(), getInt(), getString(), getDate()         

//  clearInput() — cleans up the input buffer
//  When user types something wrong, leftover characters stay
//  in cin's buffer. clearInput() removes them so the next
//  input prompt works correctly.
//
//  cin.clear()  — resets any error flags on cin
//  cin.ignore() — discards characters up to the newline

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//  getInt() — safely gets a whole number from the user
//  Keeps asking until user enters a valid number in range
//
//  Parameters:
//    prompt  — the text shown to the user (e.g. "Quantity")
//    minVal  — minimum allowed value (default: 1)
//    maxVal  — maximum allowed value (default: 99999)
//
//  HOW THE LOOP WORKS:
//  1. Show prompt and try to read an integer
//  2. If cin >> val fails (user typed letters) OR
//     value is out of range → show error, clear input, try again
//  3. If valid → clear leftover newline and return the value
int getInt(const string& prompt, int minVal = 1, int maxVal = 99999) {
    int val;
    while (true) {                                          // keep looping until valid input
        cout << CYAN << "  " << prompt << RESET << ": ";
        if (cin >> val && val >= minVal && val <= maxVal) { // valid input check
            clearInput();
            return val;                                     // exit loop and return value
        }
        cout << RED << "  [ERROR] Enter a number ("
             << minVal << " - " << maxVal << ").\n" << RESET;
        clearInput();                                       // clean up bad input
    }
}

//  getString() — safely gets a text input from the user
//  Also trims leading and trailing spaces automatically
//
//  WHY getline() instead of cin >>?
//  cin >> stops at spaces — "Fried Chicken" would only get "Fried"
//  getline() reads the ENTIRE line including spaces
//
//  TRIMMING SPACES:
//  The while loops at the end remove extra spaces from the
//  front and back of what the user typed
// ─────────────────────────────────────────────────────────────
string getString(const string& prompt) {
    string s;
    cout << CYAN << "  " << prompt << RESET << ": ";
    getline(cin, s);

    // Trim leading spaces (from the front)
    while (!s.empty() && (s.front()==' ' || s.front()=='\t'))
        s.erase(s.begin());

    // Trim trailing spaces (from the back)
    while (!s.empty() && (s.back()==' ' || s.back()=='\t'))
        s.pop_back();

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

//
//       TOPIC: UI HELPER FUNCTIONS                  
//       sectionHeader() and printMenu()                        

// ─────────────────────────────────────────────────────────────
//  sectionHeader() — prints a decorated title box
//  Called at the start of EVERY menu option to show
//  which section the user is currently in
//
//  Parameters:
//    title — the text to display (e.g. "ADD NEW STOCK")
//    color — ANSI color code (default: CYAN)
//
//  HOW IT WORKS:
//  1. Creates a line of 62 '=' characters
//  2. Centers the title by calculating padding spaces
//  3. Prints: top bar, centered title, bottom bar
// ─────────────────────────────────────────────────────────────
void sectionHeader(const string& title, const string& color = CYAN) {
    string bar(62, '=');                                    // 62 equal signs
    int pad = max(0, (int)(60 - title.size()) / 2);        // center padding
    cout << "\n" << color << BOLD
         << "  " << bar << "\n"                            // top bar
         << "  " << string(pad, ' ') << "[ " << title << " ]\n"  // title
         << "  " << bar << RESET << "\n\n";                // bottom bar
}
/ ─────────────────────────────────────────────────────────────
//  printMenu() — prints the main menu screen
//  Called at the START of every loop iteration in main()
//  Shows: program title, status bar, and all menu options
//
//  Parameters:
//    inv — the InventoryManager object (to show live stats)
//
//  STATUS BAR shows live counts:
//    - Items: how many products in inventory
//    - Queue: how many deliveries waiting
//    - Undo history: how many actions can be undone
//    - Today: current date
//
//  NOTE: inv.getItemCount(), inv.getQueueSize(),
//        inv.getUndoSize() are getter functions that return
//        private data from InventoryManager
// ─────────────────────────────────────────────────────────────

// (printMenu shown in comments since it needs InventoryManager)
//
//  void printMenu(const InventoryManager& inv) {
//
//      // Title box
//      cout << "+------------------------------------------------------------+\n";
//      cout << "|         F R E S H   T R A C K   v1.0                      |\n";
//      cout << "|    Perishable Inventory Management System                  |\n";
//      cout << "+------------------------------------------------------------+\n";
//
//      // Status bar — shows live data
//      cout << "Items: "        << inv.getItemCount()  // total products
//           << "   Queue: "     << inv.getQueueSize()  // pending deliveries
//           << "   Undo: "      << inv.getUndoSize()   // undo history size
//           << "   Today: "     << today().toString(); // current date
//
//      // Menu options
//      cout << "[1]  Add New Stock\n";
//      cout << "[2]  Sell / Process Item\n";
//      cout << "[3]  Undo Last Action        (Stack - LIFO)\n";
//      cout << "[4]  Display Inventory (Iterative)\n";
//      cout << "[5]  Display Inventory (Recursive)\n";
//      cout << "[6]  View Expiry Alerts      (Priority Queue)\n";
//      cout << "[7]  Search Product\n";
//      cout << "[8]  Sort Inventory\n";
//      cout << "[9]  Delivery Queue          (Queue - FIFO)\n";
//      cout << "[10] View Undo History\n";
//      cout << "[0]  Exit\n";
//  }


// ╔══════════════════════════════════════════════════════════════╗
// ║      YOUR FOURTH TOPIC: main() FUNCTION                     ║
// ║      The ENTRY POINT and CONTROL CENTER of the program      ║
// ╚══════════════════════════════════════════════════════════════╝
//
//  PROGRAM FLOW (how main works step by step):
//
//  1. Enable ANSI colors on Windows
//  2. Create InventoryManager object
//  3. Clear screen
//  4. Show SPLASH SCREEN (logo + team members)
//  5. Wait for Enter
//  6. Enter the MAIN LOOP (do...while):
//     a. Clear screen
//     b. Print menu
//     c. Read user's choice
//     d. Clear screen again
//     e. Execute the chosen option (switch-case)
//     f. Repeat until user chooses 0 (Exit)
//
//  THE DO...WHILE LOOP:
//  do {
//      ...menu and switch...
//  } while (choice != 0);
//
//  WHY do...while instead of while?
//  Because we want to show the menu AT LEAST ONCE before
//  checking if the user wants to exit. do...while always
//  runs the body FIRST, then checks the condition.

//
//  THE SWITCH-CASE:
//  switch (choice) {
//      case 1:  addStock        → calls inv.addStock()
//      case 2:  sellItem        → calls inv.sellItem()
//      case 3:  undoLastAction  → calls inv.undoLastAction()
//      case 4:  displayIterative→ calls inv.displayAllIterative()
//      case 5:  displayRecursive→ calls inv.displayAllRecursive()
//      case 6:  expiryAlerts    → calls inv.viewExpiryAlerts()
//      case 7:  searchProduct   → calls inv.searchProduct()
//      case 8:  sortInventory   → calls inv.sortInventory()
//      case 9:  deliveryQueue   → calls inv.enqueueDelivery()
//                                       inv.viewDeliveryQueue()
//                                       inv.processDeliveryQueue()
//      case 10: viewUndoStack   → calls inv.viewUndoStack()
//      case 0:  Exit            → loop ends
//      default: invalid input   → show error
//  }
//
//  IMPORTANT: every case ends with pressEnter() and break
//  pressEnter() — waits so user can read output
//  break        — exits the switch and goes back to loop top
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

// ============================================================
//  MAIN
// ============================================================
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

        // ======================================================
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

        // ======================================================
        case 2: {
            sectionHeader("SELL / PROCESS ITEM", YELLOW);
            string name = getString("Product name");
            int    qty  = getInt("Quantity to sell");
            inv.sellItem(name, qty);
            pressEnter();
            break;
        }

        // ======================================================
        case 3: {
            sectionHeader("UNDO LAST ACTION", CYAN);
            cout << DIM << "  Data structure: Stack (LIFO)\n"
                 << "  The most recent action is always reversed first.\n\n" << RESET;
            inv.undoLastAction();
            pressEnter();
            break;
        }

        // ======================================================
        case 4: {
            sectionHeader("DISPLAY INVENTORY  (Iterative)", CYAN);
            cout << DIM << "  Algorithm: for-loop traversal — visits each item one by one.\n" << RESET;
            inv.displayAllIterative();
            pressEnter();
            break;
        }

        // ======================================================
        case 5: {
            sectionHeader("DISPLAY INVENTORY  (Recursive)", MAGENTA);
            cout << DIM << "  Algorithm: recursive function — calls itself with index+1\n"
                 << "  until the base case (index == total items) is reached.\n" << RESET;
            inv.displayAllRecursive();
            pressEnter();
            break;
        }

        // ======================================================
        case 6: {
            sectionHeader("EXPIRY ALERTS", RED);
            cout << DIM << "  Data structure: Priority Queue (Min-Heap)\n"
                 << "  heapifyUp/Down are implemented recursively.\n"
                 << "  Items with the nearest expiry date appear first.\n\n" << RESET;
            inv.viewExpiryAlerts();
            pressEnter();
            break;
        }

        // ======================================================
        case 7: {
            sectionHeader("SEARCH PRODUCT", BLUE);
            string query = getString("Product name to search");
            inv.searchProduct(query);
            pressEnter();
            break;
        }

        // ======================================================
        case 8: {
            sectionHeader("SORT INVENTORY", CYAN);
            cout << "  " << GREEN << "[1]" << RESET << " Sort by expiry date (nearest first)\n";
            cout << "  " << GREEN << "[2]" << RESET << " Sort by product name (A-Z)\n\n";
            int opt = getInt("Choice", 1, 2);
            inv.sortInventory(opt);
            pressEnter();
            break;
        }

        // ======================================================
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

        // ======================================================
        case 10: {
            sectionHeader("UNDO HISTORY  (Stack)", CYAN);
            inv.viewUndoStack();
            pressEnter();
            break;
        }

        // ======================================================
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
//  viewExpiryAlerts() in InventoryManager
//  This uses your Priority Queue to display alerts in order

//  NOTE: In the real code this is inside InventoryManager.
//  Shown here separately so you can study it clearly.

//  void viewExpiryAlerts() {
//      if (itemCount == 0) { cout << "Inventory is empty.\n"; return; }
//
//      // Create a fresh Priority Queue
//      ExpiryPriorityQueue pq;
//
//      // Insert ALL inventory items into the Priority Queue
//      for (int i = 0; i < itemCount; i++) pq.insert(items[i]);
//
//      // Extract one by one — each extractMin() gives the
//      // MOST URGENT remaining item (soonest expiry)
//      int rank = 0;
//      while (!pq.isEmpty()) {
//          Product p = pq.extractMin();   // Always gets most urgent
//          int days  = daysUntilExpiry(p.getExpiry());
//
//          // Color code based on urgency
//          // days < 0  → RED    → EXPIRED
//          // days <= 3 → YELLOW → NEAR EXPIRY
//          // days > 3  → GREEN  → GOOD
//
//          cout << (++rank) << ". " << p.getName()
//               << "  expiry: " << p.getExpiry().toString()
//               << "  (" << days << " days)\n";
//      }
//  }