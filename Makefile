CC=g++
DEBUG=
CXXFLAGS=
CPPFLAGS=$(DEBUG) -O2
LDFLAGS=$(DEBUG)

PROJECT := zork
SRCDIR  := src
SRCS    := $(wildcard $(SRCDIR)/*.cc)
OBJDIR  := objdir
OBJS    := $(addprefix $(OBJDIR)/, \
                       $(patsubst %.cc,%.o, $(notdir $(SRCS)) ))


final: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -MMD -c $< -o $@
	@cd $(OBJDIR); cp $*.d $*.P; \
	    sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	    rm -f $*.d

-include $(SRCS:$(SRCDIR)/%.cc=$(OBJDIR)/%.P)


$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -fr $(OBJDIR)
	rm $(PROJECT)

