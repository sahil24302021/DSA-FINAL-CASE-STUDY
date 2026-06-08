# CodeClean — Automated Code Refactoring & Dependency Tool

**Problem Statement #35**

| | |
|---|---|
| **University** | ITM Skills University |
| **Program** | B.Tech CSE 2025-29 |
| **Subject** | Data Structures & Algorithms with C++ |
| **Semester** | II |

---

## 2.1 Project Title

**CodeClean — Automated Code Refactoring & Dependency Tool**

A menu-based C++ program that manages source code files, variables, dependencies, and refactoring operations using 8 core data structures and algorithms.

---

## 2.2 Problem Statement

CodeClean is a developer tool that analyzes large codebases to find unused files, untangle dependencies, and suggest optimizations. As projects grow to thousands of files, the current system fails:

- Tracking variable definitions and their scope across files is slow
- When refactoring introduces a bug, there is no reliable undo
- Files queue up for quality checks randomly
- Verifying that every function and keyword is correctly defined requires scanning everything
- The system cannot rank functions by complexity
- There is no map showing how files depend on each other
- It is impossible to trace bugs through deeply nested logic
- No way to pick the most efficient code alternative

The new system needs fast variable definition search, reliable code change rollback, sequential file inspection queuing, quick syntax and keyword verification, sorting functions by complexity score, a dependency map showing how files and modules connect, the fastest path to trace bugs through nested logic, and smart optimization that replaces slow code with faster alternatives.

---

## 2.3 Objectives

1. Implement 8 data structures/algorithms to solve 8 real-world code management problems
2. Demonstrate correct usage of HashMap, Stack, Queue, HashSet, Merge Sort, Graph, BFS, and Priority Queue
3. Justify why each data structure was chosen over alternatives
4. Provide time and space complexity analysis for every operation
5. Build a clean, interactive menu-based C++ program with pre-loaded sample data

---

## 2.4 System Overview / Architecture

```
                    ┌────────────────────────┐
                    │       MAIN MENU        │
                    │    (User Interface)     │
                    └───────────┬────────────┘
                                │
            ┌───────────────────┼───────────────────┐
            │                   │                   │
    ┌───────┴───────┐   ┌──────┴──────┐   ┌───────┴───────┐
    │ Variable      │   │ Code Undo   │   │ Inspection    │
    │ Tracker       │   │ (Stack)     │   │ Queue         │
    │ (HashMap)     │   │             │   │ (Queue)       │
    └───────────────┘   └─────────────┘   └───────────────┘
            │                   │                   │
    ┌───────┴───────┐   ┌──────┴──────┐   ┌───────┴───────┐
    │ Syntax        │   │ Complexity  │   │ Code Map      │
    │ Checker       │   │ Sorter      │   │ (Graph +      │
    │ (HashSet/Map) │   │ (Merge Sort)│   │  Adj List)    │
    └───────────────┘   └─────────────┘   └───────────────┘
            │                   │                   │
    ┌───────┴───────┐   ┌──────┴──────┐            │
    │ Deep Trace    │   │ Optimizer   │            │
    │ (BFS)         │   │ (Min-Heap)  │            │
    └───────────────┘   └─────────────┘            │
                                                    │
                    ┌───────────────────────────────┘
                    │ Cycle Detection (DFS)
                    └───────────────────────
```

**Architecture Choices:**
- **Single-file design** for simplicity and portability
- **Global data structures** (HashMap, Stack, Queue, Graph, Heap) accessible by all functions
- **Menu-driven** interaction with input validation
- **Pre-loaded sample data** so features can be demonstrated immediately
- **Color-coded output** using ANSI escape codes for better readability

---

## 2.5 Data Structures and Algorithms Used

| # | Feature | Data Structure | Algorithm | Why This DS? |
|---|---------|---------------|-----------|-------------|
| 1 | Variable Tracker | `unordered_map` (Hash Map) | Hash function | O(1) lookup — fastest for exact key search |
| 2 | Code Undo | `stack` (Two stacks: undo + redo) | LIFO push/pop | Undo is LIFO: most recent change undone first |
| 3 | Inspection Queue | `queue` | FIFO enqueue/dequeue | Fair sequential processing in arrival order |
| 4 | Syntax Checker | `unordered_set` + `unordered_map` | Hash function | O(1) to check if function/keyword exists |
| 5 | Complexity Sorter | `vector` + custom Merge Sort | Divide and conquer | Guaranteed O(n log n), stable, no worst case |
| 6 | Code Map | `unordered_map<string, vector<string>>` (Adjacency List) | DFS for cycle detection | Graph is natural for dependencies. Adj List: O(V+E) space |
| 7 | Deep Trace | `queue` (used by BFS) | BFS (Breadth-First Search) | BFS guarantees shortest path in unweighted graph |
| 8 | Optimizer | `priority_queue` (Min-Heap) | Heap extract-min | Always gives lowest cost option in O(log n) |

