/**
 * Manual Data Structures Implementation
 *
 * This file contains manually implemented data structures to replace
 * Qt's built-in QVector and QMap containers.
 *
 * Implemented Data Structures:
 * 1. LinkedList - Dynamic list using nodes and pointers
 * 2. HashTable - Hash map for key-value storage with collision handling
 */

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>
#include <functional>

// Forward declarations
struct Course;

// ============================================================================
// LINKED LIST IMPLEMENTATION
// Purpose: Replace QVector for storing Course objects
// Uses: Doubly linked list for efficient insertion/deletion
// ============================================================================

/**
 * Node structure for LinkedList
 * Contains data and pointers to next and previous nodes
 */
template <typename T>
class Node {
public:
    T data;           // The actual data stored
    Node* next;       // Pointer to next node
    Node* prev;       // Pointer to previous node (for doubly linked list)

    // Constructor
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

/**
 * LinkedList Class - Manual implementation of dynamic list
 * Replaces QVector<Course>
 *
 * Features:
 * - Dynamic size (grows/shrinks as needed)
 * - Efficient insertion at any position
 * - Supports iteration, search, insert, delete
 * - Memory management with proper cleanup
 */
template <typename T>
class LinkedList {
private:
    Node<T>* head;    // Pointer to first node
    Node<T>* tail;    // Pointer to last node
    int count;        // Number of elements in list

public:
    /**
     * Constructor - Initialize empty list
     */
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    /**
     * Copy Constructor - Deep copy of another list
     */
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node<T>* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
    }

