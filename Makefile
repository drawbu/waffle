.POSIX:
.SUFFIXES: .d

# ↓ `touch .fast` to force multi-threading
ifneq ($(shell find . -name ".fast"),)
    MAKEFLAGS += -j
endif

RM ?= rm -f
AR ?= ar

ifneq ($(shell command -v tput),)
    ifneq ($(shell tput colors),0)

C_RESET := \033[00m
C_BOLD := \e[1m
C_RED := \e[31m
C_GREEN := \e[32m
C_YELLOW := \e[33m
C_BLUE := \e[34m
C_PURPLE := \e[35m
C_CYAN := \e[36m

    endif
endif

NOW = $(shell date +%s%3N)
STIME := $(call NOW)

TIME_NS = $(shell expr $(call NOW) - $(STIME))
TIME_MS = $(shell expr $(call TIME_NS))
LOG_TIME = printf "[$(C_BLUE) %6s $(C_RESET)] %b\n" "$(call TIME_MS)"

BUILD_DIR := .build
OUT := waffle
OUT_DEBUG := debug

CC := gcc

CFLAGS += -std=gnu11
CFLAGS += -pedantic
CFLAGS += -Wp,-U_FORTIFY_SOURCE
CFLAGS += -Wformat=2
CFLAGS += -Wformat-security

CFLAGS += -MMD -MP
CFLAGS += -fanalyzer
CFLAGS += -fno-builtin
CFLAGS += -pipe

CFLAGS += -O2 -march=native -mtune=native

CFLAGS += -Wall
CFLAGS += -Wcast-qual
CFLAGS += -Wconversion
CFLAGS += -Wdisabled-optimization
CFLAGS += -Wduplicated-branches
CFLAGS += -Wduplicated-cond
CFLAGS += -Werror=return-type
CFLAGS += -Wextra
CFLAGS += -Winit-self
CFLAGS += -Winline
CFLAGS += -Wlogical-op
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wredundant-decls
CFLAGS += -Wshadow
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wsuggest-attribute=pure
CFLAGS += -Wundef
CFLAGS += -Wunreachable-code
CFLAGS += -Wwrite-strings
CFLAGS += -iquote ./include

LDFLAGS += -fwhole-program

LDLIBS += -lX11
LDLIBS += -lXcursor
LDLIBS += -lXft
LDLIBS += -lXft
LDLIBS += -lfontconfig

VPATH := src
SRC += main.c
SRC += events.c
SRC += events_debug.c
SRC += cursor_events.c
SRC += grab.c
SRC += window_action.c

LIB := $(BUILD_DIR)/libquell.a

vpath %.c $(VPATH)

OBJ := $(SRC:%.c=$(BUILD_DIR)/release/%.o)
OBJ_DEBUG := $(subst release,debug,$(OBJ))

DEP := $(OBJ:.o=.d)
DEP_DEBUG := $(OBJ_DEBUG:.o=.d)

all: $(OUT)
.PHONY: all

$(BUILD_DIR):
$(BUILD_DIR)/debug $(BUILD_DIR)/release: $(BUILD_DIR)
	mkdir -p $@

$(OUT_DEBUG): CFLAGS += -g3
$(OUT_DEBUG): CFLAGS += -D DEBUG_MODE=1
$(OUT_DEBUG): $(OBJ_DEBUG)
	$(CC) -o $@ $(CFLAGS) $(OBJ_DEBUG) $(LDFLAGS) $(LDLIBS)
	@ $(LOG_TIME) $@

$(OUT): $(OBJ)
	@ $(CC) -o $@ $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS)
	@ $(LOG_TIME) "LD $(C_YELLOW) $@ $(C_RESET)"

$(BUILD_DIR)/release/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -o $@ -c $< || exit 1
	@ $(LOG_TIME) "CC $(C_CYAN) $(notdir $@) $(C_RESET)"

$(BUILD_DIR)/debug/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -o $@ -c $< || exit 1
	@ $(LOG_TIME) "CC $(C_CYAN) $(notdir $@) $(C_RESET)"

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) -r $(BUILD_DIR) $(OUT) $(OUT_DEBUG)

.PHONY: clean fclean

re: fclean
	@ $(MAKE) all

.PHONY: re

-include $(DEP)
-include $(DEP_DEBUG)