### Why These Over Alternatives?

| Feature | Chosen | Alternative | Why Not Alternative? |
|---------|--------|-------------|---------------------|
| Variable Tracker | HashMap O(1) | Array O(n) | Array requires scanning all elements |
| Variable Tracker | HashMap O(1) | BST O(log n) | HashMap is faster than BST for exact search |
| Code Undo | Stack | Queue | Queue would undo oldest first — wrong order! |
| Inspection Queue | Queue | Stack | Stack would check last file first — unfair |
| Syntax Check | HashSet O(1) | Array O(n) | Array needs linear scan every time |
| Complexity Sort | Merge Sort O(n log n) | Bubble Sort O(n²) | Bubble Sort too slow for large data |
| Complexity Sort | Merge Sort O(n log n) | Quick Sort O(n log n) | QuickSort has O(n²) worst case |
| Code Map | Adjacency List O(V+E) | Adjacency Matrix O(V²) | Matrix wastes space for sparse graphs |
| Deep Trace | BFS | DFS | DFS doesn't guarantee shortest path |
| Optimizer | Min-Heap O(log n) | Sorted Array O(n) | Array needs re-sorting after every insert |

---

## 2.6 Implementation Approach

### Step 1: Define Data Structures
Each feature uses a specific C++ STL container:
```cpp
unordered_map<string, VariableInfo> variableMap;    // Feature 1
stack<CodeChange> undoStack, redoStack;              // Feature 2
queue<FileEntry> inspectionQueue;                    // Feature 3
unordered_set<string> functionSet;                   // Feature 4
vector<FunctionScore> functionList;                  // Feature 5
unordered_map<string, vector<string>> dependencyGraph; // Feature 6
// BFS uses queue internally                        // Feature 7
priority_queue<CodeOption, vector<CodeOption>, greater<>> optimizerHeap; // Feature 8
```

### Step 2: Implement Operations
Each feature has Add, Search/Process, and Display operations.

### Step 3: Merge Sort (Custom Implementation)
```cpp
void mergeSort(vector<FunctionScore>& arr, int left, int right) {
    if (left >= right) return;           // Base case
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);           // Sort left half
    mergeSort(arr, mid + 1, right);      // Sort right half
    merge(arr, left, mid, right);        // Merge sorted halves
}
```

### Step 4: BFS for Shortest Path
```cpp
void deepTrace() {
    queue<string> bfsQueue;
    bfsQueue.push(start);
    while (!bfsQueue.empty()) {
        string current = bfsQueue.front();
        bfsQueue.pop();
        // Explore neighbors level by level
        for (auto& neighbor : dependencyGraph[current]) {
            bfsQueue.push(neighbor);
        }
    }
}
```

### Step 5: DFS for Cycle Detection
```cpp
bool hasCycleDFS(node, visiting, visited) {
    visiting.insert(node);              // Mark as "currently exploring"
    for (auto& neighbor : graph[node]) {
        if (visiting.count(neighbor))   // Found back edge = CYCLE!
            return true;
        if (!visited.count(neighbor))
            if (hasCycleDFS(neighbor))
                return true;
    }
    visiting.erase(node);               // Backtrack
    visited.insert(node);               // Mark as "done"
    return false;
}
```

---

## 2.7 Time and Space Complexity Analysis

| Operation | Data Structure | Time Complexity | Space Complexity |
|-----------|---------------|-----------------|-----------------|
| Add variable | HashMap | O(1) amortized | O(n) |
| Search variable | HashMap | O(1) average | O(1) |
| Push change | Stack | O(1) | O(n) |
| Undo/Redo | Stack | O(1) | O(1) per operation |
| Enqueue file | Queue | O(1) | O(n) |
| Dequeue file | Queue | O(1) | O(1) per operation |
| Register function | HashSet | O(1) amortized | O(n) |
| Check syntax | HashSet/Map | O(1) average | O(1) |
| Add complexity | Vector | O(1) amortized | O(n) |
| Sort functions | Merge Sort | **O(n log n)** | O(n) extra |
| Add dependency | Graph (Adj List) | O(1) | O(V + E) |
| Show dependencies | Graph traversal | O(V + E) | O(V) |
| Cycle detection | DFS | O(V + E) | O(V) |
| Trace path (BFS) | BFS with Queue | **O(V + E)** | O(V) |
| Add option | Min-Heap | O(log n) | O(n) |
| Get best option | Min-Heap | **O(log n)** | O(1) |

