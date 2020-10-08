default: 
	@gcc -g -Wall -Wextra lab0.c -o lab0

dist: lab0.tgz
submission_files = lab0.c Makefile README backtrace.png breakpoint.png

lab0.tgz: $(submission_files)
	tar -czf lab0-805419480.tar.gz $(submission_files)

.PHONY: default dist clean

clean:
	rm -f *.tar.gz lab0 *.txt *.test

check: default
	@./smoke.sh 2>/dev/null
	@rm -f *.test
