#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <set>


using namespace std;

template <class ValueType> 
struct Node {
    ValueType key;
    Node *left;
    Node *right;
    int depth;
};

template <class ValueType>
class Set {
    size_t size;
    Node <ValueType> *root;
    Node <ValueType> *insert (const ValueType &key, Node <ValueType> *ptr) {
        if (ptr == NULL) {
            ptr = new Node <ValueType>;
            ptr -> key = key;
            ptr -> depth = 0;
            ptr -> left = NULL;
            ptr -> right = NULL;
        } else if (key < ptr -> key) {
            ptr -> left = insert(key, ptr -> left);
            if (get_depth(ptr -> left) - get_depth(ptr -> right) == 2) {
                if (key < ptr -> left -> key) {
                    ptr = rightSmall(ptr);
                } else {
                    ptr = rightBig(ptr);
                }
            }
        } else if (key > ptr -> key) {
            ptr -> right = insert(key, ptr -> right);
            if (get_depth(ptr -> right) - get_depth(ptr -> left) == 2) {
                if (key > ptr -> right -> key) {
                    ptr = leftSmall(ptr);
                } else {
                    ptr = leftBig(ptr);
                }
            }
        }
        ptr -> depth = max (get_depth(ptr -> right), get_depth(ptr -> left)) + 1;
        return ptr;
    }
    int get_depth(Node <ValueType> *ptr) {
        if (ptr == NULL)
            return -1;
        else
            return ptr -> depth;
    }
    ////////////////
    Node <ValueType> *leftSmall (Node <ValueType> *vertex) {
        Node <ValueType> *tmp =  vertex -> right;
        vertex -> right = tmp -> left;
        tmp -> left = vertex;
        vertex -> depth = max(get_depth(vertex -> left), get_depth(vertex -> right)) + 1;
        tmp -> depth = max(get_depth(vertex -> right), vertex -> depth) + 1;
        return tmp;
    }
    Node <ValueType> *rightSmall (Node <ValueType> *vertex) {
        Node <ValueType> *tmp =  vertex -> left;
        vertex -> left = tmp -> right;
        tmp -> right = vertex;
        vertex -> depth = max(get_depth(vertex -> left), get_depth(vertex -> right)) + 1;
        tmp -> depth = max(get_depth(vertex -> left), vertex -> depth) + 1;
        return tmp;
    }
    Node <ValueType> *leftBig (Node <ValueType> *vertex) {
        vertex -> right = rightSmall(vertex -> right);
        Node <ValueType> *tmp = leftSmall(vertex);
        return tmp;
    }
    Node <ValueType> *rightBig (Node <ValueType> *vertex) {
        vertex -> left = leftSmall(vertex -> left);
        Node <ValueType> *tmp = rightSmall(vertex);
        return tmp;
    }
    ////////////////
    void is_legit() {
        int fd = open("text.txt", O_RDONLY, S_IRUSR);
        int tests, finds;
        int t, f;
        set <ValueType> stlset;
        dup2(fd, 0);
        cin >> tests;
        for (int i = 0; i < tests; i++) {
            cin >> t;
            root = insert(t, root);
            stlset.insert(t);
        }
        cin >> finds;
        for (int i = 0; i < finds; i++) {
            cin >> f;
            bool a = (find(f, root) != NULL);
            bool b = (stlset.find(f) != stlset.end());
            assert (a == b);
        }
        close(fd);
    }
    Node <ValueType> *find (const ValueType &key, Node <ValueType> *ptr) {
        if (ptr == NULL) {
            return NULL;
        }
        if (ptr -> key == key) {
            return ptr;
        }
        if (ptr -> key < key) {
            return find(key, ptr -> right);
        } else {
            return find(key, ptr -> left);
        }
    }
    Node <ValueType> *erase (const ValueType &key, Node <ValueType> *ptr) {
        if (ptr == NULL) {
            return NULL;
        } else if (key < ptr -> key){
            ptr -> left = erase(key, ptr -> left);
        } else if (key > ptr -> key){
            ptr -> right = erase(key, ptr -> right);
        } else if (key == ptr -> key) {
            if (ptr -> left != NULL && ptr -> right != NULL) {
                Node <ValueType> *tmp;
                tmp = find_min(ptr -> right);
                ptr -> key = tmp -> key;
                ptr -> right = erase(ptr -> key, ptr -> right);
            } else {
                if (ptr -> left == NULL) {
                    ptr = ptr -> right;
                } else if (ptr -> right == NULL) {
                    ptr = ptr -> left;
                }
            }
        }
        if (ptr == NULL) {
            return ptr;
        }
        ptr -> depth = max (get_depth(ptr -> right), get_depth(ptr -> left)) + 1;
        if (get_depth(ptr -> left) - get_depth(ptr -> right) == 2) {
            if (get_depth(ptr -> left -> left) - get_depth(ptr -> left -> right) == 1) {
                return leftSmall(ptr);
            } else {
                return leftBig(ptr);
            }
        } else if (get_depth(ptr -> right) - get_depth(ptr -> left) == 2) {
            if (get_depth(ptr -> right -> right) - get_depth(ptr -> right -> left) == 1) {
                return rightSmall(ptr);
            } else {
                return rightBig(ptr);
            }
        }
        return ptr;
    }
    Node <ValueType> *find_min(Node <ValueType> *ptr) {
        if (ptr == NULL) {
            return NULL;
        } else if (ptr -> left == NULL) {
            return ptr;
        } else {
            return find_min(ptr -> left);
        }
    }
    void print(Node <ValueType> *ptr) {
        int d = get_depth(ptr);
        for (int i = 0; i <= d; i++) {
            print_level(ptr, i);
            cout << endl;
        }
    }
    void print_level (Node <ValueType> *ptr, int level) {
        if (ptr == NULL)
            return;
        if (level == 0)
            cout << ptr -> key << " ";
        else if (level > 0) {
            print_level(ptr -> left, level - 1);
            print_level(ptr -> right, level - 1);
        }
    }

 public:
    Set() {
        root = NULL;
    }
    void insert(const ValueType & key) {
        root = insert(key, root);
    }
    void erase(const ValueType &key) {
        root = erase(key, root);
    }
    void print() {
        cout << "TREE" << endl;
        print(root);
        cout << endl;
    }
    void check() {
        is_legit();
    }
};



int main () {
    Set <int> myset;
    /*int n;
    char c;
    cin >> c;
    while (c != 'e') {
        cin >> n;
        if (c == 'a')
            set.insert(n);
        else if (c == 'r')
            set.erase(n);
        cin >> c;
    }*/
    myset.check();
    //myset.print();
    return 0;
}
