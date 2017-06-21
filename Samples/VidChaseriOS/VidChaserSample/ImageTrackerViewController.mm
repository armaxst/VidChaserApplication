//
//  ImageTrackerViewController.mm
//  VidChaserSample
//
//  Created by 이상훈 on 2017. 6. 1..
//  Copyright © 2017년 이상훈. All rights reserved.
//

#import "ImageTrackerViewController.h"
#import "ImageReader.h"
#import "Utils.h"

#import "Sticker.h"
#import "ProjectionMatrix.h"
#import "CollectionViewCell.h"
#import "PreferenceData.h"
#import "Sticker.h"
#import "ARManager.h"

#import <vector>
#import <VidChaserEngine/VidChaserAPI.h>
#import <VidChaserEngine/VidChaserDefine.h>

@interface ImageTrackerViewController ()
{
    Sticker *selectSticker;
    CGSize surfaceSize;
    
    NSArray *stickerArray;
    NSArray *stickerImages;
    
    ImageReader *imageReader;
    
    int imageWidth;
    int imageHeight;
    int colorFormat;
    int length;
    TrackingMethod trackingMethod;
    
    bool trackingReady;
    
    float selectedGLX, selectedGLY;
    int touchStartX, touchStartY;
    UILongPressGestureRecognizer *longTapGestureRecognizer;
    UITapGestureRecognizer *singleTapGestureRecognizer;
}

@property (weak, nonatomic) IBOutlet UIButton *backBtn;
@property (weak, nonatomic) IBOutlet UIButton *stickerBtn;
@property (weak, nonatomic) IBOutlet UILabel *indexText;
@property (weak, nonatomic) IBOutlet UICollectionView *collectionView;
@property (weak, nonatomic) IBOutlet UIView *trashbox;
@property (weak, nonatomic) IBOutlet UIView *indicatorView;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *indicator;
- (IBAction)ClickBackButton:(id)sender;
- (IBAction)ClickStickerButton:(id)sender;

@end

@implementation ImageTrackerViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    GLKView *view = (GLKView *)self.view;
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    self.backBtn.layer.cornerRadius = 10;
    self.backBtn.layer.masksToBounds = YES;
    
    self.stickerBtn.layer.cornerRadius = 10;
    self.stickerBtn.layer.masksToBounds = YES;
    
    [self initValue];
    
    [self setupGL];
    
    [self startEngine];
}

- (void) initValue
{
    imageReader = new ImageReader();
    
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *path = [documentsDirectory stringByAppendingString:@"/Images"];
    
    imageReader->setPath(path);
    imageReader->readImageInfo(imageWidth, imageHeight, colorFormat);
    
    ProjectionMatrix::getInstance()->setImageSize(imageWidth, imageHeight);
    
    if(colorFormat == ColorFormat::RGBA8888)
    {
        length = imageWidth * imageHeight * 4;
    }
    else if(colorFormat == ColorFormat::RGB888)
    {
        length = imageWidth * imageHeight * 3;
    }
    
    trackingMethod = (TrackingMethod)[[PreferenceData getInstance] getValue:PreKey::TRACKING];
    
    self.indexText.text = [NSString stringWithFormat:@"000 / %d", imageReader->getLastIndex()];
    trackingReady = false;
    
    stickerArray = [NSArray arrayWithObjects:@"Arrow.png", @"Hand.png", @"Happy.png", @"Love.png", @"Smile.png", @"Sole.png", nil];
    
    CGRect rect = [[UIScreen mainScreen] bounds];
    surfaceSize = CGSizeMake(rect.size.width, rect.size.height);
    
    ProjectionMatrix::getInstance()->setSurfaceSize(surfaceSize.width, surfaceSize.height);
    
    [self.collectionView setDelegate:self];
    [self.collectionView setDataSource:self];
    [self.collectionView setHidden:YES];
    self.collectionView.alpha = 0.6f;
    
    [self.indicatorView setHidden:YES];
    self.indicator.transform = CGAffineTransformMakeScale(2.5f, 2.5f);
    
    self.trashbox.alpha = 0.6f;
    [self.trashbox setHidden:YES];
    
    longTapGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongTap:)];
    [longTapGestureRecognizer setMinimumPressDuration:0.5];
    [self.view addGestureRecognizer:longTapGestureRecognizer];
    
    singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]
                                                          initWithTarget:self action:@selector(handleSingleTap:)];
    
    singleTapGestureRecognizer.numberOfTapsRequired = 1;
    [singleTapGestureRecognizer setCancelsTouchesInView:NO];
    
    [self.collectionView addGestureRecognizer:singleTapGestureRecognizer];
    
    selectSticker = nullptr;
}

