//
//  Utils.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 31..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface Utils : NSObject

+ (unsigned char *) intToByte :(int) data;
+ (int) DataToInt:(NSData *)data;
+ (unsigned char *) imageToBytes : (UIImage *) image;

@end
