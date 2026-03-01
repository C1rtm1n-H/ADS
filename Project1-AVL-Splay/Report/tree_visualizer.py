import graphviz
import os
import time

class Node:
    """A node in a binary search tree."""
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.height = 1  # For AVL Tree

class AVLTree:
    """An AVL Tree implementation with visualization."""
    def __init__(self):
        self.root = None
        self.steps = 0
        self.output_dir = "visualizations/avl"
        os.makedirs(self.output_dir, exist_ok=True)

    def _get_height(self, node):
        return node.height if node else 0

    def _get_balance(self, node):
        return self._get_height(node.left) - self._get_height(node.right) if node else 0

    def _right_rotate(self, z):
        y = z.left
        T3 = y.right
        y.right = z
        z.left = T3
        z.height = 1 + max(self._get_height(z.left), self._get_height(z.right))
        y.height = 1 + max(self._get_height(y.left), self._get_height(y.right))
        return y

    def _left_rotate(self, y):
        x = y.right
        T2 = x.left
        x.left = y
        y.right = T2
        y.height = 1 + max(self._get_height(y.left), self._get_height(y.right))
        x.height = 1 + max(self._get_height(x.left), self._get_height(x.right))
        return x

    def insert(self, key):
        """Public method to insert a key and visualize the process."""
        self.visualize(f"Before inserting {key}")
        self.root = self._insert(self.root, key)
        self.visualize(f"After inserting {key} and rebalancing")

    def _insert(self, root, key):
        if not root:
            return Node(key)
        elif key < root.key:
            root.left = self._insert(root.left, key)
        else:
            root.right = self._insert(root.right, key)

        root.height = 1 + max(self._get_height(root.left), self._get_height(root.right))
        balance = self._get_balance(root)

        # Left Left Case
        if balance > 1 and key < root.left.key:
            return self._right_rotate(root)
        # Right Right Case
        if balance < -1 and key > root.right.key:
            return self._left_rotate(root)
        # Left Right Case
        if balance > 1 and key > root.left.key:
            root.left = self._left_rotate(root.left)
            return self._right_rotate(root)
        # Right Left Case
        if balance < -1 and key < root.right.key:
            root.right = self._right_rotate(root.right)
            return self._left_rotate(root)

        return root

    def visualize(self, title=""):
        """Generates a visualization of the current tree state."""
        dot = graphviz.Digraph(comment='Tree')
        dot.attr(label=title, labelloc='t', fontsize='20')
        
        if self.root:
            q = [self.root]
            while q:
                node = q.pop(0)
                dot.node(str(node.key), f"{node.key}\\nH:{node.height} | B:{self._get_balance(node)}")
                if node.left:
                    dot.edge(str(node.key), str(node.left.key))
                    q.append(node.left)
                if node.right:
                    dot.edge(str(node.key), str(node.right.key))
                    q.append(node.right)
        
        filename = os.path.join(self.output_dir, f"step_{self.steps:03d}")
        dot.render(filename, format='png', view=False, cleanup=True)
        self.steps += 1

class SplayTree:
    """A Splay Tree implementation with visualization."""
    def __init__(self):
        self.root = None
        self.steps = 0
        self.output_dir = "visualizations/splay"
        os.makedirs(self.output_dir, exist_ok=True)

    def _right_rotate(self, x):
        y = x.left
        x.left = y.right
        y.right = x
        return y

    def _left_rotate(self, x):
        y = x.right
        x.right = y.left
        y.left = x
        return y

    def _splay(self, root, key):
        if not root or root.key == key:
            return root

        # Bring the key to root of left/right subtree
        if key < root.key:
            if not root.left:
                return root
            if key < root.left.key: # Zig-Zig (Left Left)
                root.left.left = self._splay(root.left.left, key)
                root = self._right_rotate(root)
            elif key > root.left.key: # Zig-Zag (Left Right)
                root.left.right = self._splay(root.left.right, key)
                if root.left.right:
                    root.left = self._left_rotate(root.left)
            return self._right_rotate(root) if root.left else root
        else: # key > root.key
            if not root.right:
                return root
            if key < root.right.key: # Zag-Zig (Right Left)
                root.right.left = self._splay(root.right.left, key)
                if root.right.left:
                    root.right = self._right_rotate(root.right)
            elif key > root.right.key: # Zag-Zag (Right Right)
                root.right.right = self._splay(root.right.right, key)
                root = self._left_rotate(root)
            return self._left_rotate(root) if root.right else root

    def _bst_insert(self, root, key):
        """A standard recursive BST insert, returning the new root of the subtree."""
        if not root:
            return Node(key)
        if key < root.key:
            root.left = self._bst_insert(root.left, key)
        elif key > root.key:
            root.right = self._bst_insert(root.right, key)
        return root

    def insert(self, key):
        """Public method to insert a key and visualize the splay."""
        self.visualize(f"Before inserting {key}")
        
        # 1. Standard BST insert
        self.root = self._bst_insert(self.root, key)
        self.visualize(f"After BST insert of {key} (before splay)")

        # 2. Splay the newly inserted key to the root
        self.root = self._splay(self.root, key)
        self.visualize(f"After splaying {key} to the root")

    def visualize(self, title=""):
        """Generates a visualization of the current tree state."""
        dot = graphviz.Digraph(comment='Splay Tree')
        dot.attr(label=title, labelloc='t', fontsize='20')
        
        if self.root:
            q = [self.root]
            visited = {self.root}
            while q:
                node = q.pop(0)
                dot.node(str(node.key), str(node.key))
                if node.left:
                    dot.edge(str(node.key), str(node.left.key))
                    if node.left not in visited:
                        q.append(node.left)
                        visited.add(node.left)
                if node.right:
                    dot.edge(str(node.key), str(node.right.key))
                    if node.right not in visited:
                        q.append(node.right)
                        visited.add(node.right)

        filename = os.path.join(self.output_dir, f"step_{self.steps:03d}")
        dot.render(filename, format='png', view=False, cleanup=True)
        self.steps += 1

def main():
    """Main function to demonstrate tree visualizations."""
    print("Generating tree visualizations...")
    
    # --- Data to Insert ---
    # Use a simple, predictable sequence to see rotations clearly
    data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    # --- AVL Tree Visualization ---
    avl = AVLTree()
    print("\n--- Visualizing AVL Tree ---")
    for key in data:
        print(f"Inserting {key} into AVL tree...")
        avl.insert(key)
    print(f"AVL tree visualization complete. Images are in '{avl.output_dir}'")

    # --- Splay Tree Visualization ---
    splay = SplayTree()
    print("\n--- Visualizing Splay Tree ---")
    for key in data:
        print(f"Inserting {key} into Splay tree...")
        splay.insert(key)
    print(f"Splay tree visualization complete. Images are in '{splay.output_dir}'")

if __name__ == '__main__':
    main()