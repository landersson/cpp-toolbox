#include <cpp-toolbox/image.h>

#include <cstdio>
#include <cassert>
#include <cstring>
#include <string>
#include <csetjmp>
#include <limits>

#include <jpeglib.h>
#include <jerror.h>

#include <malloc.h>

namespace toolbox
{
//--- JPEG compressed data support --------------------------------------------

static void     jpg_decompress_hollow(j_decompress_ptr cinfo) {}
static boolean  jpg_decompress_int(j_decompress_ptr cinfo) { return FALSE; }
static void     jpg_decompress_skip(j_decompress_ptr cinfo, long num_bytes)
{
    if (num_bytes > 0)
    {
        cinfo->src->next_input_byte += num_bytes;
        cinfo->src->bytes_in_buffer -= num_bytes;
    }
}

struct my_error_mgr
{
    struct jpeg_error_mgr pub;    /* "public" fields */
    std::jmp_buf setjmp_buffer;        /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


static void my_jpeg_error_exit(j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  std::longjmp(myerr->setjmp_buffer, 1);
}


toolbox::ImageData decompress_jpeg(uint8_t *buffer, size_t len)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_source_mgr src_man;
    struct my_error_mgr jerr;
    toolbox::ImageData image;

    JSAMPROW row_pointer[1];     /* pointer to JSAMPLE row(s) */
    int row_stride;              /* physical row width in image buffer */
    cinfo.err = jpeg_std_error(&jerr.pub);

    jerr.pub.error_exit = my_jpeg_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        fprintf(stderr, "decompress_jpeg: error decompressing JPEG data");
        jpeg_destroy_decompress(&cinfo);
        return image;
    }

    jpeg_create_decompress(&cinfo);

    /* we don't want standard out, we want memory: dest_memory */
    /* jpeg_stdio_dest(&cinfo, output); */

    src_man.next_input_byte = buffer;
    src_man.bytes_in_buffer = len;
    src_man.init_source = jpg_decompress_hollow;
    src_man.fill_input_buffer = jpg_decompress_int;
    src_man.skip_input_data = jpg_decompress_skip;
    src_man.resync_to_restart = jpeg_resync_to_restart;
    src_man.term_source = jpg_decompress_hollow;

    cinfo.src = &src_man;

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    image.width  = cinfo.output_width;
    image.height = cinfo.output_height;

    row_stride = cinfo.output_width * cinfo.output_components;

    if (cinfo.output_components == 3)
        image.pixfmt = toolbox::PixelFormat::RGB888;
    else
        image.pixfmt = toolbox::PixelFormat::MONO8;;

    image.data = (unsigned char *)malloc(row_stride * image.height);
    assert(image.data);

    int failsafe_counter = std::numeric_limits<uint16_t>::max() * 5;

    while (cinfo.output_scanline < cinfo.output_height && failsafe_counter-->0)
    {
        row_pointer[0] = (JSAMPLE *)(image.data + cinfo.output_scanline * row_stride);
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);
    return image;
}


static void    jpg_compress_hollow(j_compress_ptr cinfo) {}
static boolean jpg_compress_int(j_compress_ptr cinfo) { return FALSE; }

int compress_jpeg(unsigned char **buffer, unsigned width, unsigned height, unsigned components, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_destination_mgr dest_man;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];     /* pointer to JSAMPLE row(s) */
    int row_stride;              /* physical row width in image buffer */

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    /* we don't want standard out, we want memory: dest_memory */
    /* jpeg_stdio_dest(&cinfo, output); */

    unsigned buffer_size = width * height * components;
    unsigned char *work_buffer = (unsigned char *)malloc(buffer_size);;
    assert(work_buffer);

    dest_man.next_output_byte = work_buffer;
    dest_man.free_in_buffer = buffer_size;
    dest_man.init_destination = jpg_compress_hollow;
    dest_man.empty_output_buffer = jpg_compress_int;
    dest_man.term_destination = jpg_compress_hollow;

    cinfo.dest = &dest_man;
    cinfo.image_width = width;          /* image width and height, in pixels */
    cinfo.image_height = height;

    switch (components)
    {
    case 1:
        cinfo.input_components = 1;             /* # of color components per pixel */
        cinfo.in_color_space = JCS_GRAYSCALE;   /* colorspace of input image */
        break;
    case 3:
        cinfo.input_components = 3;         /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB;     /* colorspace of input image */
        break;
    default:
        assert(0);
    }

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);  /* limit to baseline-JPEG values */
    cinfo.dct_method = JDCT_FASTEST;

    jpeg_suppress_tables(&cinfo, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = cinfo.input_components * width;  /* JSAMPLE units per row */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could pass
         * more than one scanline at a time if that's more convenient.
         */
        row_pointer[0] = *buffer + cinfo.next_scanline * row_stride;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);

    buffer_size = dest_man.next_output_byte - work_buffer;

    *buffer = (unsigned char *)malloc(buffer_size);
    assert(*buffer);

    std::memcpy(*buffer, work_buffer, buffer_size);
    free(work_buffer);

    return buffer_size;
}
}
