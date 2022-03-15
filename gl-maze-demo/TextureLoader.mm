//
//  TextureLoader.mm
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-10.
//

#import "TextureLoader.h"

#include "Assert.hpp"

@implementation TextureLoader

+ (GLuint)loadTextureFile:(NSString *)fileName
{
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    ASSERT(spriteImage);
    
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte *spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    CGContextRelease(spriteContext);
    
    GLuint texName;
    
    GL_CALL(glGenTextures(1, &texName));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texName));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData));
    free(spriteData);
    
    return texName;
}

@end
