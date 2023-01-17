#include <bits/stdc++.h>
#include "Object.hpp"
#include "Utils.hpp"

/* Global parameters */
#define CS_WIDTH 7100
#define CS_HEIGHT 6600
#define M3_WIDTH 440
#define M3_SPACING 310
#define M4_WIDTH 1000
#define M4_SPACING 490

#define CS_X1_TO_DRAIN 1260
#define CS_Y1_TO_DRAIN 4100
#define CS_LIB_NAME "MSBCS"
#define VIA34_LIB_NAME "Via34"

int main(int argc, char *argv[]) {
    /* Check arguments */
    assert(argc == 3);

    /* Get arguments */
    int n = atoi(argv[1]);
    std::string def_file_path = argv[2];

    /* Step 1: create die boundary */
    int NUM_CELL = sqrt(n * 4);
    int NUM_M3 = NUM_CELL / 2;
    int NUM_M4 = NUM_CELL / 4;
    int die_x1 = 0;
    int die_y1 = 0;
    int die_x2 = CS_WIDTH * NUM_CELL + M3_SPACING * ((NUM_M3 + 1) * NUM_CELL - 1) + M3_WIDTH * NUM_M3 * NUM_CELL;
    int die_y2 = CS_HEIGHT * NUM_CELL + M4_SPACING * ((NUM_M4 + 1) * NUM_CELL - 1) + M4_WIDTH * NUM_M4 * NUM_CELL;
    Die die("CS_APR", die_x1, die_y1, die_x2, die_y2);

    /* Step 2: create CS array */
    Component cs_array[NUM_CELL][NUM_CELL];
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            std::string inst_name = "Transistor" + std::to_string(i * NUM_CELL + j);
            int off_y = NUM_M4 * (M4_SPACING + M4_WIDTH);
            int D_y = off_y + CS_HEIGHT + M4_SPACING;
            int D_x = CS_WIDTH + NUM_M3 * (M3_SPACING + M3_WIDTH) + M3_SPACING;
            int x = i * D_x;
            int y = j * D_y + off_y;
            cs_array[i][j] = Component(CS_LIB_NAME, inst_name, x, y);    
        }
    }

    /* Step 3: create vertical ME3 */
    SpecialNet ME3_specialnet[NUM_CELL][NUM_M3];
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_M3; j++) {
            std::string inst_name = "Metal3_" + std::to_string(i * NUM_M3 + j);
            std::string layer = "ME3";
            int D_x = CS_WIDTH + M3_SPACING;
            int P_x = M3_WIDTH + M3_SPACING;
            int x1 = cs_array[i][0].x + D_x + j * P_x;
            int x2 = x1 + M3_WIDTH;
            int y1 = 0;
            int y2 = die_y2;
            ME3_specialnet[i][j] = SpecialNet(inst_name, layer, x1, y1, x2, y2);
        }
    }

    /* Step 4: create ME4 drain */
    SpecialNet ME4_specialnet_drain[NUM_CELL][NUM_CELL];
    int HALF_NUM_CELL = NUM_CELL / 2;
    for (int i = 0; i < HALF_NUM_CELL; i++) {
        for(int j = 0; j < HALF_NUM_CELL; j++) {
            std::string layer = "ME4", inst_name;
            // left bottom corner units
            inst_name = "Metal4_drain_" + std::to_string(i * HALF_NUM_CELL + j + 0 * 4);
            int x1 = cs_array[i][j].x + CS_X1_TO_DRAIN;
            int x2 = ME3_specialnet[i][j].x2;
            int y1 = cs_array[i][j].y + CS_Y1_TO_DRAIN;
            int y2 = y1 + M4_WIDTH;
            ME4_specialnet_drain[i][j] = SpecialNet(inst_name, layer, x1, y1, x2, y2);
            // right bottom corner units
            inst_name = "Metal4_drain_" + std::to_string(i * HALF_NUM_CELL + j + 1 * 4);
            x1 = cs_array[(NUM_CELL-1)-i][j].x + CS_X1_TO_DRAIN;
            x2 = ME3_specialnet[(NUM_CELL-1)-i][j].x2;
            y1 = cs_array[(NUM_CELL-1)-i][j].y + CS_Y1_TO_DRAIN;
            y2 = y1 + M4_WIDTH;
            ME4_specialnet_drain[(NUM_CELL-1)-i][j] = SpecialNet(inst_name, layer, x1, y1, x2, y2);
            // left top corner units
            inst_name = "Metal4_drain_" + std::to_string(i * HALF_NUM_CELL + j + 2 * 4);
            x1 = cs_array[i][(NUM_CELL-1)-j].x + CS_X1_TO_DRAIN;
            x2 = ME3_specialnet[i][j].x2;
            y1 = cs_array[i][(NUM_CELL-1)-j].y + CS_Y1_TO_DRAIN;
            y2 = y1 + M4_WIDTH;
            ME4_specialnet_drain[i][(NUM_CELL-1)-j] = SpecialNet(inst_name, layer, x1, y1, x2, y2);
            // right top corner units
            inst_name = "Metal4_drain_" + std::to_string(i * HALF_NUM_CELL + j + 3 * 4);
            x1 = cs_array[(NUM_CELL-1)-i][(NUM_CELL-1)-j].x + CS_X1_TO_DRAIN;
            x2 = ME3_specialnet[(NUM_CELL-1)-i][j].x2;
            y1 = cs_array[(NUM_CELL-1)-i][(NUM_CELL-1)-j].y + CS_Y1_TO_DRAIN;
            y2 = y1 + M4_WIDTH;
            ME4_specialnet_drain[(NUM_CELL-1)-i][(NUM_CELL-1)-j] = SpecialNet(inst_name, layer, x1, y1, x2, y2);
        }
    }

    /* Step 5: create ME4 port */
    SpecialNet ME4_specialnet_port[4];
    for (int i = 0; i < 4; i++) {
        std::string inst_name = "Metal4_port_" + std::to_string(i);
        std::string layer = "ME4";
        int D_y = CS_HEIGHT + M4_SPACING * 2 + M4_WIDTH;
        int x1 = 0;
        int x2 = die_x2;
        int y1 = i * D_y;
        int y2 = y1 + M4_WIDTH;
        ME4_specialnet_port[i] = SpecialNet(inst_name, layer, x1, y1, x2, y2);
    }

    /* Step 6: create Via34 from ME4 drain */
    Component Via34_drain2ME3[4][4];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            std::string inst_name = "";
            // left bottom corner units
            inst_name = "Via34_drain2ME3_" + std::to_string(i * 2 + j + 0 * 4);
            int x = ME3_specialnet[i][j].x1;
            int y = cs_array[i][j].y + CS_Y1_TO_DRAIN;
            Via34_drain2ME3[i][j] = Component(VIA34_LIB_NAME, inst_name, x, y);
            // right bottom corner units
            inst_name = "Via34_drain2ME3_" + std::to_string(i * 2 + j + 1 * 4);
            x = ME3_specialnet[3-i][j].x1;
            y = cs_array[3-i][j].y + CS_Y1_TO_DRAIN;
            Via34_drain2ME3[3-i][j] = Component(VIA34_LIB_NAME, inst_name, x, y);
            // left top corner units
            inst_name = "Via34_drain2ME3_" + std::to_string(i * 2 + j + 2 * 4);
            x = ME3_specialnet[i][j].x1;
            y = cs_array[i][3-j].y + CS_Y1_TO_DRAIN;
            Via34_drain2ME3[i][3-j] = Component(VIA34_LIB_NAME, inst_name, x, y);
            // right top corner units
            inst_name = "Via34_drain2ME3_" + std::to_string(i * 2 + j + 3 * 4);
            x = ME3_specialnet[3-i][j].x1;
            y = cs_array[3-i][3-j].y + CS_Y1_TO_DRAIN;
            Via34_drain2ME3[3-i][3-j] = Component(VIA34_LIB_NAME, inst_name, x, y);
        }
    }

    /* Step 7: create Via34 to ME4 port */
    Component Via34_port2ME3[4][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            std::string inst_name;
            int x, y;
            // left via
            inst_name = "Via34_port2ME3_" + std::to_string((i * 2 + j) * 2 + 0);
            x = ME3_specialnet[i][j].x1;
            y = ME4_specialnet_port[i * 2 + j].y1;
            Via34_port2ME3[i * 2 + j][0] = Component(VIA34_LIB_NAME, inst_name, x, y);
            // right via
            inst_name = "Via34_port2ME3_" + std::to_string((i * 2 + j) * 2 + 1);
            x = ME3_specialnet[3-i][j].x1;
            y = ME4_specialnet_port[i * 2 + j].y1;
            Via34_port2ME3[i * 2 + j][1] = Component(VIA34_LIB_NAME, inst_name, x, y);
        }
    }

    /* Create component vector */
    std::vector<Component> component_list;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            component_list.push_back(cs_array[i][j]);
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            component_list.push_back(Via34_drain2ME3[i][j]);
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            component_list.push_back(Via34_port2ME3[i][j]);
        }
    }

    /* Create special net vector */
    std::vector<SpecialNet> specialnet_list;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            specialnet_list.push_back(ME3_specialnet[i][j]);
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            specialnet_list.push_back(ME4_specialnet_drain[i][j]);
        }
    }
    for (int i = 0; i < 4; i++) {
        specialnet_list.push_back(ME4_specialnet_port[i]);
    }

    /* Final: Write info to def file */
    write_def(def_file_path, die, component_list, specialnet_list);

    return 0;
}