//
//  PreferenceData.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 31..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "PreferenceData.h"

static dispatch_once_t dis_one;
static PreferenceData *instance = nil;

@interface PreferenceData()
{
    NSUserDefaults* defaults;
}


@end

@implementation PreferenceData

+ (PreferenceData *) getInstance
{
    dispatch_once(&dis_one, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

- (void) initialize
{
    defaults = [NSUserDefaults standardUserDefaults];
    
    if([defaults objectForKey:PreKeyString(PreKey::INIT)] == nil)
    {
        [defaults setObject:@"OK" forKey:PreKeyString(PreKey::INIT)];
        [defaults setInteger:ResolutionValue::Small forKey:PreKeyString(PreKey::RESOLUTION)];
        [defaults setInteger:ColorFormat::RGBA8888 forKey:PreKeyString(PreKey::COLORFORMAT)];
        [defaults setInteger:VidChaser::TrackingMethod::RIGID forKey:PreKeyString(PreKey::TRACKING)];
        [defaults synchronize];
    }
}

- (void) getResolution : (int &) width : (int &) height
{
    int result = (int)[defaults integerForKey:PreKeyString(PreKey::RESOLUTION)];
    NSString *temp = ResolutionValueString((ResolutionValue) result);
    NSArray *tempArray = [temp componentsSeparatedByString:@"X"];
    
    width = [[tempArray objectAtIndex:0] intValue];
    height = [[tempArray objectAtIndex:1] intValue];
}

- (ColorFormat) getColorFormat
{
    return (ColorFormat)[defaults integerForKey:PreKeyString(PreKey::COLORFORMAT)];
}

- (int) getValue : (PreKey) key
{
    NSString *keyString = PreKeyString(key);
    return (int)[defaults integerForKey:keyString];
}

@end
