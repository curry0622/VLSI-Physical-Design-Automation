# Homework 3 Printer
You can use this program to visualize your floorplanning result.

#
## Preparing
* __Step1:__  
    This floorplan visualization tool is written in __Processing__.  
    Please download the [__Processing__](https://processing.org/download) and unzip it.

* __Step2:__  
    Unzip `HW3_printer.zip` and put this directory in the folder `processing`.  
    The file hierarchy is shown as below:

    ```
    processing/
    |   core/
    |   HW3_printer/
    |   |   figure/
    |   |   output/
    |   |   testcases/
    |   |   |   n100.hardblocks
    |   |   |   n100.nets
    |   |   |   n100.pl
    |   |   |   ...
    |   |   HW3_printer.pde
    |   |   README.md
    |   java/
    |   lib/
    |   modes/
    |   tools/
    |   processing.exe
    |   processing-java.exe
    |   revisions.md
    ```

* __Step3:__  
    Put your floorplanning result in the folder `output`.

    ```
    processing/
    |   HW3_printer/
    |   |   output/
    |   |   |   n100.floorplan
    |   |   |   n200.floorplan
    |   |   |   n300.floorplan
    |   |   |   ...
    ```

#
## Working Flow
* __Step1:__  
    Open `HW3_printer.pde` with `processing.exe`.  
    Modify the *caseName* and *deadSpaceRatio* in __setup()__.

    ```java
    void setup() {
        String caseName = "n300";   // change testcase
        float deadSpaceRatio = 0.1; // change dead space ratio
        
        int imageSize = 700;        // change image size
        size(700, 700);             // should be same as imageSize
        background(255);
        
        Printer printer = new Printer(imageSize, deadSpaceRatio);
        printer.readHardblocks("testcases/" + caseName + ".hardblocks");
        printer.readFloorplan("output/" + caseName + ".floorplan");
        printer.drawFloorplan("figure/" + caseName + ".png");
    }
    ```

* __Notice:__   
    The default image size is 700 x 700.  
    If you want to change the image size, please revise the value of *imageSize* and __size(x, x)__.

* __Step2:__  
    Press __run__ button to execute the drawing program.  
    A drawing window will pop up on your screen.  
    The floorplanning image will be saved in the folder `figure`.

    ```
    processing/
    |   HW3_printer/
    |   |   figure/
    |   |   |   n100.png
    |   |   |   n200.png
    |   |   |   n300.png
    |   |   |   ...
    ```

#
## Reference
* https://processing.org/reference/
