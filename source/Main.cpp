#include <memory>
#include <fstream>
#include <iostream>
#include <string>

#include "zopfli.h"

#include "Types.h"
#include "Utility.h"
#include "Format.h"
#include "Option.h"
#include "Convert.h"

namespace command
{
    struct Entry
    {
        const char* name;
        const char* argdesc;
        const char* usage;
    };

    typedef enum {
        eID_Help,
        eID_InputFilename,
        eID_OutputFilename,
        eID_GZipCompress,
        eID_DecompressMode,
        eID_Endian,
        eID_BlockSize,

        eID_Max,
        eID_Unknown
    } CommandID;

    static const Entry opts[eID_Max] = {
        {"-h", "", "show this usage."},
        {"-in", "[filename]", "input filename."},
        {"-out", "[filename]", "output filename."},
        {"-gz", "", "compress with gzip-complatible format."},
        {"-d", "", "(NOT implemented yet) decompress mode"},
        {"-endian", "", "(NOT implemented yet) swap endian"},
        {"-block", "[0|8|16|32|64|...]", "(NOT implemented yet) Do per-block compression. default=0 (no segmentation)"},
    };
} // namespace command

namespace app
{
    static std::string strInFilename;
    static std::string strOutFilename;

    static std::ifstream fIn;
    static std::ofstream fOut;

    static bool bModeGZipCompress = false;
    static bool bModeDecompress = false;
    static bool bModeEndianSwap = false;

    static unsigned int unBlockSize = 0;

    static int ParseOptions( int argc, char* argv[] )
    {
        // Show help
        if ( argc == 1 || OptionExists( argc, argv, command::opts[command::eID_Help].name ) )
        {
            std::cout << "ZopfliCompress Ver. " << __DATE__ << std::endl;
            for ( int i = 0; i < command::eID_Max; ++i )
            {
                std::cout << "\t" << command::opts[i].name << ' ' << command::opts[i].argdesc << "\t:" << command::opts[i].usage << std::endl;
            }

            return 1;
        }

        // Run as decompress mode
        if ( OptionExists( argc, argv, command::opts[command::eID_DecompressMode].name ) )
        {
            app::bModeDecompress = true;
            std::cout << "option -d found.\n";
        }

        // Check gzip mode
        if ( OptionExists( argc, argv, command::opts[command::eID_GZipCompress].name ) )
        {
            app::bModeGZipCompress = true;
        }

        if ( OptionExists( argc, argv, command::opts[command::eID_InputFilename].name ) )
        {
            app::strInFilename = OptionValue( argc, argv, command::opts[command::eID_InputFilename].name );
        }
        else
        {
            std::cout << "[ERROR] ZopfliCompress : pstrInFilename = NULL" << std::endl;
            return -1;
        }

        // Build output filename
        if ( OptionExists( argc, argv, command::opts[command::eID_OutputFilename].name ) )
        {
            app::strOutFilename = OptionValue( argc, argv, command::opts[command::eID_OutputFilename].name );
        }
        else
        {
            // gzip compress -> append ".gz" suffix.
            app::strOutFilename = app::strInFilename;
            app::strOutFilename += app::bModeGZipCompress ? ".gz" : ".bin";
        }

        // Check endian swap mode
        if ( OptionExists( argc, argv, command::opts[command::eID_Endian].name ) )
        {
            app::bModeEndianSwap = true;
        }

        // Get block size for streaming decompression
        if ( OptionExists( argc, argv, command::opts[command::eID_BlockSize].name ) )
        {
            char* pstrBockSize = OptionValue( argc, argv, command::opts[command::eID_BlockSize].name );
            if ( pstrBockSize != NULL && Convert::IsDecimal(pstrBockSize) )
                app::unBlockSize = Convert::ToDecimal(pstrBockSize);
            else
                app::unBlockSize = 0;
        }

        return 0;
    }

    static int Decompress()
    {
        // Not implemented yet
        return -1;
    }

