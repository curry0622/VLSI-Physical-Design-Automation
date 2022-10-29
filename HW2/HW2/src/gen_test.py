import argparse
import random

def parse_args():
  # Ex: python3 gen.py -fn test -cn 100 -ms 100 -nn 50 -md 10
  parser = argparse.ArgumentParser()
  parser.add_argument('-fn', default='test', help='Input file name', type=str)
  parser.add_argument('-cn', default=10, help='Input cells num', type=int)
  parser.add_argument('-ms', default=10, help='Input max cell size', type=int)
  parser.add_argument('-nn', default=10, help='Input nets num', type=int)
  parser.add_argument('-md', default=3, help='Input max net degree', type=int)
  return parser.parse_args()

def print_info(args):
  print()
  print(f'Filename: {args.fn}')
  print(f'Cells num: {args.cn}')
  print(f'Max cell size: {args.ms}')
  print(f'Nets num: {args.nn}')
  print(f'Max net degree: {args.md}')
  print()
  
def gen_cells(args):
  cells = range(1, args.cn + 1)
  f = open(f'{args.fn}.cells', 'w')
  for c in cells:
    f.write(f'c{c} {random.randint(1, args.ms)} {random.randint(1, args.ms)}\n')
  f.close()
    
def gen_nets(args):
  nets = range(1, args.nn + 1)
  f = open(f'{args.fn}.nets', 'w')
  for n in nets:
    f.write(f'NET n{n} {{ ')
    deg = random.randint(1, args.md)
    cells = random.sample(range(1, args.cn + 1), deg)
    for c in cells:
      f.write(f'c{c} ')
    f.write('}\n')
  f.close()

if __name__ == '__main__':
  args = parse_args()
  gen_cells(args)
  gen_nets(args)
  print_info(args)