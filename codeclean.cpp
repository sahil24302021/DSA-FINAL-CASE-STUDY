
//  CodeClean — Automated Code Refactoring & Dependency Tool
//  ITM Skills University | B.Tech CSE 2025-29
//  Data Structures & Algorithms with C++ | Semester II
//  Problem Statement #35

//  DSA Used:
//    1. Hash Map        → Variable Tracker     (O(1) search)
//    2. Stack           → Code Undo            (LIFO)
//    3. Queue           → Inspection Queue     (FIFO)
//    4. Hash Set/Map    → Syntax Checker       (O(1) lookup)
//    5. Merge Sort      → Complexity Sorter    (O(n log n))
//    6. Graph (Adj List)→ Code Map             (Dependency Graph)
//    7. BFS             → Deep Trace           (Shortest Path)
//    8. Priority Queue  → Optimizer            (Min-Heap)

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// ───────────────────────────────────────────────────────
//  ANSI Colors for terminal output
// ───────────────────────────────────────────────────────
const string GREEN  = "\033[32m";
const string RED    = "\033[31m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m";
const string BOLD   = "\033[1m";
const string RESET  = "\033[0m";

// ───────────────────────────────────────────────────────
//  1. VARIABLE TRACKER — Hash Map (unordered_map)
//     Why Hash Map? O(1) average search. When codebase
//     has thousands of variables, HashMap is fastest.
//     Real-world: IntelliJ IDEA uses symbol tables (hash maps).
// ───────────────────────────────────────────────────────

struct VariableInfo {
    string fileName;
    int lineNumber;
    string scope;      // global, local, class
    string dataType;   // int, string, float, etc.
};

unordered_map<string, VariableInfo> variableMap;

void addVariable() {
    string varName, fileName, scope, dataType;
    int line;

    cout << "\n  Enter variable name: ";
    cin >> varName;
    cout << "  Enter data type (int/string/float/etc): ";
    cin >> dataType;
    cout << "  Enter file name (e.g. login.cpp): ";
    cin >> fileName;
    cout << "  Enter line number: ";
    cin >> line;
    cout << "  Enter scope (global/local/class): ";
    cin >> scope;

    // Hash Map insert — O(1) amortized
    variableMap[varName] = {fileName, line, scope, dataType};

    cout << "\n  " << GREEN << "✓ Variable '" << varName << "' added to HashMap." << RESET << "\n";
    cout << "  " << CYAN << "[DSA] HashMap insert: O(1) amortized time" << RESET << "\n";
    cout << "    Total variables tracked: " << variableMap.size() << "\n";
}

void searchVariable() {
    string varName;
    cout << "\n  Enter variable name to search: ";
    cin >> varName;

    // Hash Map lookup — O(1) average
    auto it = variableMap.find(varName);
    if (it != variableMap.end()) {
        cout << "\n  " << GREEN << "✓ Found! (Hash lookup: O(1))" << RESET << "\n";
        cout << "    Variable : " << it->first << "\n";
        cout << "    Type     : " << it->second.dataType << "\n";
        cout << "    File     : " << it->second.fileName << "\n";
        cout << "    Line     : " << it->second.lineNumber << "\n";
        cout << "    Scope    : " << it->second.scope << "\n";
    } else {
        cout << "\n  " << RED << "✗ Variable '" << varName << "' not found." << RESET << "\n";
    }
}

