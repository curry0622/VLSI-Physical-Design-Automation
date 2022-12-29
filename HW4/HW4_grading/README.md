# Homework 4 Grading Script

We will use this script to grade your program.  
__Please make sure your program can be executed by this script.__

## Preparing

* Step1:  
    Go into directory `student` and generate a new directory with your student id.

    ```shell
    cd student/
    mkdir ${your_student_id}
    cd ${your_student_id}/
    ```

    e.g.:

    ```shell
    cd student/
    mkdir 111062500
    cd 111062500/
    ```

* Step2:  
    Put your compressed file in the directory which you just generated.  
    The whole path is as follow:

    ```shell
    HW4_grading/student/${your_student_id}/CS6135_HW4_${your_student_id}.tar.gz
    ```

    e.g.:

    ```shell
    HW4_grading/student/111062500/CS6135_HW4_111062500.tar.gz
    ```

### Notice  

__Do not put your original directory here__ because it will remove all directories before unzipping the compressed file.

## Working Flow

* Step1:  
    Go into directory `HW4_grading` and change the permission of `HW4_grading.sh`.

    ```shell
    chmod 500 HW4_grading.sh
    ```

* Step2:  
    Run `HW4_grading.sh`.

    ```shell
    bash HW4_grading.sh
    ```

* Step3:  
    Check your output.
  * If the status is __success__, it means your program finished in time and your output is correct. e.g.:

    ```shell
    grading on 111062500:
        testcase |       max disp. |     total disp. |     runtime | status
        adaptec1 |          109.49 |      3158146.50 |        0.54 | success
        adaptec3 |          117.66 |      5169328.00 |        1.55 | success
           ibm01 |         4936.15 |      6053148.00 |        0.03 | success
           ibm07 |         5126.87 |     29453187.50 |        0.10 | success
           ibm09 |         5189.47 |     48316579.00 |        0.12 | success
    ```

  * If the status is not __success__, it means your program failed in this case. e.g.:

    ```shell
    grading on 111062500:
        testcase |     maximum |       total |     runtime | status
        adaptec1 |        fail |        fail |         TLE | adaptec1 failed.
        adaptec3 |        fail |        fail |         TLE | adaptec3 failed.
           ibm01 |        fail |        fail |         TLE | ibm01 failed.
           ibm07 |        fail |        fail |         TLE | ibm07 failed.
           ibm09 |        fail |        fail |         TLE | ibm09 failed.
    ```