- (void)setupGL
{
    GLKView *glKitview = (GLKView *)self.view;
    [EAGLContext setCurrentContext:glKitview.context];
    glEnable(GL_DEPTH_TEST);
    
    glViewport(0, 0, surfaceSize.width, surfaceSize.height);
}

- (void) startEngine
{
    int state = VidChaser::create("nqRhAU1Lb7liez+4hznvGRptML38NVkWrsd2UdN9hMw=");
    if(state != 0)
    {
        NSLog(@"Signature Error\n");
        return;
    }
    NSLog(@"Signature Success\n");
    VidChaser::initRendering();
    VidChaser::setNewFrame(nullptr, 0, 0, 0, (ColorFormat)colorFormat, 0);
    VidChaser::updateRendering(surfaceSize.width, surfaceSize.height);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    self.indexText.text = [NSString stringWithFormat:@"%d / %d", imageReader->getCurrentIndex(), imageReader->getLastIndex()];
    if(imageReader->hasNext())
    {
        glEnable(GL_DEPTH_TEST);
        unsigned char *imageBuffer = imageReader->readFrame(trackingReady);
        
        VidChaser::setNewFrame(imageBuffer, length, imageWidth, imageHeight, (ColorFormat)colorFormat, imageReader->getCurrentIndex());
        VidChaser::renderScene();

        glDisable(GL_DEPTH_TEST);
        [[ARManager getInstance] drawSticker : imageReader->getCurrentIndex()];
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        if(self.indicatorView.hidden == false)
        {
            [self.indicator stopAnimating];
            [self.indicatorView setHidden:YES];
        }
        imageReader->reset();
    }
}

- (IBAction)ClickBackButton:(id)sender {
    VidChaser::deinitRendering();
    VidChaser::destroy();
    [[ARManager getInstance] clear];
    [[self navigationController] popViewControllerAnimated:YES];
}

- (IBAction)ClickStickerButton:(id)sender {
    self.collectionView.hidden = !self.collectionView.hidden;
}

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView{
    return 1;
}

- (NSInteger) collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return stickerArray.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"cell";
    CollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:cellIdentifier forIndexPath:indexPath];
    [cell.backgroundImage setImage:[UIImage imageNamed:[stickerArray objectAtIndex:indexPath.row]]];
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
   NSString *stickerPath = [stickerArray objectAtIndex:(int)indexPath.row];
    
    UIImage * stickerImage = [UIImage imageNamed: stickerPath];
    
    unsigned char* stickerBuffer = [Utils imageToBytes:stickerImage];
    
    Sticker * sticker = new Sticker();
    sticker->setTexture(stickerImage.size.width, stickerImage.size.height, stickerBuffer);
    float ratio = stickerImage.size.width / (float) surfaceSize.width;
    float size = stickerImage.size.width * ratio;
    
    sticker->setScale(size, size, 1.0f);
    sticker->setTranslate(selectedGLX, selectedGLY, 0.0f);
    sticker->setProjectionMatrix(ProjectionMatrix::getInstance()->getProjectionMatrix());
    
    [[ARManager getInstance] addSticker:sticker];
    
    sticker = nullptr;

    selectedGLX = 0;
    selectedGLY = 0;
    [self.collectionView setHidden:YES];
}

