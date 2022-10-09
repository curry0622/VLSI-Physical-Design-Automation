def auto_report(clk, core, congDriv, timeDriv):
  testname = f'{clk}ns_{core}_{congDriv}_{timeDriv}'
  dirName = f'./tests/{testname}'

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

if __name__ == '__main__':
  auto_report(20, 0.7, 'low', 'off')
  auto_report(4, 0.7, 'low', 'off')