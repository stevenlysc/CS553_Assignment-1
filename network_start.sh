gcc client_benchmark.c -pthread -o client_benchmark

gcc server_benchmark.c -pthread -o server_benchmark

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

rm client_benchmark server_benchmark
