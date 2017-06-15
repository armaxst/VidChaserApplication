//
//  Utils.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 31..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "Utils.h"
#import <Accelerate/Accelerate.h>

@implementation Utils

+ (unsigned char *) intToByte :(int) data
{
    unsigned char * bytesData;
    bytesData = (unsigned char *) malloc (4);
    
    bytesData[0] = (data >> 24) & 0xFF;
    bytesData[1] = (data >> 16) & 0xFF;
    bytesData[2] = (data >> 8) & 0xFF;
    bytesData[3] = data & 0xFF;
    
    return bytesData;
}

+ (int) DataToInt:(NSData *)data
{
    int intSize = sizeof(int); // change it to fixe length
    unsigned char * buffer = malloc(intSize * sizeof(unsigned char));
    [data getBytes:buffer length:intSize];
    int num = 0;
    for (int i = 0; i < intSize; i++) {
        num = (num << 8) + buffer[i];
    }
    free(buffer);
    return num;
}

+ (unsigned char *) imageToBytes : (UIImage *) image
{
    if(image == nil)
    {
        NSLog(@"Input Image Error");
        return NULL;
    }
    
    unsigned char * rawImage = (uint8_t *) malloc (image.size.width * image.size.height * 4 * sizeof(char *));
    memset(rawImage, 0, image.size.width * image.size.height * 4 * sizeof(char *));
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(rawImage, image.size.width, image.size.height, 8, image.size.width * sizeof(uint32_t), colorSpace, kCGBitmapByteOrder32Little || kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(context, CGRectMake(0, 0, image.size.width, image.size.height), image.CGImage);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    
    return rawImage;    
}

@end
