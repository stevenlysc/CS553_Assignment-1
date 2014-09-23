*****************************************************************************
*                                                                           * 
* CS553 Assignment#1 ———— Benchmarks for CPU, GPU, Memory, Disk and Network *
*                                                                           *
*****************************************************************************

————————————
Description:
————————————
    
    This code is used to benchmark different parts of a computer system, from CPU, GPU, Memory, Disk, Network.


——————————————————————————————————
Files included in this assignment:
——————————————————————————————————

	cpu_benchmark.c		disk_benchmark.c	 memory_benchmark.c
	gpu_benchmark.cu	client_benchmark.c	 server_benchmark.c
	MakeAll			runscripts.sh		 Result


——————————————————————————
How to run the benchmarks:
——————————————————————————

    First of all, all codes are supposed to be compiled and run under Mac OS or Linux.

    CPU Benchmark:
	First, locate to the folder in Terminal;
	Second, compile the source code using command:
		gcc cpu_benchmark.c -pthread -o cpu_benchmark
	Then, run the UNIX executable file cpu_benchmark using command:
		./cpu_benchmark <operation type> <thread number>
	In which:
		Operation type: flops / iops
		thread number:  1 / 2 / 4 / 8

    GPU Benchmark:
	First, locate to the folder in Terminal;
	Second, compile the source code using command:
		nvcc gpu_benchmark.cu -o gpu_benchmark
	Then, run the UNIX executable file gpu_benchmark using command:
		./gpu_benchmark
	Notification: this benchmark can only be executed on the computer which has NVIDIA GPU and CUDA supportive.

    Memory Benchmark:
	First, locate to the folder in Terminal;
	Second, compile the source code using command:
		gcc memory_benchmark.c -pthread -o memory_benchmark (for Mac)
	   or   gcc memory_benchmark.c -pthread -lm -o memory_benchmark (for Linux)
	Then, run the UNIX executable file memory_benchmark using command:
		./memory_benchmark

    Disk Benchmark:
	First, locate to the folder in Terminal;
	Second, compile the source code using command:
		gcc memory_benchmark.c -pthread -o disk_benchmark (for Mac)
	   or   gcc memory_benchmark.c -pthread -lm -o disk_benchmark (for Linux)
	Then, run the UNIX executable file disk_benchmark using command:
		./disk_benchmark <operation type> <access type> <block_size> <thread number>
	In which:
		Operation type: Write / Read
		Access type:    seq / ran
		Block_size:     1B / 1KB / 1MB
		Thread number:  1 / 2 / 4
	Warning: Do the writing operations before reading operations, otherwise the program cannot be executed because no file can be read.

    Network Benchmark:
	First, locate to the folder in Terminal;
	Second, compile the two source code (client_benchmark.c and server_benchmark.c) separately using command:
		gcc client_benchmark.c -pthread -o client_benchmark
	  and   gcc server_benchmark.c -pthread -o server_benchmark
	Then, run the UNIX executable file server_benchmark and client_benchmark, make sure you run the sever first, by using command:
		./server_benchmark <connection type> <buffer size> <thread number>
		./client_benchmark <connection type> <buffer size> <thread number>
	In which:
		Connection type: TCP / UDP
		Buffer size:     1 / 1024 / 65536
		Thread number:   1 / 2

    And we also provide a much easier and more friendly way to execute all the benchmark with only one command, instead of running the benchmarks one by one.
	First, locate to the folder in Terminal;
	Second, run the bash script file runscripts.sh using command:
		sh runscripts.sh
	Then, everything is done automatically. And a result folder will be generated for every benchmark.
 
   
——————————————————————————
Results of the benchmarks:
——————————————————————————

    If you run the benchmarks by bash script file, five result folders will be generated after the execution. In each folder, there will be one or more TXT files indicate the result of each condition.
    If you choose to run each benchmark by compiling by yourself, the result will be printed out on the screen.


————————————————————
Author of the code:
————————————————————
These benchmarks are written by Xingtan Hu, Boyang Li and Zichen Zhang.


