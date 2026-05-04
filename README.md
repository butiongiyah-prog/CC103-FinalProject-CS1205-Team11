🥬 FreshTrack — Perishable Inventory Management System

CC 103 — Data Structures and Algorithms
Batangas State University | AY 2025–2026

👥 Team
Student ID      Name                         Role  
25-09524        Buendia, Beverly Rose T.     💻 Programmer
25-03580        Butiong, Iya Elaine M.       💻 Programmer
25-04566        Gabaldon, Lance Gencedric B.

Project Title
🥬 FreshTrack — Perishable Inventory Management System

A C++ console application that helps small market vendors in the Philippines manage their perishable goods inventory — tracking stock, expiry alerts, deliveries, and undo operations through a simple menu-driven interface.

📋 Table of Contents

 The Problem
 Features
 Data Structures Used
 Algorithms
 Iterative vs Recursive
 Design Decisions
 How to Run
 Acknowledgements


🧩 The Problem
Small market vendors — especially fruit and vegetable sellers in Philippine public markets — manage their perishable goods manually. This leads to serious problems:

🗑️ Vendors forget which products are about to expire and end up throwing them away — or worse, selling expired goods
📦 When deliveries arrive at different times, it is hard to track which batch came first
✏️ If a vendor records the wrong quantity by mistake, there is no way to fix it
⏰ There is no easy way to know which items need to be sold urgently

FreshTrack solves all of these problems in a simple console interface that does not require any technical knowledge. The vendor just picks a number from the menu and the system handles everything.

✨ Features
FeatureDescription➕ Add StockRecord incoming goods with batch and expiry dates💰 Sell ItemLog sales and update inventory quantities↩️ UndoReverse the last add or sell action instantly🚚 Process DeliveriesQueue incoming deliveries and process them in order🔴 Expiry AlertsView products sorted from most urgent to least urgent🔍 SearchCase-insensitive product name search📊 Sort InventorySort by expiry date or product name🎨 Color-coded UIRed = expired · Yellow = near expiry · Green = good

🏗️ Data Structures Used
📚 Stack — Undo Mechanism (LIFO)
A Stack follows Last In, First Out — like a stack of plates. The last item added is always the first one removed.
Implementation: Fixed array of 100 Action records with a top pointer.
Every add/sell → push Action onto stack
User selects Undo → pop most recent Action → reverse it
Why Stack? Undo always reverses the most recent action first — that is exactly what LIFO does. No other data structure fits this behavior as naturally.

🚶 Queue — Delivery Intake (FIFO)
A Queue follows First In, First Out — like a line of customers at a store. The first person in line is always the first one served.
Implementation: Circular array of 50 Product slots using front, rear, and count variables. Uses % MAX modulo arithmetic so no space is wasted.
Delivery arrives → enqueue at (rear+1) % MAX
Process deliveries → dequeue from front → call addStock()
Why Queue? Deliveries must be processed in the order they arrived. FIFO ensures the earliest delivery is always recorded first.

⚡ Priority Queue — Expiry Alerts (Min-Heap)
A Priority Queue always serves the highest-priority item first. Our min-heap keeps the product with the soonest expiry date at the root at all times.
Implementation: Fixed array of 200 Product slots. heapifyUp and heapifyDown are both implemented recursively.
Insert product → place at end → heapifyUp (bubble toward root)
View alerts   → extractMin   → heapifyDown (restore heap order)
Why Min-Heap? A regular list would need re-sorting every time. The heap keeps the most urgent item at the top automatically — no extra sorting step needed.

⚙️ Algorithms
📚 Stack — Undo (LIFO)
1. User adds stock or sells an item
2. Action struct is created: { type, productName, quantity, batchDate, expiryDate }
3. Pushed onto InventoryStack:  data[++top] = action
4. On Undo: popped via         data[top--]
   - If ADD  → subtract quantity (remove product if qty drops to 0)
   - If SELL → restore quantity (recreate product if it was already removed)
🚶 Queue — Delivery Intake (FIFO)
1. Delivery arrives → enqueue at data[(rear+1) % MAX], increment count
2. Process button pressed → while queue not empty:
     dequeue from data[front], move front = (front+1) % MAX
     call addStock() for each product
     → merge quantity if product exists, create new entry if not
