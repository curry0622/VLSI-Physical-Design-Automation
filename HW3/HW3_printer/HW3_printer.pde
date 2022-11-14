/* ||=========================================|| */
/* || PDA HW3 floorplan visualization program || */
/* ||=========================================|| */

void setup() {
    String caseName = "n300";   // change testcase
    float deadSpaceRatio = 0.15; // change dead space ratio
    
    int imageSize = 700;        // change image size
    size(700, 700);             // should be same as imageSize
    background(255);
    
    Printer printer = new Printer(imageSize, deadSpaceRatio);
    printer.readHardblocks("testcases/" + caseName + ".hardblocks");
    printer.readFloorplan("output/" + caseName + ".floorplan");
    printer.drawFloorplan("figure/" + caseName + ".png");
}

class Hardblock {
    String name;
    int w, h;
    int x, y;
    Boolean rotation;
    
    Hardblock(String name, int w, int h) {
        this.name = name;
        this.w = w;
        this.h = h;
        this.x = 0;
        this.y = 0;
        this.rotation = false;
    }
    
    void setPosition(int x, int y, Boolean rotation) {
        this.x = x;
        this.y = y;
        this.rotation = rotation;
    }
}

class Printer {
    int imageSize;
    float deadSpaceRatio;
    int totalBlockArea;
    ArrayList<Hardblock> hardblocks = new ArrayList<Hardblock>();
    
    IntDict strToIdx = new IntDict();
    
    Printer(int imageSize, float deadSpaceRatio) {
        this.imageSize = imageSize;
        this.deadSpaceRatio = deadSpaceRatio;
        totalBlockArea = 0;
    }
    
    void readHardblocks(String filename) {
        BufferedReader reader = createReader(filename);
        int idx = 0;
        while(true) {
            String line;
            try {
                line = reader.readLine();
            }
            catch(IOException e) {
                e.printStackTrace();
                line = null;
            }
            
            if (line == null)
                break;
            
            String[] data = splitTokens(line, " (,)");
            if (data.length != 11)
                continue;
            
            int[] x = new int[4], y = new int[4];
            for (int i = 3; i < 11; i += 2) {
                x[i / 2 - 1] = int(data[i]);
                y[i / 2 - 1] = int(data[i + 1]);
            }
            
            int w = max(x) - min(x), h = max(y) - min(y);
            totalBlockArea += w * h;
            hardblocks.add(new Hardblock(data[0], w, h));
            strToIdx.set(data[0], idx++);
        }
    }
    
    void readFloorplan(String filename) {
        BufferedReader reader = createReader(filename);
        while(true) {
            String line;
            try {
                line = reader.readLine();
            }
            catch(IOException e) {
                e.printStackTrace();
                line = null;
            }
            
            if (line == null)
                break;
            
            String[] data = splitTokens(line, " ");
            if (data.length != 4)
                continue;
            
            hardblocks.get(strToIdx.get(data[0])).setPosition(int(data[1]), int(data[2]), boolean(int(data[3])));
        }
    }
    
    int getOutlineSize() {
        return floor(sqrt(totalBlockArea * (1 + deadSpaceRatio)));
    }
    
    int calX(int x) {
        return x + 1;
    }
    
    int calY(int y, int h) {
        return this.imageSize - y - h - 2;
    }
    
    void drawOutline() {
        int size = this.getOutlineSize();
        stroke(255, 0, 0);
        noFill();
        rect(this.calX(0), this.calY(0, size), size, size);
    }
    
    void drawHardblocks() {
        for (Hardblock it : this.hardblocks) {
            int w, h, x, y;
            if (it.rotation) {
                w = it.h;
                h = it.w;
                x = this.calX(it.x);
                y = this.calY(it.y, it.w);
            }
            else {
                w = it.w;
                h = it.h;
                x = this.calX(it.x);
                y = this.calY(it.y, it.h);
            }
            
            stroke(0);
            fill(200);
            rect(x, y, w, h);
            
            fill(0);
            textSize(8);
            text(it.name, x + 2, y + 9);
        }
    }
    
    void drawFloorplan(String filename) {
        this.drawHardblocks();
        this.drawOutline();
        save(filename);
    }
}
