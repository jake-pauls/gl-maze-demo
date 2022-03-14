//
//  TextureLoader.h
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-10.
//

#ifndef TextureLoader_h
#define TextureLoader_h

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface TextureLoader : NSObject

+ (GLuint)loadTextureFile:(NSString *)fileName;

@end

#endif /* TextureLoader_h */
