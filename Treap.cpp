#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>

using namespace std;

struct Node {
    string key;
    uint64_t number;
    int priority;
    Node* left;
    Node* right;

    Node(const string& m_key, uint64_t num) : key(m_key), number(num), priority(rand()), left(nullptr), right(nullptr) {}
};

class Treap {
private:
    Node* root;

    Node* insert(Node* root, const string& key, uint64_t number);
    Node* remove(Node* root, const string& key);
    pair<bool, uint64_t> search(Node* root, const string& key) const;
    Node* merge(Node* left, Node* right);

public:
    Treap() : root(nullptr) {}
    ~Treap();

    void insert(const string& key, uint64_t number);
    void remove(const string& key);
    pair<bool, uint64_t> search(const string& key) const;
};

Node* Treap::insert(Node* root, const string& key, uint64_t number) {
    if (!root) {
        return new Node(key, number);
    }

    if (key == root->key) {
        return root; 
    }

    if (key < root->key) {
        root->left = insert(root->left, key, number);
        if (root->left->priority > root->priority) {
            Node* left = root->left;
            root->left = left->right;
            left->right = root;
            return left;
        }
    } else {
        root->right = insert(root->right, key, number);
        if (root->right->priority > root->priority) {
            Node* right = root->right;
            root->right = right->left;
            right->left = root;
            return right;
        }
    }

    return root;
}
Node* Treap::merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    } else {
        right->left = merge(left, right->left);
        return right;
    }
}

Node* Treap::remove(Node* root, const string& key) {
    if (!root) {
        return root;
    }

    if (key < root->key) {
        root->left = remove(root->left, key);
    } else if (key > root->key) {
        root->right = remove(root->right, key);
    } else {
        Node* temp = root;
        if (!root->left) {
            root = root->right;
            delete temp;
        } else if (!root->right) {
            root = root->left;
            delete temp;
        } else {
            root = merge(root->left, root->right);
            delete temp;
        }
    }

    return root;
}


pair<bool, uint64_t> Treap::search(Node* root, const string& key) const {
    while (root) {
        if (key == root->key) {
            return {true, root->number};
        } else if (key < root->key) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return {false, 0};
}


void Treap::insert(const string& key, uint64_t number) {
    root = insert(root, key, number);
}

void Treap::remove(const string& key) {
    root = remove(root, key);
}

pair<bool, uint64_t> Treap::search(const string& key) const {
    return search(root, key);
}

Treap::~Treap() {
    while (root) {
        root = remove(root, root->key);
    }
}

void toLower(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int main() {
    Treap my_dictionary;

    string command;
    string key;
    uint64_t value;

    while (cin >> command) {
        if (command == "+") {
            cin >> key >> value;
            toLower(key);
            auto result = my_dictionary.search(key);
            if (result.first) {
                cout << "Exist\n";
            } else {
                my_dictionary.insert(key, value);
                cout << "OK\n";
            }
        } else if (command == "-") {
            cin >> key;
            toLower(key);
            auto result = my_dictionary.search(key);
            if (result.first) {
                my_dictionary.remove(key);
                cout << "OK\n";
            } else {
                cout << "NoSuchWord\n";
            }
        } else {
            toLower(command);
            auto result = my_dictionary.search(command);
            if (result.first) {
                cout << "OK: " << result.second << "\n";
            } else {
                cout << "NoSuchWord\n";
            }
        }
    }

    return 0;
}
