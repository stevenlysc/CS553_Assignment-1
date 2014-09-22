sh makeAll
######################
######################
mkdir cpu_Result
for opt in flops iops
do
for thread in 1 2 4 8
do
echo "CPU_Benchmark: Operation type: "$opt", thread number: "$thread

./cpu_benchmark $opt $thread >>cpu"_"$opt"_"$thread.txt

mv cpu"_"$opt"_"$thread.txt cpu_Result
done
done

rm cpu_benchmark memory_benchmark disk_benchmark test.bin