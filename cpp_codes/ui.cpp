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
                 << setw(😎  << p.getQuantity()
                 << DIM << setw(14) << p.getBatch().toString()
                 << setw(14) << p.getExpiry().toString() << RESET
                 << col << BOLD << statusTxt << RESET
                 << DIM << " (" << days << " day" << (abs(days)==1?"":"s") << ")" << RESET << "\n";
        }
        cout << "  " << DIM << string(74, '-') << RESET << "\n";
    }