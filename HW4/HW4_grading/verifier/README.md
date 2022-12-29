## How to Run
----
### __Input format__
The aux file and the coresponding result file.
```
./verify [aux file] [output file]
```
For example,
```
./verify ../testcase/adaptec1/adaptec1.aux ../output/adaptec1.result
```
Notice that you have to put all the files whose filenames are shown in the aux file to the __same__ folder with the aux file.
The hierarchy should be as shown below,
```
testcase
|-- adaptec1
    |-- adaptec1.aux
    |-- adaptec1.node
    |-- adaptec1.pl
    |-- adaptec1.scl
```
### __Output__
* If no file-reading error occurs, 
```
Read .aux file successfully
Trying to readfile:
  ../testcase/adaptec1/adaptec1.node
  ../testcase/adaptec1/adaptec1.pl
  ../testcase/adaptec1/adaptec1.scl
Read .scl file successfully
Parse .scl file successfully
Read .node file successfully
Parse .node file successfully
Read .pl file successfully
Parse .pl file successfully
Read .result file successfully
Parse .result file successfully
```
----

<br/>

### If your result is __illegal__,
<br/>

>  __Align-site Constraint Violation__
```
= Detected one (or more) cell doesn't align with any site.==
```
>  __Align-row Constraint Violation__
```
== Detected one (or more) cell doesn't align with any row.==
```
> __Fixed blockage Constraint Violation__
```
====== Detected one (or more) fixed blockage is moved.======
```
> __Cell Conservation Constraint Violation__
```
= Detected one (or more) cell doesn't exist in the pl file.=
```
```
=== Detected one (or more) cell is missing in the result.===
```
and then
```
=================== Verification failed.====================
```
----

<br/>

### If your result is __legal__,
<br/>

```
Your total displacement is 1234.567
Your maximum displacement is 12.34
```
> __Overlapping Constraint Violation__
```
=================== Detected overlapping.===================
```
> __No Overlapping__\
Get beautiful fireworks.
```
                                   .''.                     
         .''.             *''*    :_\/_:     .              
        :_\/_:   .    .:.*_\/_*   : /\ :  .'.:.'.           
    .''.: /\ : _\(/_  ':'* /\ *  : '..'.  -=:o:=-           
   :_\/_:'.:::. /)\*''*  .|.* '.'/.'_\(/_ '.':'.'           
   : /\ : :::::  '*_\/_* | |  -= o =-/)\     '              
    '..'  ':::'   * /\ * |'|  .'./.'.  '._____ \            
        *        __*..* |  |     :      |.   |' .---"|      
         _*   .-'   '-. |  |     .--'|  ||   | _|    |      
      .-'|  _.|  |    ||   '-__  |   |  |    ||      |      
      |' | |.    |    ||       | |   |  |    ||      |      
   ___|  '-'     '    ""       '-'   '-.'    '`      |____  
============================================================
====================== No overlapping.======================
```
and then
```
=================== Verification passed.====================
```
> __Maximum Displacement Constraint__
```
======= Maximum displacement threshold is satisfied.========
```