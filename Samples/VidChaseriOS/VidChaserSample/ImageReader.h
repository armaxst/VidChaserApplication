//
//  ImageReader.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 3. 14..
//  Copyright © 2017년 Ray. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Definitions.h"

@interface ImageReader : NSObject

- (void) setPath : (NSString *) path;
- (bool) hasNext;
- (unsigned char *) readFrame : (bool) isStopped;
- (void) readImageInfo : (int&) width : (int&) height : (int&) pixelFormat;
- (int) getCurrentIndex;
- (int) getLastIndex;
- (void) rewind;
- (void) reset;

@end
