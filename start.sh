sh makeAll
######################
######################
mkdir cpu_benchmark
for opt in flops iops
do
	for thread in 1 2 4 8
	do
		echo "Operation type: "$opt", thread number: "$thread
		
		./cpuben $opt $thread >>cpu"_"$opt"_"$thread.txt

		mv cpu"_"$opt"_"$thread.txt cpu_benchmark
	done
done

mkdir disk_benchmark
for opt in Write Read
do
	for access in seq ran
	do
		for block in 1B 1KB 1MB
		do
			for thread in 1 2 4
			do
				echo "Operation type: "$opt", access type: "$access", block_size: "$block", thread number: "$thread
		
				./diskben $opt $access $block $thread >>disk"_"$opt"_"$access"_"$block"_"$thread.txt
				
				mv disk"_"$opt"_"$access"_"$block"_"$thread.txt disk_benchmark
			done
		done
	done
done

rm cpuben diskben test.bin
