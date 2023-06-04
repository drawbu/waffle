START_TIME := $(shell date +%s%3N)
BUILD_DIR := .build

NAME := wm
NAME_DEBUG := debug
NAME_ANGRY := angry

# ↓ Sources

VPATH := src
SRC += main.c

# ↓ Debug only sources

DSRC := $(SRC)
ASRC := $(SRC)

# ↓ Config
CC := cc

CFLAGS := -W -Wall -Wextra
CFLAGS := -O2 -march=native
CFLAGS += -iquote ./include
CFLAGS += -U_FORTIFY_SOURCE

LDLIBS := -lXft -lX11 -lXcursor -lXft -lfontconfig

# ↓ Recipes
OBJ := $(SRC:%.c=$(BUILD_DIR)/release/%.o)
OBJ_DEBUG := $(DSRC:%.c=$(BUILD_DIR)/debug/%.o)
OBJ_ANGRY := $(DSRC:%.c=$(BUILD_DIR)/angry/%.o)

NAMES += $(NAME)
NAMES += $(NAME_DEBUG)
NAMES += $(NAME_ANGRY)

OBJS += $(OBJ)
OBJS += $(OBJ_DEBUG)
OBJS += $(OBJ_ANGRY)
OBJS += $(OBJ_TESTS)

# ↓ clean & fclean helpers
CLEAN := $(OBJS)
FCLEAN := $(NAMES) $(BUILD_DIR)

ifeq ($(FORCE_DEBUG),1)
    CFLAGS += -D DEBUG_MODE
endif

# ↓ `touch .fast` to force multi-threading
ifneq ($(shell find . -name ".fast"),)
    MAKEFLAGS += -j
endif

# ↓ Quiet
V ?=
ifeq ($(V), 1)
    $(info Verbose mode enabled)
    Q :=
else
    Q := @
endif

CMD_NOT_FOUND = $(error $(strip $(1)) is required for this rule)
CHECK_CMD = $(if $(shell command -v $(1)),, $(call CMD_NOT_FOUND, $(1)))

# ↓ Utils
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

_SOLVE_COLORS = $(subst :r,$(C_RED), \
    $(subst :c,$(C_CYAN),            \
    $(subst :p,$(C_PURPLE),          \
    $(subst :y,$(C_YELLOW),          \
    $(subst :b,$(C_BLUE),            \
    $(subst :g,$(C_GREEN),           \
    $(subst *,$(C_BOLD),             \
    $(subst ~,$(C_RESET),            \
    $(addprefix $(call _UNQUOTE,$(1)),~))))))))) # Do you like lisp?

## ↓ Hack to make highlighter happy
_UNQUOTE = $(subst ",,$(subst ',,$(1)))##")
_QUOTE = "$(strip $(1))"

COLORIZE = $(call _QUOTE, $(call _SOLVE_COLORS, $(1)))

CURRENT_TIME_MS = $(shell date +%s%3N)
TIME = $(shell expr $(call CURRENT_TIME_MS) - $(START_TIME))

HEADER := ":p"
LOG = @ echo -e $(call COLORIZE,$(2) ~$(TIME_BOX) $(HEADER)~ $(1)~)
TIME_BOX = "[ :b"$(call TIME)"~\t]"

all: $(NAME)

.PHONY: all

$(NAME): $(OBJ)
	$Q $(CC) $(CFLAGS) $(LIBFLAGS) $(LDLIBS) -o $@ $^
	$(call LOG,":g$@")

$(BUILD_DIR)/release/%.o: HEADER += "release"
$(BUILD_DIR)/release/%.o: %.c
	@ mkdir -p $(dir $@)
	$Q $(CC) $(CFLAGS) -c $< -o $@
	$(call LOG, ":c" $(notdir $@))

$(NAME_DEBUG): CFLAGS += -g3 -D DEBUG_MODE
$(NAME_DEBUG): HEADER += "debug"
$(NAME_DEBUG): $(OBJ_DEBUG)
	$Q $(CC) $(CFLAGS) $(LIBFLAGS) $(LDLIBS) -o $@ $^
	$(call LOG,":g$@")

$(BUILD_DIR)/debug/%.o: %.c
	@ mkdir -p $(dir $@)
	$Q $(CC) $(CFLAGS) -c $< -o $@
	$(call LOG, ":c" $(notdir $@))

$(NAME_ANGRY): CFLAGS += -D DEBUG_MODE
$(NAME_ANGRY): CFLAGS += -g3 -fsanitize=address,leak,undefined
$(NAME_ANGRY): LDFLAGS += -lasan
$(NAME_ANGRY): HEADER += "angry"
$(NAME_ANGRY): $(OBJ_ANGRY)
	$Q $(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	$(call LOG,":g$@")

$(BUILD_DIR)/angry/%.o: %.c
	@ mkdir -p $(dir $@)
	$Q $(CC) $(CFLAGS) -c $< -o $@
	$(call LOG, ":c" $(notdir $@))

clean:
	$(call _FIND_LOG, $(CLEAN))

	$Q $(RM) $(CLEAN)
	$Q $(RM) -f vgcore.*

fclean:
	$(call _FIND_LOG, $(FCLEAN))

	$Q $(RM) -f vgcore.*
	$Q $(RM) -r $(FCLEAN)

_FIND_LOG = $(if                                             \
	$(shell find $(1) 2> /dev/null),                         \
	$(call LOG,"Removed:r" $(shell find $(1) 2> /dev/null)), \
)

re:	fclean
	@ make -sC . all

%:
	$(call SENTINEL, $@)

%.c:
	$(call SENTINEL, $@)