3. Circular design reuses empty slots — no wasted space
⚡ Priority Queue — Recursive Min-Heap
Insert + heapifyUp:
Base case:      if (idx == 0) return          ← reached root, nothing above, stop
Recursive step: compare with parent at (idx-1)/2
                if current expires sooner → swap → heapifyUp(parent)
ExtractMin + heapifyDown:
Base case:      if (smallest == idx) return   ← already more urgent than both children, stop
Recursive step: find the more urgent child (left or right)
                swap → heapifyDown(smallest, size)
🔍 Search — Linear Search (Iterative)
1. Convert query to lowercase
2. Loop through all itemCount products
3. Convert each name to lowercase → check with string::find()
4. Display all matches; show error message if none found
📊 Sort — Bubble Sort (Iterative)
Two nested loops compare adjacent products.
→ Sort by expiry date: swap if next expires sooner than current
→ Sort by name:        swap if current name comes after next alphabetically
🖥️ Display — Iterative vs Recursive
cpp// Iterative
for (int i = 0; i < itemCount; i++) items[i].display(i);

// Recursive
void displayRecursiveHelper(int idx) {
    if (idx == itemCount) return;    // base case
    items[idx].display(idx);
    displayRecursiveHelper(idx + 1); // recursive step
}

⚖️ Iterative vs Recursive
Feature✅ Iterative🔁 RecursiveSpeedFaster — direct memory accessSlower due to function call overheadMemoryLess — no extra stack framesMore — each call uses the call stackStack overflow riskNonePossible for very large inputBest forDisplay, search, sort, queueHeap operations, tree traversalReadabilityEasier for simple linear loopsEasier for tree/heap structures
Used in FreshTrack
ApproachWhereIterativeMenu loop · for-loop display · bubble sort · queue processing · linear searchRecursiveheapifyUp · heapifyDown · alternate display function

Why recursive heapify? The heap is a tree-like structure. Each recursive call handles one level and passes the rest to itself. For our inventory size of up to 200 items, the maximum recursion depth is only about 8 levels (log₂(200) ≈ 8), so there is no risk of stack overflow.


🧠 Design Decisions
1. 🔧 Built data structures from scratch (no STL)
We could have used stack, queue, and priority_queue from the C++ Standard Library — but we built them manually using arrays.
Why: Our subject requires us to demonstrate understanding at the memory level, not just call a built-in function.

Trade-off: Fixed sizes (Stack: 100 · Queue: 50 · Heap: 200). A production app would use dynamic memory or STL containers.


2. 🔄 Circular array for the Queue
A linear array wastes space — after many enqueue and dequeue operations, the front becomes empty but cannot be reused. A circular array wraps front and rear using % MAX so every slot can be reused.

Trade-off: Index math is a little more complex, but memory efficiency is significantly better for repeated use.


3. 🌳 Recursive heapify
heapifyUp and heapifyDown are implemented recursively because the recursive version directly mirrors the structure of the heap tree — each call handles one level and delegates the rest to itself.

Trade-off: For much larger datasets, an iterative version would be safer. For our max of 200 items, the recursion depth is only ~8 levels.


4. 🎨 ANSI color codes for the terminal UI
Expired items appear in red, near-expiry items in yellow, and good items in green. system("chcp 65001") is called at startup to enable UTF-8 and ANSI on Windows.

Trade-off: Older Windows CMD without ANSI support may not render colors. Works correctly on VS Code terminal and most modern terminals.


5. 🔡 Case-insensitive search
Both the query and product names are lowercased via transform() before comparison — so kamatis, Kamatis, and KAMATIS all return the same result.

Trade-off: Small extra cost of transform() per search — completely negligible for typical vendor inventory size.


🚀 How to Run
bash# Compile
g++ -o freshtrack main.cpp

# Run
./freshtrack

Requirements: C++11 or later · Works on Windows (VS Code terminal recommended), Linux, and macOS.


🙏 Acknowledgements
We would like to thank the following people and resources that helped us complete this project:

👨‍🏫 Our CC 103 instructor for guiding us throughout the subject and teaching us the data structures and algorithms concepts applied in this project
🏫 Batangas State University for providing the academic environment and the opportunity to work on meaningful projects that connect classroom learning to real-world problems
🥬 The fruit and vegetable vendors in Philippine public markets whose daily challenges inspired the problem FreshTrack aims to solve — seeing how much they rely on memory and manual tracking made us want to build something that could genuinely help them