Where: n = number of elements, V = vertices (files), E = edges (dependencies)

---

## 2.8 Execution Steps

### Prerequisites
- C++17 compatible compiler (g++ or clang++)
- Terminal/Command Prompt

### How to Compile and Run

```bash
# Step 1: Open terminal and navigate to project folder
cd ~/Desktop/DSA-CASE-STUDY

# Step 2: Compile
g++ -std=c++17 -o codeclean codeclean.cpp

# Step 3: Run
./codeclean
```

### Menu Options
| Option | What It Does | DSA Demonstrated |
|--------|-------------|-----------------|
| 1 | Add a variable to the tracker | HashMap insert |
| 2 | Search for a variable by name | HashMap lookup |
| 3 | Display all tracked variables | HashMap iteration |
| 4 | Record a code change | Stack push |
| 5 | Undo last change | Stack pop (LIFO) |
| 6 | Redo undone change | Second Stack pop |
| 7 | View change history | Stack traversal |
| 8 | Add file to inspection queue | Queue enqueue |
| 9 | Process next file in queue | Queue dequeue (FIFO) |
| 10 | View files waiting in queue | Queue traversal |
| 11 | Register a function name | HashSet insert |
| 12 | Check if function/keyword exists | HashSet/HashMap lookup |
| 13 | View all registered functions | HashSet iteration |
| 14 | Add function with complexity score | Vector push_back |
| 15 | Sort and rank all functions | Merge Sort |
| 16 | Add file dependency | Graph: add edge |
| 17 | Show full dependency map + cycles | Graph traversal + DFS |
| 18 | Trace shortest bug path | BFS |
| 19 | Add optimization option | Min-Heap insert |
| 20 | Get best (cheapest) option | Min-Heap extract-min |
| 21 | View all options ranked by cost | Heap traversal |
| 22 | Dashboard showing all DS stats | Summary |

---

## 2.9 Sample Inputs and Outputs

### Example 1: Variable Search (HashMap — O(1))
```
  Enter choice: 2

  Enter variable name to search: userId

  ✓ Found! (Hash lookup: O(1))
    Variable : userId
    Type     : int
    File     : login.cpp
    Line     : 15
    Scope    : global
```

### Example 2: Undo Change (Stack — LIFO)
```
  Enter choice: 5

  ✓ Undone (Version 3): "Updated SQL query" in database.cpp
  [DSA] Stack pop: O(1) — LIFO order
    Undo stack: 2 | Redo stack: 1
```

### Example 3: Sort Functions (Merge Sort — O(n log n))
```
  Enter choice: 15

  Functions Ranked by Complexity (Merge Sort):
  ──────────────────────────────────────────────
  Rank  Function            File        Score
  ──────────────────────────────────────────────
  1.    processPayment()    payment.cpp    90
  2.    updateProfile()     profile.cpp    55
  3.    searchItem()        search.cpp     45
  4.    validateInput()     utils.cpp      35
  5.    register()          auth.cpp       30
  6.    login()             login.cpp      20
  7.    logout()            login.cpp      10

  [DSA] Merge Sort: O(n log n)
    Elements: 7
    Comparisons made: 13
    Theoretical O(n log n) ≈ 19
```

### Example 4: Dependency Map (Graph + DFS Cycle Detection)
```
  Enter choice: 17

  Adjacency List Representation:
  ─────────────────────────────────────────
    main.cpp → [login.cpp, payment.cpp, profile.cpp]
    login.cpp → [auth.cpp, database.cpp]
    payment.cpp → [database.cpp, cart.cpp]
    profile.cpp → [database.cpp]
    auth.cpp → []
    database.cpp → []
    cart.cpp → []

  Tree View:
  main.cpp
  ├── login.cpp
  │   ├── auth.cpp
  │   └── database.cpp
  ├── payment.cpp
  │   ├── database.cpp
  │   └── cart.cpp
  └── profile.cpp
      └── database.cpp

  Cycle Detection (DFS):
  ✓ No circular dependencies found.
```

### Example 5: Deep Trace (BFS — Shortest Path)
```
  Enter choice: 18

  Enter start file: main.cpp
  Enter target file: database.cpp

  [BFS] Starting from 'main.cpp'...

  ✓ Shortest path found!

    main.cpp → login.cpp → database.cpp

    Path length  : 2 steps
    Nodes explored: 3
  [DSA] BFS: O(V+E). Uses Queue internally.
```

