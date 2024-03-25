#!/bin/bash

log="test-run.log"

N1=25920
N2=43200
N3=46080

m=60
p=24
r=4

if [[ $# -eq 1 ]];
then
  v="$1"
else
  v=""
fi

values_arr=(
  "1 $N2 11000" #1
  "1 $N1  6200" #2
  "1 $N2 13000" #3
  "1 $N1  6800" #4
  "1 $N3 13000" #5
  "1 $N2 20000" #6
  "1 $N2 19000" #7
  "1 $N1  8500" #8
  "3 $N2 11000" #9
  "3 $N2 11000" #10
  "3 $N2 19000" #11
  "3 $N1  6200" #12
  "3 $N1  6200" #13
  "3 $N1 10000" #14
  "3 $N2 17000" #15
  "3 $N2 17000" #16
  "3 $N2 21000" #17
  "3 $N1 12000" #18
)

# get task params
result=$(./a.out 3 3 3 1 2>&1 | grep ': Task')
if [[ "${result}" == "" ]];
then
  result=$(./a.out 3 3 1 3 1 2>&1 | grep ': Task')
  if [[ "${result}" == "" ]];
  then
    echo "ERROR: WRONG OUTPUT FROM \"./a.out\""
    exit 1
  else
    th=1

    task=$(echo "${result}" | cut -d ' ' -f 5)
    if [[ "${task}" == "" ]];
    then
      echo "ERROR: NO TASK NUMBER (./a.out 3 3 1 3 1)"
      exit 2
    fi
  fi
else
  th=0

  task=$(echo "${result}" | cut -d ' ' -f 5)
  if [[ "${task}" == "" ]];
  then
    echo "ERROR: NO TASK NUMBER (./a.out 3 3 3 1)"
    exit 2
  fi
fi

if [[ $task -gt 18 ]];
then
  echo "ERROR: UNKNOWN PARAMETERS FOR TASK ${task}"
  exit 3
fi

values=(${values_arr[$task - 1]})
s=${values[0]}
N=${values[1]}
t_ref=${values[2]}

start_time=$(date +%s)

#clear log
echo "########################################################################" | tee $log

if [[ $th -eq 1 ]];
then
  echo "==================================================================" | tee -a $log
  echo "| Task = $task | s = $s | n = $N | m = $m | p = $p | t_ref = $t_ref |" | tee -a $log
  echo "==================================================================" | tee -a $log

  s_est=$(echo "scale=2; ${t_ref} * 1.125 / 19" | bc -l)
  h_est=$(echo "scale=2; (${t_ref} * 1.125 / 3600) / 19" | bc -l)
  echo "ESTIMATED TIME: ${s_est} SEC (${h_est} HR)"
else
  echo "=========================================================" | tee -a $log
  echo "| Task = $task | s = $s | n = $N | m = $m | t_ref = $t_ref |" | tee -a $log
  echo "=========================================================" | tee -a $log

  s_est=$(echo "scale=2; ${t_ref} * 1.125" | bc -l)
  h_est=$(echo "scale=2; ${t_ref} * 1.125 / 3600" | bc -l)
  echo "ESTIMATED TIME: ${s_est} SEC (${h_est} HR)"
fi

for (( i = 0, n = N / 8; i <= 3; i++, n *= 2 ))
do
  if [[ $th -eq 1 ]];
  then
    echo "============================== Run $i ==================================" | tee -a $log
    echo "### Run  ./a.out $n $m $p $r $s" | tee -a $log
    output=$(./a.out $n $m $p $r $s 2>&1)
  else
    echo "============================== Run $i ==================================" | tee -a $log
    echo "### Run  ./a.out $n $m $r $s" | tee -a $log
    output=$(./a.out $n $m $r $s 2>&1)
  fi

  echo "${output}" >> $log

  result=$(echo "${output}" | grep ': Task')

  if [[ "${result}" == "" ]];
  then
    echo "ERROR: NO RESULT"
    exit 4
  fi

  t[$i]=$(echo "${result}" | cut -d ' ' -f 14)
  echo "${result}"
done

echo "=======================================================================" | tee -a $log

t10=$(echo "scale=3; ${t[1]} / ${t[0]}" | bc -l)
t21=$(echo "scale=3; ${t[2]} / ${t[1]}" | bc -l)
t32=$(echo "scale=3; ${t[3]} / ${t[2]}" | bc -l)

echo "t1 / t0 = ${t10}" | tee -a $log
echo "t2 / t1 = ${t21}" | tee -a $log
echo "t3 / t2 = ${t32}" | tee -a $log

if [[ $th -eq 1 ]];
then
  if [[ "$v" != "" ]];
  then
    t3_prev=$(echo "scale=3; ${v} / ${t[3]}" | bc -l)
    echo "t_prev /t3 = ${t3_prev}" | tee -a $log
  fi
else
  t3_ref=$(echo "scale=3; ${t[3]} / ${t_ref}" | bc -l)
  echo "t3 / t_ref = ${t3_ref}" | tee -a $log
fi

echo "########################################################################" | tee -a $log

if [[ ( $(echo "${t10} > 7.8" | bc -l) -eq 1 && $(echo "${t10} < 8.2" | bc -l) -eq 1 )
      || ( $(echo "${t21} > 7.8" | bc -l) -eq 1 && $(echo "${t21} < 8.2" | bc -l) -eq 1 )
      || ( $(echo "${t32} > 7.8" | bc -l) -eq 1 && $(echo "${t32} < 8.2" | bc -l) -eq 1 ) ]];
then
  echo "SCALABILITY: SUCCESS"
else
  echo "SCALABILITY: FAILED"
fi

if [[ $th -eq 1 ]];
then
  if [[ "$v" != "" ]];
  then
    if [[ $(echo "${t3_prev} > 19" | bc -l) -eq 1 && $(echo "${t3_prev} < 24" | bc -l) -eq 1 ]];
    then
      echo "REFERENCE:   SUCCESS"
    else
      echo "REFERENCE:   FAILED"
    fi
  fi
else
  if [[ $(echo "${t3_ref} < 1.3" | bc -l) -eq 1 ]];
  then
    echo "REFERENCE:   SUCCESS"
  else
    echo "REFERENCE:   FAILED"
  fi
fi

end_time=$(date +%s)
echo "SCRIPT ELAPSED: $((end_time - start_time)) SEC"
