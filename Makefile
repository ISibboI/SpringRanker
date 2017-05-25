DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CC=g++
CFLAGS=-Wall -Werror -Wstrict-aliasing=0 -std=c++14 -O3 -fopenmp
LDFLAGS=-fopenmp
DEBUGFLAGS=-fsanitize=address -fsanitize=undefined
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

OBJ=main.o simulator.o io.o

all: spring_ranker

spring_ranker: $(OBJ)
	$(CC) $(CFLAGS) -o spring_ranker $(OBJ) $(LDFLAGS)

debug: $(OBJ)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o spring_ranker $(OBJ) $(LDFLAGS)

%.o: %.cpp %.h $(DEPDIR)/%.d
%.o: %.cpp %.h
%.o: %.cpp $(DEPDIR)/%.d
%.o: %.cpp
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(DEPDIR)/*.d)

.PHONY: clean
clean:
	rm -rf $(OBJ) $(DEPDIR) value_iteration q_learning
