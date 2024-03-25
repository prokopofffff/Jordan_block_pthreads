#!/bin/bash

# test files directory
dir="."

log="test-run.log"

N=30
r=4
hilbert_n_test=12
max_re="1e-10"

if [[ $# -eq 1 ]];
then
  v="$1"
else
  v=""
fi

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

    args1=(
      "4 3 1 4 0 ${dir}/t.ttt"
      "4 3 2 4 0 ${dir}/t.ttt"
      "4 3 1 4 0 ${dir}/s.txt"
      "4 3 2 4 0 ${dir}/s.txt"
      "6 3 1 6 0 ${dir}/g.txt"
      "6 3 2 6 0 ${dir}/g.txt"
    )

    args2=(
      "4 3 1 4 0 ${dir}/a.txt"
      "4 3 2 4 0 ${dir}/a.txt"
      "4 3 1 4 0 ${dir}/a20.txt"
      "4 3 2 4 0 ${dir}/a20.txt"
      "4 3 1 4 0 ${dir}/a40.txt"
      "4 3 2 4 0 ${dir}/a40.txt"
      "4 3 1 4 0 ${dir}/b.txt"
      "4 3 2 4 0 ${dir}/b.txt"
      "6 3 1 6 0 ${dir}/c.txt"
      "6 3 2 6 0 ${dir}/c.txt"
      "6 3 1 6 0 ${dir}/d.txt"
      "6 3 2 6 0 ${dir}/d.txt"
      "6 3 1 6 0 ${dir}/e.txt"
      "6 3 2 6 0 ${dir}/e.txt"
      "4 3 1 4 0 ${dir}/f.txt"
      "4 3 2 4 0 ${dir}/f.txt"
    )
  fi
else
  th=0

  args1=(
    "4 3 4 0 ${dir}/t.ttt"
    "4 3 4 0 ${dir}/s.txt"
    "6 3 6 0 ${dir}/g.txt"
  )

  args2=(
    "4 3 4 0 ${dir}/a.txt"
    "4 3 4 0 ${dir}/a20.txt"
    "4 3 4 0 ${dir}/a40.txt"
    "4 3 4 0 ${dir}/b.txt"
    "6 3 6 0 ${dir}/c.txt"
    "6 3 6 0 ${dir}/d.txt"
    "6 3 6 0 ${dir}/e.txt"
    "4 3 4 0 ${dir}/f.txt"
  )
fi

# minimal residual if n = hilbert_n_test, s = 4 (Hilbert matrix)
min_r4="1.0"

# maximal residual
max_r=$(echo "${max_re}" | sed -E 's/([+-]?[0-9.]+)[eE]\+?(-?)([0-9]+)/(\1*10^\2\3)/g')

start_time=$(date +%s)

#clear log
echo "########################################################################" | tee $log

# test on wrong files
for arg_i in "${args1[@]}"
do
  if [[ $th -eq 1 ]];
  then
    fname=$(basename $(echo "${arg_i}" | cut -d ' ' -f 6))
  else
    fname=$(basename $(echo "${arg_i}" | cut -d ' ' -f 5))
  fi

  echo "============================== a.out ${fname} =========================" | tee -a $log
  echo "### Run ${v} ./a.out ${arg_i}" | tee -a $log
  output=$(${v} ./a.out ${arg_i} 2>&1)
  echo "${output}" | tee -a $log
done

# test on files
for arg_i in "${args2[@]}"
do
  if [[ $th -eq 1 ]];
  then
    fname=$(basename $(echo "${arg_i}" | cut -d ' ' -f 6))
  else
    fname=$(basename $(echo "${arg_i}" | cut -d ' ' -f 5))
  fi

  echo "============================== a.out ${fname} =========================" | tee -a $log
  echo "### Run ${v} ./a.out ${arg_i}" | tee -a $log
  output=$(${v} ./a.out ${arg_i} 2>&1)
  echo "${output}" | tee -a $log

  result=$(echo "${output}" | grep ': Task')
  if [[ "${result}" == "" ]];
  then
    echo "ERROR: NO RESULT"
    exit 1
  fi

  r1=$(echo "${result}" | cut -d ' ' -f 8 | sed -E 's/([+-]?[0-9.]+)[eE]\+?(-?)([0-9]+)/(\1*10^\2\3)/g')
  r2=$(echo "${result}" | cut -d ' ' -f 11 | sed -E 's/([+-]?[0-9.]+)[eE]\+?(-?)([0-9]+)/(\1*10^\2\3)/g')

  if [[ "${fname}" == "b.txt" ]];
  then
    if [[ $(echo "${r1} != -1.0" | bc -l) -eq 1 || $(echo "${r2} != -1.0" | bc -l) -eq 1 ]];
    then
      echo "ERROR: WRONG ANSWER"
      exit 2
    fi
  else
    if [[ $(echo "${r1} < 0.0" | bc -l) -eq 1 || $(echo "${r1} > ${max_r}" | bc -l) -eq 1
          || $(echo "${r2} < 0.0" | bc -l) -eq 1 || $(echo "${r2} > ${max_r}" | bc -l) -eq 1 ]];
    then
      echo "ERROR: WRONG ANSWER"
      exit 2
    fi
  fi
done

# test by formula
for (( s = 1; s <= 4; s++ ))
do
  for (( n = 1; n <= N; n++ ))
  do
    for (( m = 3; m <= n; m += 3 ))
    do
      if [[ $th -eq 1 ]];
      then
        (( p_max = n / m + 1 ))
      else
        (( p_max = 1 ))
      fi

      for (( p = 1; p <= p_max; p++ ))
      do
        if [[ $th -eq 1 ]];
        then
          echo "============================== a.out n=$n m=$m p=$p s=$s ===============" | tee -a $log
          echo "### Run ${v} ./a.out $n $m $p $r $s" | tee -a $log
          output=$(${v} ./a.out $n $m $p $r $s 2>&1)
        else
          echo "============================== a.out n=$n m=$m s=$s ====================" | tee -a $log
          echo "### Run ${v} ./a.out $n $m $r $s" | tee -a $log
          output=$(${v} ./a.out $n $m $r $s 2>&1)
        fi

        echo "${output}" >> $log

        result=$(echo "${output}" | grep ': Task')

        if [[ "${result}" == "" ]];
        then
          echo "ERROR: NO RESULT"
          exit 1
        fi

        echo "${result}"

        r1=$(echo "${result}" | cut -d ' ' -f 8 | sed -E 's/([+-]?[0-9.]+)[eE]\+?(-?)([0-9]+)/(\1*10^\2\3)/g')
        r2=$(echo "${result}" | cut -d ' ' -f 11 | sed -E 's/([+-]?[0-9.]+)[eE]\+?(-?)([0-9]+)/(\1*10^\2\3)/g')

        if [[ $s -eq 4 ]];
        then
          if [[ $n -eq $hilbert_n_test ]];
          then
            if [[ $(echo "${r1} >= 0.0" | bc -l) -eq 1 && $(echo "${r1} < ${min_r4}" | bc -l) -eq 1 ]];
            then
              min_r4=${r1}
            fi

            if [[ $(echo "${r2} >= 0.0" | bc -l) -eq 1 && $(echo "${r2} < ${min_r4}" | bc -l) -eq 1 ]];
            then
              min_r4=${r2}
            fi
          fi
        else
          if [[ $(echo "${r1} < 0.0" | bc -l) -eq 1 || $(echo "${r1} > ${max_r}" | bc -l) -eq 1
                || $(echo "${r2} < 0.0" | bc -l) -eq 1 || $(echo "${r2} > ${max_r}" | bc -l) -eq 1 ]];
          then
            echo "ERROR: WRONG ANSWER"
            exit 2
          fi
        fi
      done
    done
  done
done

echo "########################################################################" | tee -a $log

if [[ $(echo "${min_r4} > 0.0" | bc -l) -eq 1 && $(echo "${min_r4} < 1.0" | bc -l) -eq 1 ]];
then
  echo "HILBERT: SUCCESS"
else
  echo "HILBERT: FAILED"
fi

end_time=$(date +%s)
echo "SCRIPT ELAPSED: $((end_time - start_time)) SEC"
