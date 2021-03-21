#include <iostream>
#include <cmath>

const double goldenRatio = 1.618;

template <typename Key>
struct Node {
    Key key;
    Node *right;
    Node *left;
    Node *parent;
    Node *child;
    int degree;
    bool mark;
};

template <typename Key>
class FibonacciHeap {
private:
    Node<Key> *minNode;
    int size;
    void _insert(Node<Key> *x);
    Node<Key>* _extractMinNode();
    Node<Key>* _createNode(Key key);
    void _unparentAll(Node<Key> *x);
    Node<Key>* _merge(Node<Key> *a, Node<Key>* b);
    void _removeFromRootList(Node<Key> *x);
public:
    FibonacciHeap(): size(0), minNode(nullptr) {}
    ~FibonacciHeap() = default;
    Node<Key>* minimum() { return minNode; }
    int getSize() { return size; }
    Node<Key>* insert(Key key);
    void heapUnion(FibonacciHeap<Key> &h);
    Key extractMin();
    void consolidate();
    void link(Node<Key> *x, Node<Key> *y);
    void decreaseKey(Node<Key> *x, int newKey);
    void cut(Node<Key> *x, Node<Key> *y);
    void cascadingCut(Node<Key> *y);
    void deleteNode(Node<Key> *x);
};

template <typename Key>
Node<Key>* FibonacciHeap<Key>::_extractMinNode() {
    Node<Key> *min = this->minNode;
    if (min) {
        _unparentAll(min->child);
        _merge(min, min->child);
        _removeFromRootList(min);
        if (min == min->right) {
            this->minNode = nullptr;
        } else {
            this->minNode = min->right;
            consolidate();
        }
        this->size--;
    }
    return min;
}

template <typename Key>
Node<Key>* FibonacciHeap<Key>::_createNode(Key key) {
    auto *newNode = new Node<Key>;
    newNode->key = key;
    newNode->left = newNode;
    newNode->right = newNode;
    newNode->parent = nullptr;
    newNode->child = nullptr;
    newNode->degree = 0;
    newNode->mark = false;
    return newNode;
}

template <typename Key>
void FibonacciHeap<Key>::_unparentAll(Node<Key> *x) {
    if (!x) {
        return;
    }
    Node<Key> *y = x;
    do {
        y->parent = nullptr;
        y = y->right;
    } while (y != x);
}

template <typename Key>
Node<Key>* FibonacciHeap<Key>::_merge(Node<Key> *a, Node<Key> *b) {
    if (!a) {
        return b;
    }
    if (!b) {
        return a;
    }
    if (a->key > b->key) {
        Node<Key>* temp = a;
        a = b;
        b = temp;
    }
    Node<Key> *aRight = a->right;
    Node<Key> *bLeft = b->left;
    a->right = b;
    b->left = a;
    aRight->left = bLeft;
    bLeft->right = aRight;
    return a;
}

template <typename Key>
void FibonacciHeap<Key>::_removeFromRootList(Node<Key> *x) {
    if (x->right == x) {
        return;
    }
    Node<Key> *leftNode = x->left;
    Node<Key> *rightNode = x->right;
    leftNode->right = rightNode;
    rightNode->left = leftNode;
}

template<typename Key>
void FibonacciHeap<Key>::_insert(Node<Key> *x) {
    this->minNode = _merge(this->minNode, x);
    this->size++;
}

template<typename Key>
Node<Key>* FibonacciHeap<Key>::insert(Key key) {
    auto *node = _createNode(key);
    _insert(node);
    return node;
}

template<typename Key>
void FibonacciHeap<Key>::heapUnion(FibonacciHeap<Key> &h) {
    this->minNode = _merge(this->minNode, h.minNode);
    this->size += h.getSize();
    h.minNode = nullptr;
    h.size = 0;
}

template<typename Key>
Key FibonacciHeap<Key>::extractMin() {
    Node<Key> *min = _extractMinNode();
    if (!min) {
        std::cout << "No roots left after consolidate." << std::endl;
        return 0;
    }
    Key key = min->key;
    delete min;
    return key;
}

template<typename Key>
void FibonacciHeap<Key>::consolidate() {
    int Dn = (int)(log2(this->size)/log2(goldenRatio));
    auto **a = new Node<Key>*[Dn+1];
    for (int i = 0; i < Dn+1; i++) {
        a[i] = nullptr;
    }
    Node<Key> *x = this->minNode;
    bool breakFlag = false;
    while (true) {
        int d = x->degree;
        while (a[d]) {
            Node<Key> *y = a[d];
            if (y == x) {
                breakFlag = true;
                break;
            }
            if (x->key > y->key) {
                Node<Key> *temp = x;
                x = y;
                y = temp;
            }
            link(y,x);
            a[d++] = nullptr;
        }
        if (breakFlag) {
            break;
        }
        a[x->degree] = x;
        x = x->right;
    }
    this->minNode = x;
    Node<Key> *it = x;
    do {
        if (it->key < this->minNode->key) {
            this->minNode = it;
        }
        it = it->right;
    } while (it != x);
    delete[] a;
}

