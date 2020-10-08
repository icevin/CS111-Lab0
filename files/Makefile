default: 
	gcc -g -Wall -Wextra main.c -o lab0

dist: lab0.tgz
submission_files = main.c Makefile backtrace.png breakpoint.png README

lab0.tgz: $(submission_files)
	tar -czf lab0-805419480.tar.gz $(submission_files)

.PHONY: default dist clean

clean:
	rm -f *.tgz lab0
