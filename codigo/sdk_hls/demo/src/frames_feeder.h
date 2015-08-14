/*
 * frames_feeder.h
 *
 *  Created on: Aug 1, 2015
 *      Author: miguel
 */

#ifndef FRAMES_FEEDER_H_
#define FRAMES_FEEDER_H_

void feederReleaseFrame(u32 frameIdx);
int feederGetCompleteFrame();
u32 *feederGetFramePtr(u32 frameIdx);


/* defined by each RAW mode application */
void print_app_header();
int start_application();
int transfer_data();

#endif /* FRAMES_FEEDER_H_ */