template<typename Key>
void FibonacciHeap<Key>::link(Node<Key> *x, Node<Key> *y) {
    _removeFromRootList(x);
    x->left = x->right = x;
    x->parent = y;
    y->child = _merge(y->child, x);
    y->degree++;
    x->mark = false;
}

template<typename Key>
void FibonacciHeap<Key>::decreaseKey(Node<Key> *x, int newKey) {
    if (newKey > x->key) {
        std::cout << "New key greater than current key" << std::endl;
    }
    x->key = newKey;
    Node<Key> *y = x->parent;
    if (y && x->key < y->key) {
        cut(x,y);
        cascadingCut(y);
    }
    if (x->key < this->minNode->key) {
        this->minNode = x;
    }
}

template<typename Key>
void FibonacciHeap<Key>::cut(Node<Key> *x, Node<Key> *y) {
    _removeFromRootList(x);
    if (x->right == x) {
        y->child = nullptr;
    } else {
        y->child = x->right;
    }
    y->degree--;
    _merge(this->minNode, x);
    x->parent = nullptr;
    x->mark = false;
}

template<typename Key>
void FibonacciHeap<Key>::cascadingCut(Node<Key> *y) {
    Node<Key> *z = y->parent;
    if (z) {
        if (!y->mark) {
            y->mark = true;
        } else {
            cut(y,z);
            cascadingCut(z);
        }
    }
}

template<typename Key>
void FibonacciHeap<Key>::deleteNode(Node<Key> *x) {
    if (x) {
        Node<Key> *t1 = x;
        do {
            Node<Key> *t2 = t1;
            t1 = t1->right;
            deleteNode(t2->child);
            delete t2;
        } while (t1 != x);
    }
}

template <typename Key>
void testInsert(FibonacciHeap<Key> &fibHeap) {
    std::cout << "********** BEGIN: Fibonacci heap insert test **********" << std::endl;
    fibHeap.insert(10);
    fibHeap.insert(7);
    fibHeap.insert(2);
    fibHeap.insert(4);

    displayHeap(fibHeap);
    std::cout << "********** END: Fibonacci heap insert test ************" << std::endl << std::endl;
}

template <typename Key>
void testUnion(FibonacciHeap<Key> &fibHeap) {
    std::cout << "********** BEGIN: Fibonacci heap union test ***********" << std::endl;
    FibonacciHeap<Key> another;
    another.insert(16);
    another.insert(1);
    another.insert(98);
    another.insert(3);

    fibHeap.heapUnion(another);

    displayHeap(fibHeap);
    std::cout << "********** END: Fibonacci heap union test *************" << std::endl << std::endl;
}

template <typename Key>
void testExtractMin(FibonacciHeap<Key> &fibHeap) {
    std::cout << "********** BEGIN: Fibonacci heap extract min test ******" << std::endl;
    fibHeap.extractMin();
    displayHeap(fibHeap);

    fibHeap.heapUnion(fibHeap);

    fibHeap.extractMin();
    displayHeap(fibHeap);
    std::cout << "********** END: Fibonacci heap extract min test ********" << std::endl << std::endl;
}

template <typename Key>
void testDecreaseKey(FibonacciHeap<Key> &fibHeap) {
    std::cout << "********** BEGIN: Fibonacci heap decrease key test *****" << std::endl;
    fibHeap.insert(5);
    fibHeap.insert(4);
    fibHeap.insert(12);
    fibHeap.decreaseKey(fibHeap.minimum(), 1);
    displayHeap(fibHeap);
    fibHeap.decreaseKey(fibHeap.minimum(), 2);
    std::cout << "********** END: Fibonacci heap decrease key test *******" << std::endl;
}

template <typename Key>
void displayHeap(FibonacciHeap<Key> fibHeap) {
    Node<Key> *it = fibHeap.minimum();
    if (!it) {
        return;
    }
    do {
        std::cout << "Root: " << it->key;
        if (it->child) {
            Node<Key> *child = it->child;
            std::cout << ", children: ";
            while (child) {
                std::cout << child->key << " ";
                child = child->child;
            }
        } else {
            std::cout << ", no children.";
        }
        std::cout << std::endl;
        it = it->right;
    } while (it->right && it != fibHeap.minimum());
}

int main() {
    FibonacciHeap<int> fibHeap;
    testInsert(fibHeap);
    testUnion(fibHeap);
    testExtractMin(fibHeap);
    testDecreaseKey(fibHeap);
    return 0;
}
