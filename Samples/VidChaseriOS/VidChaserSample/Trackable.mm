//
//  Trackable.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 21..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "Trackable.h"

@implementation Trackable

- (id) init
{
    self = [super init];
    if(self)
    {
        trackableId = -1;
        imageCoordX = 0;
        imageCoordY = 0;
        transformRecord.clear();
        touchIndex = -1;
        lastIndex = -1;
        sticker = nullptr;
    }
    return self;
}

- (void) start : (int) imageIndexWhenTouch : (int) lastImageIndex : (int) touchX : (int) touchY : (TrackingMethod) trackingMethod;
{
    MatrixUtil::GetImageCoordFromScreenCoord(
                                             ProjectionMatrix::getInstance()->getSurfaceWidth(),
                                             ProjectionMatrix::getInstance()->getSurfaceHeight(),
                                             ProjectionMatrix::getInstance()->getImageWidth(),
                                             ProjectionMatrix::getInstance()->getImageHeight(),
                                             touchX, touchY, imageCoordX, imageCoordY);
    
    VidChaser::addTrackingPoint(imageCoordX, imageCoordY, &trackableId, trackingMethod);
    
    touchIndex = imageIndexWhenTouch;
    lastIndex = lastImageIndex;
}

- (void) draw : (int) idx
{
    if(idx == lastIndex)
    {
        VidChaser::deactivateTrackingPoint(trackableId);
        trackableId = -1;
        touchIndex = -1;
        lastIndex = -1;
    }
    
    if(idx == touchIndex)
    {
        VidChaser::activateTrackingPoint(imageCoordX, imageCoordY, trackableId);
    }
    
    if(trackableId != -1)
    {
        int processTime = 0;
        Matrix33F transformMatrix33F;
        Matrix44F transformMatrix44F;
        
        VidChaser::ResultCode temp = VidChaser::getTrackingResult(&transformMatrix33F.m[0][0], trackableId, &processTime);
        
        if(temp == VidChaser::ResultCode::SUCCESS)
        {
            MatrixUtil::GetTransformMatrix44F(ProjectionMatrix::getInstance()->getImageWidth(),
                                              ProjectionMatrix::getInstance()->getImageHeight(),
                                              transformMatrix33F, transformMatrix44F);
            transformRecord[idx] = transformMatrix44F;
        }
        
        if(touchIndex < idx)
        {
            touchIndex = idx;
        }
    }
    
    if(idx == 0)
    {
        VidChaser::deactivateTrackingPoint(trackableId);
    }
    
    Matrix44F transform;
    if(transformRecord.find(idx) != transformRecord.end())
    {
        transform = transformRecord[idx];
    }
    sticker->draw(transform);
}

- (void) stop
{
    VidChaser::deactivateTrackingPoint(trackableId);
    trackableId = -1;
    touchIndex = -1;
    lastIndex = -1;
    transformRecord.clear();
}

@end
