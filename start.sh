#!/bin/sh

sh MakeFile

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

########################
mkdir memory_Result
echo "Memory_Benchmark: "
./memory_benchmark >>memory.txt
mv memory.txt memory_Result

########################
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

######################
mkdir network_Result
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

            mv network"_"$connect"_"$packet"_"$thread.txt network_Result
        done
    done
done

########################
mkdir GPU_Result
for i in 1 2 3
do
    echo GPU_Benchmark$i:
    ./gpu_benchmark >>gpu_result$i.txt
    mv gpu_result$i.txt GPU_Result
done

rm cpu_benchmark disk_benchmark memory_benchmark client_benchmark server_benchmark gpu_benchmark test.bin
