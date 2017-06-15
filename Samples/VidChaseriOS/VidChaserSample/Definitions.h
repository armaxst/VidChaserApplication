//
//  Definitions.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 30..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum
{
    INIT,
    RESOLUTION,
    COLORFORMAT,
    TRACKING,
} PreKey;

#define PreKeyString(enum) [@[@"Init", @"Resolution", @"ColorFormat", @"Tracking"] objectAtIndex:enum]

typedef enum
{
    Small = 0,
    Medium = 1,
    Large = 2,
} ResolutionValue;

#define ResolutionValueString(enum) [@[@"640X480", @"1280X720", @"1920X1080"] objectAtIndex:enum]

#define ColorFormatValueString(enum) [@[@"RGBA", @"RGB"] objectAtIndex:enum]
