#include <iostream>

enum Color {
    RED = 0,
    BLACK = 1
};

template <typename Key>
struct Node {
    Key key{};
    int color = Color::RED;
    Node *parent = nullptr;
    Node *left   = nullptr;
    Node *right  = nullptr;
};

template <typename Key>
class RBStablo {
private:
    Node<Key>* root;
    int size;
    void _deallocate(Node<Key> *node);
    void _inorder(Node<Key> *node);
    Node<Key>* _find(Key value);
    Node<Key>* _minimum(Node<Key> *node);
public:
    RBStablo(): root(nullptr), size(0) {}
    ~RBStablo() { _deallocate(root); };
    void inorder();
    void leftRotate(Node<Key> *x);
    void rightRotate(Node<Key> *y);
    void insert(Key key);
    void insert(Node<Key> *node, Key key);
    void insertFixup(Node<Key> *z);
    void transplant(Node<Key> *u, Node<Key> *v);
    void deleteNode(Key value);
    void deleteFixup(Node<Key> *x);
};

template<typename Key>
void RBStablo<Key>::_deallocate(Node<Key> *node) {
    if (node) {
        _deallocate(node->left);
        _deallocate(node->right);
        delete node;
    }
}

template<typename Key>
void RBStablo<Key>::_inorder(Node<Key> *node) {
    if (!node) {
        return;
    }

    _inorder(node->left);
    std::cout << node->key << " " << (node->color == Color::RED ? "(R) " : "(B) ");
    _inorder(node->right);
}

template<typename Key>
void RBStablo<Key>::inorder() {
    _inorder(this->root);
}

template<typename Key>
Node<Key>* RBStablo<Key>::_find(Key value) {
    Node<Key> *node = this->root;

    while (node) {
        if (value < node->key) {
            node = node->left;
        } else if (value > node->key) {
            node = node->right;
        } else {
            break;
        }
    }
    return node;
}

template<typename Key>
Node<Key>* RBStablo<Key>::_minimum(Node<Key> *node) {
    if (!node->left) {
        return node;
    }
    return _minimum(node->left);
}

