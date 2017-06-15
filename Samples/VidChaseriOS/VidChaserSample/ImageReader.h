//
//  ImageReader.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 3. 14..
//  Copyright © 2017년 Ray. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Definitions.h"

class ImageReader
{
public:
    ImageReader();
    void setPath(NSString *path);
    bool hasNext();
    unsigned char* readFrame(bool isStopped);
    void readImageInfo(int& width, int& height, int& pixelFormat);
    int getCurrentIndex();
    int getLastIndex();
    void rewind();
    void reset();
private:
    NSString *fileFolderPath;
    NSArray *fileList;
    int currentIndex;
    bool isRewind;
};
