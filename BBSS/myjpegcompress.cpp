#include "myjpegcompress.h"
#include"mycolorutils.h"

void MyJpegCompress::init(MyBitmap * bitmap, int quality)
{
    this->bitmap = bitmap;
    this->quality = quality;
    this->outSize = 0;
    this->bufSize = bitmap->getWidth() * bitmap->getHeight() * 3;
    this->srcBuf = new unsigned char[bufSize];
    this->outBuf = NULL;
    this->row_stride = bitmap->getWidth() * 3;	/* 设置一行的宽度。字节数。*/

    //第一步分配和初始化jpeg压缩对象。
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);//初始化压缩对象。

    //第三步设置压缩的参数。
    cinfo.image_width = bitmap->getWidth(); 	/* image width and height, in pixels */
    cinfo.image_height = bitmap->getHeight();
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

    jpeg_set_defaults(&cinfo);

    //设置图片的质量，还可以设置一些别的值。
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
}

MyJpegCompress::MyJpegCompress(MyBitmap * bitmap, int quality)
{
    this->init(bitmap ,quality);
}

MyJpegCompress::MyJpegCompress(const MyJpegCompress & myJpeg)
{
    this->init(myJpeg.bitmap, myJpeg.quality);
}

MyJpegCompress & MyJpegCompress::operator=(const MyJpegCompress & myJpeg)
{
    if (this->srcBuf != NULL)
    {
        delete [] this->srcBuf;
    }
    if (this->outBuf != NULL)
    {
        free(this->outBuf);
    }
    this->init(myJpeg.bitmap, myJpeg.quality);
    return *this;
}

MyJpegCompress::~MyJpegCompress()
{
    //第七步释放jpeg压缩对象。
    jpeg_destroy_compress(&cinfo);

    if (this->srcBuf != NULL)
    {
        delete[] this->srcBuf;
    }
    if (this->outBuf != NULL)
    {
        free(this->outBuf);
    }
}

//bgra颜色转rgb颜色。
void MyJpegCompress::bgra2rgb(unsigned char * des, unsigned char * src, int width, int height)
{
    MyColorUtils::bgra2rgb(des,src,width,height);
}

//压缩图片。
void MyJpegCompress::compress()
{
    //先转换颜色。
    memset(this->srcBuf, 0, this->bufSize);
    this->bgra2rgb(this->srcBuf, bitmap->getData(), bitmap->getWidth(), bitmap->getHeight());

    //压缩图片前先释放上次压缩的数据。
    if (this->outBuf != NULL)
    {
        free(this->outBuf);
        this->outBuf = NULL;
    }
    this->outSize = 0;


    //第二步指定压缩后数据保存的目标。第二步和第三步没有顺序要求。
    jpeg_mem_dest(&cinfo, &(this->outBuf), &(this->outSize));//这儿的输出缓存很有可能造成内存泄漏。

    //第四步开始压缩。
    jpeg_start_compress(&cinfo, TRUE);
    JSAMPROW row_pointer[1] = {0};	/* pointer to JSAMPLE row[s] */
    while (cinfo.next_scanline < cinfo.image_height) {
        //row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        //bitmap的坐标是从左下角开始的。
        row_pointer[0] = &srcBuf[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    //第六步完成压缩
    jpeg_finish_compress(&cinfo);
}

unsigned char * MyJpegCompress::getOutBuf()
{
    return this->outBuf;
}

unsigned long MyJpegCompress::getOutSize()
{
    return this->outSize;
}

void MyJpegCompress::saveJpegFile(const char * fileName)
{
    FILE * file = fopen(fileName, "wb");
    fwrite(this->outBuf, this->outSize, 1, file);
    fclose(file);
}
