import subprocess

if __name__ == '__main__':
    for i in range(1, 200):
        print(f'bash run.sh n100 0.15 {i}')
        subprocess.run([f'bash run.sh n100 0.15 {i}'], shell=True)
        print(f'bash run.sh n100 0.1 {i}')
        subprocess.run([f'bash run.sh n100 0.1 {i}'], shell=True)
        
        print(f'bash run.sh n200 0.15 {i}')
        subprocess.run([f'bash run.sh n200 0.15 {i}'], shell=True)
        print(f'bash run.sh n200 0.1 {i}')
        subprocess.run([f'bash run.sh n200 0.1 {i}'], shell=True)
        
        print(f'bash run.sh n300 0.15 {i}')
        subprocess.run([f'bash run.sh n300 0.15 {i}'], shell=True)
        print(f'bash run.sh n300 0.1 {i}')
        subprocess.run([f'bash run.sh n300 0.1 {i}'], shell=True)
