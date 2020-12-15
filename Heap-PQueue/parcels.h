// parcels.h
/////////////
#ifndef PARCELS_H
#define PARCELS_H

#include "parcel.h"
#include "pqueue.h"

///////////////////////////////////
// Public Interface: Parcels API
///////////////////////////////////

int get_parcel(PQueue* parcels, Parcel* parcel);

int put_parcel(PQueue* parcels, const Parcel* parcel);

#endif

