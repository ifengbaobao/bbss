#include "myjpegdecompress.h"
#include"mycolorutils.h"

MyJpegDecompress::MyJpegDecompress()
{
    //第一步初始化jpeg解压缩对象。
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
}

MyJpegDecompress::~MyJpegDecompress()
{
    //第八步释放jpeg解压缩对象。
    jpeg_destroy_decompress(&cinfo);
}

//解压缩。
MyBitmap MyJpegDecompress::decompress(unsigned char * srcData, unsigned long srcSize)
{
    //第二步指定数据源。
    jpeg_mem_src(&cinfo, srcData, srcSize);

    //第三步读取文件参数。
    (void)jpeg_read_header(&cinfo, TRUE);

    MyBitmap bitmap(cinfo.image_width, cinfo.image_height, 4);

    //第五步开始解压缩。
    (void)jpeg_start_decompress(&cinfo);

    int row_stride = cinfo.output_width * cinfo.output_components;/* physical row width in output buffer */
    JSAMPARRAY buffer;
    /* Output row buffer */
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);


    int lineSize = cinfo.image_height -1;
    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could ask for
         * more than one scanline at a time if that's more convenient.
         */
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        /* Assume put_scanline_someplace wants a pointer and sample count. */

        //put_scanline_someplace(buffer[0], row_stride);
        rgb2bgra(bitmap.getData() + (bitmap.getWidthBytes()*lineSize),buffer[0], cinfo.output_width,1);
        --lineSize;
    }

    //第七步完成解压缩。
    (void)jpeg_finish_decompress(&cinfo);

    return bitmap;
}

//颜色转换。
void MyJpegDecompress::rgb2bgra(unsigned char * des, unsigned char * src, int width, int height)
{
    MyColorUtils::rgb2bgra(des,src,width,height);
}

