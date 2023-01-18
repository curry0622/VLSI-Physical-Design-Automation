# Homework 5 Grading Script
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
    HW5_grading/student/${your_student_id}/CS6135_HW5_${your_student_id}.tar.gz
    ```

    e.g.:

    ```
    HW5_grading/student/111062500/CS6135_HW5_111062500.tar.gz
    ```

### Notice:  
__Do not put your original directory here__ because it will remove all directories before unzipping the compressed file.

#
## Working Flow
* Step1:  
    Go into directory `HW5_grading` and change the permission of `HW5_grading.sh`.

    ```shell
    $ chmod 500 HW5_grading.sh
    ```

* Step2:  
    Run `HW5_grading.sh`.

    ```shell
    $ bash HW5_grading.sh
    ```

* Step3:  
    Check your output.
    * If the status is __success__, it means your program output is correct. e.g.:

        ```
        grading on 111062500:
          testcase |     result | status
            python |       pass | success
                 4 |       pass | success
                16 |       pass | success
                36 |       pass | success
                64 |       pass | success
               100 |       pass | success
        ```
    * If the status is not __success__, it means your program failed in this case. e.g.:

        ```
        grading on 111062500:
          testcase |     result | status
            python |       pass | success
                 4 |       fail | The number of transistors is wrong for testcase 4.
                16 |       pass | success
                36 |       pass | success
                64 |       pass | success
               100 |       pass | success
        ```
