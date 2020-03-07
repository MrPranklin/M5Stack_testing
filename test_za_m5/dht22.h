namespace dht22
{
void begin();

float read_temperature();

float read_humidity();

bool is_sensor_ready(long interval);
} // namespace dht22
