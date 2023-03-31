format:
	clang-format -i *.c *.cpp

git_avg:
	git add *
	git commit -m "avg_save"
	git push

all:
	g++ lab2.cpp -o a.out
	./a.out
	rm a.out

clean:
	rm a.out

update:
	git pull
	rm a.out

run_multicore_cpu:
	export PGI_ACC_TIME=1
	pgcc main.c -pg -ta=multicore -o file.out
	./file.out
	gprof file.out

run_gpu:
	export PGI_ACC_TIME=1
	pgcc main.c -acc -Minfo=accel -o file.out
	nsys profile -t cuda file.out
