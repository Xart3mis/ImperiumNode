all:
	pio -f -c vim run

upload:
	pio -f -c vim run --target upload

clean:
	pio -f -c vim run --target clean

program:
	pio -f -c vim run --target program

uploadfs:
	pio -f -c vim run --target uploadfs

update:
	pio -f -c vim update

monitor:
	pio device monitor

__sleep__:
	sleep 1

upload_and_monitor: upload __sleep__ monitor

erase_flash:
	python -m esptool erase_flash

compiledb:
	pio run -t compiledb
