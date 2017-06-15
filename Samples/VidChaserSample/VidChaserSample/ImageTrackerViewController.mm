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
#import "RendererAPI.h"
#import "ProjectionMatrix.h"
#import "CollectionViewCell.h"
#import "PreferenceData.h"
#import "Sticker.h"

#import <vector>
#import <VidChaserEngine/VidChaserAPI.h>
#import <VidChaserEngine/VidChaserDefine.h>
#import <VidChaserEngine/CoordiCvtUtil.h>
#import <MaxstARAPI.h>


@interface ImageTrackerViewController ()
{
    Renderer::Sticker *selectSticker;
    CGSize screenSize;
    CGSize surfaceSize;
    
    NSArray *stickerArray;
    NSArray *stickerImages;
    
    ImageReader *imageReader;
    
    int imageWidth;
    int imageHeight;
    int colorFormat;
    int length;
    VidChaser::TrackingMethod trackingMethod;
    
    bool trackingReady;
    
    float selectedGLX, selectedGLY;
    int touchStartX, touchStartY;
    int selectedSticker;
    UILongPressGestureRecognizer *longTapGestureRecognizer;
    UITapGestureRecognizer *singleTapGestureRecognizer;
}
@property (weak, nonatomic) IBOutlet UILabel *indexText;
@property (weak, nonatomic) IBOutlet UICollectionView *collectionView;
@property (weak, nonatomic) IBOutlet UIView *removeview;
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
    
    trackingMethod = (VidChaser::TrackingMethod)[[PreferenceData getInstance] getValue:PreKey::TRACKING];
    
    self.indexText.text = [NSString stringWithFormat:@"000 / %d", imageReader->getLastIndex()];
    trackingReady = false;
    
    stickerArray = [NSArray arrayWithObjects:@"Clipart.png", @"Flower.png", @"Heart.png", @"Rainbow.png", @"Smile.png", nil];
    
    CGRect rect = [[UIScreen mainScreen] bounds];
    surfaceSize = CGSizeMake(rect.size.width, rect.size.height);
    
    ProjectionMatrix::getInstance()->setSurfaceSize(surfaceSize.width, surfaceSize.height);
    
    CGRect rectNative = [[UIScreen mainScreen] nativeBounds];
    screenSize = CGSizeMake(rectNative.size.height, rectNative.size.width);

    [self.collectionView setDelegate:self];
    [self.collectionView setDataSource:self];
    [self.collectionView setHidden:YES];
    self.collectionView.alpha = 0.6f;
    
    [self.indicatorView setHidden:YES];
    self.indicator.transform = CGAffineTransformMakeScale(2.5f, 2.5f);
    
    self.removeview.alpha = 0.6f;
    [self.removeview setHidden:YES];
    
    longTapGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongTap:)];
    [longTapGestureRecognizer setMinimumPressDuration:0.5];
    [self.view addGestureRecognizer:longTapGestureRecognizer];
    
    singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]
                                                          initWithTarget:self action:@selector(handleSingleTap:)];
    
    singleTapGestureRecognizer.numberOfTapsRequired = 1;
    [singleTapGestureRecognizer setCancelsTouchesInView:NO];
    [self.collectionView addGestureRecognizer:singleTapGestureRecognizer];
    [self.view addGestureRecognizer:singleTapGestureRecognizer];
    
    selectedSticker = -1;
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
    maxstAR::initRendering();
    Renderer::initRendering();
    VidChaser::init("icWQYj5ucBSSl2DXHNVSTdDalq+Doh1uEfCs+kgITS8=");
    int state = VidChaser::create();
    if(state != 0)
    {
        NSLog(@"Signature Error\n");
        return;
    }
    NSLog(@"Signature Success\n");
    maxstAR::updateRendering(screenSize.width, screenSize.height);
   
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
  
    if(imageReader->getCurrentIndex() == 0)
    {
        if(self.indicatorView.hidden == false)
        {
            [self.indicator stopAnimating];
            [self.indicatorView setHidden:YES];
        }
    }
    
    self.indexText.text = [NSString stringWithFormat:@"%d / %d", imageReader->getCurrentIndex(), imageReader->getLastIndex() - 1];
    if(imageReader->hasNext())
    {
        unsigned char *imageBuffer = imageReader->readFrame(trackingReady);
        
        maxstAR::setNewFrame(imageBuffer, length, imageWidth, imageHeight, (ColorFormat)colorFormat);
        maxstAR::renderScene();
        VidChaser::setTrackingImage(imageBuffer, imageWidth, imageHeight, (ColorFormat)colorFormat, imageReader->getCurrentIndex());
        Renderer::draw(imageReader->getCurrentIndex());
    }
    else
    {
        imageReader->reset();
    }
}

