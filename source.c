#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node Node;
typedef struct Tree Tree;
struct Node {
    int value;
    Node* leftson, * rightson;
    bool isRed;
    Node* parent;
};
//Node functions
Node* nodeConstructor(Node* myNode, int value) {
    // myNode->leftson=malloc(sizeof(Node));
    myNode->leftson = NULL;
    myNode->rightson = NULL;
    myNode->value = value;
    myNode->parent = NULL;
    myNode->isRed = false; //false means black
    return myNode;
}

void setParent(Node* myNode, Node* parent) {
    myNode->parent = parent;
}

Node* getParent(Node* myNode) {
    return myNode->parent;
}

bool getColor(Node* myNode) {
    return myNode->isRed;
}

void setColor(Node* myNode, bool color) {
    myNode->isRed = color;
}

void setValue(Node* myNode, int value) {
    myNode->value = value;
}

int getValue(Node* myNode) {
    return myNode->value;
}

void setRightSon(Node* myNode, Node* rightSon) {
    myNode->rightson = rightSon;
}

Node* getRightSon(Node* myNode) {
    return myNode->rightson;
}

void setLeftSon(Node* myNode, Node* leftSon) {
    myNode->leftson = leftSon;
}

Node* getleftSon(Node* myNode) {
    return myNode->leftson;
}

//Tree class
struct Tree {
    Node* root;
    int blackHeight;
};

Tree* TreeConstructorVoid(Tree* myTree) {
    myTree->root = NULL;
    myTree->blackHeight = 1;
    return myTree;
}

Tree* TreeConstructorNotVoid(Node* root, Tree* myTree) {
    myTree->root = root;
    return myTree;
}

void rotateLeft(Tree* myTree, Node* node) {
    Node* temp1 = node->rightson;
    node->rightson = temp1->leftson;
    if (temp1->leftson != NULL) {
        temp1->leftson->parent = node;
    }
    temp1->parent = node->parent;
    if (node->parent == NULL) {
        myTree->root = temp1;
    }
    else if (node == node->parent->leftson) {
        node->parent->leftson == temp1;
    }
    else {
        node->parent->rightson = temp1;
    }
    temp1->leftson = node;
    node->parent = temp1;
}

void rotateRight(Tree* myTree, Node* node) {
    Node* temp1 = node->leftson;
    node->leftson = temp1->rightson;
    if (temp1->rightson != NULL) {
        temp1->rightson->parent = node;
    }
    temp1->parent = node->parent;
    if (node->parent == NULL) {
        myTree->root = temp1;
    }
    else if (node == node->parent->leftson) {
        node->parent->leftson == temp1;
    }
    else {
        node->parent->rightson = temp1;
    }
    temp1->rightson = node;
    node->parent = temp1;
}

