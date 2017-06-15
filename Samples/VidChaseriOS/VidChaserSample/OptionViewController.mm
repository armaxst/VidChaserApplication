//
//  OptionViewController.m
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 5. 30..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "OptionViewController.h"
#import "PreferenceData.h"

@interface OptionViewController ()
{
    NSUserDefaults *defaults;
    int resolutionIndex;
    int colorFormatIndex;
    int methodIndex;
    
    NSArray *resolutionButtonArray;
    NSArray *colorformatButtonArray;
    NSArray *methodButtonArray;
}
@property (weak, nonatomic) IBOutlet UIView *resolutionView;
@property (weak, nonatomic) IBOutlet UIView *colorFormatView;
@property (weak, nonatomic) IBOutlet UIView *methodView;
@property (weak, nonatomic) IBOutlet UIButton *resolution1Btn;
@property (weak, nonatomic) IBOutlet UIButton *resolution2Btn;
@property (weak, nonatomic) IBOutlet UIButton *resolution3Btn;
@property (weak, nonatomic) IBOutlet UIButton *colorformat1Btn;
@property (weak, nonatomic) IBOutlet UIButton *colorformat2Btn;
@property (weak, nonatomic) IBOutlet UIButton *method1Btn;
@property (weak, nonatomic) IBOutlet UIButton *method2Btn;
@property (weak, nonatomic) IBOutlet UIButton *method3Btn;
@property (weak, nonatomic) IBOutlet UIButton *method4Btn;

@property (weak, nonatomic) IBOutlet UIButton *okBtn;

- (IBAction)ClickResolutionButton:(id)sender;
- (IBAction)ClickColorFormatButton:(id)sender;
- (IBAction)ClickMethodButton:(id)sender;

- (IBAction)ClickOKButton:(id)sender;

@end

@implementation OptionViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    defaults = [NSUserDefaults standardUserDefaults];
    
    [self initUI];
    
    resolutionIndex = [[PreferenceData getInstance] getValue:PreKey::RESOLUTION];
    colorFormatIndex = [[PreferenceData getInstance] getValue:PreKey::COLORFORMAT];
    methodIndex = [[PreferenceData getInstance] getValue:PreKey::TRACKING];
    
    [self setResolutionValue:resolutionIndex];
    [self setColorFormatValue:colorFormatIndex];
    [self setTrackingValue:methodIndex];
}

- (void) initUI
{
    self.resolutionView.layer.cornerRadius = 5;
    self.resolutionView.layer.masksToBounds = YES;

    self.colorFormatView.layer.cornerRadius = 5;
    self.colorFormatView.layer.masksToBounds = YES;
    
    self.methodView.layer.cornerRadius = 5;
    self.methodView.layer.masksToBounds = YES;
    
    self.okBtn.layer.cornerRadius = 5;
    self.okBtn.layer.masksToBounds = YES;
    
    resolutionButtonArray = [NSArray arrayWithObjects:self.resolution1Btn, self.resolution2Btn, self.resolution3Btn, nil];
    colorformatButtonArray = [NSArray arrayWithObjects:self.colorformat1Btn, self.colorformat2Btn, nil ];
    methodButtonArray = [NSArray arrayWithObjects:self.method1Btn, self.method2Btn, self.method3Btn, self.method4Btn, nil];
    
    for(int i = 0; i < [resolutionButtonArray count]; i++)
    {
        [resolutionButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"uncheck.png"] forState:UIControlStateNormal];
        [resolutionButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"check.png"] forState:UIControlStateSelected];
        [resolutionButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"check.png"] forState:UIControlStateHighlighted];
    }
    
    for(int i = 0; i < [colorformatButtonArray count]; i++)
    {
        [colorformatButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"uncheck.png"] forState:UIControlStateNormal];
        [colorformatButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"check.png"] forState:UIControlStateSelected];
        [colorformatButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"check.png"] forState:UIControlStateHighlighted];
    }
    
    for(int i = 0; i < [methodButtonArray count]; i++)
    {
        [methodButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"uncheck.png"] forState:UIControlStateNormal];
        [methodButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"check.png"] forState:UIControlStateSelected];
        [methodButtonArray[i] setBackgroundImage:[UIImage imageNamed:@"check.png"] forState:UIControlStateHighlighted];
    }
}

- (void) setResolutionValue:(int) index
{
    resolutionIndex = index;
    for(int i = 0; i < [resolutionButtonArray count]; i++)
    {
        if(i == index)
        {
            [[resolutionButtonArray objectAtIndex:i] setSelected:true];
        }
        else
        {
            [[resolutionButtonArray objectAtIndex:i] setSelected:false];
        }
    }
}

- (void) setColorFormatValue:(int) index
{
    colorFormatIndex = index;
    for(int i = 0 ; i < [colorformatButtonArray count]; i++)
    {
        if(i == index)
        {
            [[colorformatButtonArray objectAtIndex:i] setSelected:true];
        }
        else
        {
            [[colorformatButtonArray objectAtIndex:i] setSelected:false];
        }
    }
}

- (void) setTrackingValue:(int) index
{
    methodIndex = index;
    for(int i = 0 ; i < [methodButtonArray count]; i++)
    {
        if(i == index)
        {
            [[methodButtonArray objectAtIndex:i] setSelected:true];
        }
        else
        {
            [[methodButtonArray objectAtIndex:i] setSelected:false];
        }
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)ClickResolutionButton:(id)sender {
    [self setResolutionValue:(int)[sender tag]];
}

- (IBAction)ClickColorFormatButton:(id)sender {
    [self setColorFormatValue:(int)[sender tag]];
}

- (IBAction)ClickMethodButton:(id)sender {
    [self setTrackingValue:(int)[sender tag]];
}

- (IBAction)ClickOKButton:(id)sender {
    [defaults setInteger:resolutionIndex forKey:PreKeyString(PreKey::RESOLUTION)];
    [defaults setInteger:colorFormatIndex forKey:PreKeyString(PreKey::COLORFORMAT)];
    [defaults setInteger:methodIndex forKey:PreKeyString(PreKey::TRACKING)];
    
    [defaults synchronize];
    [[self navigationController] popViewControllerAnimated:YES];
}

@end
