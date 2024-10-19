PROJECT_NAME = s21_smartcalc
FLAGS = -Wall -Wextra -Werror -std=c11
CHECKFLAG = -lcheck
TEST_NAME = tests

all: create 
	cmake --build build --target all

$(PROJECT_NAME).a:
	gcc -c $(FLAGS) back/$(PROJECT_NAME).c
	ar rcs $(PROJECT_NAME).a $(PROJECT_NAME).o
	ranlib $(PROJECT_NAME).a

create: clean
	mkdir -p build
	echo "Create"
	cmake . -B build/ -D CMAKE_BUILD_TYPE=Debug

install: all
	-rm -rf app
	-mkdir app
ifeq ($(shell uname),Linux) # Linux
	cp -r build/s21_smartcalc app
endif
ifeq ($(shell uname),Darwin) # MacOS
	cp -r build/s21_smartcalc.app app
endif

uninstall:
	-rm -rf app

dist: clean
	-rm files.list
	find pwd -type f -not -name ".*" -mindepth 1 |cut -c `pwd|wc -c`- | cut -c 2- >> files.list
	find . -depth -type f -not -name ".*" -print | cpio -pdm $(PROJECT_NAME)
	tar -czvf $(PROJECT_NAME).tar.gz $(PROJECT_NAME)
	rm -rf $(PROJECT_NAME)
	-rm files.list

dvi:
	open doc.dvi

test: clean $(PROJECT_NAME).a
	checkmk $(TEST_NAME).check > $(TEST_NAME).c
	gcc $(FLAGS) $(CHECKFLAG) $(PROJECT_NAME).a $(TEST_NAME).c -o s21_test
	./s21_test
 
gcov_report: 
	gcc -c $(FLAGS) tests.c
	gcc $(FLAGS) --coverage $(CHECKFLAG) back/$(PROJECT_NAME).c tests.o -o s21_test
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open report/index.html

clean: uninstall
	rm -rf *.o *.a *.gcno *.gcda *.info main.dSYM CMakeLists.txt.user s21_smartcalc.tar.gz s21_test report *.out build tests.c 


clang: clean
	find . -type f -name ".c" -o -name ".h" -o -name "*.cc" | xargs clang-format -style=Google -n
