def write_def(def_name=None, die=None, component_list=None, specialnet_list=None):
    if def_name is None:
        print('[Error] Please provide a def file name.\n')
        return False

    with open(def_name, 'w', newline='\n') as fout:
        fout.write('VERSION 5.6 ;\n')
        fout.write('DIVIDERCHAR "/" ;\n')
        fout.write('BUSBITCHARS "[]" ;\n')

        if die is None:
            print('[Error] Die info is not defined.\n')
            return False

        fout.write(f'DESIGN {die.design_name} ;\n\n')
        fout.write('UNITS DISTANCE MICRONS 1000 ;\n\n')
        fout.write('PROPERTYDEFINITIONS\n')
        fout.write('  COMPONENTPIN text STRING ;\n')
        fout.write('END PROPERTYDEFINITIONS\n\n')
        fout.write(f'DIEAREA ( {die._x1} {die._y1} ) ( {die._x2} {die._y2} ) ;\n\n')

        if component_list is not None:
            fout.write(f'COMPONENTS {len(component_list)} ;\n')
            for component in component_list:
                fout.write(f'- {component.inst_name} {component.lib_name}\n')
                fout.write(f'  + PLACED ( {component._x} {component._y} ) N ;\n')
            fout.write("END COMPONENTS\n\n")

        if specialnet_list is not None:
            fout.write(f'SPECIALNETS {len(specialnet_list)} ;\n')
            for i, specialnet in enumerate(specialnet_list):
                if specialnet.layer == 'ME3':
                    name = specialnet.inst_name
                    x = (specialnet._x1 + specialnet._x2) // 2
                    y1 = specialnet._y1
                    y2 = specialnet._y2
                    fout.write(f'- {name}\n')
                    fout.write(f'  + ROUTED ME3 440 ( {x} {y1} ) ( * {y2} ) ;\n')
                elif specialnet.layer == 'ME4':
                    name = specialnet.inst_name
                    x1 = specialnet._x1
                    x2 = specialnet._x2
                    y = (specialnet._y1 + specialnet._y2) // 2
                    fout.write(f'- {name}\n')
                    fout.write(f'  + ROUTED ME4 1000 ( {x1} {y} ) ( {x2} * ) ;\n')
                else:
                    print(f'[Error] Wrong layer definition of {i}-th specialnet.\n')
                    return False
            fout.write('END SPECIALNETS\n\n')

        fout.write('END DESIGN\n')
    return True
