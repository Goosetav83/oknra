CC = g++
CFLAGS = -O2 -pthread -g

# Uzycie: make FILE=normalize1
FILE ?= program
TARGET = $(FILE).out

PROGRAMS = normalize1.out normalize2.out normalize3.out normalize4.out normalize5.out normalize6.out

all: $(TARGET)

$(TARGET): $(FILE).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(FILE).cpp

normalize%.out: normalize%.cpp
	$(CC) $(CFLAGS) -o $@ $<

recompile: clean
	@for prog in $(PROGRAMS); do \
		$(CC) $(CFLAGS) -o $$prog $${prog%.out}.cpp; \
	done

# Uzycie: make test1K, make test4K, make test16K, make test64K
test%: $(PROGRAMS) FORCE
	@for prog in $(PROGRAMS); do \
		echo "=== $$prog ==="; \
		./$$prog test$*; \
	done

FORCE:

SIZES    = 16K 64K 256K 1M 4M
DATASETS = nonprintable regulartests repeatingword spaces

test_all: $(PROGRAMS) FORCE
	@mkdir -p results
	@for dataset in $(DATASETS); do \
		for size in $(SIZES); do \
			outfile="results/$${dataset}_$${size}.txt"; \
			echo "size=$${size}" > $$outfile; \
			for prog in $(PROGRAMS); do \
				t=$$(./$$prog $${dataset}/test$${size} 2>/dev/null); \
				echo "$${prog%.out} $$t" | tee -a $$outfile; \
			done; \
		done; \
	done

clean:
	rm -f $(TARGET) $(PROGRAMS) gmon.out profile_$(FILE).txt

.PHONY: all test clean

memtest: $(PROGRAMS) FORCE
	@for prog in $(PROGRAMS); do \
		printf "%-20s " "$${prog%.out}"; \
		/usr/bin/time -v ./$$prog regulartests/test1M 2>&1 | grep "Maximum resident" | awk '{print $$NF " KB"}'; \
	done