    /**
     * Assignment Operator - Deep copy assignment
     */
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current) {
                append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    /**
     * Destructor - Clean up all nodes
     */
    ~LinkedList() {
        clear();
    }

    /**
     * Append element to end of list
     * Time Complexity: O(1)
     */
    void append(const T& value) {
        Node<T>* newNode = new Node<T>(value);

        if (!head) {
            // First element
            head = tail = newNode;
        } else {
            // Add to end
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    /**
     * Insert element at specific index
     * Time Complexity: O(n)
     */
    void insert(int index, const T& value) {
        if (index < 0 || index > count) return;

        if (index == count) {
            append(value);
            return;
        }

        if (index == 0) {
            Node<T>* newNode = new Node<T>(value);
            newNode->next = head;
            if (head) head->prev = newNode;
            head = newNode;
            if (!tail) tail = head;
            count++;
            return;
        }

        Node<T>* current = getNodeAt(index);
        if (current) {
            Node<T>* newNode = new Node<T>(value);
            newNode->next = current;
            newNode->prev = current->prev;
            if (current->prev) current->prev->next = newNode;
            current->prev = newNode;
            count++;
        }
    }

    /**
     * Remove element at specific index
     * Time Complexity: O(n)
     */
    void removeAt(int index) {
        if (index < 0 || index >= count) return;

        Node<T>* toDelete = getNodeAt(index);
        if (!toDelete) return;

        // Update links
        if (toDelete->prev) {
            toDelete->prev->next = toDelete->next;
        } else {
            head = toDelete->next;
        }

        if (toDelete->next) {
            toDelete->next->prev = toDelete->prev;
        } else {
            tail = toDelete->prev;
        }

        delete toDelete;
        count--;
    }

    /**
     * Remove last element
     * Time Complexity: O(1)
     */
    void removeLast() {
        if (count > 0) {
            removeAt(count - 1);
        }
    }

    /**
     * Get element at index (by reference for modification)
     * Time Complexity: O(n)
     */
    T& operator[](int index) {
        Node<T>* node = getNodeAt(index);
        return node->data;
    }

    /**
     * Get element at index (const version)
     * Time Complexity: O(n)
     */
    const T& operator[](int index) const {
        Node<T>* node = getNodeAt(index);
        return node->data;
    }

    /**
     * Get element at index (const version with bounds checking)
     * Time Complexity: O(n)
     */
    const T& at(int index) const {
        return (*this)[index];
    }

    /**
     * Get number of elements
     * Time Complexity: O(1)
     */
    int size() const {
        return count;
    }

    /**
     * Check if list is empty
     * Time Complexity: O(1)
     */
    bool isEmpty() const {
        return count == 0;
    }

    /**
     * Clear all elements
     * Time Complexity: O(n)
     */
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        count = 0;
    }

    /**
     * Get first element
     */
    const T& first() const {
        return head->data;
    }

    /**
     * Get last element
     */
    const T& last() const {
        return tail->data;
    }

    /**
     * QUICK SORT ALGORITHM - Manual Implementation
     * Purpose: Sort the linked list using QuickSort algorithm
     * Time Complexity: O(n log n) average, O(n²) worst case
     *
     * This is a custom implementation of QuickSort adapted for linked lists
     */
    void quickSort(std::function<bool(const T&, const T&)> compare) {
        if (count <= 1) return;

        // Convert to array for easier sorting
        T* arr = new T[count];
        Node<T>* current = head;
        for (int i = 0; i < count; i++) {
            arr[i] = current->data;
            current = current->next;
        }

        // Perform QuickSort on array
        quickSortHelper(arr, 0, count - 1, compare);

        // Copy back to linked list
        current = head;
        for (int i = 0; i < count; i++) {
            current->data = arr[i];
            current = current->next;
        }

        delete[] arr;
    }

    /**
     * LINEAR SEARCH ALGORITHM - Manual Implementation
     * Purpose: Search for elements matching a condition
     * Time Complexity: O(n)
     * Returns: LinkedList of indices where condition is true
     */
    LinkedList<int> linearSearch(std::function<bool(const T&)> condition) const {
        LinkedList<int> results;
        Node<T>* current = head;
        int index = 0;

        while (current) {
            if (condition(current->data)) {
                results.append(index);
            }
            current = current->next;
            index++;
        }

        return results;
    }

    /**
     * Iterator support for range-based for loops
     */
    class Iterator {
    private:
        Node<T>* current;
    public:
        Iterator(Node<T>* node) : current(node) {}

        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    const Iterator begin() const { return Iterator(head); }
    const Iterator end() const { return Iterator(nullptr); }

private:
    /**
     * Helper: Get node at specific index
     */
    Node<T>* getNodeAt(int index) const {
        if (index < 0 || index >= count) return nullptr;

        Node<T>* current;
        // Optimize by starting from head or tail
        if (index < count / 2) {
            current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            current = tail;
            for (int i = count - 1; i > index; i--) {
                current = current->prev;
            }
        }
        return current;
    }

    /**
     * QuickSort Helper - Recursive partitioning
     */
    void quickSortHelper(T* arr, int low, int high, std::function<bool(const T&, const T&)> compare) {
        if (low < high) {
            int pivotIndex = partition(arr, low, high, compare);
            quickSortHelper(arr, low, pivotIndex - 1, compare);
            quickSortHelper(arr, pivotIndex + 1, high, compare);
        }
    }

    /**
     * QuickSort Partition - Choose pivot and partition array
     */
    int partition(T* arr, int low, int high, std::function<bool(const T&, const T&)> compare) {
        T pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (compare(arr[j], pivot)) {
                i++;
                // Swap arr[i] and arr[j]
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        // Swap arr[i+1] and arr[high] (pivot)
        T temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        return i + 1;
    }
};


// ============================================================================
// HASH TABLE IMPLEMENTATION
// Purpose: Replace QMap for key-value storage
// Uses: Separate chaining for collision handling
// ============================================================================

/**
 * HashEntry - Key-Value pair for hash table
 */
template <typename K, typename V>
class HashEntry {
public:
    K key;
    V value;
    HashEntry* next;  // For chaining (collision handling)

    HashEntry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
};

/**
 * HashTable Class - Manual implementation of hash map
 * Replaces QMap<QString, int>
 *
 * Features:
 * - Fast O(1) average lookup time
 * - Separate chaining for collision handling
 * - Dynamic resizing when load factor exceeds threshold
 * - Custom hash function for strings
 */
template <typename K, typename V>
class HashTable {
private:
    static const int INITIAL_SIZE = 16;
    static const int MAX_LOAD_FACTOR = 75;  // 75% load factor threshold

    HashEntry<K, V>** table;  // Array of hash entry pointers
    int capacity;             // Size of hash table
    int count;                // Number of elements stored

    /**
     * Hash Function - Convert key to index
     * Uses simple polynomial rolling hash
     */
    int hashFunction(const QString& key) const {
        unsigned int hashValue = 0;
        for (int i = 0; i < key.length(); i++) {
            hashValue = hashValue * 31 + key[i].unicode();
        }
        return hashValue % capacity;
    }

    /**
     * Generic hash for other types (fallback)
     */
    template<typename KeyType>
    int hashFunction(const KeyType& key) const {
        return std::hash<KeyType>{}(key) % capacity;
    }

    /**
     * Unified hash dispatcher
     */
    int hash(const K& key) const {
        return hashFunction(key);
    }

    /**
     * Resize table when load factor is too high
     */
    void resize() {
        int oldCapacity = capacity;
        HashEntry<K, V>** oldTable = table;

        // Double the capacity
        capacity *= 2;
        table = new HashEntry<K, V>*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
        count = 0;

        // Rehash all entries
        for (int i = 0; i < oldCapacity; i++) {
            HashEntry<K, V>* entry = oldTable[i];
            while (entry) {
                insert(entry->key, entry->value);
                HashEntry<K, V>* temp = entry;
                entry = entry->next;
                delete temp;
            }
        }

        delete[] oldTable;
    }

public:
    /**
     * Constructor
     */
    HashTable() : capacity(INITIAL_SIZE), count(0) {
        table = new HashEntry<K, V>*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    /**
     * Destructor
     */
    ~HashTable() {
        clear();
        delete[] table;
    }

    /**
     * Insert or update key-value pair
     * Time Complexity: O(1) average
     */
    void insert(const K& key, const V& value) {
        // Check load factor and resize if needed
        if (count * 100 / capacity > MAX_LOAD_FACTOR) {
            resize();
        }

        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        // Search for existing key
        while (entry) {
            if (entry->key == key) {
                entry->value = value;  // Update existing
                return;
            }
            entry = entry->next;
        }

        // Insert new entry at beginning of chain
        HashEntry<K, V>* newEntry = new HashEntry<K, V>(key, value);
        newEntry->next = table[index];
        table[index] = newEntry;
        count++;
    }

    /**
     * Get value by key
     * Time Complexity: O(1) average
     * Returns default value if key not found
     */
    V value(const K& key, const V& defaultValue = V()) const {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        while (entry) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;
        }

        return defaultValue;
    }

    /**
     * Check if key exists
     * Time Complexity: O(1) average
     */
    bool contains(const K& key) const {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        while (entry) {
            if (entry->key == key) {
                return true;
            }
            entry = entry->next;
        }

        return false;
    }

    /**
     * Remove key-value pair
     * Time Complexity: O(1) average
     */
    void remove(const K& key) {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];
        HashEntry<K, V>* prev = nullptr;

        while (entry) {
            if (entry->key == key) {
                if (prev) {
                    prev->next = entry->next;
                } else {
                    table[index] = entry->next;
                }
                delete entry;
                count--;
                return;
            }
            prev = entry;
            entry = entry->next;
        }
    }

    /**
     * Clear all entries
     */
    void clear() {
        for (int i = 0; i < capacity; i++) {
            HashEntry<K, V>* entry = table[i];
            while (entry) {
                HashEntry<K, V>* temp = entry;
                entry = entry->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    /**
     * Get number of entries
     */
    int size() const {
        return count;
    }

    /**
     * Check if empty
     */
    bool isEmpty() const {
        return count == 0;
    }

    /**
     * Get all keys
     */
    LinkedList<K> keys() const {
        LinkedList<K> result;
        for (int i = 0; i < capacity; i++) {
            HashEntry<K, V>* entry = table[i];
            while (entry) {
                result.append(entry->key);
                entry = entry->next;
            }
        }
        return result;
    }

    /**
     * Operator[] for easy access (returns reference for modification)
     */
    V& operator[](const K& key) {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        // Search for existing key
        while (entry) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;
        }

        // Key not found, insert with default value
        insert(key, V());
        return (*this)[key];  // Recursive call to get reference
    }
};

#endif // DATASTRUCTURES_H