    static int Compress()
    {
        // Open input
        try
        {
            app::fIn.open( app::strInFilename.c_str(), std::ios_base::in | std::ios_base::binary );
        }
        catch ( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        // Open output
        try
        {
            app::fOut.open( app::strOutFilename.c_str(), std::ios_base::out | std::ios_base::binary );
        }
        catch ( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        // Calculate file size
        std::streampos nInFileSize = app::fIn.tellg();
        app::fIn.seekg( 0, std::ios_base::end );
        nInFileSize = app::fIn.tellg() - nInFileSize;
        app::fIn.seekg( 0, std::ios_base::beg ); // rewind

        // Read whole input
        std::unique_ptr<unsigned char[]> pInFileContent( new unsigned char[nInFileSize] ); // C++11
        app::fIn.read( (char*)pInFileContent.get(), nInFileSize );

        // Do compress
        ZopfliOptions cZopfliOptions;
        ZopfliInitOptions( &cZopfliOptions );

        unsigned char* pOutFileContent = NULL; // will be 'malloc'ed inside Zopfli
        size_t nOutFileSize = 0; // will be set inside Zopfli

        if ( app::bModeGZipCompress )
        {
            // Generate gzip-compatible file ('gzip -d' or 'gunzip')
            ZopfliCompress( &cZopfliOptions, ZOPFLI_FORMAT_GZIP, pInFileContent.get(), nInFileSize, &pOutFileContent, &nOutFileSize );
            app::fOut.write( (char*)pOutFileContent, nOutFileSize );
        }
        else
        {
            // Generate file for streaming zlib decompression
            BinaryHeader header;
            WriteFourCC(   &header.FourCC, 'Z','P','F','L' );
            WriteValueU32( &header.Version, 0x00000001, app::bModeEndianSwap );
            WriteValueU32( &header.FileSize, nInFileSize, app::bModeEndianSwap );
            WriteValueU32( &header.BlockSize, app::unBlockSize, app::bModeEndianSwap );
            app::fOut.write( (char*)&header, sizeof(BinaryHeader) );

            if ( app::unBlockSize == 0 ) // no segmentation required
            {
                ZopfliCompress( &cZopfliOptions, ZOPFLI_FORMAT_ZLIB, pInFileContent.get(), nInFileSize, &pOutFileContent, &nOutFileSize );
                app::fOut.write( (char*)pOutFileContent, nOutFileSize );
                free( pOutFileContent ); // 'malloc'ed inside Zopfli
            }
            else
            {
                size_t nCurrentInOffset = 0;
                size_t nAvailableFileSize = nInFileSize;
                while ( nAvailableFileSize > 0 )
                {
                    ZopfliCompress( &cZopfliOptions, ZOPFLI_FORMAT_ZLIB, pInFileContent.get()+nCurrentInOffset, nAvailableFileSize >= app::unBlockSize ? app::unBlockSize : nAvailableFileSize, &pOutFileContent, &nOutFileSize );
                    unsigned int uOutSize;
                    WriteValueU32( &uOutSize, nOutFileSize, app::bModeEndianSwap );
                    app::fOut.write( (char*)&uOutSize, sizeof(unsigned int) );
                    app::fOut.write( (char*)pOutFileContent, nOutFileSize );
#ifdef ENABLE_ZEROFILL
                    {
                        unsigned int nOutFileSizeAigned = RoundUp( sizeof(unsigned int) + nOutFileSize, 16 );
                        unsigned int nZeroFillLength = nOutFileSizeAigned - (sizeof(unsigned int) + nOutFileSize);
                        if ( nZeroFillLength > 0 )
                        {
                            char zero = 0;
                            app::fOut.write( &zero, nZeroFillLength );
                        }
                    }
#endif // ENABLE_ZEROFILL
                    free( pOutFileContent );
                    nOutFileSize = 0;

                    if ( nAvailableFileSize >= app::unBlockSize )
                    {
                        nCurrentInOffset += app::unBlockSize;
                        nAvailableFileSize -= app::unBlockSize;
                    }
                    else
                    {
                        nCurrentInOffset += (app::unBlockSize - nAvailableFileSize);
                        nAvailableFileSize = 0;
                    }
                }
            }
        }

        app::fIn.close();
        app::fOut.close();

        return 0;
    }

} // namespace app



int main( int argc, char* argv[] )
{
    int nParseResult = app::ParseOptions( argc, argv );
    if ( nParseResult != 0 )
        return nParseResult;

    int nOperationResult = app::bModeDecompress ? app::Decompress() : app::Compress();
    return nOperationResult;
}
