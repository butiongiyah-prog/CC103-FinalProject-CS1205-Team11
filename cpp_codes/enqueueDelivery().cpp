//  enqueueDelivery() — adds one product to the queue
//  void enqueueDelivery(const Product& p) {
//      deliveryQueue.enqueue(p);
//      cout << "[QUEUE] '" << p.getName()
//           << "' added. Queue size: "
//           << deliveryQueue.size() << "\n";
//  }

//  viewDeliveryQueue() — shows queue WITHOUT processing
//  void viewDeliveryQueue() const {
//      deliveryQueue.displayAll();
//  }

//  processDeliveryQueue() — empties queue into inventory FIFO
//  void processDeliveryQueue() {
//      if (deliveryQueue.isEmpty()) { return; }
//      int processed = 0;
//      while (!deliveryQueue.isEmpty()) {
//          Product p = deliveryQueue.dequeue(); // OLDEST first
//          addStock(p.getName(), p.getQuantity(),
//                   p.getBatch(), p.getExpiry());
//          processed++;
//      }
//      cout << "[OK] " << processed << " delivery(ies) processed.\n";
//  }