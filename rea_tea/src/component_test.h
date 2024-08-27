#ifndef COMPONENT_TEST_H_
#define COMPONENT_TEST_H_

#include "anemometer.h"
#include "SdFat.h"
#include "timerrtc.h"
#include "relay.h"
#include "mks_mfc.h"
#include "l298n.h"
#define SPI_SPEED SD_SCK_MHZ(4)
//void anem_test();
void sd_test();
void rtc_test();
void mfc_test();
void relay_test(int start, int end);
void pump_test(int in1, int in2, int enA = -1);
void stream_processing_test();

#endif //COMPONENT_TEST_H_