#include "../include/main.h"

void play_pluck()
{
    ao_device *device;
	ao_sample_format format;
	int default_driver;
	char *buffer;
	int buf_size;
	int sample;
	float freq = 300.0;
	int i;

	/* -- Setup for default driver -- */
	default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 2;
	format.rate = 44100 * 2;
	format.byte_format = AO_FMT_LITTLE;

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		return;
	}

	/* -- Play some stuff -- */
	buf_size = format.bits/8 * format.channels * format.rate;
	buffer = calloc(buf_size,
			sizeof(char));
    float faided = 0.03;
	for (i = 0; i < format.rate; i++) {
		sample = (int)(faided * 32768.0 *
			sin(2 * M_PI * freq * ((float) i/format.rate)));

		/* Put the same stuff in left and right channel */
		buffer[4*i] = buffer[4*i+2] = sample & 0xff;
		buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
        faided *= 0.999;
		freq-=0.001;
	}
	ao_play(device, buffer, buf_size);

	free(buffer);

	/* -- Close and shutdown -- */
	ao_close(device);
}

void * play_pluck_th(void * pv)
{
	play_pluck();
	pthread_exit(NULL);

	return pv;
}