/**
 * ============================================================================
 * Custom Data Structures Implementation (datastructures.h)
 * ============================================================================
 *
 * This file contains manually implemented data structures used throughout
 * the Course Registration System. No STL containers (vector, map, etc.) are used.
 *
 * Data Structures Implemented:
 *
 * 1. LINKED LIST (Doubly Linked)
 *    - Purpose: Store courses, combinations, search results
 *    - Operations: append, insert, remove, access by index
 *    - Time Complexity: O(1) append, O(n) access/search
 *    - Includes QuickSort and Linear Search algorithms
 *
 * 2. HASH TABLE (with Chaining)
 *    - Purpose: Map day names to indices, group courses by name
 *    - Operations: insert, lookup, remove
 *    - Time Complexity: O(1) average for all operations
 *    - Handles collisions using linked chains
 *
 * Why These Structures?
 * - LinkedList: Dynamic size, efficient insert/delete, no wasted memory
 * - HashTable: Fast lookup by key, efficient for grouping and mapping
 *
 * ============================================================================
 */

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>
#include <functional>

struct Course;  /* Forward declaration for Course struct */

/**
 * Node for linked list - each box stores data and links to next/previous box
 */
template <typename T>
class Node {
public:
    T data;           /* Data stored in this box */
    Node* next;       /* Link to next box */
    Node* prev;       /* Link to previous box */

    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

/**
 * LinkedList - chain of connected boxes that can grow or shrink
 * Used to: store courses, save combinations, keep search results
 * Why use: easy to add/remove items, can go forward and backward
 */
template <typename T>
class LinkedList {
private:
    Node<T>* head;    /* First box in chain */
    Node<T>* tail;    /* Last box in chain */
    int count;        /* How many boxes we have */

public:
    /* Constructor - start with empty chain */
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    /* Copy constructor - make a copy of another chain */
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node<T>* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
    }

    /* Assignment operator - copy one chain to another */
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

    /* Destructor - clean up all boxes when done */
    ~LinkedList() {
        clear();
    }

    /* Add new box to end of chain - create new link */
    void append(const T& value) {
        Node<T>* newNode = new Node<T>(value);

        if (!head) {
            head = tail = newNode;  /* First box */
        } else {
            tail->next = newNode;   /* Old last box links to new box */
            newNode->prev = tail;   /* New box links back to old last box */
            tail = newNode;         /* Update last box pointer */
        }
        count++;
    }

