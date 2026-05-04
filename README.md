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

- Vendors forget which products are about to expire and end up throwing them away or worse, selling expired goods
- When deliveries arrive at different times, it is hard to keep track of which batch came first
- If a vendor records the wrong quantity by mistake, there is no way to fix it
- There is no easy way to know which items need to be sold urgently

Our program, FreshTrack, solves all of these problems in a simple console interface that does not require any technical knowledge to use. The vendor just picks a number from the menu and the system handles everything.


✨ Features
Feature  Description
➕ Add StockRecord incoming goods with batch and expiry dates
💰 Sell ItemLog sales and update inventory quantities
↩️ UndoReverse the last add or sell action instantly
🚚 Process DeliveriesQueue incoming deliveries and process them in order
🔴 Expiry AlertsView products sorted from most urgent to least urgent
🔍 SearchCase-insensitive product name search
📊 Sort InventorySort by expiry date or product name
🎨 Color-coded UIRed = expired · Yellow = near expiry · Green = good

🏗️ Data Structures Used
Stack — Undo Mechanism (LIFO)

What it is:
A Stack is a data structure that follows **Last In, First Out (LIFO)**. Think of it like a stack of plates — you can only add or remove from the top. The last item you put in is always the first one to come out.

In our program, we built the Stack manually using a fixed array of 100 `Action` records with a `top` variable that tracks the topmost item.

Why we used it:
Every time the vendor adds stock or sells an item, we save the action details (product name, quantity, type, dates) and push it onto the Stack. When the vendor makes a mistake and selects Undo, we pop the most recent action and reverse it. We chose Stack because Undo always reverses the most recent action first, which is exactly what LIFO does. No other data structure fits this behavior as naturally as a Stack.



Queue — Delivery Intake (FIFO)

What it is:
A Queue is a data structure that follows First In, First Out (FIFO). Think of it like a line of customers at a store — the first person in line is always the first one served.

In our program, we built the Queue using a circular array of 50 `Product` slots with `front`, `rear`, and `count` variables. The circular design reuses empty slots using modulo arithmetic (`% MAX`) so no space is wasted.

Why we used it:
When multiple deliveries arrive, they should be processed in the order they came in — not randomly. By enqueuing each delivery as it arrives and dequeuing them one by one, we make sure the earliest delivery is always recorded first. This matches the real-world flow of delivery intake for market vendors.



Priority Queue — Expiry Alerts (Min-Heap)

What it is:
A Priority Queue is a data structure where each item has a priority and the highest priority item is always served first. We implemented it as a Min-Heap using a fixed array of 200 `Product` slots. In our case, the product with the soonest expiry date always rises to the top.

The two key operations — `heapifyUp` and `heapifyDown` — are both implemented **recursively**.

Why we used it:
Vendors need to know which products are about to expire so they can sell them first or remove them before they go bad. If we just used a regular list, we would have to sort it every time. The min-heap keeps the most urgent item at the top automatically. When the vendor views expiry alerts, the items are always shown from most urgent to least urgent without any extra sorting step.


⚙️ Algorithms
Stack — Undo (LIFO)
- When the user adds stock or sells an item, an `Action` struct is created with: `type` (ADD or SELL), `productName`, `quantity`, `batchDate`, `expiryDate`
- The action is pushed onto the `InventoryStack` by doing `data[++top] = a`
- When the user picks Undo, the top action is popped by returning `data[top--]`
- If the popped action was ADD → find the product and subtract the quantity (or remove it if qty drops to zero)
- If the popped action was SELL → find the product and restore the quantity (or recreate the product if it was already removed)

Queue — Delivery Intake (FIFO)
- When a delivery arrives, `enqueue()` places the product at `data[(rear+1) % MAX]` and increments `count`
- When deliveries are processed, `dequeue()` takes from `data[front]`, moves `front = (front+1) % MAX`, and decrements `count`
- A `while` loop runs `dequeue()` repeatedly until the queue is empty
- For each dequeued product, `addStock()` is called — if it already exists in inventory the quantity is merged, if not a new product entry is created
- The circular array makes sure no space is wasted even after many enqueue and dequeue cycles

