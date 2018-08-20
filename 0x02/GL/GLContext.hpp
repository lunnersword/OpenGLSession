//
//  GLContext.hpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#ifndef GLContext_hpp
#define GLContext_hpp

#include <stdio.h>
#include "gl.h"

namespace MTCB {
    
    enum GLRenderAPI{
        GLRenderAPIES1 = 1,
        GLRenderAPIES2 = 2,
        GLRenderAPIES3 = 3,
    };
    
    class GLContext{
        const void *mSharedObject = NULL;
    protected:
        GLRenderAPI mRenderAPI = GLRenderAPIES2;
    public:

        GLContext(GLRenderAPI renderAPI = GLRenderAPIES2, const void *sharedObject = NULL);

        GLRenderAPI renderAPI() const ;

        const void *sharedObject() const;
		
        virtual bool isCurrentContext() = 0;
        virtual void use() = 0;
        virtual ~GLContext(){};
        virtual void swapToScreen() = 0;
    };
}



#endif /* GLContext_hpp */
