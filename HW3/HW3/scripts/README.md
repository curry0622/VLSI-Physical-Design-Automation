# Scripts

## Draw Floorplan

Usage: Draw the image of the floorplan.

Parameters:

* **-hb**: File path of the hardblocks file. (Required)
* **-fp**: File path of the hardblocks file. (Required)
* **-dsr**: Dead space ratio. (Required)
* **-o**: File path of the output image. (Default: "output.png")

```shell
python3 draw.py -hb ../testcases/n100.hardblocks -fp ../output/n100.floorplan -dsr 0.1 -o ../images/n100_0.1.png
```
