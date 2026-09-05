class LFUCache {
public:

    class Node {
    public:
        int key;
        int value;
        int freq;
        Node* prev;
        Node* next;

        Node(int key, int value) {
            this->key = key;
            this->value = value;
            this->freq = 1;

            prev = NULL;
            next = NULL;
        }
    };

    class DoublyLinkedList {
    public:
        Node* head;
        Node* tail;
        int size;

        DoublyLinkedList() {
            head = new Node(-1, -1);
            tail = new Node(-1, -1);

            head->next = tail;
            tail->prev = head;

            size = 0;
        }

        void addNode(Node* node) {

            Node* nextNode = head->next;

            node->next = nextNode;
            node->prev = head;

            head->next = node;
            nextNode->prev = node;

            size++;
        }

        void removeNode(Node* node) {

            Node* prevNode = node->prev;
            Node* nextNode = node->next;

            prevNode->next = nextNode;
            nextNode->prev = prevNode;

            size--;
        }

        Node* removeLRU() {

            if(size == 0)
                return NULL;

            Node* node = tail->prev;

            removeNode(node);

            return node;
        }
    };

    int capacity;
    int minFreq;

    unordered_map<int, Node*> keyMap;

    unordered_map<int, DoublyLinkedList*> freqMap;


    LFUCache(int capacity) {

        this->capacity = capacity;
        minFreq = 0;
    }


    void updateFrequency(Node* node) {

        int oldFreq = node->freq;

        // Remove from old frequency list
        DoublyLinkedList* oldList = freqMap[oldFreq];

        oldList->removeNode(node);

        // If this was the minimum frequency
        if(oldFreq == minFreq && oldList->size == 0) {
            minFreq++;
        }

        // Increase frequency
        node->freq++;

        int newFreq = node->freq;

        // Create list if it doesn't exist
        if(freqMap.find(newFreq) == freqMap.end()) {
            freqMap[newFreq] = new DoublyLinkedList();
        }

        // Add to new frequency list
        freqMap[newFreq]->addNode(node);
    }


    int get(int key) {

        // Key doesn't exist
        if(keyMap.find(key) == keyMap.end()) {
            return -1;
        }

        Node* node = keyMap[key];

        // Increase frequency
        updateFrequency(node);

        return node->value;
    }


    void put(int key, int value) {

        // Capacity is 0
        if(capacity == 0)
            return;


        // Key already exists
        if(keyMap.find(key) != keyMap.end()) {

            Node* node = keyMap[key];

            node->value = value;

            // Update frequency
            updateFrequency(node);

            return;
        }


        // Cache is full
        if(keyMap.size() == capacity) {

            DoublyLinkedList* list = freqMap[minFreq];

            Node* lruNode = list->removeLRU();

            keyMap.erase(lruNode->key);

            delete lruNode;
        }


        // Create new node
        Node* newNode = new Node(key, value);

        keyMap[key] = newNode;

        // New nodes always have frequency 1
        minFreq = 1;

        // Create frequency 1 list if needed
        if(freqMap.find(1) == freqMap.end()) {
            freqMap[1] = new DoublyLinkedList();
        }

        freqMap[1]->addNode(newNode);
    }
};