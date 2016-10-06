#!/bin/bash

score=0
out=("goldout1" "goldout2" "goldout3")
tests=("test1" "test2" "test3")
time=(3 19 1000)

end=$((${#tests[@]}-1))
for i in $(seq 0 1 $end); do
  ./bin/game_of_life ${tests[i]} ${time[i]}
  diff -wb tests/${out[i]} output/${tests[i]} &> /dev/null
  res=$?

  if [[ $res -eq 0 ]]; then
    echo "Test" $((i + 1))": passed"
    ((score += 1))
  else
    echo "Test" $((i + 1))": failed"
  fi
done

echo "Score: "$score"/"${#tests[@]}
