TARGETS = expr

all: $(TARGETS)

expr: expr.c expr.h

clean:
	$(RM) $(TARGETS)

.PHONY: all clean
