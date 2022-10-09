import subprocess
import time
from datetime import datetime

def time_log(str):
  logStr = f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] {str}'
  logStr = logStr.replace('\n', '')
  print(logStr)
  fout = open('./test.log', 'a')
  fout.write(f'{logStr}\n')
  fout.close()


def run_test(clk, core, congDriv, timeDriv):
  # start time
  startTime = time.time()

  # create base files
  testname = f'{clk}ns_{core}_{congDriv}_{timeDriv}'
  subprocess.run([f'cp -r untar/ tests/{testname}'], shell=True)

  # print time log
  time_log('')
  time_log(f'Start running test {testname}')
  time.sleep(3)

  # modify design.sdc
  cmds = []
  fin = open(f'./tests/{testname}/design.sdc', 'r')
  for line in fin:
    if 'create_clock' in line:
      oldStr = line
      newStr = f'create_clock [get_ports {{clk}}] -name VCLK -period {float(clk):.1f} -waveform {{0.0 {(float(clk) / 2):.1f}}}\n'
      line = newStr
      time_log(f'{testname}/design.sdc -> (-) {oldStr}')
      time_log(f'{testname}/design.sdc -> (+) {newStr}')
    cmds.append(line)
  fin.close()

  fout = open(f'./tests/{testname}/design.sdc', 'w')
  fout.writelines(cmds)
  fout.close()

  # modify apr.tcl
  cmds = []
  fin = open(f'./tests/{testname}/apr.tcl', 'r')
  for line in fin:
    if 'floorPlan' in line:
      oldStr = line
      newStr = f'floorPlan -coreMarginsBy die -site FreePDK45_38x28_10R_NP_162NW_34O -r 1.0 {core} 4.0 4.0 4.0 4.0\n'
      line = newStr
      time_log(f'{testname}/apr.tcl -> (-) {oldStr}')
      time_log(f'{testname}/apr.tcl -> (+) {newStr}')
    if 'congEffort' in line and 'timingDriven' in line:
      oldStr = line
      newStr = f'setPlaceMode -congEffort {congDriv} -timingDriven {0 if timeDriv == "off" else 1} -clkGateAware 1 -powerDriven 0 -ignoreScan 1 -reorderScan 1 -ignoreSpare 0 -placeIOPins 1 -moduleAwareSpare 0 -preserveRouting 1 -rmAffectedRouting 0 -checkRoute 0 -swapEEQ 0\n'
      time_log(f'{testname}/apr.tcl -> (-) {oldStr}')
      time_log(f'{testname}/apr.tcl -> (+) {newStr}')
    cmds.append(line)

  fout = open(f'./tests/{testname}/apr.tcl', 'w')
  fout.writelines(cmds)
  fout.close()

  # run innovus
  time_log('innovus -files apr.tcl -batch -no_gui')
  subprocess.run(['innovus -files apr.tcl -batch -no_gui'], shell=True, cwd=f'./tests/{testname}', stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

  # print time log
  time_log(f'Finish running test {testname}, elapsed time: {time.time() - startTime:.2f} sec')
  time_log('')


def file_report(clk, core, congDriv, timeDriv):
  # test info
  testname = f'{clk}ns_{core}_{congDriv}_{timeDriv}'
  dirName = f'./tests/{testname}'

  # print time log
  time_log('')
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
  time_log(f'[test]: {testname}')
  fout.write(f'[timing]: {clk}ns\n')
  time_log(str(f'[timing]: {clk}ns'))
  fout.write(f'[core_utilization]: {core}\n')
  time_log(str(f'[core_utilization]: {core}'))
  fout.write(f'[congestion-driven]: {congDriv}\n')
  time_log(str(f'[congestion-driven]: {congDriv}'))
  fout.write(f'[timing-driven]: {timeDriv}\n')
  time_log(str(f'[timing-driven]: {timeDriv}'))
  fout.write(f'[slack]: {slack}\n')
  time_log(str(f'[slack]: {slack}'))
  fout.write(f'[total-wirelength]: {totalWirelength} um\n')
  time_log(str(f'[total-wirelength]: {totalWirelength} um'))
  fout.write(f'[area]: {area} um^2\n')
  time_log(str(f'[area]: {area} um^2'))
  fout.write(f'[violations]: {violations}\n')
  time_log(str(f'[violations]: {violations}'))
  fout.close()

  # append report
  fout = open(f'./result.csv', 'a')
  fout.write(f'{testname},{clk},{core},{congDriv},{timeDriv},{slack},{totalWirelength},{area},{violations}\n')
  fout.close()

  # print time log
  time_log(f'Finish generating report of {testname}')
  time_log('')

def auto_test_and_report():
  for clk in range(1, 11):
    for core in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
      for congDriv in ['low', 'medium', 'high']:
        for timeDriv in ['off', 'on']:
          run_test(clk, core, congDriv, timeDriv)
          file_report(clk, core, congDriv, timeDriv)


def test():
  for congDriv in ['low', 'medium', 'high']:
    for timeDriv in ['off', 'on']:
      run_test(5, 0.8, congDriv, timeDriv)
      file_report(5, 0.8, congDriv, timeDriv)


if __name__ == '__main__':
  fout = open('./test.log', 'a')
  fout.write('\n')
  fout.write('||||||||||||||||||||||||||||||||||||||||\n')
  fout.write('|||||||||||||| Test Start ||||||||||||||\n')
  fout.write('||||||||||||||||||||||||||||||||||||||||\n')
  fout.write('\n')
  fout.close()

  test()
  # auto_test_and_report()

  fout = open('./test.log', 'a')
  fout.write('\n')
  fout.write('||||||||||||||||||||||||||||||||||||||||\n')
  fout.write('||||||||||||||| Test End |||||||||||||||\n')
  fout.write('||||||||||||||||||||||||||||||||||||||||\n')
  fout.write('\n')
  fout.close()
