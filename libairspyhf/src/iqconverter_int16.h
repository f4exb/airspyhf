///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef LIBAIRSPYHF_SRC_IQCONVERTER_INT16_H_
#define LIBAIRSPYHF_SRC_IQCONVERTER_INT16_H_

#include <stdint.h>

extern const int ncoTableSize;

typedef struct {
    int32_t iRange;
    int32_t qRange;
    int32_t imbalance;
    double iOffset;
    double qOffset;
    int16_t *ncoTable;
    int ncoPhase;
    int ncoPhaseIncrement;
    int32_t freq_shift; //!< frequency shift in Hz from nearest kHz used to drive NCO
} iqconverter_int16_t;

/**
 * len is in number of int16_t samples
 */
void iqconverter_int16_process(iqconverter_int16_t *cnv, int16_t *src, int16_t *dest, int len);


#endif /* LIBAIRSPYHF_SRC_IQCONVERTER_INT16_H_ */
