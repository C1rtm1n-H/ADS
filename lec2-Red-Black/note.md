# Red-Black Tree
## Properties
 1. Every node is either red or black
 2. The root is black
 3. Every leaf (NIL) is black
 4. If a node is red, then both its children must be black (no two red nodes can be adjacent)
 5. Every path from a node to its descendant NIL nodes must have the same number of black nodes
- These properties ensure that the tree remains approximately balanced, with a height of O(log n), thus guaranteeing O(log n) time complexity for insertion, deletion, and search operations.
> Lemma: A red-black tree with n internal nodes has a height of at most 2*log2(n+1).

## Concepts
- **Black Height**: The number of black nodes from a node to its descendant leaves
- **NIL Node**: A sentinel node representing the absence of a child, always black

## Insertion
> 三红只变色，二红才旋转
1. Perform a standard BST insertion and color the new node red
2. Fix any violations of the red-black properties:
    - If the tree is empty, color the new node black
    - If the parent is black, no further action is needed
    - If the parent is red, there are two cases:
        - case 1: The uncle is red
            - Recolor the parent and uncle to black, and the grandparent to red
            - Recursively fix violations at the grandparent(grandparent as new node)
        - case 2: The uncle is black (or NIL) and zig-zag
            - rotate to case 3
        - case 3: The uncle is black and zig-zig
            - color the parent black and grandparent red, rotate
3. Implementation:
    ```c
    
    ```

## Deletion
Assume the node to be deleted is x
1. deg(x) = 0:
    a. x is red: simply remove x(set to NIL)
    b. x is black: set x to NIL, then fix double black at parent(x's parent as new node)
2. deg(x) = 1:
    a. x is red: replace x with its child
    b. x is black and x's child is red: replace x with its child, color the child black
    c. x is black and x's child is black(NIL): replace x with its child(NIL), then fix double black at parent(x's parent as new node)
3. deg(x) = 2:
    - find x's in-order successor y(maximum value in left subtree), copy y's value to x, then delete y(deg(y) = 0 or 1)
 ### Fix double black:
    - If the node is the root, simply remove the double black
