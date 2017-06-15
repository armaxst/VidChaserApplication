//
//  ViewController.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 30..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "ViewController.h"
#import "PreferenceData.h"
#import "ImageTrackerViewController.h"

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [[PreferenceData getInstance] initialize];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)ClickTrackerButton:(id)sender {
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *folderPath = [documentsDirectory stringByAppendingPathComponent:@"Images"];
    
    if(![[NSFileManager defaultManager] fileExistsAtPath:folderPath])
    {
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Error" message:@"You must write Images."
                                    preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction* ok = [UIAlertAction
                             actionWithTitle:@"OK"
                             style:UIAlertActionStyleDefault
                             handler:^(UIAlertAction * action)
                             {
                                 [alert dismissViewControllerAnimated:YES completion:nil];
                                 
                             }];
        [alert addAction:ok];
        [self presentViewController:alert animated:YES completion:nil];
    }
    else
    {
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
        ImageTrackerViewController *IVC = [storyboard instantiateViewControllerWithIdentifier:@"ImageTrackerViewController"];
        [[self navigationController] pushViewController:IVC animated:YES];
    }
}


@end
