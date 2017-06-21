//
//  ARManager.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 19..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Trackable.h"

@interface ARManager : NSObject

+ (ARManager *) getInstance;

- (Sticker *) getTouchedSticker : (int) touchX : (int) touchY;
- (void) addSticker : (Sticker *) trackable;
- (void) removeSticker : (Sticker *) sticker;
- (void) startTracking : (Sticker *) sticker : (int) imageIndexWhenTouch : (int) lastImageIndex : (float) touchX : (float) touchY : (TrackingMethod) trackingMethod;
- (void) drawSticker : (int) imageIndex;
- (void) stopTracking : (Sticker *) sticker;
- (void) clear;

@end
