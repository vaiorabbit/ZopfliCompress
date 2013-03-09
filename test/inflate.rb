require 'zlib'

=begin
struct BinaryHeader
{
    uint32 FourCC;
    uint32 Version;
    uint32 FileSize;
    uint32 BlockSize;
};

Binary Body:
If header.BlockSize > 0 then
  The body is made up from a sequence of compressed blocks:
    |Compressed Block Size(Length==sizeof(uint32))| |Compressed Block (Length==Compressed Block Size)|
    |Compressed Block Size(Length==sizeof(uint32))| |Compressed Block (Length==Compressed Block Size)|
    :
  To decompress, pass each block to zlib 'inflate' function.
Else ( header.BlockSize == 0 )
  To decompress, just pass the whole content to zlib 'inflate' function.
End
=end

def decompress( f_in, f_out )
  header_fourcc    = f_in.read(4).unpack("L")[0]
  header_version   = f_in.read(4).unpack("L")[0]
  header_filesize  = f_in.read(4).unpack("L")[0]
  header_blocksize = f_in.read(4).unpack("L")[0]

  if header_blocksize > 0 then
    while !f_in.eof?
      comp_segment_size = f_in.read(4).unpack("L")[0]
      comp_segment = f_in.read(comp_segment_size)
      orig_segment = Zlib::Inflate.new.inflate( comp_segment )

      f_out << orig_segment

      break if comp_segment_size == 0
    end
  else
    comp_content = f_in.read
    comp_content.size
    orig_content = Zlib::Inflate.new.inflate( comp_content )
    f_out << orig_content
  end
end

if __FILE__ == $0
  if ARGV[0].slice(/....$/) == ".bin"
    in_filename = ARGV[0]
    out_filename = in_filename.chomp(".bin")
    f_in = File.new( in_filename, "r:BINARY" )
    f_out = File.new( out_filename, "w:BINARY" )

    decompress( f_in, f_out )

    f_in.close()
    f_out.close()
  end
end
