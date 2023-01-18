#!/bin/bash
homeworkName=HW5
binaryName=hw5
testcasePool="python 4 16 36 64 100"

root=$(pwd)
outputDir=$root/output
studentDir=$root/student
verifierBin=$root/verifier/verify
chmod 500 $verifierBin
csvFile=$root/${homeworkName}_grade.csv

function executionCmd() {
	local argv="$1 $outputDir/CS_$1.def"
	local log=$(./$binaryName $argv 2>&1 >/dev/null)
}

function verifyCmd() {
	local argv="$1 $outputDir/CS_$1.def"
	if [[ $1 == "python" ]]; then
		argv="4 ../python/DEF/CS_4.def"
	fi

	local log=$($verifierBin $argv | cat)
	if [[ $log =~ "Usage:" ]]; then
		echo "Verifier fails in testcase $1."
	elif [[ $log =~ "Cannot open" ]]; then
		echo "Cannot open output for testcase $1."
	elif [[ $log =~ "Wrong format" ]]; then
		echo "Wrong format in DEF file for testcase $1."
	elif [[ $log =~ "The number of transistors" ]]; then
		echo "The number of transistors is wrong for testcase $1."
	elif [[ $log =~ "Exact 1 transistor need to connect to" ]]; then
		echo "Connectivity error for some drain and transistors in testcase $1."
	elif [[ $log =~ "There are no metal 3 and metal 4 wires connected by" ]]; then
		echo "Connectivity errors for some via in testcase $1."
	elif [[ $log =~ "Exact two transistors need to connect to" ]]; then
		echo "The number of transistors connected to some metal3 wires is wrong in testcase $1."
	elif [[ $log =~ "Your layout is not common centroid" ]]; then
		echo "Layout for testcase $1 is not common centroid."
	elif [[ $log =~ "[Success]" ]]; then
		echo "success"
	else
		echo "Testcase $1 has some errors."
	fi
}

echo "|------------------------------------------------|"
echo "|                                                |"
echo "|    This script is used for PDA $homeworkName grading.    |"
echo "|                                                |"
echo "|------------------------------------------------|"

csvTitle="student id"
for testcase in $testcasePool; do
	csvTitle="$csvTitle, $testcase"
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

		pythonOutputDir=$studentDir/$studentId/$homeworkName/python/DEF

		cd python/
		rm -rf __pycache__ *.pyc DEF/*
		python3 main.py
		cd ..

		cd src/
		make clean >/dev/null
		rm -rf ../bin/*
		make >/dev/null
		cd ..

		rm -rf $outputDir/*
		cd bin/
		csvContent="$studentId"
		statusList=""
		printf "%10s | %10s | %s\n" testcase "result" status
		for testcase in $testcasePool; do
			printf "%10s | " $testcase
			if [[ testcase != "python" ]]; then
				executionCmd $testcase
			fi
			status=$(verifyCmd $testcase $pythonOutputDir | tr '\n' ' ')
			IFS=' ' read -r status <<< "$status"
			result=pass
			if [[ $status != "success" ]]; then
				result=fail
			fi
			printf "%10s | " "$result"

			if [[ $status != "success" ]]; then
				statusList="$statusList $status"
			fi

			printf "%s\n" "$status"
			csvContent="$csvContent, $result"
		done
		cd ..

		echo "$csvContent,$statusList" >> $csvFile
		cd $studentDir/
	fi
done

echo "|-----------------------------------------------------|"
echo "|                                                     |"
echo "|    Successfully generate grades to ${homeworkName}_grade.csv    |"
echo "|                                                     |"
echo "|-----------------------------------------------------|"
