CC=gcc
GRUPO=G3
NTAR=3

SRC_DIR=src
OBJ_DIR=build/obj
BIN_DIR=build/bin
BUILD_DIR=build
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=
LIBS=

CFLAGS=-Wall -Wextra -Wpedantic -O3 -MMD -MP
LDFLAGS=-Wall -lm
IS_MSYS2 := $(findstring MSYS,$(MSYSTEM))$(findstring MINGW,$(MSYSTEM))

# Detectar OS
ifeq ($(OS),Windows_NT)
# --- WINDOWS ---
ifeq ($(IS_MSYS2),)
# Windows CMD nativo
EXEC=pvv.exe
SHELL := cmd.exe
RM = del /Q
RMDIR = rmdir /S /Q
MKDIR = if not exist
SEP = \\
RUNNER = CMD
else
# MSYS2 / MinGW
EXEC=pvv.exe
SHELL := /usr/bin/bash
RM = rm -f
RMDIR = rm -rf
MKDIR = mkdir -p
SEP = /
RUNNER = MSYS
endif
else
# --- LINUX / MAC ---
EXEC=pvv
SHELL := /bin/sh
RM = rm -f
RMDIR = rm -rf
MKDIR = mkdir -p
SEP = /
RUNNER = UNIX
endif

.PHONY: all

all: folders $(BIN_DIR)/$(EXEC)
	@echo "Proyecto compilado y ejecutado correctamente."

$(BIN_DIR)/$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@ $(INCLUDE) $(LIBS) $(LDFLAGS)
	@echo "Ejecutable generado: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | folders
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@echo "Compilado: $< -> $@"


.PHONY: folders
folders:
ifeq ($(OS),Windows_NT)
ifeq ($(IS_MSYS2),)
	@if not exist "$(SRC_DIR)" mkdir "$(SRC_DIR)"
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
else
	@mkdir -p $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR)
endif
else
	@mkdir -p $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR)
endif
	@echo "Directorios creados."

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
ifeq ($(IS_MSYS2),)
# --- Windows CMD ---
	-$(RM) "$(OBJ_DIR)$(SEP)*.o" >nul 2>&1 || true
	-$(RM) "$(BIN_DIR)$(SEP)$(EXEC)" >nul 2>&1 || true
	@if exist "$(BIN_DIR)" rmdir /S /Q "$(BIN_DIR)" >nul 2>&1 || true
	@if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)" >nul 2>&1 || true
else
# --- MSYS2 / MinGW ---
	-$(RM) $(OBJ_FILES)
	-$(RM) $(BIN_DIR)/$(EXEC)
	-$(RMDIR) $(OBJ_DIR) || true
	-$(RMDIR) $(BIN_DIR) || true
	-$(RMDIR) $(BUILD_DIR) || true
endif
else
# --- Linux / Mac ---
	-$(RM) $(OBJ_FILES)
	-$(RM) $(BIN_DIR)/$(EXEC)
	-$(RMDIR) $(OBJ_DIR) || true
	-$(RMDIR) $(BIN_DIR) || true
	-$(RMDIR) $(BUILD_DIR) || true
endif
	@echo "Archivos compilados eliminados."

.PHONY: run
run: 
ifeq ($(RUNNER),CMD)
# Ejecutar en Windows CMD con rutas convertidas
	@cmd /C "$(subst /,\\,$(BIN_DIR))\\$(EXEC)"
else
# Ejecutar normalmente en MSYS2 / Linux / Mac
	@$(BIN_DIR)/$(EXEC)
endif

-include $(OBJ_FILES:.o=.d)