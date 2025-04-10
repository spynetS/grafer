##
# Project Title
#
# @file
# @version 0.1

c_files = $(filter-out ./src/tests.c, $(wildcard ./src/*.c))
c_test_files = $(filter-out ./src/main.c, $(wildcard ./src/*.c))

grafer : $(c_files)
	cc -o grafer $(c_files) -lm

test:
	cc -o test-grafer $(c_test_files) -lm
	./test-grafer

run: grafer
	./grafer





# end
