#import "StatusWindow.h"
#import "server_objc_part.h"

@implementation StatusWindow

- (id) init
{
  [super init];

  lines_ = [NSMutableDictionary new];
  for (NSUInteger i = 0; i < STATUSMESSAGETYPE__END__; ++i) {
    [lines_ addObject:@""];
  }

  return self;
}

- (void) dealloc
{
  if (lines_) {
    [lines_ release];
  }
  [super dealloc];
}

- (void) resetStatusMessage
{
  for (NSUInteger i = 0; i < STATUSMESSAGETYPE__END__; ++i) {
    [lines_ replaceObjectAtIndex:i withObject:@""];
  }
}

- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message
{
  [lines_ replaceObjectAtIndex:lineIndex withObject:message];
}

@end
