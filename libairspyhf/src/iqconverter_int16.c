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

#include "iqconverter_int16.h"

void iqconverter_int16_process(iqconverter_int16_t *cnv, int16_t *src, int16_t *dest, int len)
{
    int iMin = 0;
    int iMax = 0;
    int qMin = 0;
    int qMax = 0;
    double count;
    int io = 0;
    int qo = 0;
    int16_t iCorr = (int16_t) cnv->iOffset;
    int16_t qCorr = (int16_t) cnv->qOffset;
    int i;

    // find value ranges for both I and Q
    // both intervals should be same same size (for a perfect circle)
    for (i = 0; i < len-1; i += 2)
    {
        // IQ imbalance
        if (i != 0)
        {
            if (src[i+1] < iMin) {
                iMin = src[i+1];
            } else if (src[i+1] > iMax) {
                iMax = src[i+1];
            }

            if (src[i] < qMin) {
                qMin = src[i];
            } else if (src[i] > qMax) {
                qMax = src[i];
            }
        }
        else
        {
            iMin = src[i+1];
            iMax = src[i+1];
            qMin = src[i];
            qMax = src[i];
        }

        // DC removal
        io += src[i+1];
        qo += src[i];
    }

    // sliding average (el cheapo again) for IQ imbalance
    cnv->iRange = (cnv->iRange * 15 + (iMax - iMin)) >> 4;
    cnv->qRange = (cnv->qRange * 15 + (qMax - qMin)) >> 4;

    // calculate IQ imbalance as Q15.16
    if (cnv->qRange != 0) {
        cnv->imbalance = ((uint32_t) cnv->iRange << 16) / (uint32_t) cnv->qRange;
    }

    // moving average for DC removal
    count = len/2;
    cnv->iOffset = (15.0 * cnv->iOffset + (double)io / count) / 16.0;
    cnv->qOffset = (15.0 * cnv->qOffset + (double)qo / count) / 16.0;

    // correct DC offset and IQ imbalance and convert back to signed int 16 with the I/Q inversion
    for (i = 0; i < len-1; i += 2)
    {
        dest[i] = src[i+1] - iCorr;
        dest[i+1] = ((src[i] * cnv->imbalance) >> 16) - qCorr;
    }
}

