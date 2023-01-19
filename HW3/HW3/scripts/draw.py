import math
import argparse
from PIL import Image, ImageDraw

WIDTH = 640
HEIGHT = 640
OFF_X = 30
OFF_Y = -30
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-hb", help="File path of the hardblocks file", required=True)
    parser.add_argument("-fp", help="File path of the floorplan file", required=True)
    parser.add_argument("-dsr", help="Dead space ratio", required=True)
    parser.add_argument("-o", help="File path of the output image", default="output.png")
    return parser.parse_args()

def draw_fp(hb_file, fp_file, dsr, output_file):
    img = Image.new("RGB", (WIDTH, HEIGHT), WHITE)
    draw = ImageDraw.Draw(img)
    
    area = 0
    num_blk = 0
    blks = {}
    
    # Read hardblocks
    with open(hb_file, "r") as f:
        line = f.readline()
        num_blk = int(line[27:])
        f.readline()
        f.readline()
        
        for i in range(num_blk):
            line = f.readline()
            line = line.split()
            name = line[0]
            x1 = int(line[3][1:-1])
            y1 = int(line[4][0:-1])
            x2 = int(line[7][1:-1])
            y2 = int(line[8][0:-1])
            w = x2 - x1
            h = y2 - y1
            area += w * h
            blks[name] = (w, h, 0, 0, 0) # (w, h, x, y, rot)
        
        f.close()
    
    # Read floorplan
    with open(fp_file, "r") as f:
        f.readline()
        f.readline()
        
        for i in range(num_blk):
            line = f.readline()
            line = line.split()
            name = line[0]
            x = int(line[1])
            y = int(line[2])
            rot = int(line[3])
            blks[name] = (blks[name][0], blks[name][1], x, y, rot) # (w, h, x, y, rot)
            
        f.close()
            
    # Draw outline
    outline_w = math.sqrt(area * (1 + dsr))
    outline_h = outline_w
    draw.rectangle(
        xy=[(OFF_X, HEIGHT+OFF_Y), (outline_w+OFF_X, HEIGHT-outline_h+OFF_Y)],
        fill=None,
        outline=RED
    )
    
    # Draw blocks
    for blk in blks:
        w = blks[blk][0]
        h = blks[blk][1]
        x = blks[blk][2]
        y = blks[blk][3]
        rot = blks[blk][4]
        draw.text(
            xy=[x+OFF_X, HEIGHT-y+OFF_Y-10],
            text=blk,
            fill=BLACK
        )
        if rot == 0:
            draw.rectangle(
                xy=[(x+OFF_X, HEIGHT-y+OFF_Y), (x+w+OFF_X, HEIGHT-(y+h)+OFF_Y)],
                fill=None,
                outline=BLACK
            )
        else:
            draw.rectangle(
                xy=[(x+OFF_X, HEIGHT-y+OFF_Y), (x+h+OFF_X, HEIGHT-(y+w)+OFF_Y)],
                fill=None,
                outline=BLACK
            )
    
    # Save image
    img.save(fp=output_file, format="PNG")

if __name__ == "__main__":
    args = parse_args()
    draw_fp(args.hb, args.fp, float(args.dsr), args.o)