### Example 6: Optimizer (Min-Heap — Best Option First)
```
  Enter choice: 20

  ✓ Best option (lowest cost):
    Name : Binary Search
    Cost : 10
    Info : Fast O(log n) for sorted data

  [DSA] Min-Heap extract-min: O(log n). Remaining: 5
```

---

## 2.10 Screenshots
<img width="525" height="693" alt="Screenshot 2026-06-08 at 11 28 28 PM" src="https://github.com/user-attachments/assets/1f2cd440-ed87-45b4-a8be-04e2f1cef6cd" />

<img width="380" height="160" alt="Screenshot 2026-06-08 at 11 31 05 PM" src="https://github.com/user-attachments/assets/9d616fb6-2225-45d3-a5fd-3ec03297e0a2" />


<img width="526" height="236" alt="Screenshot 2026-06-08 at 11 31 29 PM" src="https://github.com/user-attachments/assets/48cf1d4a-5192-4af5-a375-f8ca9d324adc" />



<img width="503" height="174" alt="Screenshot 2026-06-08 at 11 32 50 PM" src="https://github.com/user-attachments/assets/f746d0f2-f903-4207-b5c8-a97a17dc1c29" />








---

## 2.11 Results and Observations

### Results

| Feature | Works Correctly? | DSA Verified? |
|---------|:---:|:---:|
| Variable Tracker (HashMap) | ✅ | ✅ O(1) search |
| Code Undo (Stack) | ✅ | ✅ LIFO order |
| Inspection Queue (Queue) | ✅ | ✅ FIFO order |
| Syntax Checker (HashSet/Map) | ✅ | ✅ O(1) lookup |
| Complexity Sorter (Merge Sort) | ✅ | ✅ O(n log n) verified by comparison counter |
| Code Map (Graph) | ✅ | ✅ Adjacency List + DFS cycle detection |
| Deep Trace (BFS) | ✅ | ✅ Shortest path found correctly |
| Optimizer (Min-Heap) | ✅ | ✅ Always returns minimum cost |

### Key Observations

1. **HashMap vs Array**: Searching "userId" among 7 variables takes 1 step with HashMap, but up to 7 steps with an array. The O(1) vs O(n) difference becomes critical at scale.

2. **Stack LIFO**: When we push changes v1, v2, v3 and undo, we correctly get v3 first (not v1). This is why Stack is correct for undo, not Queue.

3. **Queue FIFO**: Files are checked in submission order — login.cpp first, then payment.cpp. Fair and predictable.

4. **Merge Sort Comparisons**: For 7 elements, Merge Sort made 13 comparisons. Theoretical O(n log n) = 7 × log₂(7) ≈ 19. Actual is within expected range, confirming O(n log n) performance.

5. **BFS Shortest Path**: BFS found main.cpp → database.cpp in 2 steps (main→login→database), not 3 steps (main→payment→database). BFS always finds the shortest path in unweighted graphs.

6. **Min-Heap Ordering**: Binary Search (cost 10) always comes out first, then Quick Sort (15), then Merge Sort (20) — correct min-heap behavior.

7. **Cycle Detection**: DFS correctly reports no cycles in our sample data. If we add `database.cpp → main.cpp`, it would detect the circular dependency.

---

## 2.12 Conclusion

CodeClean successfully demonstrates 8 core data structures and algorithms applied to a real-world code management problem:

- **HashMap** provides O(1) variable lookup — the fastest possible search
- **Stack** naturally supports undo/redo with LIFO ordering
- **Queue** ensures fair FIFO file inspection
- **HashSet/HashMap** enables instant keyword/function verification
- **Merge Sort** guarantees O(n log n) ranking without worst-case degradation
- **Graph with Adjacency List** efficiently represents sparse file dependencies
- **BFS** finds the shortest bug trace path through the dependency graph
- **Min-Heap** always delivers the optimal (lowest cost) code alternative

Each data structure was chosen because it is the **most efficient** for its specific problem. The program runs correctly with pre-loaded sample data, handles edge cases (empty structures, invalid input), and prints complexity information during execution to demonstrate DSA understanding.

The project relates to real-world tools like **SonarQube** (static analysis), **IntelliJ IDEA** (code navigation), and **VS Code** (undo/redo), showing how these fundamental data structures power the tools developers use every day.

---

## How to Run

```bash
g++ -std=c++17 -o codeclean codeclean.cpp
./codeclean
```

---

*ITM Skills University | B.Tech CSE 2025-29 | Data Structures & Algorithms with C++*
