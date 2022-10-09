import subprocess
from datetime import datetime

def time_log(str):
  print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] {str}')

def run_test(clk, core, congDriv, timeDriv):
  # create base files
  testname = f'{clk}ns_{core}_{congDriv}_{timeDriv}'
  subprocess.run([f'cp -r untar/ tests/{testname}'], shell=True)

  # print time log
  time_log(f'Start running test {testname}')

  # modify design.sdc
  cmds = []
  fin = open(f'./tests/{testname}/design.sdc', 'r')
  for line in fin:
    if 'create_clock' in line:
      line = 'create_clock [get_ports {clk}] -name VCLK -period ' + '%.1f' % float(clk) + ' -waveform {0.0 ' + '%.1f' % (float(clk) / 2) + '}\n'
      print(line)
    cmds.append(line)
  fin.close()

  fout = open(f'./tests/{testname}/design.sdc', 'w')
  fout.writelines(cmds)
  fout.close()

  # modify apr.tcl
  cmds = []
  fin = open(f'./tests/{testname}/apr.tcl', 'r')
  for line in fin:
    if 'floorplan' in line:
      line = f'floorPlan -coreMarginsBy die -site FreePDK45_38x28_10R_NP_162NW_34O -r 1.0 {core} 4.0 4.0 4.0 4.0\n'
      print(line)
    if 'congEffort' in line and 'timingDriven' in line:
      line = f'setPlaceMode -congEffort {congDriv} -timingDriven {0 if timeDriv == "off" else 1} -clkGateAware 1 -powerDriven 0 -ignoreScan 1 -reorderScan 1 -ignoreSpare 0 -placeIOPins 1 -moduleAwareSpare 0\n'
      print(line)
    cmds.append(line)

  fout = open(f'./tests/{testname}/apr.tcl', 'w')
  fout.writelines(cmds)
  fout.close()

  # run innovus
  subprocess.run(['innovus -files apr.tcl -batch -no_gui'], shell=True)

  # print time log
  time_log(f'Finish running test {testname}')

def file_report(clk, core, congDriv, timeDriv):
  # test info
  testname = f'{clk}ns_{core}_{congDriv}_{timeDriv}'
  dirName = f'./tests/{testname}'

  # print time log
  time_log(f'Start generating report of {testname}')

  # get slack
  slack = -404
  fin = open(f'{dirName}/innovus.log', 'r')
  for line in fin:
    if 'Slack Time' in line:
      slack = line.split(' ')[-1].replace('\n', '')
      break
  fin.close()

  # get total-wirelength and area
  totalWirelength = -404
  fin = open(f'{dirName}/summary.rpt', 'r')
  for line in fin:
    if 'Total wire length' in line:
      totalWirelength = line.split(' ')[-4]
    if 'Total area of Chip' in line:
      area = line.split(' ')[-4]
  fin.close()

  # get violations
  violations = -404
  fin = open(f'{dirName}/drc.rpt', 'r')
  for line in fin:
    if 'Verification Complete' in line:
      violations = line.split(' ')[-2]
      break
  fin.close()

  # write file
  fout = open(f'{dirName}/report.txt', 'w')
  fout.write(f'[test]: {testname}\n')
  fout.write(f'[timing]: {clk}ns\n')
  fout.write(f'[core_utilization]: {core}\n')
  fout.write(f'[congestion-driven]: {congDriv}\n')
  fout.write(f'[timing-driven]: {timeDriv}\n')
  fout.write(f'[slack]: {slack}\n')
  fout.write(f'[total-wirelength]: {totalWirelength} um\n')
  fout.write(f'[area]: {area} um^2\n')
  fout.write(f'[violations]: {violations}\n')
  fout.close()

  # print time log
  time_log(f'Finish generating report of {testname}')

if __name__ == '__main__':
  run_test('4', '0.7', 'low', 'off')
  run_test('4', '0.7', 'low', 'on')
  run_test('4', '0.7', 'medium', 'off')
  run_test('4', '0.7', 'medium', 'on')
  run_test('4', '0.7', 'high', 'off')
  run_test('4', '0.7', 'high', 'on')

  file_report('4', '0.7', 'low', 'off')
  file_report('4', '0.7', 'low', 'on')
  file_report('4', '0.7', 'medium', 'off')
  file_report('4', '0.7', 'medium', 'on')
  file_report('4', '0.7', 'high', 'off')
  file_report('4', '0.7', 'high', 'on')