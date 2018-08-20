//
//  ViewController.m
//  0x00
//
//  Created by ZhangXiaoJun on 2018/6/26.
//  Copyright © 2018年 meitu. All rights reserved.
//

#import "ViewController.h"
#import "GLView.h"
#include "Renderer.hpp"

@interface ViewController () <GLViewDelegate>
{
    __strong IBOutlet GLView *_glView;
}
@end

@implementation ViewController

- (void)dealloc
{
    [super dealloc];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidCreateGLResource:(GLView *)view{
    view.renderer = new EffectRenderer([[[NSBundle mainBundle] pathForResource:@"1" ofType:@"jpg"] UTF8String]);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onValueChange:(id)sender {
    UISlider *slider = (UISlider *)sender;
    void *renderer = _glView.renderer;;
    EffectRenderer *effect = (EffectRenderer *)renderer;
    effect->onValueChange(slider.value);
}

@end
