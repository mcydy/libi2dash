// Privada. Función recoge h264 y obtiene sps y pps, rellena el context con width y height.
uint32_t nal_handler(byte *input_data, uint32_t size_input, i2ctx *context);
// Privada. Función recoge h264 y obtiene sps y pps, rellena el context con width y height.
uint8_t is_key_frame(byte *input_data, uint32_t size_input);