- (IBAction)ClickBackButton:(id)sender {
    maxstAR::deinitRendering();
    VidChaser::destroy();
    Renderer::deinit();
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
    selectedSticker = (int)indexPath.row;
    
    NSString *stickerPath = [stickerArray objectAtIndex:selectedSticker];
    
    UIImage * stickerImage = [UIImage imageNamed: stickerPath];
    
    unsigned char* stickerBuffer = [Utils imageToBytes:stickerImage];
    
    Renderer::Sticker * sticker = Renderer::createSticker();
    sticker->init();
    sticker->setTexture(stickerImage.size.width, stickerImage.size.height, stickerBuffer);
    
    float ratio = stickerImage.size.width / (float) surfaceSize.width;
    float size = stickerImage.size.width * ratio;
    
    sticker->setScale(size, size, 1.0f);
    sticker->setTranslate(selectedGLX, selectedGLY, 0.0f);
    Renderer::addSticker(sticker);

    selectedGLX = 0;
    selectedGLY = 0;
    selectedSticker = -1;
    [self.collectionView setHidden:YES];
}

-(void) handleSingleTap:(UITapGestureRecognizer *)sender
{
    NSLog(@"Single Tap\n");
    CGPoint location = [sender locationInView:self.view];
    
    selectedGLX = 0.0f;
    selectedGLY = 0.0f;
    
    CoordiCvtUtil::GetGLCoordiFromScreenCoordi(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, selectedGLX, selectedGLY);
}

-(void) handleLongTap:(UITapGestureRecognizer *)sender
{
    CGPoint location = [sender locationInView:self.view];
    
    float glX = 0.0f;
    float glY = 0.0f;
    
    CoordiCvtUtil::GetGLCoordiFromScreenCoordi(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, glX, glY);
    
    Renderer::Sticker *sticker = Renderer::getTouchedSticker(glX, glY);

    int longtapStartX = 0;
    int longtapStartY = 0;
    
    if(sticker == nullptr)
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
                sticker->setTranslate(glX, glY);
                
                longtapStartX = location.x;
                longtapStartY = location.y;
            }
        }
        else if(sender.state == UIGestureRecognizerStateEnded)
        {
            [self.indicatorView setHidden:NO];
            [self.indicator startAnimating];
            
            sticker->setTranslate(glX, glY);
            sticker->startTracking(imageReader->getCurrentIndex(), imageReader->getLastIndex(), location.x, location.y, trackingMethod);
            
            longtapStartX = 0;
            longtapStartY = 0;
            
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
    
    CoordiCvtUtil::GetGLCoordiFromScreenCoordi(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, glX, glY);
    
    Renderer::Sticker *sticker = Renderer::getTouchedSticker(glX, glY);
    if(sticker == nullptr)
    {
        touchStartX = 0;
        touchStartY = 0;
        return;
    }
    else
    {
        sticker->stopTracking();
        selectSticker = sticker;
        touchStartX = location.x;
        touchStartY = location.y;
    }
}

- (void) touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];

    float glX = 0.0f;
    float glY = 0.0f;
    
    CoordiCvtUtil::GetGLCoordiFromScreenCoordi(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, glX, glY);
    
    if(selectSticker != nullptr)
    {
        [self.removeview setHidden:NO];
        if(CGRectContainsPoint(self.removeview.frame, location))
        {
            self.removeview.alpha = 1.0f;
        }
        else
        {
            self.removeview.alpha = 0.6f;
        }
        
        if(abs(touchStartX - location.x) > 5 || abs(touchStartY - location.y) > 5)
        {
            selectSticker->setTranslate(glX, glY);
            
            touchStartX = location.x;
            touchStartY = location.y;
        }
    }
}

- (void) touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];

    float glX = 0.0f;
    float glY = 0.0f;
    
    CoordiCvtUtil::GetGLCoordiFromScreenCoordi(surfaceSize.width, surfaceSize.height,
                                               imageWidth, imageHeight, location.x, location.y, glX, glY);
    
    if(selectSticker != nullptr)
    {
        selectSticker->setTranslate(glX, glY);
        if(CGRectContainsPoint(self.removeview.frame, location))
        {
            Renderer::removeSticker(selectSticker);
        }
        selectSticker = nullptr;
        touchStartX = 0;
        touchStartY = 0;
    }
    else
    {
        Renderer::Sticker *sticker = Renderer::getTouchedSticker(glX, glY);
        
        if(sticker == nullptr)
        {
            return;
        }
        else
        {
            sticker->stopTracking();
            sticker->setTranslate(glX, glY);
        }
    }

    [self.removeview setHidden:YES];
    self.removeview.alpha = 0.6f;
}

@end