template<typename Key>
void RBStablo<Key>::leftRotate(Node<Key>* x) {
    Node<Key> *y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template<typename Key>
void RBStablo<Key>::rightRotate(Node<Key> *y) {
    Node<Key>* x = y->left;
    y->left = x->right;
    if (x->right) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (!y->parent) {
        this->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

template<typename Key>
void RBStablo<Key>::insert(Node<Key> *node, Key key) {
    if (key < node->key) {
        if (!node->left) {
            node->left = new Node<Key>();
            node->left->key = key;
            node->left->parent = node;
            insertFixup(node->left);
        } else {
            insert(node->left, key);
        }
    } else {
        if (!node->right) {
            node->right = new Node<Key>();
            node->right->key = key;
            node->right->parent = node;
            insertFixup(node->right);
        } else {
            insert(node->right, key);
        }
    }
}

template<typename Key>
void RBStablo<Key>::insert(Key key) {
    if (!this->root) {
        this->root = new Node<Key>();
        this->root->key = key;
        this->root->color = Color::BLACK;
    } else {
        insert(this->root, key);
    }
}

template<typename Key>
void RBStablo<Key>::insertFixup(Node<Key> *z) {
    while (z->parent && z->parent->color == Color::RED) {
        Node<Key> *grandparent = z->parent->parent;
        if (!grandparent) {
            break;
        }
        Node<Key> *uncle = this->root;
        if (z->parent == grandparent->left) {
            if (grandparent->right) {
                uncle = grandparent->right;
            }
            if (uncle && uncle->color == Color::RED) {
                z->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                grandparent->color = Color::RED;
                if (grandparent->key != this->root->key) {
                    z = grandparent;
                } else {
                    break;
                }
            } else if (z == grandparent->left->right) {
                leftRotate(z->parent);
                z = z->left;
            } else {
                z->parent->color = Color::BLACK;
                grandparent->color = Color::RED;
                rightRotate(grandparent);
                if (grandparent->key != this->root->key) {
                    z = grandparent;
                } else {
                    break;
                }
            }
        } else {
            if (grandparent->left) {
                uncle = grandparent->left;
            }
            if (uncle && uncle->color == Color::RED) {
                z->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                grandparent->color = Color::RED;
                if (grandparent->key != this->root->key) {
                    z = grandparent;
                } else {
                    break;
                }
            } else if (z == grandparent->right->left) {
                rightRotate(z->parent);
                z = z->right;
            } else {
                z->parent->color = Color::BLACK;
                grandparent->color = Color::RED;
                leftRotate(grandparent);
                if (grandparent->key != this->root->key) {
                    z = grandparent;
                } else {
                    break;
                }
            }
        }
    }
    this->root->color = Color::BLACK;
}

template<typename Key>
void RBStablo<Key>::transplant(Node<Key> *u, Node<Key> *v) {
    if (!u->parent) {
        this->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

template<typename Key>
void RBStablo<Key>::deleteNode(Key value) {
    Node<Key> *z = this->_find(value);
    if (!z) {
        std::cout << "Nepostojeci cvor" << std::endl;
        return;
    }
    Node<Key> *y = z, *x = nullptr;
    int yOrigColor = y->color;
    if (!z->left) {
        x = z->right;
        transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = _minimum(z->right);
        yOrigColor = y->color;
        x = y->right;
        if (x && y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z,y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (yOrigColor == Color::BLACK) {
        deleteFixup(x);
    }
}

template<typename Key>
void RBStablo<Key>::deleteFixup(Node<Key> *x) {
    while (x && x != this->root && x->color == Color::BLACK) {
        Node<Key> *w = this->root;
        if (x == x->parent->left) {
            if (x->parent->right) {
                w = x->parent->right;
            }
            if (w) {
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((!w->left && !w->right) ||
                    (w->left && w->right && w->left->color == Color::BLACK && w->right->color == Color::BLACK)) {
                    w->color = Color::RED;
                    x = x->parent;
                } else if (w->right && w->right->color == Color::BLACK) {
                    w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                } else {
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    if (w->right) {
                        w->right->color = Color::BLACK;
                    }
                    leftRotate(x->parent);
                    x = this->root;
                }
            }
        } else if (x == x->parent->right) {
            if (x->parent->left) {
                w = x->parent->left;
            }
            if (w) {
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((!w->left && !w->right) ||
                    (w->left && w->right && w->left->color == Color::BLACK && w->right->color == Color::BLACK)) {
                    w->color = Color::RED;
                    x = x->parent;
                } else if (w->left && w->left->color == Color::BLACK) {
                    w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                } else {
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    if (w->left) {
                        w->left->color = Color::BLACK;
                    }
                    rightRotate(x->parent);
                    x = this->root;
                }
            }
        }
    }
    if (x) {
        x->color = Color::BLACK;
    }
}

void options() {
    std::cout << "\n1 - Umetanje novog cvora";
    std::cout << "\n2 - INORDER ispis cvorova na ekranu";
    std::cout << "\n3 - Obrisi cvor";
    std::cout << "\n4 - Izlaz\n";
    std::cout << "\nUnesite izbor: ";
}

int main() {
    RBStablo<int> rbStablo;
    for(;;) {
        options();
        int unos;
        int value;
        std::cin >> unos;

        switch (unos) {
            case 1:
                std::cout << "\nUnesi vrijednost: ";
                std:: cin >> value;
                rbStablo.insert(value);
                break;
            case 2:
                std::cout << "\n";
                rbStablo.inorder();
                std::cout << "\n";
                break;
            case 3:
                std::cout << "\nUnesi vrijednost: ";
                std:: cin >> value;
                rbStablo.deleteNode(value);
                break;
            default:
                return 0;
        }
    }
}