Priority Queue — Expiry Alerts (Recursive Min-Heap)

Insert + heapifyUp:
- New product is placed at the end of the heap array at index `size_`
- `heapifyUp(idx)` is called recursively
- Base case: `if (idx == 0) return` — reached the root, nothing above it, stop
- Recursive step: compare with parent at `(idx-1)/2` — if current expires sooner, swap and call `heapifyUp(parent)`

ExtractMin + heapifyDown:
- Root (most urgent product) is saved
- Last item in heap replaces the root, `size_` is decremented
- `heapifyDown(0, size_)` is called recursively
- Base case: `if (smallest == idx)` — current is already more urgent than both children, stop
- Recursive step: find the more urgent child (left or right), swap, and call `heapifyDown(smallest, sz)`

Display All — Iterative
- A `for` loop runs from `i = 0` to `i < itemCount`
- Each iteration calls `items[i].display(i)` to print that row
- Stops naturally when `i` reaches `itemCount`

Display All — Recursive
- `displayAllRecursive()` calls `displayRecursiveHelper(0)` to start
- Base case: `if (idx == itemCount) return` — all items printed, stop
- Recursive step: print `items[idx]`, then call `displayRecursiveHelper(idx + 1)`

Search — Linear Search (Iterative)
- Convert the user's query to lowercase
- Loop through all `itemCount` products
- Convert each product name to lowercase and use `string::find()` to check if the query appears anywhere in the name
- Display all matching products; show error if none found

Sort — Bubble Sort (Iterative)
- Two nested `for` loops compare adjacent products
- Sort by expiry date: swap if next product expires sooner than current
- Sort by name: swap if current name comes after next name alphabetically
- After sorting, display the updated inventory

⚖️ Iterative vs Recursive
Iterative vs Recursive Comparison

Which is faster?

Iterative is faster. When we use a for-loop, the program just increments a counter and accesses memory directly. When we use recursion, every function call adds a new frame to the call stack, which takes extra time and memory. For simple tasks like displaying a list or searching through inventory, iterative will always be faster.

Which is easier to understand?

It depends on what you are doing. For simple list traversal like displaying all products or searching, iterative is easier because you can just read the loop from top to bottom and see exactly what it does.

However, for heap operations like `heapifyUp` and `heapifyDown`, **recursive is actually easier to understand** because the heap is a tree-like structure. Each recursive call handles one level of the tree and passes the rest to itself. If we wrote heapify iteratively, we would need a manual loop with complex index calculations that is harder to follow.

Summary Table

 Feature             | Iterative                      | Recursive 
                     |                                |                                  
 Speed               |  Faster                        | Slower due to function call overhead 
 Memory              |  Less — no extra stack frames  | More — each call uses call stack     
 Stack overflow risk |  None                          | Possible for very large input        
 Best for            |  Display, search, sort, queue  | Heap operations, tree traversal      
 Readability         |  Easier for simple loops       |  Easier for tree/heap structures     

In FreshTrack, we used iterative for the menu loop, for-loop display, bubble sort, queue processing, and linear search. We used recursive for `heapifyUp`, `heapifyDown`, and the alternate display function.

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




Acknowledgements
We would like to thank the following people and resources that helped us complete this project:

👨‍🏫 Our CC 103 instructor for guiding us throughout the subject and teaching us the data structures and algorithms concepts applied in this project
🏫 Batangas State University for providing the academic environment and the opportunity to work on meaningful projects that connect classroom learning to real-world problems
🥬 The fruit and vegetable vendors in Philippine public markets whose daily challenges inspired the problem FreshTrack aims to solve — seeing how much they rely on memory and manual tracking made us want to build something that could genuinely help them
