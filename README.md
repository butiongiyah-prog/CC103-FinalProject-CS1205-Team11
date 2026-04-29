FreshTrack — Perishable Inventory Management System

CC 103 — Data Structures and Algorithms
Batangas State University | AY 2025–2026

👥 Team Members


25-09524  BUENDIA, BEVERLY ROSE T. 
25-03580  BUTIONG, IYA ELAINE M. 
25-04566  GABALDON, LANCE GENCEDRIC B.

1. Project Title

FreshTrack — Perishable Inventory Management System**

FreshTrack is a C++ console application that helps small market vendors in the Philippines manage their perishable goods inventory. It uses a menu-driven interface where vendors just type a number and press Enter to perform actions like adding stock, selling items, viewing expiry alerts, and undoing mistakes.

2. Problem Description

What problem are we solving?

Small market vendors — especially fruit and vegetable sellers in Philippine public markets — manage their perishable goods manually. This leads to serious problems:

- Vendors forget which products are about to expire and end up throwing them away or worse, selling expired goods
- When deliveries arrive at different times, it is hard to keep track of which batch came first
- If a vendor records the wrong quantity by mistake, there is no way to fix it
- There is no easy way to know which items need to be sold urgently

Our program, FreshTrack, solves all of these problems in a simple console interface that does not require any technical knowledge to use. The vendor just picks a number from the menu and the system handles everything.


3. Data Structures Used
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

4. Algorithm Explanation

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
- **Base case:** `if (idx == itemCount) return` — all items printed, stop
- **Recursive step:** print `items[idx]`, then call `displayRecursiveHelper(idx + 1)`

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

5. Iterative vs Recursive Comparison

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



6. Design Decisions

Decision 1 — We built the data structures from scratch instead of using STL

We could have used `stack`, `queue`, and `priority_queue` from the C++ Standard Library but we chose to build them manually using arrays. We did this because our subject requires us to show that we understand how these data structures actually work at the memory level — not just how to call a built-in function.

Trade-off: Our implementation has fixed sizes (Stack: 100, Queue: 50, Heap: 200). This means there is a hard limit on how many items can be stored. In a real production app, we would use dynamic memory or STL containers instead.

Decision 2 — Circular Array for the Queue

We used a circular array for `DeliveryQueue` instead of a simple linear array. A linear array would waste space — after many enqueue and dequeue operations, the front of the array becomes empty but cannot be reused. With a circular array, `front` and `rear` wrap around using `% MAX` so every slot can be reused.

Trade-off: The index math is a little more complex to write and understand, but it is much more memory efficient for repeated use.

Decision 3 — Recursive heapify

We implemented `heapifyUp` and `heapifyDown` recursively instead of iteratively. We chose this because the recursive version directly mirrors the structure of the heap tree — each call handles one level and delegates the rest to itself. It is also cleaner and shorter to write.

Trade-off: For our inventory size of up to 200 items, the maximum recursion depth is only about 8 levels (log₂(200) ≈ 8), so there is no risk of stack overflow. For much larger datasets, an iterative version would be safer.

Decision 4 — ANSI Color Codes for the Terminal UI

We added color to the terminal output using ANSI escape codes. Expired items appear in red, near-expiry items in yellow, and good items in green. This makes it much easier for vendors to spot problems at a glance without having to read every single row.

Trade-off: ANSI colors work well on VS Code's terminal and most modern terminals. On some older Windows CMD windows without ANSI support, the colors may not render. We added `system("chcp 65001")` at startup to enable UTF-8 and ANSI on Windows.

Decision 5 — Case-Insensitive Search

We made the search function case-insensitive by converting both the query and product names to lowercase before comparing. This way, typing "kamatis", "Kamatis", or "KAMATIS" all return the same result. This is important because not all vendors type consistently.

Trade-off: There is a small extra cost of calling `transform()` on every item during each search, but for the small number of products a market vendor would have, this cost is completely negligible.

7. Acknowledgement

We would like to thank the following people and resources that helped us complete this project:

First and foremost, we thank our instructor for guiding us throughout this subject and for teaching us the concepts of data structures and algorithms that we applied in this project. Without the lessons learned in CC 103, we would not have been able to design and implement FreshTrack.

We also thank Batangas State University for providing us with the academic environment and the opportunity to work on meaningful projects that connect classroom learning to real-world problems.

We extend our gratitude to the **fruit and vegetable vendors** in Philippine public markets whose daily challenges inspired the problem that FreshTrack aims to solve. Seeing how much they rely on memory and manual tracking made us want to build something that could genuinely help them.

Lastly, we thank our classmates and friends for their encouragement, feedback, and for helping us test the program during development.

This project was a great learning experience for all of us. It taught us not just how to code data structures but also how to think about real problems and design solutions for them.

---

> "Ingatan ang inyong paninda."
> — FreshTrack v1.0 | CC 103 — Data Structures and Algorithms | BatStateU AY 2025–2026