    /* Insert box at specific spot - keep all links working */
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
            if (current->prev) current->prev->next = newNode;  /* Fix link from previous box */
            current->prev = newNode;  /* Fix link from current box */
            count++;
        }
    }

    /* Remove box at position - reconnect the chain */
    void removeAt(int index) {
        if (index < 0 || index >= count) return;

        Node<T>* toDelete = getNodeAt(index);
        if (!toDelete) return;

        /* Fix link from previous box */
        if (toDelete->prev) {
            toDelete->prev->next = toDelete->next;
        } else {
            head = toDelete->next;  /* Removing first box */
        }

        /* Fix link from next box */
        if (toDelete->next) {
            toDelete->next->prev = toDelete->prev;
        } else {
            tail = toDelete->prev;  /* Removing last box */
        }

        delete toDelete;
        count--;
    }

    /* Remove last box - used when backtracking combinations */
    void removeLast() {
        if (count > 0) {
            removeAt(count - 1);
        }
    }

    /* Get data from box at position (can modify) */
    T& operator[](int index) {
        Node<T>* node = getNodeAt(index);
        if (!node) {
            /* Bounds check failed - create a static default value to return reference to */
            static T defaultValue = T();
            return defaultValue;
        }
        return node->data;
    }

    /* Get data from box at position (read only) */
    const T& operator[](int index) const {
        Node<T>* node = getNodeAt(index);
        if (!node) {
            /* Bounds check failed - return reference to static default value */
            static T defaultValue = T();
            return defaultValue;
        }
        return node->data;
    }

    const T& at(int index) const {
        return (*this)[index];
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    /* Clear all boxes - break all links and free memory */
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;  /* Move to next box */
            delete temp;        /* Delete current box */
        }
        head = tail = nullptr;
        count = 0;
    }

    const T& first() const {
        if (!head) {
            /* Empty list - return default value */
            static T defaultValue = T();
            return defaultValue;
        }
        return head->data;
    }

    const T& last() const {
        if (!tail) {
            /* Empty list - return default value */
            static T defaultValue = T();
            return defaultValue;
        }
        return tail->data;
    }

    /**
     * ========================================================================
     * QUICKSORT ALGORITHM
     * ========================================================================
     * Time Complexity: O(n log n) average, O(n²) worst case
     * Space Complexity: O(n) for temporary array
     *
     * How it works:
     * 1. Choose a pivot element (last element in partition)
     * 2. Partition: move smaller elements left, larger elements right
     * 3. Recursively sort left and right partitions
     *
     * Used for: Sorting courses by name, day, time, or classroom
     * ========================================================================
     */
    void quickSort(std::function<bool(const T&, const T&)> compare) {
        if (count <= 1) return;  /* Base case: already sorted */

        /* Step 1: Convert linked list to array (easier to partition) */
        T* arr = new T[count];
        Node<T>* current = head;
        for (int i = 0; i < count; i++) {
            arr[i] = current->data;
            current = current->next;
        }

        /* Step 2: Apply QuickSort algorithm */
        quickSortHelper(arr, 0, count - 1, compare);

        /* Step 3: Copy sorted data back to linked list */
        current = head;
        for (int i = 0; i < count; i++) {
            current->data = arr[i];
            current = current->next;
        }

        delete[] arr;
    }

    /**
     * ========================================================================
     * LINEAR SEARCH ALGORITHM
     * ========================================================================
     * Time Complexity: O(n) - checks every element once
     * Space Complexity: O(k) where k = number of matches found
     *
     * How it works:
     * 1. Start from the first node (head)
     * 2. Check each node against the search condition
     * 3. If match found, add index to results
     * 4. Continue until end of list (null)
     *
     * Used for: Finding courses that match search keywords
     * ========================================================================
     */
    LinkedList<int> linearSearch(std::function<bool(const T&)> condition) const {
        LinkedList<int> results;
        Node<T>* current = head;  /* Start from first node */
        int index = 0;

        /* Traverse entire list checking each element */
        while (current) {
            if (condition(current->data)) {
                results.append(index);  /* Found a match! */
            }
            current = current->next;  /* Move to next node via link */
            index++;
        }

        return results;  /* Return all matching indices */
    }

    /* Iterator - allows traversing linked list connections with for-each loop */
    class Iterator {
    private:
        Node<T>* current;  /* Current position in linked list */
    public:
        Iterator(Node<T>* node) : current(node) {}

        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }

        /* Move to next element by following next pointer */
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
     * Get node at index - optimized bidirectional traversal
     * Uses head->next connections for first half, tail->prev for second half
     */
    Node<T>* getNodeAt(int index) const {
        if (index < 0 || index >= count) return nullptr;

        Node<T>* current;
        if (index < count / 2) {
            /* Traverse from head using next connections */
            current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            /* Traverse from tail using prev connections (faster for end elements) */
            current = tail;
            for (int i = count - 1; i > index; i--) {
                current = current->prev;
            }
        }
        return current;
    }

    /**
     * QuickSort Helper - Recursive function that sorts subarrays
     * Uses divide-and-conquer strategy
     */
    void quickSortHelper(T* arr, int low, int high, std::function<bool(const T&, const T&)> compare) {
        if (low < high) {
            /* Partition the array and get pivot position */
            int pivotIndex = partition(arr, low, high, compare);

            /* Recursively sort elements before and after pivot */
            quickSortHelper(arr, low, pivotIndex - 1, compare);   /* Left subarray */
            quickSortHelper(arr, pivotIndex + 1, high, compare);  /* Right subarray */
        }
    }

    /**
     * Partition Function - Core of QuickSort algorithm
     *
     * Rearranges array so that:
     * - Elements smaller than pivot are on the left
     * - Elements larger than pivot are on the right
     * - Pivot is in its final sorted position
     *
     * @return The final position of the pivot element
     */
    int partition(T* arr, int low, int high, std::function<bool(const T&, const T&)> compare) {
        T pivot = arr[high];  /* Choose last element as pivot */
        int i = low - 1;      /* Index of smaller element */

        /* Compare each element with pivot */
        for (int j = low; j < high; j++) {
            if (compare(arr[j], pivot)) {
                i++;
                /* Swap arr[i] and arr[j] */
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        /* Place pivot in correct position */
        T temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        return i + 1;  /* Return pivot's final position */
    }
};


/**
 * Hash table entry - stores key-value pair and connection to next entry in chain
 */
template <typename K, typename V>
class HashEntry {
public:
    K key;             /* The key for lookup */
    V value;           /* The stored value */
    HashEntry* next;   /* Connection to next entry in case of collision (chaining) */

    HashEntry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
};

/**
 * Hash Table with Chaining - fast key-value storage using hash function
 * Used to map day names to row indices, group courses by name, etc.
 * Advantages: O(1) average lookup/insert, handles collisions with linked chains
 */
template <typename K, typename V>
class HashTable {
private:
    static const int INITIAL_SIZE = 16;
    static const int MAX_LOAD_FACTOR = 75;  /* Resize when 75% full */

    HashEntry<K, V>** table;  /* Array of pointers to entry chains */
    int capacity;             /* Size of hash table */
    int count;                /* Number of elements stored */

    /* Hash function for QString - converts string to index */
    int hashFunction(const QString& key) const {
        unsigned int hashValue = 0;
        for (int i = 0; i < key.length(); i++) {
            hashValue = hashValue * 31 + key[i].unicode();  /* Polynomial rolling hash */
        }
        return hashValue % capacity;  /* Map to table size */
    }

    /* Generic hash function for other types */
    template<typename KeyType>
    int hashFunction(const KeyType& key) const {
        return std::hash<KeyType>{}(key) % capacity;
    }

    int hash(const K& key) const {
        return hashFunction(key);
    }

    /* Resize table when load factor exceeded - rehashes all entries */
    void resize() {
        int oldCapacity = capacity;
        HashEntry<K, V>** oldTable = table;

        capacity *= 2;  /* Double the size */
        table = new HashEntry<K, V>*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
        count = 0;

        /* Rehash all entries (connections will be rebuilt) */
        for (int i = 0; i < oldCapacity; i++) {
            HashEntry<K, V>* entry = oldTable[i];
            while (entry) {  /* Follow chain connections */
                insert(entry->key, entry->value);
                HashEntry<K, V>* temp = entry;
                entry = entry->next;  /* Move to next in chain */
                delete temp;
            }
        }

        delete[] oldTable;
    }

public:
    /* Constructor - initializes empty hash table with chains */
    HashTable() : capacity(INITIAL_SIZE), count(0) {
        table = new HashEntry<K, V>*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;  /* No chains initially */
        }
    }

    /* Destructor - cleans up all chains */
    ~HashTable() {
        clear();
        delete[] table;
    }

    /* Insert key-value pair - handles collisions by chaining */
    void insert(const K& key, const V& value) {
        if (count * 100 / capacity > MAX_LOAD_FACTOR) {
            resize();  /* Prevent chains from getting too long */
        }

        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        /* Search chain for existing key */
        while (entry) {
            if (entry->key == key) {
                entry->value = value;  /* Update existing */
                return;
            }
            entry = entry->next;  /* Follow chain connection */
        }

        /* Add new entry at head of chain */
        HashEntry<K, V>* newEntry = new HashEntry<K, V>(key, value);
        newEntry->next = table[index];  /* Connect to existing chain */
        table[index] = newEntry;
        count++;
    }

    /* Get value by key - follows chain to find match */
    V value(const K& key, const V& defaultValue = V()) const {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        while (entry) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;  /* Follow chain */
        }

        return defaultValue;  /* Not found */
    }

    /* Check if key exists - searches chain at hashed index */
    bool contains(const K& key) const {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        while (entry) {
            if (entry->key == key) {
                return true;
            }
            entry = entry->next;  /* Follow chain */
        }

        return false;
    }

    /* Remove entry - updates chain connections */
    void remove(const K& key) {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];
        HashEntry<K, V>* prev = nullptr;

        while (entry) {
            if (entry->key == key) {
                if (prev) {
                    prev->next = entry->next;  /* Reconnect chain */
                } else {
                    table[index] = entry->next;  /* Update head */
                }
                delete entry;
                count--;
                return;
            }
            prev = entry;
            entry = entry->next;  /* Follow chain */
        }
    }

    /* Clear all entries - breaks all chain connections */
    void clear() {
        for (int i = 0; i < capacity; i++) {
            HashEntry<K, V>* entry = table[i];
            while (entry) {  /* Follow chain */
                HashEntry<K, V>* temp = entry;
                entry = entry->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    /* Get all keys - traverses all chains */
    LinkedList<K> keys() const {
        LinkedList<K> result;
        for (int i = 0; i < capacity; i++) {
            HashEntry<K, V>* entry = table[i];
            while (entry) {  /* Follow chain */
                result.append(entry->key);
                entry = entry->next;
            }
        }
        return result;
    }

    /* Access operator - auto-creates entry if not exists */
    V& operator[](const K& key) {
        int index = hash(key);
        HashEntry<K, V>* entry = table[index];

        while (entry) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;  /* Follow chain */
        }

        insert(key, V());  /* Create new entry in chain */
        return (*this)[key];
    }
};

#endif // DATASTRUCTURES_H
