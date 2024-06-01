# SPDX-FileCopyrightText: © 2022 Uri Shaked <uri@wokwi.com>
# SPDX-License-Identifier: MIT

SOURCES = src/main.c
TARGET  = dist/chip.wasm
testFileName = logic 
testFolderName = test

.PHONY: all
all: $(TARGET) dist/chip.json ; \

.PHONY: clean
clean:
		rm -rf dist ; \
		rm -rf $(testFolderName) ; \
		

dist:
		mkdir -p dist

$(TARGET): dist $(SOURCES) src/wokwi-api.h
	  clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles -Wl,--import-memory -Wl,--export-table -Wl,--no-entry -Werror -o $(TARGET) $(SOURCES)

dist/chip.json: dist chip.json
	  cp chip.json dist


.PHONY: test
test:
	mkdir -p $(testFolderName) ; \
	cd test && arduino-cli sketch new $(testFileName)   && arduino-cli compile -e -b arduino:avr:uno $(testFileName) ; \