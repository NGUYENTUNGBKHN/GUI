# setup direction 
PROJ_DIR=./
OUT_DIR=bin
OUT_OBJ_FILE=bin/obj

TARGET=gui
# phony
.PHONY:		all clean

# library
INC_DIR=./
INC_LIB=./lib/inc
INC_GUI=./lib/inc/gui
INC_IMPL=./lib/inc/gui/depend_impl
INC_FREETYPE=/usr/local/include/freetype2
INC_FOLDERS = \
	-I$(INC_DIR)\
	-I$(INC_GUI)\
	-I$(INC_LIB)\
	-I$(INC_IMPL)\
	-I$(INC_FREETYPE)
# source
# SRC_FILE = \
# 	main.cpp

SRC_FILE = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp)


# flag
CPP_FLAG += -lX11 -lm 	# X11 library
CPP_FLAG += -O2			# Optimize
CPP_FLAG += -lfreetype
CPP_FLAG += -lpng16
CPP_FLAG += -MMD -MP -MF"$(@:%.opp=%.d)" # dependency
# Toolchain
CXX=$(GUI_CXX)
CC=$(GUI_CC)

# obj file
OBJSCPP = $(patsubst %.cpp, $(OUT_OBJ_FILE)/%.opp,$(SRC_FILE))

TMPOBJCPP+=$(OUT_OBJ_FILE)/__tmp__port_linux_xwindow.opp

$(START):
	@echo "*** GUI_APP compile start ***"

all: $(START) $(TARGET)


$(TARGET): $(OBJSCPP) $(TMPOBJCPP)
	@echo "LINK $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) -o $(OUT_DIR)/$@ $^ $(CPP_FLAG)


$(OUT_OBJ_FILE)/%.opp: %.cpp Makefile
	@echo "CXX $<"
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CPP_FLAG) $(INC_FOLDERS) -c $< -o $@


$(OUT_OBJ_FILE)/__tmp__port_linux_xwindow.opp: ./lib/inc/gui/depend_impl/port_linux_xwindow.cpp
	@echo "CXX $<"
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CPP_FLAG) $(INC_FOLDERS) -c $< -o $@

clean: $(OUT_DIR)
	@echo "remove bin"
	@rm -r $<


-include $(wildcard $(OUT_OBJ_FILE)/*.d)
-include $(wildcard $(OUT_OBJ_FILE)/*/*.d)
-include $(wildcard $(OUT_OBJ_FILE)/*/*/*.d)
-include $(wildcard $(OUT_OBJ_FILE)/*/*/*/*.d)
-include $(wildcard $(OUT_OBJ_FILE)/*/*/*/*/*.d)

