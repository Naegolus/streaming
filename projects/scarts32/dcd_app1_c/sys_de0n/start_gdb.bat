
@echo Content of gdb_init.txt
@cat gdb_init.txt
@echo.

scarts_32-none-eabi-gdb objs/dcd_app1.elf -b 115200 --directory=../SRC -x gdb_init.txt
