# Homework 3 Grading Script
We will use this script to grade your program.  
__Please make sure your program can be executed by this script.__

#
## Preparing
* Step1:  
    Go into directory `student` and generate a new directory with your student id.

    ```shell
    $ cd student/
    $ mkdir ${your_student_id}
    $ cd ${your_student_id}/
    ```

    e.g.:

    ```shell
    $ cd student/
    $ mkdir 111062500
    $ cd 111062500/
    ```

* Step2:  
    Put your compressed file in the directory which you just generated.  
    The whole path is as follow: 

    ```
    HW3_grading/student/${your_student_id}/CS6135_HW3_${your_student_id}.tar.gz
    ```

    e.g.:

    ```
    HW3_grading/student/111062500/CS6135_HW3_111062500.tar.gz
    ```

### Notice:  
__Do not put your original directory here__ because it will remove all directories before unzipping the compressed file.

#
## Working Flow
* Step1:  
    Go into directory `HW3_grading` and change the permission of `HW3_grading.sh`.

    ```shell
    $ chmod 500 HW3_grading.sh
    ```

* Step2:  
    Run `HW3_grading.sh`.

    ```shell
    $ bash HW3_grading.sh
    ```

* Step3:  
    Check your output.
    * If the status is __success__, it means your program finished in time and your output is correct. e.g.:

        ```
        grading on 111062500:
          testcase |      ratio | wirelength |    runtime | status
              n100 |       0.15 |     205686 |       3.66 | success
              n200 |       0.15 |     372652 |      18.30 | success
              n300 |       0.15 |     500496 |      57.45 | success
              n100 |        0.1 |     218287 |       5.11 | success
              n200 |        0.1 |     378920 |      25.53 | success
              n300 |        0.1 |     519189 |     101.77 | success
        ```
    * If the status is not __success__, it means your program failed in this case. e.g.:

        ```
        grading on 111062500:
          testcase |      ratio | wirelength |    runtime | status
              n100 |       0.15 |       fail |        TLE | n100 failed.
              n200 |       0.15 |       fail |        TLE | n200 failed.
              n300 |       0.15 |       fail |        TLE | n300 failed.
              n100 |        0.1 |       fail |        TLE | n100 failed.
              n200 |        0.1 |       fail |        TLE | n200 failed.
              n300 |        0.1 |       fail |        TLE | n300 failed.
        ```
