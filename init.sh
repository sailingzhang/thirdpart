if [ ${1} -eq 1 ];then
cat libtensorflow1.15/lib/linux_cpu/libtensorflow.so.1.* > libtensorflow1.15/lib/linux_cpu/libtensorflow.so.1
fi


if [ ${1} -eq 0 ];then
	rm -rf  libtensorflow1.15/lib/linux_cpu/libtensorflow.so.1
fi

