#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED

#include "Types.h"

struct BinaryHeader
{
    uint32 FourCC;    // 'Z','P','F','L'
    uint32 Version;   // 0x00000001
    uint32 FileSize;  // Original (uncompressed) size of file
    uint32 BlockSize; // if > 0, the binary body is in streaming decompression format (See 'Binary Body' below)
};

/* Binary Body:
    If header.BlockSize > 0 then
      The body is made up from a sequence of compressed blocks:
        |Compressed Block Size(Length==sizeof(uint32))| |Compressed Block (Length==Compressed Block Size)|
        |Compressed Block Size(Length==sizeof(uint32))| |Compressed Block (Length==Compressed Block Size)|
        :
      To decompress, pass each block to zlib 'inflate' function.
    Else ( header.BlockSize == 0 )
      To decompress, just pass the whole content to zlib 'inflate' function.
    end
*/

#endif // FORMAT_H_INCLUDED
