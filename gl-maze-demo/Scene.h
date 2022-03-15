//
//  ViewRenderer.h
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef ViewRenderer_h
#define ViewRenderer_h

#import <GLKit/GLKit.h>

@interface Scene : NSObject

@property bool useFog;
@property bool useLight;
@property bool isDay;

- (void)setup:(GLKView *)view;
- (void)loadModels;
- (void)update;
- (void)draw;

@end

#endif /* ViewRenderer_h */
