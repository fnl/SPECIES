CC     = g++
CFLAGS = -fpic -pthread -Wall -O3
LFLAGS = -fpic -shared -lboost_regex

all: organisms species

clean:
	rm -f organisms species

%: %.cxx acronyms.h document.h file.h hash.h mutex.h match_handlers.h base_handlers.h batch_tagger.h tagger.h tagger_core.h tagger_types.h tightvector.h tokens.h
	$(CC) $(CFLAGS) -o $@ $< -lm -lboost_regex
