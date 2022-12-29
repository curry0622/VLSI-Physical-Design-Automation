#!/bin/bash
homeworkName=HW4
binaryName=hw4
testcasePool="adaptec1 adaptec3 ibm01 ibm07 ibm09"
timeLimit=180

root=$(pwd)
outputDir=$root/output
studentDir=$root/student
testcasesDir=$root/testcase
verifierBin=$root/verifier/verify
chmod 500 $verifierBin
csvFile=$root/${homeworkName}_grade.csv

function executionCmd() {
	local argv="$testcasesDir/$1/$1.aux $outputDir/$1.result"
	local log=$(timeout $timeLimit time -p ./$binaryName $argv 2>&1 >/dev/null)
	if [[ $log =~ "real " ]] && [[ $log =~ "user " ]] && [[ $log =~ "sys " ]]; then
		echo "$(echo "$log" | grep real | tail -1 | cut -d ' ' -f 2)"
	else
		echo "TLE"
	fi
}

function verifyCmd() {
	local argv="$testcasesDir/$1/$1.aux $outputDir/$1.result"
	local log=$($verifierBin $argv | cat)
	total=$(echo "{$log}" | grep "total displacement" | cut -d ' ' -f 5)
	max=$(echo "{$log}" | grep "maximum displacement" | cut -d ' ' -f 5)
	if [[ $total == "" ]]; then
		total=fail
	fi
	if [[ $max == "" ]]; then
		max=fail
	fi
	echo "$max $total"
	if [[ $log =~ "Usage:" ]]; then
		echo "Verifier fails in $1."
	elif [[ $log =~ "Detected one (or more) cell doesn't align with any site" ]]; then
		echo "Site aligning constraint was violated for $1."
	elif [[ $log =~ "Detected one (or more) cell doesn't align with any row" ]]; then
		echo "Row aligning constraint was violated for $1."
	elif [[ $log =~ "Detected one (or more) fixed blockage is moved" ]]; then
		echo "Some fixed blockages in $1 was moved."
	elif [[ $log =~ "Detected one (or more) cell doesn't exist in the pl file" ]]; then
		echo "$1 has some cells doesn't exist in the pl file."
	elif [[ $log =~ "Detected one (or more) cell is missing in the result" ]]; then
		echo "$1 has some cells missing."
	elif [[ $log =~ "Detected overlapping" ]]; then
		echo "$1 has some overlapping cells."
	elif [[ $log =~ "Maximum displacement threshold is not satisfied" ]]; then
		echo "Maximum displacement constraint was violated for $1."
	elif [[ $log =~ "Verification passed" ]]; then
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
for testcase in $testcasePool; do
	csvTitle="$csvTitle, $testcase maximum disp., $testcase total disp., $testcase runtime"
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
		printf "%10s | %10s | %15s | %10s | %s\n" testcase "max disp." "total disp." runtime status
		for testcase in $testcasePool; do
			printf "%10s | " $testcase
			runtime=$(executionCmd $testcase)
			max=fail
			total=fail
			status="$testcase failed."
			if [[ $runtime != "TLE" ]]; then
				status=$(verifyCmd $testcase | tr '\n' ' ')
				IFS=' ' read -r max total status <<< "$status"
				if [[ $status != "success" ]]; then
					if [[ ! $status =~ "Maximum displacement" ]]; then
						max=fail
						total=fail
					fi
				fi
				printf "%10s | %15s | " "$max" "$total"

				if [[ $status != "success" ]]; then
					statusList="$statusList $status"
				fi
			else
				printf "%10s | %15s | " "$max" "$total"
				statusList="$statusList $testcase failed."
			fi

			printf "%10s | %s\n" $runtime "$status"
			csvContent="$csvContent, $max, $total, $runtime"
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
