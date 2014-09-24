#!/bin/sh
sh MakeFile

rm -r Result
mkdir Result
######################
######################
mkdir Result/cpu_Result
for opt in flops iops
do
	for thread in 1 2 4 8
	do
		echo "CPU_Benchmark: Operation type: "$opt", thread number: "$thread
		
		./cpu_benchmark $opt $thread >>cpu"_"$opt"_"$thread.txt

		mv cpu"_"$opt"_"$thread.txt Result/cpu_Result
	done
done
echo "\n"

########################
mkdir Result/memory_Result

    echo "Memory_Benchmark$i: "
    ./memory_benchmark >>memory$i.txt
    mv memory$i.txt Result/memory_Result

echo "\n"

########################
mkdir Result/disk_Result
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
				
				mv disk"_"$opt"_"$access"_"$block"_"$thread.txt Result/disk_Result
			done
		done
	done
done
echo "\n"

######################
mkdir Result/network_Result
for connect in TCP UDP
do
    for packet in 1 1024 65536
    do
        for thread in 1 2
        do
            echo "Network_Benchmark: connect type: "$connect", packet size:"$packet	\
            ", thread number: "$thread

            ./server_benchmark $connect $packet $thread &
            ./client_benchmark $connect "127.0.0.1" $packet $thread >>	\
            network"_"$connect"_"$packet"_"$thread.txt

            mv network"_"$connect"_"$packet"_"$thread.txt Result/network_Result
        done
    done
done
echo "\n"

########################
mkdir Result/GPU_Result

    echo GPU_Benchmark$i:
    ./gpu_benchmark >>gpu_result$i.txt
    mv gpu_result$i.txt Result/GPU_Result

echo "\n"

rm cpu_benchmark disk_benchmark memory_benchmark client_benchmark server_benchmark gpu_benchmark test.bin
