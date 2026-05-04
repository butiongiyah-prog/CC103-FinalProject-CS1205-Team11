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