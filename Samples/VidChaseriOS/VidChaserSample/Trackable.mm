//
//  Trackable.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 21..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "Trackable.h"

@interface Trackable()
{
    int trackableId;
    int imageCoordX, imageCoordY;
    map<int, gl_helper::Mat4> transformRecord;
    int restartTrackingImageIndex;
    bool trackingMode;
}

@end

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
        restartTrackingImageIndex = -1;
        sticker = nullptr;
        trackingMode = false;
    }
    return self;
}

- (void) start : (int) imageIndexWhenTouch : (int) touchX : (int) touchY : (TrackingMethod) trackingMethod;
{
    MatrixUtil::GetImageCoordFromScreenCoord(ProjectionMatrix::getInstance()->getSurfaceWidth(),
                                             ProjectionMatrix::getInstance()->getSurfaceHeight(),
                                             ProjectionMatrix::getInstance()->getImageWidth(),
                                             ProjectionMatrix::getInstance()->getImageHeight(),
                                             touchX, touchY, imageCoordX, imageCoordY);
    
    VidChaser::addTrackingPoint(imageCoordX, imageCoordY, &trackableId, trackingMethod);
    VidChaser::activateTrackingPoint(imageCoordX, imageCoordY, trackableId);
    
    restartTrackingImageIndex = imageIndexWhenTouch;
    trackingMode = true;
}

- (void) drawSticker:(int)idx
{
    if(trackableId < 0)
    {
        sticker->draw(gl_helper::Mat4::Identity());
        return;
    }
    
    if(!trackingMode && idx >= restartTrackingImageIndex)
    {
        VidChaser::activateTrackingPoint(imageCoordX, imageCoordY, trackableId);
        trackingMode = true;
    }
    
    if(trackingMode)
    {
        int processTime = 0;
        gl_helper::Mat3 transformMatrix33F;
        gl_helper::Mat4 transformMatrix44F;
        
        VidChaser::ResultCode resultCode = VidChaser::getTrackingResult(transformMatrix33F.Ptr(), trackableId, &processTime);
        transformMatrix33F.Transpose();
        
        if(resultCode == VidChaser::ResultCode::SUCCESS)
        {
            MatrixUtil::GetTransformMatrix44F(ProjectionMatrix::getInstance()->getImageWidth(),
                                              ProjectionMatrix::getInstance()->getImageHeight(),
                                              transformMatrix33F, transformMatrix44F);
            transformRecord[idx] = transformMatrix44F;
        }
        
        if(restartTrackingImageIndex < idx)
        {
            restartTrackingImageIndex = idx;
        }
    }
    
    gl_helper::Mat4 transform;
    if(transformRecord.find(idx) != transformRecord.end())
    {
        transform = transformRecord[idx];
    }
    sticker->draw(transform);
}

- (void) deactivateTrackingPoint
{
    if(trackableId >= 0)
    {
        VidChaser::deactivateTrackingPoint(trackableId);
    }
    trackingMode = false;
}

- (void) stop
{
    VidChaser::removeTrackingPoint(trackableId);
    trackableId = -1;
    restartTrackingImageIndex = -1;
    trackingMode = false;
    transformRecord.clear();
}

@end
