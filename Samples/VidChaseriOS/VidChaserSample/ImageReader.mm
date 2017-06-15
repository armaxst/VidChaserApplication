//
//  ImageReader.mm
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 3. 14..
//  Copyright © 2017년 Ray. All rights reserved.
//

#import "ImageReader.h"
#import "Utils.h"

ImageReader::ImageReader()
{
    currentIndex = 0;
    isRewind = false;
}

void ImageReader::setPath(NSString *path)
{
    fileFolderPath = path;
    fileList = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path error:nil];
}

bool ImageReader::hasNext()
{
    if(isRewind)
    {
        return (currentIndex >= 0);
    }
    else
    {
        return (currentIndex < fileList.count);
    }
}

unsigned char* ImageReader::readFrame(bool isStopped)
{
    NSString *fileFullPath = [NSString stringWithFormat:@"%@/%@", fileFolderPath, fileList[currentIndex]];
    NSData *imageFullData = [NSData dataWithContentsOfFile:fileFullPath];
    NSData *imageRawData = [imageFullData subdataWithRange:NSMakeRange(12, imageFullData.length -12)];
    
    if(isRewind)
    {
        currentIndex--;
    }
    else
    {
        if(isStopped == false)
        {
            currentIndex++;
        }
    }
    
    return (unsigned char *) [imageRawData bytes];
}

void ImageReader::readImageInfo(int& width, int& height, int& pixelFormat)
{
    NSString *fileFullPath = [NSString stringWithFormat:@"%@/%@", fileFolderPath, fileList[0]];
    NSData *imageFullData = [NSData dataWithContentsOfFile:fileFullPath];
    
    NSData *widthData  = [imageFullData subdataWithRange:NSMakeRange(0, 4)];
    NSData *heightData  = [imageFullData subdataWithRange:NSMakeRange(4, 4)];
    NSData *pixelFormatData  = [imageFullData subdataWithRange:NSMakeRange(8, 4)];
    
    width = [Utils DataToInt:widthData];
    height = [Utils DataToInt:heightData];
    pixelFormat = [Utils DataToInt:pixelFormatData];
}

int ImageReader::getCurrentIndex()
{
    return currentIndex;
}

int ImageReader::getLastIndex()
{
    return (int)fileList.count;
}

void ImageReader::rewind()
{
    isRewind = true;
}

void ImageReader::reset()
{
    if(isRewind)
    {
        isRewind = false;
    }
    currentIndex = 0;
}

