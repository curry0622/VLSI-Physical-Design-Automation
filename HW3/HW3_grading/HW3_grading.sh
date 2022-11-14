#!/bin/bash
homeworkName=HW3
binaryName=hw3
testcasePool="n100 n200 n300"
ratioPool="0.15 0.1"
timeLimit=600

root=$(pwd)
outputDir=$root/output
studentDir=$root/student
testcasesDir=$root/testcases
verifierBin=$root/verifier/verifier
chmod 500 $verifierBin
csvFile=$root/${homeworkName}_grade.csv

function executionCmd() {
	local argv="$testcasesDir/$1.hardblocks $testcasesDir/$1.nets $testcasesDir/$1.pl $outputDir/$1.floorplan $2"
	local log=$(timeout $timeLimit time -p ./$binaryName $argv 2>&1 >/dev/null)
	if [[ $log =~ "real " ]] && [[ $log =~ "user " ]] && [[ $log =~ "sys " ]]; then
		echo "$(echo "$log" | grep real | tail -1 | cut -d ' ' -f 2)"
	else
		echo "TLE"
	fi
}

function verifyCmd() {
	local argv="$testcasesDir/$1.hardblocks $testcasesDir/$1.nets $testcasesDir/$1.pl $outputDir/$1.floorplan $2"
	local log=$($verifierBin $argv | cat)
	if [[ $log =~ "Usage:" ]]; then
		echo "Verifier fails in $1."
	elif [[ $log =~ "Error: Checking overlap" ]]; then
		echo "$1 has some overlapping cells."
	elif [[ $log =~ "Error: Checking fixed-outline" ]]; then
		echo "$1 has some cells out of outline."
	elif [[ $log =~ "Error: Wirelength" ]]; then
		echo "$1 has wrong wirelength."
	elif [[ $log =~ "OK!!" ]]; then
		echo "success"
	else
		echo "$1 has some errors."
	fi
}

echo "|------------------------------------------------|"
echo "|                                                |"
echo "|    This script is used for PDA $homeworkName grading.    |"
echo "|                                                |"
echo "|------------------------------------------------|"

csvTitle="student id"
for ratio in $ratioPool; do
	for testcase in $testcasePool; do
		csvTitle="$csvTitle, $testcase-$ratio wirelength, $testcase-$ratio runtime"
	done
done
echo "$csvTitle, status" >$csvFile

cd $studentDir/
for studentId in *; do
	if [[ -d $studentId ]]; then
		printf "grading on %s:\n" $studentId
		cd $studentId/

		rm -rf $(find . -mindepth 1 -maxdepth 1 -type d)
		tar -zxf CS6135_${homeworkName}_$studentId.tar.gz
		cd $homeworkName/

		cd src/
		make clean >/dev/null
		rm -rf ./bin/*
		make >/dev/null
		cd ..

		rm -rf $outputDir/*
		cd bin/
		csvContent="$studentId"
		statusList=""
		printf "%10s | %10s | %10s | %10s | %s\n" testcase ratio wirelength runtime status
		for ratio in $ratioPool; do
			for testcase in $testcasePool; do
				printf "%10s | %10s | " $testcase $ratio

				wirelength=fail
				status="$testcase failed."
				runtime=$(executionCmd $testcase $ratio)
				if [[ $runtime != "TLE" ]]; then
					status=$(verifyCmd $testcase $ratio)

					if [[ $status == "success" ]]; then
						wirelength=$(cat $outputDir/$testcase.floorplan | grep Wirelength | cut -d ' ' -f 2)
					else
						statusList="$statusList $status"
					fi
				else
					statusList="$statusList $status"
				fi

				printf "%10s | %10s | %s\n" $wirelength $runtime "$status"
				csvContent="$csvContent, $wirelength, $runtime"
			done
		done
		cd ..

		echo "$csvContent,$statusList" >>$csvFile
		cd $studentDir/
	fi
done

echo "|-----------------------------------------------------|"
echo "|                                                     |"
echo "|    Successfully generate grades to ${homeworkName}_grade.csv    |"
echo "|                                                     |"
echo "|-----------------------------------------------------|"
