//
//  GLView.h
//  0x00
//
//  Created by ZhangXiaoJun on 2018/6/26.
//  Copyright © 2018年 meitu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLContext_iOS.h"
#import "Renderer.hpp"

@class GLView;
@protocol GLViewDelegate<NSObject>
- (void)viewDidCreateGLResource:(GLView *)view;
@end

@interface GLView : UIView
{
    MTCB::GLContext_iOS *_context;
    __unsafe_unretained CAEAGLLayer *_eaglLayer;
    GLuint _framebuffer, _renderbuffer;
    CGSize _oldSize;
    EAGLContext *_eaglContext;
}

@property (nonatomic, readonly, assign) MTCB::GLContext_iOS *context;

@property (nonatomic, assign) Renderer *renderer;

@property (nonatomic, weak) IBOutlet id<GLViewDelegate> delegate;

- (void)present;

@end
