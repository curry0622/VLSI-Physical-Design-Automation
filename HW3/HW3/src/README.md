# HW3

## How to compile

In this directory, enter the following command. It'll generate the executable file **hw3** in **../bin/**

```shell
make
```

If you want to remove it, please enter the following command.

```shell
make clean
```

## How to run

In this directory, enter the following command to run.

```shell
../bin/hw3 [*.hardblocks] [*.nets] [*.pl] [*.floorplan] [dead_space_ratio]
```

e.g.

```shell
../bin/hw3 ../testcases/n100.hardblocks  ../testcases/n100.nets ../
testcases/n100.pl  ../output/n100.floorplan 0.15
```

## How to judge

In this directory, enter the following command to judge.

```shell
make judge
```