void showAllVariables() {
    if (variableMap.empty()) {
        cout << "\n  " << RED << "✗ No variables tracked." << RESET << "\n";
        return;
    }
    cout << "\n  " << BOLD << "All Tracked Variables (HashMap):" << RESET << "\n";
    cout << "  ─────────────────────────────────────────────────────\n";
    cout << "  " << setw(15) << left << "Variable"
         << setw(8) << "Type"
         << setw(15) << "File"
         << setw(6) << "Line"
         << setw(8) << "Scope" << "\n";
    cout << "  ─────────────────────────────────────────────────────\n";
    for (auto& [name, info] : variableMap) {
        cout << "  " << setw(15) << left << name
             << setw(8) << info.dataType
             << setw(15) << info.fileName
             << setw(6) << info.lineNumber
             << setw(8) << info.scope << "\n";
    }
    cout << "\n  " << CYAN << "[DSA] HashMap stores " << variableMap.size()
         << " variables. Each search = O(1)" << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  2. CODE UNDO — Stack (LIFO)
//     Why Stack? Undo is Last-In-First-Out.
//     The most recent change must be undone first.
//     Real-world: VS Code, Vim use two-stack undo/redo.
// ───────────────────────────────────────────────────────

struct CodeChange {
    string description;
    string fileName;
    int version;
};

stack<CodeChange> undoStack;
stack<CodeChange> redoStack;
int versionCounter = 0;

void pushChange() {
    string change, fileName;
    cout << "\n  Enter file name: ";
    cin >> fileName;
    cout << "  Enter change description: ";
    cin.ignore();
    getline(cin, change);

    versionCounter++;
    undoStack.push({change, fileName, versionCounter});

    // Clear redo stack (standard editor behavior)
    while (!redoStack.empty()) redoStack.pop();

    cout << "\n  " << GREEN << "✓ Change saved (Version " << versionCounter << ")" << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Stack push: O(1). Stack size: " << undoStack.size() << RESET << "\n";
}

void undoChange() {
    if (undoStack.empty()) {
        cout << "\n  " << RED << "✗ Nothing to undo. Stack is empty." << RESET << "\n";
        return;
    }

    // Pop from undo stack, push to redo stack — O(1) each
    CodeChange last = undoStack.top();
    undoStack.pop();
    redoStack.push(last);

    cout << "\n  " << GREEN << "✓ Undone (Version " << last.version << "): \""
         << last.description << "\" in " << last.fileName << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Stack pop: O(1) — LIFO order" << RESET << "\n";
    cout << "    Undo stack: " << undoStack.size()
         << " | Redo stack: " << redoStack.size() << "\n";
}

void redoChange() {
    if (redoStack.empty()) {
        cout << "\n  " << RED << "✗ Nothing to redo." << RESET << "\n";
        return;
    }

    CodeChange last = redoStack.top();
    redoStack.pop();
    undoStack.push(last);

    cout << "\n  " << GREEN << "✓ Redone (Version " << last.version << "): \""
         << last.description << "\"" << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Redo uses second stack: O(1)" << RESET << "\n";
}

void showChangeHistory() {
    if (undoStack.empty()) {
        cout << "\n  " << RED << "✗ No changes recorded." << RESET << "\n";
        return;
    }
    // Copy stack to display without modifying
    stack<CodeChange> temp = undoStack;
    vector<CodeChange> changes;
    while (!temp.empty()) {
        changes.push_back(temp.top());
        temp.pop();
    }

    cout << "\n  " << BOLD << "Change History (Stack — top to bottom):" << RESET << "\n";
    cout << "  ─────────────────────────────────────────────\n";
    for (auto& c : changes) {
        cout << "    v" << c.version << " | " << c.fileName
             << " | " << c.description << "\n";
    }
    cout << "  ─────────────────────────────────────────────\n";
    cout << "  " << CYAN << "[DSA] Stack: LIFO. Top = v" << changes[0].version
         << " (will be undone first)" << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  3. INSPECTION QUEUE — Queue (FIFO)
//     Why Queue? Files must be checked in arrival order.
//     First file submitted = first file checked. Fair.
//     Real-world: SonarQube processes files in FIFO order.
// ───────────────────────────────────────────────────────

struct FileEntry {
    string fileName;
    int priority;  // 1=low, 2=medium, 3=high
};

queue<FileEntry> inspectionQueue;
int filesProcessed = 0;

void addFileForInspection() {
    string fileName;
    int priority;
    cout << "\n  Enter file name: ";
    cin >> fileName;
    cout << "  Enter priority (1=low, 2=medium, 3=high): ";
    cin >> priority;

    inspectionQueue.push({fileName, priority});

    cout << "\n  " << GREEN << "✓ '" << fileName << "' added to queue (priority: "
         << priority << ")" << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Queue enqueue: O(1). Queue size: "
         << inspectionQueue.size() << RESET << "\n";
}

void checkNextFile() {
    if (inspectionQueue.empty()) {
        cout << "\n  " << RED << "✗ Queue is empty. No files to check." << RESET << "\n";
        return;
    }

    FileEntry next = inspectionQueue.front();
    inspectionQueue.pop();
    filesProcessed++;

    cout << "\n  " << GREEN << "✓ Now checking: " << next.fileName
         << " (priority: " << next.priority << ")" << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Queue dequeue: O(1) — FIFO order" << RESET << "\n";
    cout << "    Files remaining: " << inspectionQueue.size()
         << " | Total processed: " << filesProcessed << "\n";
}

void showQueue() {
    if (inspectionQueue.empty()) {
        cout << "\n  " << RED << "✗ Queue is empty." << RESET << "\n";
        return;
    }
    queue<FileEntry> temp = inspectionQueue;
    cout << "\n  " << BOLD << "Inspection Queue (front → back):" << RESET << "\n";
    cout << "  ─────────────────────────────────\n";
    int pos = 1;
    while (!temp.empty()) {
        FileEntry f = temp.front();
        temp.pop();
        string pLabel = (f.priority == 3) ? "HIGH" : (f.priority == 2) ? "MED" : "LOW";
        cout << "    " << pos++ << ". " << f.fileName << " [" << pLabel << "]\n";
    }
    cout << "  ─────────────────────────────────\n";
    cout << "  " << CYAN << "[DSA] Queue: FIFO. Position 1 = next to be checked" << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  4. SYNTAX CHECKER — Hash Set + Hash Map
//     Why Hash Set? O(1) lookup to check if a function
//     or keyword is defined. No scanning required.
//     Real-world: Compilers use symbol tables (hash maps).
// ───────────────────────────────────────────────────────

unordered_set<string> functionSet;
unordered_map<string, string> keywordMap;  // keyword → category

void initKeywords() {
    // Pre-load C++ keywords into HashMap
    vector<pair<string, string>> keywords = {
        {"int", "type"}, {"float", "type"}, {"double", "type"},
        {"char", "type"}, {"string", "type"}, {"bool", "type"},
        {"void", "type"}, {"auto", "type"}, {"long", "type"},
        {"if", "control"}, {"else", "control"}, {"for", "control"},
        {"while", "control"}, {"do", "control"}, {"switch", "control"},
        {"case", "control"}, {"break", "control"}, {"continue", "control"},
        {"return", "control"}, {"class", "oop"}, {"struct", "oop"},
        {"public", "oop"}, {"private", "oop"}, {"virtual", "oop"},
        {"new", "memory"}, {"delete", "memory"}, {"nullptr", "memory"},
        {"try", "exception"}, {"catch", "exception"}, {"throw", "exception"},
        {"const", "qualifier"}, {"static", "qualifier"},
        {"#include", "preprocessor"}, {"#define", "preprocessor"},
        {"namespace", "scope"}, {"using", "scope"},
        {"template", "generic"}, {"typename", "generic"},
        {"cout", "io"}, {"cin", "io"}, {"endl", "io"}
    };
    for (auto& [kw, cat] : keywords) {
        keywordMap[kw] = cat;
    }
}

void addFunction() {
    string funcName;
    cout << "\n  Enter function name: ";
    cin >> funcName;

    functionSet.insert(funcName);
    cout << "\n  " << GREEN << "✓ Function '" << funcName
         << "' registered in Hash Set." << RESET << "\n";
    cout << "  " << CYAN << "[DSA] HashSet insert: O(1). Total functions: "
         << functionSet.size() << RESET << "\n";
}

void checkSyntax() {
    string token;
    cout << "\n  Enter function or keyword to check: ";
    cin >> token;

    // Check in function set — O(1)
    if (functionSet.count(token)) {
        cout << "\n  " << GREEN << "✓ '" << token
             << "' is a registered function." << RESET << "\n";
        return;
    }

    // Check in keyword map — O(1)
    auto it = keywordMap.find(token);
    if (it != keywordMap.end()) {
        cout << "\n  " << GREEN << "✓ '" << token
             << "' is a C++ keyword (category: " << it->second << ")" << RESET << "\n";
        return;
    }

    cout << "\n  " << RED << "✗ '" << token
         << "' is NOT defined. Possible syntax error!" << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Hash lookup: O(1) — checked both HashSet and HashMap" << RESET << "\n";
}

void showAllFunctions() {
    if (functionSet.empty()) {
        cout << "\n  " << RED << "✗ No functions registered." << RESET << "\n";
        return;
    }
    cout << "\n  " << BOLD << "Registered Functions (Hash Set):" << RESET << "\n";
    for (auto& f : functionSet) {
        cout << "    • " << f << "()\n";
    }
    cout << "  " << CYAN << "[DSA] HashSet: " << functionSet.size()
         << " functions, each lookup O(1)" << RESET << "\n";
    cout << "\n  C++ Keywords loaded: " << keywordMap.size() << "\n";
}

// ───────────────────────────────────────────────────────
//  5. COMPLEXITY SORTER — Merge Sort
//     Why Merge Sort? Guaranteed O(n log n), stable sort.
//     No O(n²) worst case like QuickSort.
//     Case study specifically asks for "large, randomly
//     ordered data sets" — Merge Sort is ideal.
// ───────────────────────────────────────────────────────

struct FunctionScore {
    string name;
    int complexity;
    string fileName;
};

vector<FunctionScore> functionList;
int mergeSortComparisons = 0;  // Count comparisons to demonstrate O(n log n)

void merge(vector<FunctionScore>& arr, int left, int mid, int right) {
    vector<FunctionScore> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<FunctionScore> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < (int)leftArr.size() && j < (int)rightArr.size()) {
        mergeSortComparisons++;
        if (leftArr[i].complexity >= rightArr[j].complexity) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < (int)leftArr.size()) arr[k++] = leftArr[i++];
    while (j < (int)rightArr.size()) arr[k++] = rightArr[j++];
}

void mergeSort(vector<FunctionScore>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);       // Sort left half
    mergeSort(arr, mid + 1, right);  // Sort right half
    merge(arr, left, mid, right);    // Merge sorted halves
}

void addFunctionComplexity() {
    string name, fileName;
    int score;
    cout << "\n  Enter function name: ";
    cin >> name;
    cout << "  Enter file name: ";
    cin >> fileName;
    cout << "  Enter complexity score (1-100): ";
    cin >> score;

    functionList.push_back({name, score, fileName});
    cout << "\n  " << GREEN << "✓ Added: " << name << "() [score: " << score
         << "] in " << fileName << RESET << "\n";
}

void sortFunctions() {
    if (functionList.empty()) {
        cout << "\n  " << RED << "✗ No functions to sort." << RESET << "\n";
        return;
    }

    vector<FunctionScore> sorted = functionList;
    mergeSortComparisons = 0;

    mergeSort(sorted, 0, (int)sorted.size() - 1);

    cout << "\n  " << BOLD << "Functions Ranked by Complexity (Merge Sort):" << RESET << "\n";
    cout << "  ──────────────────────────────────────────────\n";
    cout << "  " << setw(6) << left << "Rank"
         << setw(20) << "Function"
         << setw(12) << "File"
         << setw(8) << right << "Score" << "\n";
    cout << "  ──────────────────────────────────────────────\n";

    for (int i = 0; i < (int)sorted.size(); i++) {
        cout << "  " << setw(6) << left << (to_string(i + 1) + ".")
             << setw(20) << (sorted[i].name + "()")
             << setw(12) << sorted[i].fileName
             << setw(8) << right << sorted[i].complexity << "\n";
    }

    cout << "\n  " << CYAN << "[DSA] Merge Sort: O(n log n)" << RESET << "\n";
    cout << "    Elements: " << sorted.size() << "\n";
    cout << "    Comparisons made: " << mergeSortComparisons << "\n";
    cout << "    Theoretical O(n log n) ≈ "
         << (int)(sorted.size() * log2(sorted.size())) << "\n";
}

// ───────────────────────────────────────────────────────
//  6. CODE MAP — Graph (Adjacency List)
//     Why Graph? File dependencies naturally form a
//     directed graph. Adjacency List is space-efficient
//     for sparse graphs: O(V + E) vs O(V²) for matrix.
//     Real-world: Build systems (Make, CMake) use dependency graphs.
// ───────────────────────────────────────────────────────

unordered_map<string, vector<string>> dependencyGraph;

void addDependency() {
    string from, to;
    cout << "\n  Enter source file: ";
    cin >> from;
    cout << "  Enter dependency file (that it depends on): ";
    cin >> to;

    dependencyGraph[from].push_back(to);
    if (dependencyGraph.find(to) == dependencyGraph.end()) {
        dependencyGraph[to] = {};
    }

    cout << "\n  " << GREEN << "✓ Dependency: " << from << " → " << to << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Graph edge added to Adjacency List" << RESET << "\n";
}

// Print tree-style dependency map
void printTree(const string& node, const string& prefix, bool isLast,
               unordered_set<string>& visited) {
    cout << prefix;
    cout << (isLast ? "└── " : "├── ");
    cout << node << "\n";

    if (visited.count(node)) {
        cout << prefix << (isLast ? "    " : "│   ") << YELLOW << "(cycle detected)" << RESET << "\n";
        return;
    }
    visited.insert(node);

    auto it = dependencyGraph.find(node);
    if (it == dependencyGraph.end() || it->second.empty()) return;

    for (int i = 0; i < (int)it->second.size(); i++) {
        bool last = (i == (int)it->second.size() - 1);
        printTree(it->second[i], prefix + (isLast ? "    " : "│   "), last, visited);
    }
}

// Detect cycles using DFS
bool hasCycleDFS(const string& node, unordered_set<string>& visiting,
                 unordered_set<string>& visited, vector<string>& cyclePath) {
    visiting.insert(node);
    cyclePath.push_back(node);

    auto it = dependencyGraph.find(node);
    if (it != dependencyGraph.end()) {
        for (auto& neighbor : it->second) {
            if (visiting.count(neighbor)) {
                cyclePath.push_back(neighbor);
                return true;  // Cycle found!
            }
            if (!visited.count(neighbor)) {
                if (hasCycleDFS(neighbor, visiting, visited, cyclePath))
                    return true;
            }
        }
    }

    visiting.erase(node);
    visited.insert(node);
    cyclePath.pop_back();
    return false;
}

void showDependencyMap() {
    if (dependencyGraph.empty()) {
        cout << "\n  " << RED << "✗ No dependencies added." << RESET << "\n";
        return;
    }

    // 1. Show Adjacency List
    cout << "\n  " << BOLD << "Adjacency List Representation:" << RESET << "\n";
    cout << "  ─────────────────────────────────────────\n";
    for (auto& [node, deps] : dependencyGraph) {
        cout << "    " << node << " → [";
        for (int i = 0; i < (int)deps.size(); i++) {
            cout << deps[i];
            if (i < (int)deps.size() - 1) cout << ", ";
        }
        cout << "]\n";
    }

    // 2. Show Tree View
    unordered_set<string> allDeps;
    for (auto& [node, deps] : dependencyGraph) {
        for (auto& d : deps) allDeps.insert(d);
    }

    cout << "\n  " << BOLD << "Tree View:" << RESET << "\n";
    for (auto& [node, deps] : dependencyGraph) {
        if (!allDeps.count(node)) {
            unordered_set<string> visited;
            cout << "  " << node << "\n";
            visited.insert(node);
            for (int i = 0; i < (int)deps.size(); i++) {
                bool last = (i == (int)deps.size() - 1);
                printTree(deps[i], "  ", last, visited);
            }
        }
    }

    // 3. Cycle Detection
    cout << "\n  " << BOLD << "Cycle Detection (DFS):" << RESET << "\n";
    unordered_set<string> visiting, visited;
    vector<string> cyclePath;
    bool cycleFound = false;

    for (auto& [node, _] : dependencyGraph) {
        if (!visited.count(node)) {
            if (hasCycleDFS(node, visiting, visited, cyclePath)) {
                cycleFound = true;
                cout << "  " << RED << "⚠ Circular dependency detected: ";
                // Find the start of the cycle
                string cycleStart = cyclePath.back();
                bool printing = false;
                for (auto& n : cyclePath) {
                    if (n == cycleStart) printing = true;
                    if (printing) cout << n << " → ";
                }
                cout << RESET << "\n";
                break;
            }
        }
    }

    if (!cycleFound) {
        cout << "  " << GREEN << "✓ No circular dependencies found." << RESET << "\n";
    }

    // Count nodes and edges
    int edges = 0;
    for (auto& [_, deps] : dependencyGraph) edges += deps.size();
    cout << "\n  " << CYAN << "[DSA] Graph: " << dependencyGraph.size()
         << " nodes, " << edges << " edges. Space: O(V+E)" << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  7. DEEP TRACE — BFS (Breadth-First Search)
//     Why BFS? Finds shortest path in unweighted graph.
//     BFS explores all nodes at distance d before d+1.
//     This guarantees the shortest bug trace path.
//     Real-world: Debuggers trace call stacks similarly.
// ───────────────────────────────────────────────────────

void deepTrace() {
    string start, target;
    cout << "\n  Enter start file: ";
    cin >> start;
    cout << "  Enter target file: ";
    cin >> target;

    if (dependencyGraph.find(start) == dependencyGraph.end()) {
        cout << "\n  " << RED << "✗ '" << start << "' not in dependency graph." << RESET << "\n";
        return;
    }

    // BFS using Queue — explores level by level
    queue<string> bfsQueue;
    unordered_map<string, string> parent;
    unordered_set<string> visited;

    bfsQueue.push(start);
    visited.insert(start);
    parent[start] = "";

    bool found = false;
    int nodesExplored = 0;

    cout << "\n  " << CYAN << "[BFS] Starting from '" << start << "'..." << RESET << "\n";

    while (!bfsQueue.empty()) {
        string current = bfsQueue.front();
        bfsQueue.pop();
        nodesExplored++;

        if (current == target) {
            found = true;
            break;
        }

        auto it = dependencyGraph.find(current);
        if (it != dependencyGraph.end()) {
            for (auto& neighbor : it->second) {
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    bfsQueue.push(neighbor);
                }
            }
        }
    }

    if (!found) {
        cout << "\n  " << RED << "✗ No path from '" << start << "' to '"
             << target << "'." << RESET << "\n";
        cout << "    Nodes explored: " << nodesExplored << "\n";
        return;
    }

    // Reconstruct shortest path
    vector<string> path;
    string node = target;
    while (!node.empty()) {
        path.push_back(node);
        node = parent[node];
    }
    reverse(path.begin(), path.end());

    cout << "\n  " << GREEN << "✓ Shortest path found!" << RESET << "\n\n    ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << BOLD << path[i] << RESET;
        if (i < (int)path.size() - 1) cout << " → ";
    }
    cout << "\n\n";
    cout << "    Path length  : " << path.size() - 1 << " steps\n";
    cout << "    Nodes explored: " << nodesExplored << "\n";
    cout << "  " << CYAN << "[DSA] BFS: O(V+E). Uses Queue internally." << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  8. OPTIMIZER — Priority Queue (Min-Heap)
//     Why Min-Heap? Always gives lowest cost option.
//     When choosing between algorithms, we want the
//     fastest (lowest cost) alternative.
//     Real-world: Compiler optimizers choose cheapest code path.
// ───────────────────────────────────────────────────────

struct CodeOption {
    string name;
    int cost;
    string description;

    bool operator>(const CodeOption& other) const {
        return cost > other.cost;
    }
};

priority_queue<CodeOption, vector<CodeOption>, greater<CodeOption>> optimizerHeap;

void addCodeOption() {
    string name, desc;
    int cost;
    cout << "\n  Enter algorithm/approach name: ";
    cin.ignore();
    getline(cin, name);
    cout << "  Enter description: ";
    getline(cin, desc);
    cout << "  Enter execution cost/time: ";
    cin >> cost;

    optimizerHeap.push({name, cost, desc});

    cout << "\n  " << GREEN << "✓ Added: \"" << name << "\" (cost: " << cost << ")" << RESET << "\n";
    cout << "  " << CYAN << "[DSA] Min-Heap insert: O(log n). Heap size: "
         << optimizerHeap.size() << RESET << "\n";
}

void getBestOption() {
    if (optimizerHeap.empty()) {
        cout << "\n  " << RED << "✗ No options in heap." << RESET << "\n";
        return;
    }

    CodeOption best = optimizerHeap.top();
    optimizerHeap.pop();

    cout << "\n  " << GREEN << "✓ Best option (lowest cost):" << RESET << "\n";
    cout << "    Name : " << BOLD << best.name << RESET << "\n";
    cout << "    Cost : " << best.cost << "\n";
    cout << "    Info : " << best.description << "\n";
    cout << "\n  " << CYAN << "[DSA] Min-Heap extract-min: O(log n). Remaining: "
         << optimizerHeap.size() << RESET << "\n";
}

void showAllOptions() {
    if (optimizerHeap.empty()) {
        cout << "\n  " << RED << "✗ Heap is empty." << RESET << "\n";
        return;
    }
    // Copy heap to display
    auto temp = optimizerHeap;
    cout << "\n  " << BOLD << "All Options (Min-Heap order — lowest cost first):" << RESET << "\n";
    cout << "  ──────────────────────────────────────────────\n";
    int rank = 1;
    while (!temp.empty()) {
        CodeOption opt = temp.top();
        temp.pop();
        cout << "    " << rank++ << ". " << setw(20) << left << opt.name
             << " cost: " << setw(4) << opt.cost
             << " | " << opt.description << "\n";
    }
    cout << "  " << CYAN << "[DSA] Min-Heap always gives minimum first: O(1) peek, O(log n) extract"
         << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  DASHBOARD — Show summary of all data structures
// ───────────────────────────────────────────────────────

void showDashboard() {
    cout << "\n  " << BOLD << "╔═══════════════════════════════════════════════╗" << RESET << "\n";
    cout << "  " << BOLD << "║         CODECLEAN — DASHBOARD                 ║" << RESET << "\n";
    cout << "  " << BOLD << "╚═══════════════════════════════════════════════╝" << RESET << "\n\n";

    cout << "  " << setw(25) << left << "  1. Variable Tracker"
         << "[HashMap]     " << variableMap.size() << " variables\n";

    cout << "  " << setw(25) << left << "  2. Code Undo"
         << "[Stack]       " << undoStack.size() << " undo, " << redoStack.size() << " redo\n";

    cout << "  " << setw(25) << left << "  3. Inspection Queue"
         << "[Queue]       " << inspectionQueue.size() << " files waiting\n";

    cout << "  " << setw(25) << left << "  4. Syntax Checker"
         << "[HashSet/Map] " << functionSet.size() << " functions, " << keywordMap.size() << " keywords\n";

    cout << "  " << setw(25) << left << "  5. Complexity Sorter"
         << "[MergeSort]   " << functionList.size() << " functions\n";

    int edges = 0;
    for (auto& [_, deps] : dependencyGraph) edges += deps.size();
    cout << "  " << setw(25) << left << "  6. Code Map"
         << "[Graph]       " << dependencyGraph.size() << " nodes, " << edges << " edges\n";

    cout << "  " << setw(25) << left << "  7. Deep Trace"
         << "[BFS]         Ready\n";

    cout << "  " << setw(25) << left << "  8. Optimizer"
         << "[Min-Heap]    " << optimizerHeap.size() << " options\n";
}

// ───────────────────────────────────────────────────────
//  SAMPLE DATA
// ───────────────────────────────────────────────────────

void loadSampleData() {
    // Variables
    variableMap["userId"]    = {"login.cpp", 15, "global", "int"};
    variableMap["userName"]  = {"login.cpp", 16, "global", "string"};
    variableMap["password"]  = {"auth.cpp", 8, "local", "string"};
    variableMap["balance"]   = {"payment.cpp", 10, "global", "double"};
    variableMap["cartItems"] = {"cart.cpp", 5, "class", "vector"};
    variableMap["isAdmin"]   = {"auth.cpp", 12, "global", "bool"};
    variableMap["dbConn"]    = {"database.cpp", 3, "global", "Connection"};

    // Code changes
    versionCounter = 3;
    undoStack.push({"Created login module", "login.cpp", 1});
    undoStack.push({"Fixed payment bug #42", "payment.cpp", 2});
    undoStack.push({"Updated SQL query", "database.cpp", 3});

    // Inspection queue
    inspectionQueue.push({"login.cpp", 3});
    inspectionQueue.push({"payment.cpp", 2});
    inspectionQueue.push({"profile.cpp", 1});
    inspectionQueue.push({"cart.cpp", 2});

    // Functions
    functionSet.insert("login");
    functionSet.insert("register");
    functionSet.insert("logout");
    functionSet.insert("processPayment");
    functionSet.insert("searchItem");
    functionSet.insert("updateProfile");

    // Complexity
    functionList.push_back({"login", 20, "login.cpp"});
    functionList.push_back({"processPayment", 90, "payment.cpp"});
    functionList.push_back({"searchItem", 45, "search.cpp"});
    functionList.push_back({"register", 30, "auth.cpp"});
    functionList.push_back({"logout", 10, "login.cpp"});
    functionList.push_back({"updateProfile", 55, "profile.cpp"});
    functionList.push_back({"validateInput", 35, "utils.cpp"});

    // Dependencies
    dependencyGraph["main.cpp"] = {"login.cpp", "payment.cpp", "profile.cpp"};
    dependencyGraph["login.cpp"] = {"auth.cpp", "database.cpp"};
    dependencyGraph["payment.cpp"] = {"database.cpp", "cart.cpp"};
    dependencyGraph["profile.cpp"] = {"database.cpp"};
    dependencyGraph["auth.cpp"] = {};
    dependencyGraph["database.cpp"] = {};
    dependencyGraph["cart.cpp"] = {};

    // Optimizer options
    optimizerHeap.push({"Bubble Sort", 50, "Simple but O(n^2)"});
    optimizerHeap.push({"Quick Sort", 15, "Fast avg O(n log n)"});
    optimizerHeap.push({"Merge Sort", 20, "Stable O(n log n)"});
    optimizerHeap.push({"Linear Search", 40, "Scans everything O(n)"});
    optimizerHeap.push({"Binary Search", 10, "Fast O(log n) for sorted data"});
    optimizerHeap.push({"Insertion Sort", 35, "Good for small data"});

    cout << GREEN << "  ✓ Sample data loaded successfully." << RESET << "\n";
}

// ───────────────────────────────────────────────────────
//  MAIN MENU
// ───────────────────────────────────────────────────────

void printMenu() {
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════╗\n";
    cout << "  ║           " << BOLD << "CODE CLEAN — MAIN MENU" << RESET << "               ║\n";
    cout << "  ╠════════════════════════════════════════════════╣\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Variable Tracker — HashMap]" << RESET << "                  ║\n";
    cout << "  ║   1. Add Variable                              ║\n";
    cout << "  ║   2. Search Variable                           ║\n";
    cout << "  ║   3. Show All Variables                        ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Code Undo — Stack]" << RESET << "                           ║\n";
    cout << "  ║   4. Push Code Change                          ║\n";
    cout << "  ║   5. Undo Change                               ║\n";
    cout << "  ║   6. Redo Change                               ║\n";
    cout << "  ║   7. Show Change History                       ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Inspection Queue — Queue]" << RESET << "                    ║\n";
    cout << "  ║   8. Add File to Queue                         ║\n";
    cout << "  ║   9. Check Next File                           ║\n";
    cout << "  ║  10. Show Queue                                ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Syntax Checker — Hash Set/Map]" << RESET << "               ║\n";
    cout << "  ║  11. Register Function                         ║\n";
    cout << "  ║  12. Check Syntax                              ║\n";
    cout << "  ║  13. Show All Functions                        ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Complexity Sorter — Merge Sort]" << RESET << "              ║\n";
    cout << "  ║  14. Add Function Complexity                   ║\n";
    cout << "  ║  15. Sort & Rank Functions                     ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Code Map — Graph]" << RESET << "                            ║\n";
    cout << "  ║  16. Add Dependency                            ║\n";
    cout << "  ║  17. Show Dependency Map                       ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Deep Trace — BFS]" << RESET << "                            ║\n";
    cout << "  ║  18. Trace Bug Path                            ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  " << CYAN << "[Optimizer — Min-Heap]" << RESET << "                        ║\n";
    cout << "  ║  19. Add Code Option                           ║\n";
    cout << "  ║  20. Get Best Option                           ║\n";
    cout << "  ║  21. Show All Options                          ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ║  22. " << BOLD << "Dashboard" << RESET << "                                  ║\n";
    cout << "  ║   0. Exit                                      ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ╚════════════════════════════════════════════════╝\n";
    cout << "\n  Enter choice: ";
}

int main() {
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════╗\n";
    cout << "  ║                                                ║\n";
    cout << "  ║          " << BOLD << "C O D E   C L E A N" << RESET << "                   ║\n";
    cout << "  ║    Automated Code Refactoring & Dependency     ║\n";
    cout << "  ║                    Tool                        ║\n";
    cout << "  ║                                                ║\n";
    cout << "  ╠════════════════════════════════════════════════╣\n";
    cout << "  ║  ITM Skills University                         ║\n";
    cout << "  ║  B.Tech CSE 2025-29 | DSA with C++            ║\n";
    cout << "  ║  Problem Statement #35                         ║\n";
    cout << "  ╠════════════════════════════════════════════════╣\n";
    cout << "  ║  DSA Used:                                     ║\n";
    cout << "  ║   1. Hash Map       → Variable Tracker         ║\n";
    cout << "  ║   2. Stack          → Code Undo                ║\n";
    cout << "  ║   3. Queue          → Inspection Queue         ║\n";
    cout << "  ║   4. Hash Set/Map   → Syntax Checker           ║\n";
    cout << "  ║   5. Merge Sort     → Complexity Sorter        ║\n";
    cout << "  ║   6. Graph          → Code Map                 ║\n";
    cout << "  ║   7. BFS            → Deep Trace               ║\n";
    cout << "  ║   8. Priority Queue → Optimizer                ║\n";
    cout << "  ╚════════════════════════════════════════════════╝\n\n";

    initKeywords();
    loadSampleData();

    int choice;

    while (true) {
        printMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n  " << RED << "✗ Invalid input. Enter a number." << RESET << "\n";
            continue;
        }

        switch (choice) {
            case 1: addVariable(); break;
            case 2: searchVariable(); break;
            case 3: showAllVariables(); break;
            case 4: pushChange(); break;
            case 5: undoChange(); break;
            case 6: redoChange(); break;
            case 7: showChangeHistory(); break;
            case 8: addFileForInspection(); break;
            case 9: checkNextFile(); break;
            case 10: showQueue(); break;
            case 11: addFunction(); break;
            case 12: checkSyntax(); break;
            case 13: showAllFunctions(); break;
            case 14: addFunctionComplexity(); break;
            case 15: sortFunctions(); break;
            case 16: addDependency(); break;
            case 17: showDependencyMap(); break;
            case 18: deepTrace(); break;
            case 19: addCodeOption(); break;
            case 20: getBestOption(); break;
            case 21: showAllOptions(); break;
            case 22: showDashboard(); break;
            case 0:
                cout << "\n  " << GREEN << "Exiting CodeClean. Goodbye!" << RESET << "\n\n";
                return 0;
            default:
                cout << "\n  " << RED << "✗ Invalid choice. Enter 0-22." << RESET << "\n";
        }
    }

    return 0;
}
