
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import csv
import sys
import random
#read the csv file
def read_csv(filename):
    rectangles = []
    metadata = {}
    
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        first_row = next(reader)
        metadata['algorithm'] = first_row[0]
        metadata['test_mode'] = first_row[1]
        metadata['container_w'] = int(first_row[2])
        metadata['container_h'] = int(first_row[3])
        metadata['n'] = int(first_row[4])
        metadata['fill_ratio'] = float(first_row[5])
        
        for row in reader:
            if len(row) >= 5:
                rect = {
                    'id': int(row[0]),
                    'x': int(row[1]),
                    'y': int(row[2]),
                    'width': int(row[3]),
                    'height': int(row[4])
                }
                rectangles.append(rect)
    
    return metadata, rectangles

def generate_colors(n):
    colors = []
    random.seed(42)  
    for i in range(n):
        hue = i / n
        saturation = 0.6 + random.random() * 0.3
        value = 0.7 + random.random() * 0.2
        
        # HSV to RGB
        c = value * saturation
        x = c * (1 - abs((hue * 6) % 2 - 1))
        m = value - c
        
        if hue < 1/6:
            r, g, b = c, x, 0
        elif hue < 2/6:
            r, g, b = x, c, 0
        elif hue < 3/6:
            r, g, b = 0, c, x
        elif hue < 4/6:
            r, g, b = 0, x, c
        elif hue < 5/6:
            r, g, b = x, 0, c
        else:
            r, g, b = c, 0, x
        
        colors.append((r + m, g + m, b + m))
    random.shuffle(colors)
    return colors

def draw_packing(metadata, rectangles):
    container_w = metadata['container_w']
    container_h = metadata['container_h']
    algorithm = metadata['algorithm']
    test_mode = metadata['test_mode']
    fill_ratio = metadata['fill_ratio']
    n = metadata['n']
    fig, ax = plt.subplots(1, 1, figsize=(12, 10))
    ax.set_xlim(0, container_w)
    ax.set_ylim(0, container_h)
    ax.set_aspect('equal')
    ax.set_xlabel('X', fontsize=12)
    ax.set_ylabel('Y', fontsize=12)
    title = f"Algorithm: {algorithm} | Test Mode: {test_mode}"
    ax.set_title(title, fontsize=14)
    
    container = patches.Rectangle((0, 0), container_w, container_h,
                                   linewidth=2, edgecolor='black',
                                   facecolor='white')
    ax.add_patch(container)
    
    colors = generate_colors(len(rectangles))
    total_area = sum(r['width'] * r['height'] for r in rectangles)
    
    for i, rect in enumerate(rectangles):
        if rect['x'] < 0 or rect['y'] < 0:
            continue
            
        r = patches.Rectangle(
            (rect['x'], rect['y']),
            rect['width'],
            rect['height'],
            linewidth=0.5,
            edgecolor='black',
            facecolor=colors[i % len(colors)],
            alpha=0.7
        )
        ax.add_patch(r)
        cx = rect['x'] + rect['width'] / 2
        cy = rect['y'] + rect['height'] / 2
        fontsize = min(rect['width'], rect['height']) * 0.4
        fontsize = max(4, min(fontsize, 10))  # 限制在4-10之间
        ax.text(cx, cy, str(rect['id']), 
               ha='center', va='center', fontsize=fontsize, fontweight='bold',
               color='black')
    
    info_text = f"Rectangles: {n}\n"
    info_text += f"Container: {container_w} x {container_h}\n"
    info_text += f"Height: {container_h}\n"
    info_text += f"Fill Ratio: {fill_ratio * 100:.2f}%"
    fig.text(0.75, 0.5, info_text, fontsize=11, verticalalignment='center',
             bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
    
    plt.subplots_adjust(right=0.7)
    ax.grid(True, linestyle='--', alpha=0.3)
    
    return fig

def main():
    if len(sys.argv) <= 1:
        files = ["results/visual_BL_mode1.csv"]
    else:
        files = sys.argv[1:] 
    
    for filename in files:
        try:

            metadata, rectangles = read_csv(filename)
            fig = draw_packing(metadata, rectangles)
            output_file = filename.replace('.csv', '.png')
            fig.savefig(output_file, dpi=300, bbox_inches='tight')
            plt.close(fig)
            
        except FileNotFoundError:
            print(f"Error: File '{filename}' not found!")
        except Exception as e:
            print(f"Error processing {filename}: {e}")

if __name__ == "__main__":
    main()