void redBlackBalance(Tree* myTree, Node* node) {
    while (node != myTree->root && node->parent->isRed == true) {
        Node* uncle = NULL;
        bool left = false;
        if (node->parent->parent->rightson == node->parent) {
            uncle = node->parent->parent->leftson;
            left = false;
        }
        else {
            uncle = node->parent->parent->rightson;
            left = true;
        }
        if (uncle != NULL && uncle->isRed) {
            node->parent->isRed = false;
            uncle->isRed = false;
            node->parent->parent->isRed = true;
            node = node->parent->parent;
        }
        else {
            if (left) {
                if (node == node->parent->rightson) {
                    node = node->parent;
                    rotateLeft(myTree, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                rotateRight(myTree, node->parent->parent);
            }
            else {
                if (node == node->parent->leftson) {
                    node = node->parent;
                    rotateRight(myTree, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                rotateLeft(myTree, node->parent->parent);
            }
        }

    }
}

Node* insertRecursive(Tree* myTree, Node* node, Node* parent, Node* nodeToInsert) {
    if (node == NULL) {
        nodeToInsert->isRed = true;
        nodeToInsert->parent = parent;
        return nodeToInsert;
    }
    if (node->value > nodeToInsert->value) {
        setLeftSon(node, insertRecursive(myTree, getleftSon(node), node, nodeToInsert));
    }
    else if (getValue(node) <= getValue(nodeToInsert)) {
        setRightSon(node, insertRecursive(myTree, getRightSon(node), node, nodeToInsert));
    }
    setParent(nodeToInsert, getParent(nodeToInsert));
    return node;
}

void insertRecursiveV(Tree* myTree, int value) {
    if (myTree->root == NULL) {
        myTree->root = malloc(sizeof(Node));
        nodeConstructor(myTree->root, value);
        //setValue(myTree->root, value);
    }
    else {
        Node* newNode = malloc(sizeof(Node));
        nodeConstructor(newNode, value);
        Node* rootFake = NULL;
        myTree->root = insertRecursive(myTree, myTree->root, rootFake, newNode);
        redBlackBalance(myTree, newNode);
        myTree->root->isRed = false;
    }

}

void ordered(Node* node) {
    if (node != NULL) {
        ordered(node->leftson);
        printf("%d, color:  %d\n", node->value, node->isRed);
        ordered(node->rightson);
    }
}

void preOrdered(Node* node) {
    if (node != NULL) {
        printf("%d, color:  %d\n", node->value, node->isRed);
        preOrdered(node->leftson);
        preOrdered(node->rightson);
    }
}

void postOrdered(Node* node) {
    if (node != NULL) {
        postOrdered(node->leftson);
        postOrdered(node->rightson);
        printf("%d, color:  %d\n", node->value, node->isRed);
    }
}

void inOrder(Tree* myTree) {
    ordered(myTree->root);
}

Node* search(Node* node, int value) {
    while (node != NULL && node->value != value) {
        if (node->value > value) {
            node = node->leftson;
        }
        else {
            node = node->rightson;
        }
    }
    if (node == NULL) {
        return NULL;
    }
    return node;
}

void moveSubTree(Tree* myTree, Node* nodeOr, Node* nodeT) {
    if (nodeOr->parent == NULL) {
        myTree->root = nodeT;
    }
    else if (nodeOr == nodeOr->parent->leftson) {
        nodeOr->parent->leftson = nodeT;
    }
    else {
        nodeOr->parent->rightson = nodeT;
    }
    if (nodeT != NULL) {
        nodeT->parent = nodeOr->parent;
    }
}

Node* findMinimum(Node* node) {
    if (node->leftson == NULL) {
        return node;
    }
    node = findMinimum(node->leftson);
    return node;
}

Node* findMaximum(Node* node) {
    if (node->rightson == NULL) {
        return node;
    }
    node = findMaximum(node->rightson);
    return node;
}

Node* callSearch(Tree* myTree, int value) {
    return search(myTree->root, value);
}

Tree* redBlackBalanceDeletion(Tree* myTree, Node* node) {
    while (node != myTree->root && node->isRed == false) {
        if (node == node->parent->leftson) {
            Node* uncle = node->parent->rightson;
            if (uncle->isRed) {
                uncle->isRed = false;
                node->parent->isRed = true;
                rotateLeft(myTree, node->parent);
                uncle = node->parent->rightson;
            }
            if ((uncle->leftson == NULL && uncle->rightson == NULL) || (uncle->leftson->isRed == false && uncle->rightson->isRed == false)) {
                uncle->isRed = true;
                node = node->parent;
            }
            else {
                if (uncle->rightson == NULL || uncle->rightson->isRed == false) {
                    uncle->leftson->isRed = false;
                    uncle->isRed = true;
                    rotateRight(myTree, uncle);
                    uncle = node->parent->rightson;
                }
                uncle->isRed = node->parent->isRed;
                node->parent->isRed = false;
                uncle->rightson->isRed = false;
                rotateLeft(myTree, node->parent);
                node = myTree->root;
            }
        }
        else {
            Node* uncle = node->parent->leftson;
            if (uncle->isRed) {
                uncle->isRed = false;
                node->parent->isRed = true;
                rotateRight(myTree, node->parent);
                uncle = node->parent->leftson;
            }
            if ((uncle->leftson == NULL && uncle->rightson == NULL) || (uncle->leftson->isRed == false && uncle->rightson->isRed == false)) {
                uncle->isRed = true;
                node = node->parent;
            }
            else {
                if (uncle->leftson == NULL || uncle->leftson->isRed == false) {
                    uncle->rightson->isRed = false;
                    uncle->isRed = true;
                    rotateLeft(myTree, uncle);
                    uncle = node->parent->leftson;
                }
                uncle->isRed = node->parent->isRed;
                node->parent->isRed = false;
                uncle->leftson->isRed = false;
                rotateRight(myTree, node->parent);
                node = myTree->root;
            }
        }
    }
    node->isRed = false;
    return myTree;
}

Tree* deleteNode(Tree* myTree, Node* node) {
    if (myTree->root == NULL) {
        return;
    }
    Node* temp = node, * toFix;
    bool tmp = temp->isRed;
    if (node->leftson == NULL) {
        toFix = node->rightson;
        moveSubTree(myTree, node, node->rightson);
    }
    else if (node->rightson == NULL) {
        toFix = node->leftson;
        moveSubTree(myTree, node, node->leftson);
    }
    else {
        temp = findMinimum(node->rightson);
        tmp = temp->isRed;
        toFix = temp->rightson;
        if (toFix != NULL && temp->parent == node) {
            toFix->parent = temp;
        }
        else {
            moveSubTree(myTree, temp, temp->rightson);
            temp->rightson = node->rightson;
            if (temp->rightson!=NULL){temp->rightson->parent = temp; }
        }
        moveSubTree(myTree, node, temp);
        temp->leftson = node->leftson;
        if (temp->leftson!=NULL) { temp->leftson->parent = temp; }
        temp->isRed = node->isRed;
    }
    if (tmp == false && toFix != NULL) {
        myTree = redBlackBalanceDeletion(myTree, toFix);
    }
    return myTree;
}

void callDeletion(Tree* myTree, int value) {
    Node* node = search(myTree->root, value);
    //printf("%d", node->value);
    myTree = deleteNode(myTree, node);
}

int main() {
    /*Node myNode;
    myNode = *nodeConstructor(&myNode, 20);
    Node newNode;
    newNode = *nodeConstructor(&newNode, 35);*/
    //setParent(&myNode, &newNode);
    int array[] = { 14,6,68,3,12,51,74,2,9,44, 63,70,78,54,74,83,74,82,97 };
    Tree myTree;
    TreeConstructorVoid(&myTree);
    //insertRecursiveV(&myTree, 1020);
    //printf("%d, %d, %d", myTree.root->value, myTree.root->leftson->value, myTree.root->rightson->value);
    for (int i = 0; i < 19; i++) {
        insertRecursiveV(&myTree, array[i]);
    }

    callDeletion(&myTree, 14); 
    printf("root: %d\n", myTree.root->value);
    inOrder(&myTree);
    printf("\n");
    preOrdered(myTree.root); printf("\n");
    postOrdered(myTree.root);


    return 0;
}