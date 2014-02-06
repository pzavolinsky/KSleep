# The MIT License (MIT)
# 
# Copyright (c) 2014 Patricio Zavolinsky
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 


###############################################################################
# Config:
MOCS        := src/KSleep.moc
SOURCES     := src/main.cpp src/KSleep.cpp

.PHONY: all clean install uninstall
all: bin/ksleepd

###############################################################################
# Dependencies:
src/KSleep.cpp    : src/KSleep.h     ; @touch $@
src/main.cpp      : src/KSleep.h     ; @touch $@
$(MOCS:.moc=.cpp) : %.cpp : %.moc

###############################################################################
# Automagic variables:
QTMODULES :=Core Gui DBus
ifneq ($(shell uname -m | grep x86_64),)
QTDIR     :=/usr/lib64/qt/
else
QTDIR     :=/usr/lib/qt/
endif
OBJECTS  := $(patsubst src/%,obj/%,$(SOURCES:.cpp=.o)) 
CXXFLAGS := $(CXXFLAGS) -DQT_THREAD_SUPPORT  -D_REENTRANT -O2 -fno-exceptions \
                        -fno-check-new -fno-common                            \
                        $(patsubst %,-I $(QTDIR)/include/Qt%,$(QTMODULES))
LDFLAGS  := $(LDFLAGS)  -lkdeui -lkdecore -L $(QTDIR)/lib                               \
                        $(patsubst %,-lQt%,$(QTMODULES))
PRINT     = printf '%-10s %s\n' '[$(1)]' '$(2)' 

###############################################################################
# Installed tools:
MOC         := $(shell which moc)

bin/ksleepd: $(OBJECTS)
	@$(call PRINT,LD,$@)
	@mkdir -p bin
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

obj/%.o : src/%.cpp
	@$(call PRINT,CPP,$<)
	@mkdir -p obj
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(MOCS) : %.moc : %.h
	@$(call PRINT,MOC,$<)
	@$(MOC) $< -o $@
	

clean:
	rm -rf bin obj $(MOCS) src/.deps

define COPY
	$(call PRINT,COPY,$(1) to $(2))
	mkdir -p $(dir $(2))
	cp --remove-destination $(1) $(2)
endef
define REMOVE
	$(call PRINT,RM,$(1))
	rm -rf $(1)
endef

install:
	@$(call COPY,config/*,~/.ksleep/)
	@$(call COPY,bin/ksleepd,~/.kde/bin/)
	@$(call COPY,desktop/ksleep.desktop,~/.kde/Autostart/)

uninstall:
	@$(call REMOVE,~/.ksleep)
	@$(call REMOVE,~/.kde/bin/ksleepd)
	@$(call REMOVE,~/.kde/Autostart/ksleep.desktop)

	