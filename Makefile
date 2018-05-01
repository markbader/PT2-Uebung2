TARGETS = expr

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)

.PHONY: all clean
