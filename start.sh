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

mkdir memory_Result
echo "Memory_Benchmark: "
./memory_benchmark >>memory.txt
mv memory.txt memory_Result

mkdir disk_Result
for opt in Write Read
do
	for access in seq ran
	do
		for block in 1B 1KB 1MB
		do
			for thread in 1 2 4
			do
				echo "Disk_Benchmark: Operation type: "$opt", access type: "$access", block_size: "$block", thread number: "$thread
		
				./disk_benchmark $opt $access $block $thread >>disk"_"$opt"_"$access"_"$block"_"$thread.txt
				
				mv disk"_"$opt"_"$access"_"$block"_"$thread.txt disk_Result
			done
		done
	done
done


rm cpu_benchmark memory_benchmark disk_benchmark test.bin