-(void) handleSingleTap:(UITapGestureRecognizer *)sender
{
    CGPoint location = [sender locationInView:self.view];
    
    selectedGLX = 0.0f;
    selectedGLY = 0.0f;
    
    MatrixUtil::GetGLCoordFromScreenCoord(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, selectedGLX, selectedGLY);
}

-(void) handleLongTap:(UITapGestureRecognizer *)sender
{
    [self.trashbox setHidden:YES];
    self.trashbox.alpha = 0.6f;
    
    CGPoint location = [sender locationInView:self.view];
    
    float glX = 0.0f;
    float glY = 0.0f;
    
    MatrixUtil::GetGLCoordFromScreenCoord(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, glX, glY);
    
    selectSticker = [[ARManager getInstance] getTouchedSticker:glX :glY];
    int longtapStartX = 0;
    int longtapStartY = 0;
    
    if(selectSticker == nullptr)
    {
        return;
    }
    else
    {
        if(UIGestureRecognizerStateBegan == sender.state)
        {
            trackingReady = true;
            longtapStartX = location.x;
            longtapStartY = location.y;
        }
        else if(UIGestureRecognizerStateChanged == sender.state)
        {
            if(abs(touchStartX - location.x) > 5 || abs(touchStartY - location.y) > 5)
            {
                selectSticker->setTranslate(glX, glY);
                
                longtapStartX = location.x;
                longtapStartY = location.y;
            }
        }
        else if(sender.state == UIGestureRecognizerStateEnded)
        {
            [self.indicatorView setHidden:NO];
            [self.indicator startAnimating];
            
            selectSticker->setTranslate(glX, glY);
            
            [[ARManager getInstance] startTracking:selectSticker :imageReader->getCurrentIndex() :imageReader->getLastIndex() :location.x :location.y :trackingMethod];
            
            longtapStartX = 0;
            longtapStartY = 0;
            selectSticker = nullptr;
            
            imageReader->rewind();
            trackingReady = false;
        }
    }
}

- (void) touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    
    float glX = 0.0f;
    float glY = 0.0f;
    
    MatrixUtil::GetGLCoordFromScreenCoord(surfaceSize.width, surfaceSize.height,
                                          imageWidth, imageHeight, location.x, location.y, glX, glY);
    
    selectSticker = [[ARManager getInstance] getTouchedSticker:glX :glY];
    if(selectSticker == nullptr)
    {
        touchStartX = 0;
        touchStartY = 0;
        return;
    }
    else
    {
        [self.trashbox setHidden:NO];
        if(CGRectContainsPoint(self.trashbox.frame, location))
        {
            self.trashbox.alpha = 1.0f;
        }
        else
        {
            self.trashbox.alpha = 0.6f;
        }
        
        [[ARManager getInstance] stopTracking:selectSticker];
        touchStartX = location.x;
        touchStartY = location.y;
    }
}

- (void) touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];

    if(selectSticker != nullptr)
    {
        if(abs(touchStartX - location.x) > 5 || abs(touchStartY - location.y) > 5)
        {
            float glX = 0.0f;
            float glY = 0.0f;
            
            MatrixUtil::GetGLCoordFromScreenCoord(surfaceSize.width, surfaceSize.height,
                                                       imageWidth, imageHeight, location.x, location.y, glX, glY);
            
            selectSticker->setTranslate(glX, glY);
            touchStartX = location.x;
            touchStartY = location.y;
        }
    }
}

- (void) touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if(selectSticker == nullptr)
    {
        return;
    }
    
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];

    if(CGRectContainsPoint(self.trashbox.frame, location))
    {
        [[ARManager getInstance] removeSticker:selectSticker];
    }
    selectSticker = nullptr;
    touchStartX = 0;
    touchStartY = 0;

    [self.trashbox setHidden:YES];
    self.trashbox.alpha = 0.6f;
}

@end
