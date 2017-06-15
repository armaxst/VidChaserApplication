//
//  PreferenceData.h
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 31..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Definitions.h"
#import <MaxstARDef.h>
#import <VidChaserEngine/VidChaserDefine.h>

@interface PreferenceData : NSObject

+ (PreferenceData *) getInstance;
- (void) initialize;
- (void) getResolution : (int&) width : (int&) height;
- (ColorFormat) getColorFormat;
- (int) getValue : (PreKey) key;

@end
