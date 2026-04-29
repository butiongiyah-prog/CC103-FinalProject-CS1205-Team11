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