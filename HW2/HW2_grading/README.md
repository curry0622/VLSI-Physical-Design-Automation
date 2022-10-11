# Homework 2: Grading Script

We will use this script to judge your program.
**Please make sure your program can be executed by this script.**

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
    HW2_grading/student/${your_student_id}/CS6135_HW2_${your_student_id}.tar.gz
    ```

    e.g.:

    ```shell
    HW2_grading/student/111062500/CS6135_HW2_111062500.tar.gz
    ```

### Notice

**Do not put your original directory here** because it will remove all directories before unzipping the compressed file.

## Working Flow

* Step1:
    Go into directory `HW2_grading` and change the permission of `HW2_grading.sh`.

    ```shell
    chmod 500 HW2_grading.sh
    ```

* Step2:
    Run `HW2_grading.sh`.

    ```shell
    bash HW2_grading.sh
    ```

* Step3:
    Check your output.
  * If the status is **success**, it means your program finished in time and your output is correct. e.g.:

    ```shell
    grading on 111062500:
        testcase |    cutsize |    runtime | status
            p2-1 |        277 |       0.01 | success
            p2-2 |       2776 |       0.04 | success
            p2-3 |      26854 |       1.02 | success
            p2-4 |      98881 |       4.81 | success
            p2-5 |     198664 |       8.55 | success
    ```

  * If the status is not **success**, it means your program failed in this case. e.g.:

    ```shell
    grading on 111062500:
        testcase |    cutsize |    runtime | status
            p2-1 |       fail |        TLE | p2-1 failed.
            p2-2 |       fail |        TLE | p2-2 failed.
            p2-3 |       fail |        TLE | p2-3 failed.
            p2-4 |       fail |        TLE | p2-4 failed.
            p2-5 |       fail |        TLE | p2-5 failed.
    ```
