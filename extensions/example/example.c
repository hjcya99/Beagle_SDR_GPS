// Copyright (c) 2016 John Seamons, ZL/KF6VO

#include "example.h"

#ifndef EXT_EXAMPLE
	void example_main() {}
#else

#include "kiwi.h"
#include "ext.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <strings.h>

//#define EXAMPLE_DEBUG_MSG	true
#define EXAMPLE_DEBUG_MSG	false

// rx_chan is the receiver channel number we've been assigned, 0..RX_CHAN
// We need this so the extension can support multiple users, each with their own example[] data structure.

struct example_t {
	int rx_chan;
	
	int cmd, data;
};

static example_t example[RX_CHANS];

// tasks
// FFT

// messaging examples
//	if (ext_send_msg(e->rx_chan, EXAMPLE_DEBUG_MSG, "EXT example_xxx=1") < 0) {}
//	if (ext_send_encoded_msg(e->rx_chan, EXAMPLE_DEBUG_MSG, "EXT", "EXAMPLE_PEAKS", "%s", peaks_s) < 0) {}
//	if (ext_send_data_msg(e->rx_chan, EXAMPLE_DEBUG_MSG, EXAMPLE_DATA, ws, nbins_411+1) < 0) {}

void example_data(int rx_chan, int nsamps, TYPECPX *samps)
{
	example_t *e = &example[rx_chan];
	int i;

	if (1) {
		ext_unregister_receive_iq_samps(e->rx_chan);
		return;
	}
    for (i=0; i<nsamps; i++) {
		float re = (float) samps[i].re;
		float im = (float) samps[i].im;
    }
}

bool example_msgs(char *msg, int rx_chan)
{
	example_t *e = &example[rx_chan];
	int n;
	
	printf("### example_msgs RX%d <%s>\n", rx_chan, msg);
	
	if (strcmp(msg, "SET ext_init") == 0) {
		e->rx_chan = rx_chan;
		return true;
	}

	n = sscanf(msg, "SET cmd=%d data=%d", &e->cmd, &e->data);
	if (n == 2) {
		ext_register_receive_iq_samps(example_data, rx_chan);
		ext_unregister_receive_iq_samps(rx_chan);
		return true;
	}
	
	return false;
}

void example_main();

ext_t example_ext = {
	"example",
	example_main,
	example_msgs,
};

void example_main()
{
    double frate = ext_get_sample_rateHz();
    printf("### example_main audio sample rate = %.1f\n", frate);

	ext_register(&example_ext);
}

#endif
