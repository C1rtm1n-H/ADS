# Tree Visualizer

This script provides a step-by-step visualization for AVL and Splay trees using Python and Graphviz.

## Prerequisites

1.  **Python 3**: Ensure you have Python 3 installed.
2.  **Graphviz System Package**: This is the underlying software that draws the graphs. You must install it on your system.

    *   **On Ubuntu/Debian:**
        ```bash
        sudo apt-get update
        sudo apt-get install graphviz
        ```
    *   **On macOS (using Homebrew):**
        ```bash
        brew install graphviz
        ```
    *   **On Windows:**
        Download and run the installer from the [official Graphviz website](https://graphviz.org/download/). Make sure to add the Graphviz `bin` directory to your system's `PATH` environment variable.

3.  **Python `graphviz` Library**: This is the Python interface for Graphviz.

    ```bash
    pip install graphviz
    ```

## How to Run

1.  Make sure you have completed the prerequisite installations.
2.  Run the script from your terminal:

    ```bash
    python tree_visualizer.py
    ```

3.  The script will create a directory named `visualizations` in your current folder. Inside, you will find two subdirectories:
    *   `visualizations/avl/`: Contains the step-by-step PNG images for the AVL tree creation.
    *   `visualizations/splay/`: Contains the step-by-step PNG images for the Splay tree creation.

4.  Open the image files (e.g., `step_000.png`, `step_001.png`, etc.) in order to see how the tree structure changes with each insertion.