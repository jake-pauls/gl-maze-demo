//
//  ViewRenderer.h
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef ViewRenderer_h
#define ViewRenderer_h

#import <GLKit/GLKit.h>

enum { SwipeUp=1, SwipeDown, SwipeLeft, SwipeRight };

@interface Scene : NSObject

@property bool useFog;
@property bool useLight;
@property bool isDay;

- (void)setup:(GLKView *)view;
- (void)loadModels;
- (void)update;
- (void)draw;
- (void)doDoubleTap;
- (void)swipe:(int)direction;
- (void)look:(CGPoint)lookdirection;

@end

#endif /* ViewRenderer_h */
