TARGET  = terminal
SRC_CC  = main.cc
LIBS    = base config
SRC_BIN = $(notdir $(wildcard $(PRG_DIR)/*.tff))
