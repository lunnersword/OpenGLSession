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
    __weak IBOutlet GLView *_glView;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _glView.delegate = self;
    [self.view addSubview:_glView];
    // Do any additional setup after loading the view, typically from a nib.
	
}

- (void)viewDidCreateGLResource:(GLView *)view{
    _glView.renderer = new CircleRenderer();
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
