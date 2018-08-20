//
//  GLView.m
//  0x00
//
//  Created by ZhangXiaoJun on 2018/6/26.
//  Copyright © 2018年 meitu. All rights reserved.
//

#import "GLView.h"

@interface GLView()
@property (nonatomic, strong) CADisplayLink *displayLink;
@end

@implementation GLView

- (void)dealloc
{
    if (_context) {
        delete _context;
        _context = NULL;
    }
}

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [self commonInit];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self commonInit];
    }
    return self;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self commonInit];
    }
    return self;
}

- (void)commonInit{
    self.contentScaleFactor = [UIScreen mainScreen].scale;
    _context = new MTCB::GLContext_iOS(MTCB::GLRenderAPIES2);
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    [_displayLink addToRunLoop:[NSRunLoop mainRunLoop]
                       forMode:NSDefaultRunLoopMode];
    _eaglContext = (__bridge EAGLContext *)_context->EAGLCtx();
}

+ (Class)layerClass{
    return [CAEAGLLayer class];
}

- (void)layoutSubviews{
    [super layoutSubviews];
    
    CGSize size = self.frame.size;
    if (CGSizeEqualToSize(_oldSize, CGSizeZero) ||
        !CGSizeEqualToSize(_oldSize, size)) {
        [self setup];
        _oldSize = size;
    }
}

- (void)setup{
    // 用于显示的layer
    _eaglLayer = (CAEAGLLayer *)self.layer;
    
    //  CALayer默认是透明的，而透明的层对性能负荷很大。所以将其关闭。
    _eaglLayer.opaque = YES;
    
    // 释放旧的renderbuffer
    if (_renderbuffer) {
        glDeleteRenderbuffers(1, &_renderbuffer);
        _renderbuffer = 0;
    }
    
    // 释放旧的framebuffer
    if (_framebuffer) {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    
    // 生成renderbuffer(renderbuffer=用于展示的窗口)
    glGenRenderbuffers(1, &_renderbuffer);
    
    // 绑定renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    
    // GL_RENDERBUFFER的内容存储到实现EAGLDrawable协议的CAEAGLLayer
    [_eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
	
    // 生成framebuffer(framebuffer=画布)
    glGenFramebuffers(1, &_framebuffer);
    
    // 绑定Fraembuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    // framebuffer不对绘制的内容做存储，所以这一步是将framebuffer绑定到renderbuffer(绘制的结果就存在renderbuffer)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _renderbuffer);
    
    // 检查framebuffer是否创建成功
    NSError *error;
    NSAssert1([self checkFramebuffer:&error], @"%@",error.userInfo[@"ErrorMessage"]);
	
    if ([self.delegate respondsToSelector:@selector(viewDidCreateGLResource:)]) {
        [self.delegate viewDidCreateGLResource:self];
    }
}

- (BOOL)checkFramebuffer:(NSError *__autoreleasing *)error
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    NSString *errorMessage = nil;
    BOOL result = NO;
    
    switch (status)
    {
        case GL_FRAMEBUFFER_UNSUPPORTED:
            errorMessage = @"framebuffer不支持该格式";
            result = NO;
            break;
        case GL_FRAMEBUFFER_COMPLETE:
            NSLog(@"framebuffer 创建成功");
            result = YES;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            errorMessage = @"Framebuffer不完整 缺失组件";
            result = NO;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            errorMessage = @"Framebuffer 不完整, 附加图片必须要指定大小";
            result = NO;
            break;
        default:
            // 一般是超出GL纹理的最大限制
            errorMessage = @"未知错误 error !!!!";
            result = NO;
            break;
    }
    
    NSLog(@"%@",errorMessage ? errorMessage : @"");
    *error = errorMessage ? [NSError errorWithDomain:@"com.meitu.error"
                                                code:status
                                            userInfo:@{@"ErrorMessage" : errorMessage}] : nil;
    
    return result;
}

- (void)bindDrawable{
    
    if ([EAGLContext currentContext] != _eaglContext) {
        [EAGLContext setCurrentContext:_eaglContext];
    }
    
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

- (void)present{
    self.context->swapToScreen();
}

- (CGSize)viewPortSize
{
    return CGSizeMake(_oldSize.width * self.contentScaleFactor, _oldSize.height * self.contentScaleFactor);
}

- (void)update{
    [self bindDrawable];
    if (self.renderer) {
        self.renderer->render(_Size((float)self.viewPortSize.width, (float)self.viewPortSize.height));
    }
    [self present];
}

- (void)setRenderer:(Renderer *)renderer{
    if (_renderer != renderer) {
        if (_renderer) {
            delete _renderer;
        }
        _renderer = renderer;
    }
}

@end
