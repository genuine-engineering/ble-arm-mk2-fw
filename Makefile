THISDIR:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))

HEX_OUT ?= $(THISDIR)/build/nrf51dk-gcc/source/ble-arm-mk2-combined.hex
flash:
	openocd 	-c "source [find interface/stlink-v2.cfg]"\
							-c "source [find target/nrf51.cfg]"\
							-c init -c targets -c "reset halt" \
							-c "nrf51 mass_erase"\
			        -c "program $(HEX_OUT) verify reset exit"\
			        -c "reset run" -c shutdown