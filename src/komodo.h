/******************************************************************************
 * Copyright © 2014-2016 The SuperNET Developers.                             *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#ifndef H_KOMODO_H
#define H_KOMODO_H

#include <stdint.h>
#include <stdio.h>

int32_t IS_KOMODO_NOTARY,USE_EXTERNAL_PUBKEY;
std::string NOTARY_PUBKEY;

int32_t komodo_checkmsg(void *bitcoinpeer,uint8_t *data,int32_t datalen)
{
    fprintf(stderr,"KOMODO.[%d] message from peer.%p\n",datalen,bitcoinpeer);
    return(0);
}

void komodo_connectblock(CBlock *block)
{
    // update voting results and official (height, notaries[])
}

int32_t komodo_is_notaryblock(CBlockHeader *blockhdr)
{
    // extract height from coinbase
    // extract miner's pubkey from vout[0]
    // compare against elected notary pubkeys as of height
    return(0);
}

int32_t komodo_blockhdrcheck(const CBlockHeader *blockhdr,uint32_t *nBitsp)
{
    if ( komodo_is_notaryblock((CBlockHeader *)blockhdr) != 0 )
        *nBitsp = KOMODO_MINDIFF_NBITS;
    // 1 -> valid notary block, change nBits to KOMODO_MINDIFF_NBITS
    // -1 -> invalid, ie, prior to notarized block
    return(0); // normal PoW block
}

int32_t komodo_blockindexcheck(CBlockIndex *pindex,uint32_t *nBitsp)
{
    return(0); // normal PoW block
}

int32_t komodo_blockcheck(CBlock *block,uint32_t *nBitsp)
{
    // 1 -> valid notary block, change nBits to KOMODO_MINDIFF_NBITS
    // -1 -> invalid, ie, prior to notarized block
    return(0); // normal PoW block
}

#endif
