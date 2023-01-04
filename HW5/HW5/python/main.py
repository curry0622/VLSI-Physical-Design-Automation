#!/usr/bin/python3
# import required classes and functions
from myObject import Die, Component, SpecialNet
from write_def import write_def

# define global parameters
CS_WIDTH = 7100
CS_HEIGHT = 6600
M3_WIDTH = 440
M3_SPACING = 310
M4_WIDTH = 1000
M4_SPACING = 490

CS_X1_TO_DRAIN = 1260
CS_Y1_TO_DRAIN = 4100
CS_LIB_NAME = 'MSBCS'
VIA34_LIB_NAME = 'Via34'

##### Step 1: create die boundary #####
design_name = 'CS_APR'
die_x1 = 0
die_y1 = 0
die_x2 =
die_y2 =
die = Die(design_name, die_x1, die_y1, die_x2, die_y2)

##### Step 2: create CS array #####
cs_array = [[Component for j in range(4)] for i in range(4)]
for i in range(4):
    for j in range(4):
        cs_lib_name = CS_LIB_NAME
        cs_instance_name = 'Transistor' + str(i * 4 + j)
        x =
        y =
        cs_array[i][j] = Component(cs_lib_name, cs_instance_name, x, y)

##### Step 3: create vertical ME3 #####
# ME3 nets
ME3_specialnet = [[SpecialNet for j in range(2)] for i in range(4)]
for i in range(4):
    for j in range(2):
        inst_name = 'Metal3_' + str(i * 2 + j)
        layer = 'ME3'
        x1 =
        x2 =
        y1 =
        y2 =
        ME3_specialnet[i][j] = SpecialNet(inst_name, layer, x1, y1, x2, y2)

##### Step 4: create ME4 drain #####
# ME4 drains
ME4_specialnet_drain = [[SpecialNet for j in range(4)] for i in range(4)]
for i in range(2):
    for j in range(2):
        layer = 'ME4'
        # left bottom corner units
        inst_name = 'Metal4_drain_' + str(i * 2 + j + 0 * 4)
        x1 = cs_array[i][j]._x + CS_X1_TO_DRAIN
        x2 = ME3_specialnet[i][j]._x2
        y1 = cs_array[i][j]._y + CS_Y1_TO_DRAIN
        y2 = y1 + M4_WIDTH
        ME4_specialnet_drain[i][j] = SpecialNet(inst_name, layer, x1, y1, x2, y2)
        # right bottom corner units
        inst_name = 'Metal4_drain_' + str(i * 2 + j + 1 * 4)
        x1 = cs_array[3-i][j]._x + CS_X1_TO_DRAIN
        x2 = ME3_specialnet[3-i][j]._x2
        y1 = cs_array[3-i][j]._y + CS_Y1_TO_DRAIN
        y2 = y1 + M4_WIDTH
        ME4_specialnet_drain[3-i][j] = SpecialNet(inst_name, layer, x1, y1, x2, y2)
        # left top corner units
        inst_name = 'Metal4_drain_' + str(i * 2 + j + 2 * 4)
        x1 = cs_array[i][3-j]._x + CS_X1_TO_DRAIN
        x2 = ME3_specialnet[i][j]._x2
        y1 = cs_array[i][3-j]._y + CS_Y1_TO_DRAIN
        y2 = y1 + M4_WIDTH
        ME4_specialnet_drain[i][3-j] = SpecialNet(inst_name, layer, x1, y1, x2, y2)
        # right top corner units
        inst_name = 'Metal4_drain_' + str(i * 2 + j + 3 * 4)
        x1 =
        x2 =
        y1 =
        y2 =
        ME4_specialnet_drain[3-i][3-j] = SpecialNet(inst_name, layer, x1, y1, x2, y2)

##### Step 5: create ME4 port #####
# ME4 ports
ME4_specialnet_port = [SpecialNet for i in range(4)]
for i in range(4):
    inst_name = 'Metal4_port_' + str(i)
    layer = 'ME4'
    x1 =
    x2 =
    y1 =
    y2 =
    ME4_specialnet_port[i] = SpecialNet(inst_name, layer, x1, y1, x2, y2)

##### Step 6: create Via34 from ME4 drain #####
# drain to ME3
Via34_drain2ME3 = [[Component for j in range(4)] for i in range(4)]
for i in range(2):
    for j in range(2):
        lib_name = VIA34_LIB_NAME
        # left bottom corner units
        inst_name = 'Via34_drain2ME3_' + str(i * 2 + j + 0 * 4)
        x = ME3_specialnet[i][j]._x1
        y = cs_array[i][j]._y + CS_Y1_TO_DRAIN
        Via34_drain2ME3[i][j] = Component(lib_name, inst_name, x, y)
        # right bottom corner units
        inst_name = 'Via34_drain2ME3_' + str(i * 2 + j + 1 * 4)
        x = ME3_specialnet[3-i][j]._x1
        y = cs_array[3-i][j]._y + CS_Y1_TO_DRAIN
        Via34_drain2ME3[3-i][j] = Component(lib_name, inst_name, x, y)
        # left top corner units
        inst_name = 'Via34_drain2ME3_' + str(i * 2 + j + 2 * 4)
        x = ME3_specialnet[i][j]._x1
        y = cs_array[i][3-j]._y + CS_Y1_TO_DRAIN
        Via34_drain2ME3[i][3-j] = Component(lib_name, inst_name, x, y)
        # right top corner units
        inst_name = 'Via34_drain2ME3_' + str(i * 2 + j + 3 * 4)
        x =
        y =
        Via34_drain2ME3[3-i][3-j] = Component(lib_name, inst_name, x, y)

##### Step 7: create Via34 to ME4 port #####
# port to ME4
# Precaution:
# 1. create a port list 'Via34_port2ME3' to contain port 'Component'
# 2. lib_name = VIA34_LIB_NAME
# 3. inst_name = 'Via34_port2ME3_'
# TODO


# write info to def file
component_list = []
for i in range(4):
    for j in range(4):
        component_list.append(cs_array[i][j])

# 4. add 'Via34_port2ME3' component to 'component_list'
# TODO


specialnet_list = []
for i in range(4):
    for j in range(2):
        specialnet_list.append(ME3_specialnet[i][j])

for i in range(4):
    for j in range(4):
        specialnet_list.append(ME4_specialnet_drain[i][j])

for i in range(4):
    specialnet_list.append(ME4_specialnet_port[i])

for i in range(4):
    for j in range(4):
        component_list.append(Via34_drain2ME3[i][j])

file_name = './DEF/CS_4.def'
write_def(file_name, die, component_list, specialnet_list)
