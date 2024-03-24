#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int getHeight(Node *root) {
    if (root == NULL) return -1;
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int isKBalanced(Node *root, int k) {
    if (root == NULL) return 1;
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    if (abs(leftHeight - rightHeight) <= k && isKBalanced(root->left, k) && isKBalanced(root->right, k))
        return 1;
    return 0;
}

Node* rotateRight(Node* root) {
    Node* newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    return newRoot;
}

Node* rotateLeft(Node* root) {
    Node* newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    return newRoot;
}

Node* balance(Node* root) {
    int balanceFactor = getHeight(root->left) - getHeight(root->right);
    if (balanceFactor > 1) {
        if (getHeight(root->left->left) < getHeight(root->left->right))
            root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balanceFactor < -1) {
        if (getHeight(root->right->left) > getHeight(root->right->right))
            root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

Node* insert(Node* root, int data) {
    if (root == NULL) return newNode(data);
    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);
    return balance(root);
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int data) {
    if (root == NULL) return root;

    if (data < root->data)
        root->left = deleteNode(root->left, data);
    else if (data > root->data)
        root->right = deleteNode(root->right, data);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }
    if (root == NULL) return root;
    return balance(root);
}

Node* search(Node* root, int data) {
    if (root == NULL || root->data == data)
        return root;
    if (root->data < data)
        return search(root->right, data);
    return search(root->left, data);
}

Node* update(Node* root, int oldData, int newData) {
    root = deleteNode(root, oldData);
    root = insert(root, newData);
    return root;
}

void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main() {
    Node* root = NULL;
    int k = 1;

    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 3);
    root = insert(root, 7);
    root = insert(root, 12);
    root = insert(root, 17);

    printf("Inorder traversal of the tree: ");
    inorder(root);
    printf("\n");

    if (isKBalanced(root, k))
        printf("The tree is %d-balanced.\n", k);
    else
        printf("The tree is not %d-balanced.\n", k);

    root = deleteNode(root, 7);

    printf("Inorder traversal after deletion: ");
    inorder(root);
    printf("\n");

    root = update(root, 10, 20);
    printf("Inorder traversal after updating 10 to 20: ");
    inorder(root);
    printf("\n");

    return 0;
}
