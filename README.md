# ZopfliCompress #

A demonstration program showing how to use zopfli compression library (https://code.google.com/p/zopfli/).

*   Created: 2013-03-09 (Sat)
*   Last Modified: 2013-03-09 (Sat)

## Prerequisites ##

Tested on Mac OS X 10.7.5.

    $ clang++ --version
    Apple clang version 3.1 (tags/Apple/clang-318.0.54) (based on LLVM 3.1svn)
    Target: x86_64-apple-darwin11.4.2
    Thread model: posix

## Contents ##

*   /external
    *   GetLatestZopfli.sh : Get latest codes.
    *   BuildLibZopfli.sh : Build 'zopfli/libzopfli.a'.

*   /source
    *   Source codes for ZopfliCompress command.
    *   Uses /external/zopfli/libzopfli.a
    *   Just hit 'make'. './ZopfliCompress -h' shows usage.
    *   Demo:

            $ ./ZopfliCompress -in loremipsum.txt
            $ ls loremipsum*
            -rw-r--r--   65536  loremipsum.txt
            -rw-r--r--   15716  loremipsum.txt.bin

*   /test
    *   inflate.rb : Decompressor for ZopfliCompress output (generated without '-gzip' option).
    *   Usage:

            $ ruby inflate.rb loremipsum.txt.bin


## License ##

zlib/libpng license.

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.
    
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
    
        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    
        2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    
        3. This notice may not be removed or altered from any source
        distribution.

<!--
Local Variables:
mode: markdown
coding: utf-8-dos
End:
-->
