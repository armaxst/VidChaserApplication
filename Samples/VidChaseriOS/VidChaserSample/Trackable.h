//
//  Trackable.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 19..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import <VidChaserEngine/VidChaserAPI.h>
#import <VidChaserEngine/VidChaserDefine.h>
#import <VidChaserEngine/vecmath.h>
#import <Foundation/Foundation.h>
#import <map>
#import "Sticker.h"
#import "ProjectionMatrix.h"

using namespace std;

@interface Trackable : NSObject
{
@public
    Sticker *sticker;
}

- (void) start : (int) imageIndexWhenTouch : (int) touchX : (int) touchY : (TrackingMethod) trackingMethod;
- (void) drawSticker : (int) idx;
- (void) deactivateTrackingPoint;
- (void) stop;

@end

