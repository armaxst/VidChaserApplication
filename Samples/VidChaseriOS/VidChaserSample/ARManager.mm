//
//  ARManager.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 19..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "Trackable.h"
#import "ARManager.h"
#import "ProjectionMatrix.h"
#import <VidChaserEngine/VidChaserAPI.h>
#import <vector>

@interface ARManager()
{
    vector<Trackable *> trackableList;
}
@end

@implementation ARManager

- (id) init
{
    self = [super init];
    if(self)
    {
        trackableList.clear();
    }
    return self;
}

- (void) startTracking : (Sticker *) sticker : (int) imageIndexWhenTouch : (float) touchX : (float) touchY : (TrackingMethod) trackingMethod
{
    for (vector<Trackable *>::iterator itor = trackableList.begin();
         itor != trackableList.end();
         itor++)
    {
        if((*itor)->sticker == sticker)
        {
            [(*itor) start:imageIndexWhenTouch :touchX :touchY :trackingMethod];
        }
    }
}

- (void) deactivateAllTrackables
{
    for (vector<Trackable *>::iterator itor = trackableList.begin();
         itor != trackableList.end();
         itor++)
    {
        [(*itor) deactivateTrackingPoint];
    }
}

- (void) stopTracking : (Sticker *) sticker;
{
    for (vector<Trackable *>::iterator itor = trackableList.begin();
         itor != trackableList.end();
         itor++)
    {
        if((*itor)->sticker == sticker)
        {
            [(*itor) stop];
            break;
        }
    }
}

- (void) addSticker:(Sticker *) sticker
{
    Trackable *trackable = [[Trackable alloc] init];
    trackable->sticker = sticker;
    trackableList.push_back(trackable);
}

- (void) drawSticker : (int) imageIndex
{
    for (vector<Trackable *>::iterator itor = trackableList.begin();
         itor != trackableList.end();
         itor++)
    {
        [(*itor) drawSticker:imageIndex];
    }
}

- (void) removeSticker : (Sticker *) sticker
{
    for (vector<Trackable *>::iterator itor = trackableList.begin();
         itor != trackableList.end();
         itor++)
    {
        if((*itor)->sticker == sticker)
        {
            trackableList.erase(itor);
            return;
        }
    }
}

- (Sticker *) getTouchedSticker : (int) touchX : (int) touchY
{
    for(int i = (int)trackableList.size() - 1; 0 <= i ; i--)
    {
        if(trackableList[i]->sticker->isTouched(touchX, touchY))
        {
            return trackableList[i]->sticker;
        }
    }
    return nullptr;
}

- (void) clear
{
    trackableList.clear();
}

@end
