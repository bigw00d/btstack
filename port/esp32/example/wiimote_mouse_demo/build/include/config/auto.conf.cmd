deps_config := \
	/home/daiki/esp/esp-idf/components/app_trace/Kconfig \
	/home/daiki/esp/esp-idf/components/aws_iot/Kconfig \
	/home/daiki/esp/esp-idf/components/bt/Kconfig \
	/home/daiki/esp/esp-idf/components/driver/Kconfig \
	/home/daiki/esp/esp-idf/components/efuse/Kconfig \
	/home/daiki/esp/esp-idf/components/esp32/Kconfig \
	/home/daiki/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/daiki/esp/esp-idf/components/esp_event/Kconfig \
	/home/daiki/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/daiki/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/daiki/esp/esp-idf/components/esp_https_ota/Kconfig \
	/home/daiki/esp/esp-idf/components/espcoredump/Kconfig \
	/home/daiki/esp/esp-idf/components/ethernet/Kconfig \
	/home/daiki/esp/esp-idf/components/fatfs/Kconfig \
	/home/daiki/esp/esp-idf/components/freemodbus/Kconfig \
	/home/daiki/esp/esp-idf/components/freertos/Kconfig \
	/home/daiki/esp/esp-idf/components/heap/Kconfig \
	/home/daiki/esp/esp-idf/components/libsodium/Kconfig \
	/home/daiki/esp/esp-idf/components/log/Kconfig \
	/home/daiki/esp/esp-idf/components/lwip/Kconfig \
	/home/daiki/esp/esp-idf/components/mbedtls/Kconfig \
	/home/daiki/esp/esp-idf/components/mdns/Kconfig \
	/home/daiki/esp/esp-idf/components/mqtt/Kconfig \
	/home/daiki/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/daiki/esp/esp-idf/components/openssl/Kconfig \
	/home/daiki/esp/esp-idf/components/pthread/Kconfig \
	/home/daiki/esp/esp-idf/components/spi_flash/Kconfig \
	/home/daiki/esp/esp-idf/components/spiffs/Kconfig \
	/home/daiki/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/daiki/esp/esp-idf/components/unity/Kconfig \
	/home/daiki/esp/esp-idf/components/vfs/Kconfig \
	/home/daiki/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/daiki/esp/esp-idf/components/app_update/Kconfig.projbuild \
	/home/daiki/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/daiki/esp/esp-idf/components/btstack/Kconfig.projbuild \
	/home/daiki/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/daiki/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/daiki/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
