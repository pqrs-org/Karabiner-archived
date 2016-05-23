#import "SparkleDelegate.h"

@implementation SparkleDelegate

- (void)updaterDidNotFindUpdate:(SUUpdater *)update {
  [NSApp terminate:self];
}

@end
