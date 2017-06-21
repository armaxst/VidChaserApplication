//
//  Trackable.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 19..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#include <VidChaserEngine/VidChaserAPI.h>
#include <VidChaserEngine/VidChaserDefine.h>
#import <Foundation/Foundation.h>
#import <map>
#import "Sticker.h"
#import "ProjectionMatrix.h"

using namespace std;

@interface Trackable : NSObject
{
    @public
    int trackableId;
    int imageCoordX, imageCoordY;
    map<int, Matrix44F> transformRecord;
    int touchIndex;
    int lastIndex;
    Sticker *sticker;
}

- (void) start : (int) imageIndexWhenTouch : (int) lastImageIndex : (int) touchX : (int) touchY : (TrackingMethod) trackingMethod;
- (void) draw : (int) idx;
- (void) stop;

@end

